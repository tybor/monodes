/*
    Copyright (c) 2010 Paolo Redaelli

    This file is part of .

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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "node.h"
#include "beam.h"
#include "assert.h"

Node::Node(Canvas *a_canvas,double an_x, double an_y)
{
        assert(a_canvas!=NULL);
        canvas=a_canvas;
        x = an_x;
        y = an_y;
 }

    void Node::add_beam(Beam *a_beam) {
        assert(a_beam!=NULL);
        beams_list << a_beam;
    }

    QList<Beam *> Node::beams() const {
        return beams_list;
    }

    QRectF Node::boundingRect() const
    {
        qreal s=25;
        return QRectF(-s, -s, 2*s, 2*s);
    }

    QPainterPath Node::shape() const
    {
        QPainterPath path;
        path.addEllipse(-10, -10, 20, 20);
        return path;
    }

    void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
    {
        painter->setPen(QPen(Qt::black, 0));
        painter->drawEllipse(-10, -10, 20, 20);
    }
