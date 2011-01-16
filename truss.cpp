/*
    Copyright (c) 2010 Paolo Redaelli

    This file is part of Monodes.

    Monodes is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Monodes is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <limits>

#include "commonmath.h"
#include "truss.h"
#include "node.h"
#include "beam.h"

// We need to use isfinite, so we
#define _ISOC99_SOURCE 1

Truss::Truss()
{
    material = new Material(210000.0,0.9,1.4e-6); // Steel
    section = new Section(8.0, 15.0);
    longest = 0.0;
    shortest = std::numeric_limits<qreal>::infinity();
    highest = 0.0;
    deformation_scale=1.0;
    axial_scale = 1.0;

    strongest_beam = 0.0;
    load_scale = 1.0;
    immediate_solving = false;

}

void Truss::add_node(Node &a_node) {
    a_node.setParentItem(this); // a_node will be a child item of current truss, so there's no need to add it.
    //std::cout<<"Adding node"<<a_node<<"\n"<<std::flush;
    assert(!nodes().contains(&a_node));
    nodes_list.append(&a_node);
    //a_node.scale(bearing_size);
}

void Truss::add_beam(Beam &a_beam) {
    assert(!beams().contains(&a_beam));
    beams_list.append(&a_beam);
    longest = fmax(a_beam.length(), longest);
    shortest = fmin(a_beam.length(), shortest);
    highest = fmax(a_beam.section.height(),highest);
}

QList<Node*> Truss::nodes() const {
    return nodes_list;
}

QList<Beam*> Truss::beams() const {
    return beams_list;
}

QRectF Truss::boundingRect() const
{
    // Truss bounding rectangle is the one bounding all its children, slightly enlarged.
    QRectF bound = childrenBoundingRect();
    return bound.adjusted(-bound.width()/10,-bound.height()/10,bound.width()/10,bound.height()/10);
}

void Truss::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Nothing; the drawing of a truss is made by its children items, namely nodes and beams.

    // Compute the proper scale for loads, deformed beams and bearing reactions.
}

void Truss::update_scales() {
    // Find the biggest deformation, load, axial, shear, moment, action and reaction
    qreal max_deflection = 0.0;
    qreal highest_load=0.0;
    qreal highest_axial=0.0;
    qreal highest_shear=0.0;
    qreal highest_moment=0.0;
    qreal highest_action=0.0;
    foreach (Beam *a_beam, beams()) {
        max_deflection = fmax(fabs(a_beam->maximum_deflection()), max_deflection);
        highest_load = fmax(highest_load, fabs(a_beam->constant_load()));
        highest_axial = fmax(fabs(a_beam->maximum_axial()), highest_axial);
        highest_shear = fmax(fabs(a_beam->maximum_shear()), highest_shear);
        highest_moment= fmax(fabs(a_beam->maximum_moment()), highest_moment);
    }

    // Updating load scale: highest load will be high the half of the longest beam.
    load_scale= longest / highest_load/2.0;
    // Updates node-scale and finding the highest reaction
    qreal node_scale = fmax( /* the radius of the nodes will be the biggest of */
            2.0*highest /* two times the highest beam */,
            longest/20.0 /* 1/20 of longest beam. */ );
    foreach (Node *n, nodes()) {
        n->setScale(node_scale);
        highest_action = fmax(highest_action, fmax(n->horizontal,n->vertical));
    }
    // The highest reaction will be drawn a little larger than the highest load, i.e. 60% of the longest beam
    action_scale = 0.6 * longest / highest_action;

    // Maximum deflection, will be scaled to be as big as the smallest beam.
    if (max_deflection<1e-20) deformation_scale = 1.0;
    else deformation_scale = shortest / max_deflection;
    //std::cout<<"deformation scale = "<<shortest<<" / "<<max_deflection<<" = "<<deformation_scale<<std::endl;

    std::cout<<QString("Truss highest: axial %1 shear %2 moment %3")
            .arg(highest_axial).arg(highest_shear).arg(highest_moment).toStdString()<<std::endl;
    if (highest_axial<1e-20) axial_scale = 1.0; // Reasonable default for almost zero
    else axial_scale = longest / highest_axial; // Maximum axial will be scaled to be as big as the longest beam.
    if (highest_shear<1e-20) shear_scale = 1.0; // Reasonable default for almost zero
    else shear_scale = longest / highest_shear; // Maximum shear will be scaled to be as big as the longest beam.
    if (highest_moment<1e-20) moment_scale = 1.0; // Reasonable default for almost zero
    else moment_scale = longest / highest_moment; // Maximum moment will be scaled to be as big as the longest beam.

    foreach (Beam *a_beam, beams()) a_beam->update_plots();
    // Update bearing-reaction-scale

    actions_font.setPointSizeF(fmax(shortest/5,longest/10));
}

