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

class Node : public QGraphicsItem
{
public:
    Node(Canvas *a_canvas,double an_x, double an_y);
    // Create a new node on a_canvas

    // Coordinates
    qreal x,y;

    inline QPointF to_point() {return QPointF(x,y); };
    void add_beam (Beam *a_beam);
    QList<Beam *> beams() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    // QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
   // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Beam *> beams_list;
    Canvas *canvas;

signals:

public slots:

};

#endif // NODE_H


