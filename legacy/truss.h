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


#ifndef TRUSS_H
#define TRUSS_H

#include <QtGui>
#include "commonmath.h"
#include "node.h"
#include "beam.h"
#include "linearsystem.h"

class Truss : public QGraphicsItem
        // TODO: inherit from "public QGraphicsLayoutItem" to allow dynamic placing of widgets like "+" and "-"
{
public:
    Truss();
    QList<Node*> nodes() const;
    QList<Beam*> beams() const;

    void enumerate_dofs(); ///< Assign identificative numbers to degree of freedom.
    void solve();
    unsigned int dofs_count() const; /// Number of degrees of freedom of the solution.

    void add_node (Node &a_node);
    void add_beam (Beam &a_beam);
    unsigned int support_conditions_count() const; // The number of support conditions of all nodes

    Material *material; ///< The one material currently supported
    Section *section; ///< The one section currently supported

    bool is_solving_immediate()
            /// Will any change in the Current truss trigger a new solution?
    { return immediate_solving; };

    void set_immediate_solving (bool a_value)
            /// Set is_solving_immediate to a_value.
    { immediate_solving = a_value; };

   QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void update_scales(); ///< Updates the scale used to draw nodes, deformed beams, loads, bearing reactions. Also set actions_font
    qreal action_scale; /// The scale applied to concentrated actions and node reactions.
    qreal load_scale; /// The scale applied to loads
    qreal deformation_scale; /// Scale applied to displacement of the beam.
    qreal axial_scale; /// The scale applied to axial internal forces
    qreal shear_scale; /// The scale applied to shear internal forces
    qreal moment_scale; /// The scale applied to moment internal forces
    QFont actions_font;

    QList<Node *> nodes_list;
    QList<Beam *> beams_list;

    qreal longest; /// Length of the longest beam
    qreal shortest; /// Length of the shortest beam
    qreal highest; /// Height of the highest beam
    qreal strongest_beam; /// The highest moment of inertia of a beam.

    Matrix<qreal, Dynamic, Dynamic> stiffness; /// Stiffness matrix
    Matrix<qreal, Dynamic, 1> loads; /// Loads
    Matrix<qreal, Dynamic, Dynamic> displacements; /// Solution

//    Please note that everything is made public on purpose as I find C++
//    information hiding clumsy and limited. Feature accessibility should be made
//    Eiffel-like, that is at class level and allowing changes to the object only
//    within its commands

    LinearSystem *solving_system;
    bool immediate_solving;
};

#endif // TRUSS_H
