/*
    Copyright (c) 2010 Paolo Redaelli

    This file is part of Monodes.

    Monodes is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Monodes: is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QtGui>
#include <QList>

// Forward declarations
class Beam;
class Canvas;
class Reactions;

enum Constrain {
    uncostrained, // Free no degrees of freedom bounded
    //vertical_trailer, // Carrello verticale, horizontal displacement locked, free vertical displacement and rotation
    //horizontal_trailer, // Carrello orizzontale, vertical displacement locker, free horizontal displacement and rotation.
    hinge, // Cerniera, displacements fixed, rotation free.
    //vertical_shoe, // Pattino verticale, free vertical displacement, fixed horizontal displacement and rotation
    //horizontal_shoe, // Pattino orizzontale, free horizontal displacement, fixed vertical displacement and rotation
    restrained // Incastro, no free degrees
};

/// Drawing constants
static const qreal fraction_of_span = 12.0; ///< the node will be big as this fraction of its spans.
static const qreal smaller=0.5;
static const qreal bigger=1.0;

static const QPen lightest(Qt::gray, smaller/12.0);
static const QPen light(Qt::black, smaller/10.0);
static const QPen heavy(Qt::black, smaller/8.0);
static const QPointF origin;

class Node : public QGraphicsItem
{
public:
    Node (qreal an_x, qreal an_y, enum Constrain a_constrain=uncostrained);

    void add_beam (Beam *a_beam);
    QList<Beam *> beams() const;
    qreal longest_span() const; /// The length of the longest beam connected to current Node.
    unsigned int support_conditions_count() const; // The number of support conditions of this node

    qreal distance(const Node &anode) const;
    qreal u();
    qreal v();
    qreal fi();

    void set_u (qreal a_value);
    void set_v (qreal a_value);
    void set_fi (qreal a_value);

    QPointF displacement(); ///< Displacement of the deformed node.

    qreal vertical, horizontal, moment; /// Bearing reactions.
    // Constrains
    enum Constrain constrain() const;
    void set_constrain(enum Constrain a_constrain);
    void update_reactions();
    // the indexes of the degrees of freedom of displacements and rotation; the index is -1 when the degree of freedom is locked by a restrain.
    int dof_x, dof_y, dof_tetha;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    // QVariant itemChange(GraphicsItemChange change, const QVariant &value);

   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
   void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );


private:
    enum Constrain my_constrain;

    // Longest span
    qreal longest;
    // Displacements
    qreal node_u, node_v, node_fi;
    QList<Beam *> beams_list;
    Reactions *reactions;

signals:

public slots:

};

std::ostream &operator<<(std::ostream &s, Node &a_node);

#endif // NODE_H


