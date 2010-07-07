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


#include "canvas.h"
#include "node.h"
#include "beam.h"

Canvas::Canvas()
{
    /* Drawing this structure for now
     *
     *  D--E--F--G  30
     *  |  |  |
     *  A  B  C     0
     *  0 50 100 120
     */
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-50, -50, 150, 50);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    Node *a = new Node(this,   0.0,  0.0); scene->addItem(a);
    Node *b = new Node(this,  50.0,  0.0); scene->addItem(b);
    Node *c = new Node(this, 100.0,  0.0); scene->addItem(c);
    Node *d = new Node(this,   0.0, 30.0); scene->addItem(d);
    Node *e = new Node(this,  50.0, 30.0); scene->addItem(e);
    Node *f = new Node(this, 100.0, 30.0); scene->addItem(f);
    Node *g = new Node(this, 120.0, 30.0); scene->addItem(g);

    scene->addItem(new Beam(a,d));
    scene->addItem(new Beam(d,e));
    scene->addItem(new Beam(e,f));
    scene->addItem(new Beam(f,g));
    scene->addItem(new Beam(b,e));
    scene->addItem(new Beam(c,f));

    //scale(qreal(0.8), qreal(0.8));
    setMinimumSize(150, 100);
    setWindowTitle(tr("Mobile nodes"));
}
