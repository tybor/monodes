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


#include "truss.h"
#include "node.h"
#include "beam.h"

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

Truss::Truss()
{
    material = new Material(210000.0,0.9,1.4e-6); // Steel
    section = new Section(8.0, 15.0);
    longest_beam = 0.0;
    highest_load = 0.0;
    strongest_beam = 0.0;
    load_scale = 1.0;
//    left->set_material(*stee/l); left->set_section(*section);
//    right->set_material(*steel); right; right->set_section(*section);
}

void Truss::add_node(Node &a_node) {
    a_node.setParentItem(this); // a_node will be a child item of current truss, so there's no need to add it.
    //std::cout<<"Adding node"<<a_node<<"\n"<<std::flush;
    nodes_list.append(&a_node);
    //a_node.scale(bearing_size);
}

void Truss::add_beam(Beam &a_beam) {
    a_beam.setParentItem(this); // a_beam will be a child item of current truss, so there's no need to add it.
    beams_list.append(&a_beam);
}

QList<Node*> Truss::nodes() const {
    return nodes_list;
}

QList<Beam*> Truss::beams() const {
    return beams_list;
}

QRectF Truss::boundingRect() const
{
    QRectF result;
    foreach (Beam *beam, beams()) {result |=beam->boundingRect();}
    foreach (Node *node, nodes()) {result |=node->boundingRect();}
    return result;
}

void Truss::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    // Nothing; the drawing of a truss is made by its children items, namely nodes and beams.
}

unsigned int Truss::support_conditions_count() const {
    unsigned int result=0;
    foreach (Node *node, nodes())
        result += node->support_conditions_count();
}
void Truss::solve() {
    int dofs=nodes().size()*3 - support_conditions_count(); // degrees of freedom
    int lcc=1; // load cases count; currently fixed at 1.

    // Degrees of freedom numbering
    unsigned int dof = 0;
    foreach (Node *n, nodes()) {
        assert (/* non null node*/ n != NULL);
        switch (n->constrain()) {
        case uncostrained:
            n->dof_x = dof++;
            n->dof_y = dof++;
            n->dof_tetha = dof++;
            std::cout<<n;
            break;
            // TODO:	case vertical_trailer: break;
            // TODO:    case horizontal_trailer: break;
        case hinge:
            n->dof_x = -1;
            n->dof_y = -1;
            n->dof_tetha = dof++;
            std::cout<<n;
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

    std::cout<<"Degrees of freedom:";
    foreach (Node *n, nodes()) std::cout<<*n<<std::endl;
    // TODO: move the matrices into a linearsystem!
    Matrix<qreal, Dynamic, Dynamic> stiffness(dofs,dofs);
    Matrix<qreal, Dynamic, 1> loads(dofs);
    // Sum all the stiffness at the proper degree of freedom
    foreach (Beam *beam, beams()) {
        Matrix<qreal,6,6> &beam_stiffness = beam->stiffness();
        Node &n1 = beam->first(); Node &n2 = beam->second();
        int dofs[6] = {n1.dof_x, n1.dof_y, n1.dof_tetha,
                       n2.dof_x, n2.dof_y, n2.dof_tetha};
        for (int i=0; i<6; ++i) {
            int dof1 = dofs[i];
            if (dof1>=0) { /* This degree of freedom is unbounded */
                // TODO: Adding distributed loads
                for (int j=0; j<6; ++j) {
                    int dof2=dofs[j];
                    if (dof2>=0) {
                        std::cout<<"stiffness("<<dof1<<","<<dof2<<") += beam_stiffness("<<i<<","<<j<<");"<<std::endl<<std::flush;
                        stiffness(dof1,dof2) += beam_stiffness(i,j);
                    }
                }
            }
        }
    }
    std::cout<<"Stiffness matrix:"<<std::endl<<stiffness<<std::endl<<std::flush;
}
