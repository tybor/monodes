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
#include "truss.h"

#include "trussdialog.h"
#include "materialdialog.h"
#include "material.h"
#include "sectiondialog.h"
#include "section.h"

#include "QTransform"
#include <QDialog>
#include <QPushButton>
#include <QFormLayout>
#include <QDoubleSpinBox>

#include <assert.h>
#include <stdio.h>
#include <iostream>

Canvas::Canvas() {
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    // when the main dialog is closed update the canvas
    connect(&dialog, SIGNAL(finished(int)), this, SLOT(dialog_closed(int)));

    // Add zoom buttons; the following is clumsy; "-" overrides "+" button.
//    zooms = new QGraphicsAnchorLayout();
//    zoomin_button = new QPushButton("+");
//    zoomout_button = new QPushButton ("-");
//    zoomin = scene->addWidget (zoomin_button);
//    //zoomout = scene->addWidget (zoomout_button);

    //setMinimumSize(150, 100);
    setWindowTitle(tr("Mobile nodes"));
    // Prepare a nice squared paper background
    setBackgroundBrush(QBrush(QImage(":/images/paper4.jpg")));
    // Trasformation to view the model as the engineers expects, i.e. y growing upwards.
//    setTransform(QTransform
//                (1.0,  0.0, 0.0,
//                 0.0, -1.0, 0.0,
//                 0.0,  0.0, 1.0), false);

    dialog.show();
}

void Canvas::dialog_closed(int res) {
    //std::cout<<"Dialog closed\n";
#   if defined(Q_WS_S60)
    showMaximized();
#   else
    show();
#   endif
    t = new Truss();
    scene()->addItem(t);

    Material *material = new Material(
            dialog.material_dialog.elasticModulus->value(),
            dialog.material_dialog.poissonRatio->value(),
            dialog.material_dialog.thermalExpansion->value());
    Section *section = new Section(dialog.section_dialog.widthSpin->value(),dialog.section_dialog.heightSpin->value());
    t->material = material;
    t->section = section;

    qreal x = 0.0;
    Node *left = new Node(x,0.0, hinge);
    t->add_node(*left);
    int spans=dialog.spans_count->value(); // Caching the number of spans
    for (int span=0; span<spans; ++span){
        QDoubleSpinBox *spinbox = dialog.lengths[span];
        x += spinbox->value();
        Node *right = new Node(x,0.0, hinge);
        t->add_node(*right);
        Beam *b = new Beam(left,right);
        b->set_section(*section);
        b->set_material(*material);
        b->load = dialog.loads[span]->value();
        t->add_beam(*b);
        left = right; // The left node of the next span is the right of current
    }
    zoom_to_fit();
    t->solve();
}

void Canvas::zoom_to_fit() {
    qreal length=0.0, highest_beam =0.0, highest_load=0.0;
    foreach (Beam *a_beam, t->beams()) {
        length = a_beam->length();
        if (length>t->longest_beam) t->longest_beam = length;
        qreal h=a_beam->section().height();
        if (h>highest_beam) highest_beam=h;
        qreal load_module = fabs(a_beam->load);
        if (load_module>highest_load) highest_load=load_module;
    }
    t->load_scale=t->longest_beam/highest_load/2.0; /// The highest load will be high the half of the longest beam.
    qreal s = fmax( /* the radius of the nodes will be the biggest of */
            2.0*highest_beam /* two times the highest beam */,
            t->longest_beam/20.0 /* 1/20 of longest beam. */ );
    foreach (Node *n, t->nodes()) {
        std::cout<<*n<<" scale "<<n->scale()<<" ";
        n->setScale(s);
    }

   fitInView(t,Qt::KeepAspectRatio);
   // Move "+" and "-" buttons
   // zooms->addCornerAnchors(&t,Qt::TopLeftCorner, zoomin,Qt::BottomRightCorner);

   // Set a proper size for the squared paper background
   //backgroundBrush().setMatrix (QMatrix(10.0, 2.0, 2.0, 10.0, 0.0,0.0));
           // QMatrix(t->longest_beam, 0.0,0.0,t->longest_beam,0.0,0.0));
}

void Canvas::zoom_in(){
    // Showing the central third (1/9 area)
    QGraphicsScene *s =  scene();
    QRectF r = s->sceneRect();
    qreal dx = r.width()/3;
    qreal dy = r.height()/3;
    s->setSceneRect(r.adjusted(dx,dy,-dx,-dy));
}

void Canvas::zoom_out(){
    // Old view will be new the central third (1/9 of area)
    QGraphicsScene *s =  scene();
    QRectF r = s->sceneRect();
    qreal dx = r.width();
    qreal dy = r.height();
    s->setSceneRect(r.adjusted(-dx,-dy,dx,dy));
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
//    case Qt::Key_Up:
//        centerNode->moveBy(0, -20);
//        break;
//    case Qt::Key_Down:
//        centerNode->moveBy(0, 20);
//        break;
//    case Qt::Key_Left:
//        centerNode->moveBy(-20, 0);
//        break;
//    case Qt::Key_Right:
//        centerNode->moveBy(20, 0);
//        break;
    case Qt::Key_Plus:
        scale(1.2,1.2);
        break;
    case Qt::Key_Minus:
        //qreal out(0.8);
        scale(0.8,0.8);
        break;
//    case Qt::Key_Space:
//    case Qt::Key_Enter:
//        foreach (QGraphicsItem *item, scene()->items()) {
//            if (qgraphicsitem_cast<Node *>(item))
//                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
//        }
//        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    qreal s = pow((double)2, -event->delta() / 240.0);
    scale(s,s);
}

