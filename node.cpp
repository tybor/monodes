/*
    Copyright (c) 20fraction_of_span Paolo Redaelli

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

#include <stdio.h>
#include <iostream>

#include <assert.h>

#include "node.h"
#include "nodedialog.h"
#include "beam.h"
#include "reactions.h"
#include "truss.h"

//static const QPoint horiz_trail[3] = {
//    QPoint(),
//    QPoint(-bigger, -bigger),
//    QPoint(bigger, -bigger)
//};
//static const QPoint vert_trail[3] = {
//    QPoint(),
//    QPoint(bigger, bigger),
//    QPoint(bigger, -bigger)
//};

/* end of constants used for drawings */

Node::Node (qreal an_x, qreal an_y, enum Constrain a_constrain)
{
    setPos(an_x, an_y);
    // Initialization
    node_u=0.0;
    node_v=0.0; // Only rotations!
    node_fi=0.0;
    dof_x = 0; dof_y=0; dof_tetha=0;
    my_constrain = a_constrain;
    vertical = 0.0;
    horizonal = 0.0;
    moment = 0.0;
#ifdef DEBUG
    std::cout<<"New node "<<*this<<std::endl<<std::flush;
#endif
}


void Node::add_beam(Beam *a_beam) {
    assert(a_beam!=NULL);
    assert((this==&(a_beam->first())) || (this ==&a_beam->second()));
    beams_list << a_beam;
}

QList<Beam *> Node::beams() const {
    return beams_list;
}

unsigned int Node::support_conditions_count() const {
    /// The number of locked degrees of freedom of current node.
    switch(my_constrain) {
    case uncostrained: return 0; break;
        // TODO:	case vertical_trailer: break;
        // TODO:    case horizontal_trailer: break;
    case hinge: return 2; break;
                // TODO: case vertical_shoe:break;
                // TODO: case horizontal_shoe:break;
    case restrained: return 3; break;
    default: return 0; break;
    }
}

qreal Node::distance (const Node &another) const {
    /// Distance between Current and another
    qreal dx = x()-another.x();
    qreal dy = y()-another.y();
    qreal dx2 = dx*dx; /* what a nice language! In C++ you can't write x^2 when x is a qreal! */
    qreal dy2 = dy*dy;
    return sqrt(dx2+dy2);
}

qreal Node::u() { return node_u; }
qreal Node::v() { return node_v; }
qreal Node::fi() { return node_fi; }

void Node::set_u(qreal anu) { node_u = anu; }
void Node::set_v(qreal av) { node_v = av; }
void Node::set_fi(qreal afi) { node_fi = afi; }

void Node::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    NodeDialog dialog(*this); // A dialog for current node
    //dialog.show();
    /* unused int res = */ dialog.exec();
    update();
    //QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
#ifdef DEBUG
    std::cout<<"rilasciato ("<<x()<<","<<y()<<")"
            <<std::endl<<std::flush;
#endif
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


QRectF Node::boundingRect() const
{
    QPainterPath path;
    static const qreal halfpen = smaller/8.0;
    qreal low = bigger+halfpen;
    qreal high = 2*low;
    return QRectF(-low,-low,high,high)|childrenBoundingRect();
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    static const qreal halfpen = smaller/8.0;
    qreal low = bigger+halfpen;
    qreal high = 2*low;
    path.addEllipse(-low,-low,high,high);
    return path;
}

