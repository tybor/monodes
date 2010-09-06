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

#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDialog>
#include <QRadioButton>
#include <QPushButton>
#include <QGraphicsLinearLayout>
#include <QDoubleSpinBox>

#include <stdio.h>
#include <iostream>

#include <assert.h>

#include "node.h"
#include "nodedialog.h"
#include "beam.h"
#include "truss.h"


/* Constants used for drawings */
static const qreal fraction_of_span = 12.0; ///< the node will be big as this fraction of its spans.
static const qreal smaller=0.5;
static const qreal bigger=1.0;
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
    // TODO: remove those!
    node_u=0.0;
    node_v=0.0; // Only rotations!
    node_fi=0.5;
    my_constrain = a_constrain;
    std::cout<<"New node "<<*this<<std::endl<<std::flush;
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
    case hinge: return 1; break;
                // TODO: case vertical_shoe:break;
                // TODO: case horizontal_shoe:break;
    case restrained: return 3; break;
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

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    NodeDialog dialog(*this); // A dialog for current node
    //dialog.show();
    int res = dialog.exec();
    update();
    //QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout<<"rilasciato ("<<x()<<","<<y()<<")"
            <<std::endl<<std::flush;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


QRectF Node::boundingRect() const
{
    // bouding rectangle - the region where the item is paing - is made a little bigger than it may be for simplicy sake. I'm not sure that a proper switch and a more precise bounding may be useful, perhpas the computational burden is a waste of efforts.
    return QRectF(-bigger,bigger,2*bigger,2*bigger);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
    return path;
}

QPointF Node::deformed_pos() {
    return QPointF(x()+node_u,y()+node_v);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    const QPen lightest(Qt::gray, smaller/12.0);
    const QPen light(Qt::black, smaller/10.0);
    const QPen heavy(Qt::black, smaller/8.0);
    static const QPointF origin;

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
        painter->drawLine(QLineF(0.0, 0.0, -bigger, -smaller));
        painter->drawLine(QLineF(0.0, 0.0, -smaller, -bigger));
        painter->drawLine(QLineF(0.0, 0.0, 0.0, -bigger));
        painter->drawLine(QLineF(0.0, 0.0,  smaller, -bigger));
        painter->drawLine(QLineF(0.0, 0.0, bigger, -smaller));
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

std::ostream &operator<<(std::ostream &s, Node &a_node) {
    s<<"("<<a_node.x()<<","<<a_node.y()<<")";
    return s;
}

