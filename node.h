/*
    Copyright (c) 2010 Paolo Redaelli

    This file is part of Monodes.

    0MQ is free software; you can redistribute it and/or modify it under
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


#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

// Forward declarations
class Beam;
class Canvas;

enum Constrain {
    uncostrained, // Free no degrees of freedom bounded
    vertical_trailer, // Carrello verticale, horizontal displacement locked, free vertical displacement and rotation
    horizontal_trailer, // Carrello orizzontale, vertical displacement locker, free horizontal displacement and rotation.
    hinge, // Cerniera, displacements fixed, rotation free.
    vertical_shoe, // Pattino verticale, free vertical displacement, fixed horizontal displacement and rotation
    horizontal_shoe, // Pattino orizzontale, free horizontal displacement, fixed vertical displacement and rotation
    restrained // Incastro, no free degrees
};

class Node : public QGraphicsItem
{
public:
    Node (qreal an_x, qreal an_y, enum Constrain a_constrain=uncostrained);

    void add_beam (Beam *a_beam);
    QList<Beam *> beams() const;
    unsigned int support_conditions_count() const; // The number of support conditions of this node

    qreal distance(const Node &anode) const;
    qreal u();
    qreal v();
    qreal fi();

    QPointF deformed_pos(); ///< Position of node in deformed

    // Constrains
    enum Constrain constrain() const;
    void set_constrain(enum Constrain a_constrain);

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


//    void set_free(); // Free no degrees of freedom bounded
//    void set_vertical_trailer(); // Carrello verticale, horizontal displacement locked, free vertical displacement and rotation
//    void set_horizontal_trailer(); // Carrello orizzontale, vertical displacement locker, free horizontal displacement and rotation.
//    void set_hinge(); // Cerniera, displacements fixed, rotation free.
//    void set_vertical_show(); // Pattino verticale, free vertical displacement, fixed horizontal displacement and rotation
//    void set_horizontal_shoe(); // Pattino orizzontale, free horizontal displacement, fixed vertical displacement and rotation
//    void set_fixed_joint(); // Incastro, no free degrees

protected:
    // QVariant itemChange(GraphicsItemChange change, const QVariant &value);

   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    // the indexes of the degrees of freedom
    // they are -1 when the dof is bound.
    int dof_x, dof_y, dof_tetha;
    enum Constrain my_constrain;

    // Displacements
    qreal node_u, node_v, node_fi;
    QList<Beam *> beams_list;

signals:

public slots:

};

#endif // NODE_H


