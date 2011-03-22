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


#ifndef CANVAS_H
#define CANVAS_H

//#include <QGraphicsView>
#include <QtGui>
#include <commonmath.h>
#include <stdlib.h>

#include "node.h"
#include "beam.h"
#include "truss.h"
#include "trussdialog.h"

class Canvas : public QGraphicsView
        /// A representation of a structural model. Currently supporting only continous beams.
{
    Q_OBJECT
public:
    Canvas();
    /// Ask the user input data to make a continous beam.

    Truss *t; /// The truss currently being showd by this view.
    TrussDialog dialog; ///< The entry dialog to set spans, lengths, loads, material and section

    void zoom_to_fit();
    void zoom_in();
    void zoom_out();

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
   // void resizeEvent (QResizeEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent ( QMouseEvent * event );


signals:

public slots:
    void dialog_closed(int res); ///< The feature called when the main dialog is closed.

private:
    QPushButton *zoomin_button, *zoomout_button;
    QGraphicsAnchorLayout *zooms;
    QGraphicsProxyWidget *zoomin,*zoomout;
    //QPropertyAnimation animation;
};

#endif // CANVAS_H
