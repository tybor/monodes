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

#include <Eigen/Core>
#include <Eigen/Array>

// import most common Eigen types
USING_PART_OF_NAMESPACE_EIGEN
using Eigen::Dynamic;
// To avoid alignment issues. See http://eigen.tuxfamily.org/dox/UnalignedArrayAssert.html
#define EIGEN_DONT_VECTORIZE 1
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1

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

    void solve();
    unsigned int dofs_count() const; /// Number of degrees of freedom of the solution.

    void add_node (Node &a_node);
    void add_beam (Beam &a_beam);
    unsigned int support_conditions_count() const; // The number of support conditions of all nodes
    //qreal longest_beam; /// The lenght of the longest beam, used to determinate the drawing size of bearings
    //qreal highest_load; /// The maximum distributed load

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
    void update_scales(); ///< Updates the scale used to draw nodes, deformed beams, loads, bearing reactions.
    qreal load_scale; /// The scale applied to loads

private:
    QList<Node *> nodes_list;
    QList<Beam *> beams_list;

    qreal longest; /// Length of the longest beam
    qreal shortest; /// Length of the shortest beam
    qreal highest; /// Height of the highest beam

    qreal deformation_scale; /// Scale applied to displacement of the beam.
    qreal strongest_beam; // The highest moment of inertia of a beam.

    LinearSystem *solving_system;
    bool immediate_solving;
};

#endif // TRUSS_H