QPointF Node::displacement() {
    return QPointF(node_u,node_v);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    switch (my_constrain) {
    case uncostrained: // nothing
        painter->setPen(light);
        painter->drawLine(QLineF(-smaller,0.0, smaller,0.0));
        painter->drawLine(QLineF(0.0,-smaller,0.0,smaller));
        break;
        // TODO:
        //	case vertical_trailer: // A triangle with a vertical line
        //            painter->setPen(light);
        //	    painter->drawConvexPolygon(vert_trail,3);
        //	    painter->drawLine(bigger+smaller,-bigger,bigger+smaller,bigger);
        //	    break;
        //	case horizontal_trailer: // A triangle with an horizontal line
        //            painter->setPen(light);
        //	    // Maybe a drawChord look better
        //	    painter->drawConvexPolygon(horiz_trail,3);
        //	    painter->drawLine(-bigger,-bigger-smaller,bigger,-bigger-smaller);
        //	    break;
    case hinge: // A circle with some radial lines
        painter->setPen(heavy);
        painter->drawLine(QLineF(0.0, 0.0, -bigger, smaller));
        painter->drawLine(QLineF(0.0, 0.0, -smaller, bigger));
        painter->drawLine(QLineF(0.0, 0.0, 0.0, bigger));
        painter->drawLine(QLineF(0.0, 0.0,  smaller, bigger));
        painter->drawLine(QLineF(0.0, 0.0, bigger, smaller));
        painter->setBrush(QBrush(Qt::white));
        painter->setPen(light);
        painter->drawEllipse(origin, smaller,smaller);
        break;
        // TODO: support them
        //	case vertical_shoe:
        //	    painter->setPen(heavy);
        //	    painter->drawLine(-smaller,bigger,-smaller,-bigger);
        //	    painter->drawLine(smaller,bigger,smaller,-bigger);
        //	    break;
        //	case horizontal_shoe:
        //	    painter->setPen(heavy);
        //	    painter->drawLine(-bigger,smaller,bigger,smaller);
        //	    painter->drawLine(-bigger,-smaller,bigger,-smaller);
        //	    break;
    case restrained: // Incastro, no free degrees
        painter->setPen(heavy);
        painter->drawLine(0.0, -bigger, 0.0, bigger);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::black,Qt::FDiagPattern));
        painter->drawRect(QRect(0, -bigger, bigger*2, bigger*2));
        break;
        // default: // nothing should NOT be necessary
    }
}

enum Constrain Node::constrain() const {
    return my_constrain;
}

void Node::set_constrain(enum Constrain a_constrain) {
    my_constrain = a_constrain;
    switch (a_constrain) {
    case uncostrained: break;
        // TODO:	case vertical_trailer: break;
        // TODO:    case horizontal_trailer: break;
    case hinge: dof_x = -1; dof_y = -1; break;
                // TODO: case vertical_shoe:break;
                // TODO: case horizontal_shoe:break;
    case restrained: dof_x = -1; dof_y = -1; dof_tetha = -1; break;
    }
}

void Node::update_reactions() {
    /// Add the reaction of each connected beam
    foreach (Beam *b, beams()) {
        int horiz_idx, vert_idx, mom_idx;
        if (this == &b->first()) {
            horiz_idx = 0;
            vert_idx = 1;
            mom_idx = 2;
        } else if (this == &b->second()) {
            horiz_idx = 3;
            vert_idx = 4;
            mom_idx = 5;
        };
        Matrix<qreal,6,1> gmef /*global_member_end_forces */=  b->transformation()*b->member_end_forces();
        horizonal += gmef [horiz_idx];
        vertical+= gmef [vert_idx];
        moment+= gmef [mom_idx];
    }
    /// TODO: remove eventual old reactions.
    Reactions *reactions=new Reactions(*this);
}

std::ostream &operator<<(std::ostream &s, Node &a_node) {
    s<<"("<<a_node.x()<<","<<a_node.y()
            <<" dofs x:"<<a_node.dof_x
            <<" y:"<<a_node.dof_y
            <<" tetha:"<<a_node.dof_tetha
            <<" u:"<<a_node.u()<<" v:"<<a_node.v()<<" fi:"<<a_node.fi();
    if (a_node.horizonal!=0.0) s<<" H: "<<a_node.horizonal;
    if (a_node.vertical!=0.0) s<<" V: "<<a_node.vertical;
    if (a_node.moment!=0.0) s<<" M: "<<a_node.moment;
    s<<") ";
    return s;
}