unsigned int Truss::support_conditions_count() const {
    unsigned int result=0;
    foreach (Node *node, nodes())
        result += node->support_conditions_count();
    return result;
}

unsigned int Truss::dofs_count() const {
    return nodes().size()*3 - support_conditions_count(); // degrees of freedom
}

void Truss::enumerate_dofs() {
    // Degrees of freedom numbering
    unsigned int dof = 0;
    foreach (Node *n, nodes()) {
        assert (/* non null node*/ n != NULL);
        switch (n->constrain()) {
        case uncostrained:
            n->dof_x = dof++;
            n->dof_y = dof++;
            n->dof_tetha = dof++;
//#ifdef DEBUG
//            std::cout<<n;
//#endif
            break;
            // TODO:	case vertical_trailer: break;
            // TODO:    case horizontal_trailer: break;
        case hinge:
            n->dof_x = -1;
            n->dof_y = -1;
            n->dof_tetha = dof++;
//#ifdef DEBUG
//            std::cout<<n;
//#endif
            break;
            // TODO: case vertical_shoe:break;
            // TODO: case horizontal_shoe:break;
        case restrained:
            n->dof_x = -1;
            n->dof_y = -1;
            n->dof_tetha = -1;
            break;
        }
    }

//#ifdef DEBUG
//    std::cout<<"Degrees of freedom:";
//    foreach (Node *n, nodes()) std::cout<<*n<<std::endl;
//#endif
}

void Truss::solve() {
    int dofs=dofs_count();
    //int lcc=1; // load cases count; currently fixed at 1.
    enumerate_dofs();

    // TODO: move the matrices into a linearsystem!
    stiffness = Matrix<qreal, Dynamic, Dynamic>(dofs,dofs);
    stiffness.setZero(); // Why, oh WHY C++ DOES NOT INITIALIZE objects??????? Without this command the following assertion that most people would take for granted will miserably fail.
    assert (stiffness.isZero());
    loads = Matrix<qreal, Dynamic, 1>(dofs);
    loads.setZero();
    assert(loads.isZero());
    // Sum all the stiffness at the proper degree of freedom
    foreach (Beam *beam, beams()) {
        Matrix<qreal,6,6> &beam_stiffness = beam->stiffness();
        //std::cout<<beam_stiffness;
        assert (beam_stiffness == beam_stiffness.transpose());
        Matrix<qreal,6,1> &nodal_forces = beam->fixed_end_forces();
        Node &n1 = beam->first(); Node &n2 = beam->second();
        int dofs[6] = {n1.dof_x, n1.dof_y, n1.dof_tetha,
                       n2.dof_x, n2.dof_y, n2.dof_tetha};
        for (int i=0; i<6; ++i) {
            int dof1 = dofs[i];
            if (dof1>=0) { /* This degree of freedom is unbounded */
                loads(dof1) += nodal_forces[i];
                for (int j=0; j<6; ++j) {
                    int dof2=dofs[j];
                    if (dof2>=0) stiffness(dof1,dof2) += beam_stiffness(i,j);
                }
            }
        }
    }

//    std::cout<<"(st- st^t).max = "<<(stiffness - stiffness.transpose()).maxCoeff()<<std::endl
//            <<"Stiffness:"<<std::endl<<stiffness<<std::endl
//            <<"Loads: "<<loads<<std::endl;
    assert(/* Sometime the strictly symmetrical assertion, stiffness == stiffness.transpose() will fail, */
            /* stiffness shall be symmetrical, accounting for numerical approximations */
            stiffness.isApprox(stiffness.transpose()));

    // TODO: We shall compute it in a separate thread!
    displacements = Matrix<qreal, Dynamic, Dynamic>(stiffness.rows(), loads.cols());
    stiffness.lu().solve(loads, &displacements);
//#ifdef DEBUG
//    std::cout<<"Displacements: "<<std::endl<<displacements<<std::endl;
//#endif
    prepareGeometryChange(); // Before updating node displacements
    /// Updates all node deformations
    foreach (Node *n, nodes()) {
        if (n->dof_x >= 0) n->set_u(displacements[n->dof_x]);
        if (n->dof_y >= 0) n->set_v(displacements[n->dof_y]);
        if (n->dof_tetha >= 0) n->set_fi(displacements[n->dof_tetha]);
    }
    foreach (Beam *b, beams()) b->compute_deformed();
    foreach (Node *n, nodes()) n->update_reactions();
    update_scales(); /// used for drawing

}
