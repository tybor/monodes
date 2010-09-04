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


#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include "node.h"
#include "beam.h"
#include "trussdialog.h"

class Canvas : public QGraphicsView
        /// A representation of a structural model. Currently supporting only continous beams.
{
    Q_OBJECT
public:
    Canvas();
    /// Ask the user input data to make a continous beam.

    void solve();

    QList<Node*> nodes() const;
    QList<Beam*> beams() const;

    void zoom_to_fit();
    void zoom_in();
    void zoom_out();
    qreal bearing_size; /// Drawing size of bearing
    qreal load_scale; /// The scale applied to loads

    void add_node (Node &a_node);
    void add_beam (Beam &a_beam);
    unsigned int support_conditions_count() const; // The number of support conditions of all nodes

    Material *material; ///< The one material currently supported
    Section *section; ///< The one section currently supported
    TrussDialog dialog; ///< The entry dialog to set spans, lengths, loads, material and section

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    QList<Node *> nodes_list;
    QList<Beam *> beams_list;

signals:

public slots:
    void dialog_closed(int res); ///< The feature called when the main dialog is closed.

};

#endif // CANVAS_H
