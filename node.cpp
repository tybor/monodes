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

#include "node.h"
#include "nodedialog.h"
#include "beam.h"
#include "assert.h"

/* Constants used for drawings */

static const int smaller=3;
static const int bigger=6;

static const QPen lightest(Qt::gray, 0.75);
static const QPen light(Qt::black, 2);
static const QPen heavy(Qt::black,4);
static const QPoint origin;
static const QPoint horiz_trail[3] = {
    QPoint(),
    QPoint(-bigger, -bigger),
    QPoint(bigger, -bigger)
};
static const QPoint vert_trail[3] = {
    QPoint(),
    QPoint(bigger, bigger),
    QPoint(bigger, -bigger)
};

/* end of constants used for drawings */

Node::Node (qreal an_x, qreal an_y, enum Constrain a_constrain)
{
	setPos(an_x, an_y);
	// TODO: remove those!
	node_u=0.0;
	node_v=0.0;
	node_fi=0.3;
	my_constrain = a_constrain;
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
	assert(/*unimplemented*/ NULL);
    }

    qreal Node::distance (const Node &another) const {
	/// Distance between Current and another
	qreal dx = x()-another.x();
	qreal dy = y()-another.y();
	qreal dx2 = dx*dx;
	qreal dy2 = dy*dy;
	return sqrt(dx2+dy2);
	/* what a nice language C++ you can't write x^2 when x is a qreal! */
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
	return QRectF(-25, -25, 50, 50);
    }

    QPainterPath Node::shape() const
    {
        QPainterPath path;
    	switch (my_constrain) {
        case uncostrained:
	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
	    break;
            //TODO:
            //	case vertical_trailer:
            //	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
            //	    break;
            //	case horizontal_trailer:
            //	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
            //	    break;
	case hinge:
	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
	    break;
            //TODO
            //	case vertical_shoe:
            //	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
            //	    break;
            //	case horizontal_shoe:
            //	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
            //	    break;
	case restrained:
	    path.addEllipse(-bigger, -bigger, 2*bigger, 2*bigger);
	    break;
	}
        return path;
    }

    QPointF Node::deformed_pos() {
	return QPointF(x()+node_u,y()+node_v);
    }

    void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
    {
	switch (my_constrain) {
        case uncostrained: // nothing
	    painter->setPen(lightest);
	    painter->drawLine(-smaller,0, smaller,0);
	    painter->drawLine(0,-smaller,0,smaller);
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
	case hinge: // A circle with some diagonal lines
	    painter->setPen(light);
	    painter->drawLine(-smaller*3,0, smaller*3,0);
	    painter->setPen(Qt::NoPen);
	    painter->setBrush(QBrush(Qt::FDiagPattern));
	    painter->drawRect(QRect(-smaller*3,-bigger,6*smaller,bigger));
	    painter->setPen(light);
	    painter->setBrush(QBrush(Qt::white));
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
	    painter->drawLine(0,-2*bigger,0,2*bigger);
	    painter->setPen(Qt::NoPen);
	    painter->setBrush(QBrush(Qt::black,Qt::FDiagPattern));
	    painter->drawRect(QRect(0, -2*bigger, 2*bigger,4*bigger));
	    break;
	// default: should NOT be necessary painter->drawRoundedRect(QRect(-smaller,-smaller,2*smaller,2*smaller),smaller/2,smaller/2);
	}
    }

    enum Constrain Node::constrain() const {
	return my_constrain;
    }

    void Node::set_constrain(enum Constrain a_constrain) {
	my_constrain = a_constrain;
    }

