/*
    Copyright (c) 2010 Paolo Redaelli

    This file is part of Monodes.

    Monodes is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BEAM_H
#define BEAM_H

#include <QtGui>
#include <Eigen/Core>
#include <Eigen/Array> // To compare matrices

// import most common Eigen types
USING_PART_OF_NAMESPACE_EIGEN
using Eigen::Dynamic;
// To avoid alignment issues. See http://eigen.tuxfamily.org/dox/UnalignedArrayAssert.html
#define EIGEN_DONT_VECTORIZE 1
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1

#include "section.h"
#include "material.h"

/* Hermite functions */
qreal f1(qreal csi);
qreal f2(qreal csi);
qreal f3(qreal csi);
qreal f4(qreal csi);
qreal f5(qreal csi);
qreal f6(qreal csi);

class Node; // Forward declaration
class Truss; // Forward declaration

class Beam : public QGraphicsItem
{
public:
    Beam(Node *a_node, Node *another_node);
    enum { Type = UserType + 2 };
    int type() const { return Type; }

    Node &first() const;
    Node &second() const;
    qreal length() const;

    // Currently sticking to one, uniform load orthogonal to the beam
    qreal load;

    void set_section (Section &a_section);
    Section &section() const;

    void set_material (Material &a_material);
    Material &material() const;


    EIGEN_MAKE_ALIGNED_OPERATOR_NEW // See http://eigen.tuxfamily.org/dox/StructHavingEigenMembers.html
    Matrix<qreal, 6, 6> &stiffness(); // Stiffness matrix in global coordinates
    Matrix<qreal, 6, 6> &local_stiffness(); // Stiffness matrix in local coordinates
    Matrix<qreal, 6, 6> &transformation(); // the transformation matrix
    Matrix<qreal, 6, 1> &fixed_end_forces(); // fixed-end nodal forces in global coordinates
    Matrix<qreal, 6, 1> &member_end_forces(); // The forces at the end of current element resulting from the applied loads and deformation of the structure.

    void compute_deformed();
    qreal maximum_deflection();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Node *first_node, *second_node;
    qreal beam_length;
    QPolygonF deformed;
    qreal max_deflection;

    Section *s;
    Material *m;

    Truss &truss() const; // The truss containing current beam

    void compute_stiffness();
    bool stiffness_computed;
    Matrix<qreal, 6, 6> st; ///< Global stiffness.
    Matrix<qreal, 6, 6> local_st; ///< Local stiffness.
    Matrix<qreal, 6, 6> tr; ///< Transformation matrix.
    Matrix<qreal, 6, 1> gf; ///< nodal fixed-end forces in global cohordinates.
    Matrix<qreal, 6, 1> f; ///< nodal fixed-end forces.
    bool member_end_forces_computed;
    Matrix<qreal, 6, 1> mef; ///< precomputed member end forces
    qreal u(qreal csi);
    qreal v(qreal csi);
// no signals:

// no public slots:

};

std::ostream &operator<<(std::ostream &s, Beam &a_beam);

#endif // BEAM_H
