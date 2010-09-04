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
#include "matrix.h"
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

    Matrix &stiffness(); // In global coordinates
    Matrix &local_stiffness();
    Matrix &transformation(); // the transformation matrix

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Node *first_node, *second_node;
    qreal beam_length;

    Section *s;
    Material *m;


    void compute_stiffness();
    bool stiffness_computed;
    Matrix st;
    Matrix local_st;
    Matrix tr;

    qreal u(qreal csi);
    qreal v(qreal csi);
// no signals:

// no public slots:

};

#endif // BEAM_H
