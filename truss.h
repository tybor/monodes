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
#include "node.h"
#include "beam.h"

class Truss : public QGraphicsItem
        // TODO: inherit from "public QGraphicsLayoutItem" to allow dynamic placing of widgets like "+" and "-"
{
public:
    Truss();
    QList<Node*> nodes() const;
    QList<Beam*> beams() const;

    void solve();

    void add_node (Node &a_node);
    void add_beam (Beam &a_beam);
    unsigned int support_conditions_count() const; // The number of support conditions of all nodes
    qreal longest_beam; /// The lenght of the longest beam, used to determinate the drawing size of bearings
    qreal highest_load; /// The maximum distributed load
    qreal load_scale; /// The scale applied to loads
    qreal strongest_beam; // The highest moment of inertia of a beam.

    Material *material; ///< The one material currently supported
    Section *section; ///< The one section currently supported

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QList<Node *> nodes_list;
    QList<Beam *> beams_list;

};

#endif // TRUSS_H
