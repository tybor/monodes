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

#include <QPropertyAnimation> // To make the zooming animations
 #include <QEasingCurve>

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include "flickcharm.h"

Canvas::Canvas()
    // No animation : animation(this, "sceneRect") // the shown rectangle of this canvas will be animated
{
    // FlickCharm provides kinetic scrolling
    FlickCharm charm;
    charm.activateOn (this);
    // See http://www.slideshare.net/qtbynokia/special-effects-with-qt-graphics-view
    // and http://labs.qt.nokia.com/2009/07/19/kinetic-scrolling-on-any-widgets/

    setScene(new QGraphicsScene(this));
    scene()->setItemIndexMethod(QGraphicsScene::NoIndex);

    setDragMode(ScrollHandDrag); // Enabling scrolling
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    // We will animate the rectangle shown by thus canvas
    //    animation.setDuration(3000);
    //    animation.setEasingCurve(QEasingCurve::OutBack);
    // when the main dialog is closed update the canvas
    connect(&dialog, SIGNAL(finished(int)), this, SLOT(dialog_closed(int)));

    // Add zoom buttons; the following is clumsy; "-" overrides "+" button.
    //zooms = new QGraphicsAnchorLayout();
     QGraphicsWidget *zoomin_button = scene()->addWidget(new QPushButton("+"));
     QGraphicsWidget *zoomout_button = scene()->addWidget(new QPushButton ("-"));

     QGraphicsGridLayout *layout = new QGraphicsGridLayout;
     layout->addItem(zoomin_button, 1, 0, Qt::AlignBottom);
     layout->addItem(zoomout_button, 1, 1, Qt::AlignBottom);

     QGraphicsWidget *form = new QGraphicsWidget;
     form->setLayout(layout);
     scene()->addItem(form);

    setMinimumSize(300, 300);
    setWindowTitle(tr("Mobile nodes"));
    // Prepare a nice squared paper background
    // setBackgroundBrush(QBrush(QImage(":/images/paper4.jpg")));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // We want to show informations about the currently pointed object. So we need to switch on mouse traking in this view and in its viewport.
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    // Trasformation to view the model as the engineers expects, i.e. y growing upwards.
//    setTransform(QTransform
//                (1.0,  0.0, 0.0,
//                 0.0, -1.0, 0.0,
    //                 0.0,  0.0, 1.0), false);

    dialog.show();
}

void Canvas::dialog_closed(int res) {
    if (res) {
        //std::cout<<"Dialog closed\n";
        // Remove eventual old items from the scene
        foreach (QGraphicsItem *item, scene()->items()) scene()->removeItem(item);
        // TODO: this may be ineffeficient, perhaps its better to throw away the old scene and get a new one
        t = new Truss();
        scene()->addItem(t);
#   if defined(Q_WS_S60)
        showMaximized();
#   else
        show();
#   endif
        Material *material = new Material(
                dialog.material_dialog.elasticModulus->value(),
                dialog.material_dialog.poissonRatio->value(),
                dialog.material_dialog.thermalExpansion->value());
        Section *section = new Section(dialog.section_dialog.widthSpin->value(),dialog.section_dialog.heightSpin->value());
        t->material = material;
        t->section = section;

        t->set_immediate_solving(false);
        // To avoid re-solving the truess at each and every change. If immediate solving is not turned off during truss building this feature would have O(n^4) complexity with n the degress of freedom. In fact we will solve n/6 linear systems each one an O(m^3) problem with m growing from 1 to n, so the complexity is O( 1/6 * (n^4/4 + n^3/2 + n^2/4)) = O(n^4) . Keeping such a bad performance to get results that will be thrown away as soon as they get computed is a plain waste of resources.

        // See http://www.math.com/tables/expansion/power.htm and http://en.wikipedia.org/wiki/Geometric_series for power series.
        qreal x = 0.0;
        Node *left = new Node(x,0.0, hinge);
        t->add_node(*left);
        int spans=dialog.spans_count->value(); // Caching the number of spans
        for (int span=0; span<spans; ++span){
            QDoubleSpinBox *spinbox = dialog.lengths[span];
            x += spinbox->value();
            Node *right = new Node(x,0.0, hinge);
            t->add_node(*right);
            Beam *b = new Beam(*left,*right,*t,*section,*material);
            b->set_load(-dialog.loads[span]->value());
            left = right; // The left node of the next span is the right of current
        }
        zoom_to_fit();
        t->solve();
#ifdef DEBUG
        std::cout<<"DOFs: "<<t->dofs_count()<<" nodes "<<t->nodes().count()<<std::endl;
#endif
        assert(/* the solutions has the right number of degrees of freedom*/
                int(t->dofs_count()) == t->nodes().count());
        t->set_immediate_solving(true);
    }
}

void Canvas::zoom_to_fit() {
   fitInView(t,Qt::KeepAspectRatio);
   // Move "+" and "-" buttons
   // zooms->addCornerAnchors(&t,Qt::TopLeftCorner, zoomin,Qt::BottomRightCorner);

   // Set a proper size for the squared paper background
   // backgroundBrush().setMatrix (QMatrix(t->longest_beam, 0.0,0.0,t->longest_beam,0.0,0.0));
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
    std::cerr<<"Got key "<<event->key()<<std::endl;
    switch (event->key()) {
    case Qt::Key_Up:
        translate(0,-20);
        break;
    case Qt::Key_Down:
        translate(0,20);
        break;
    case Qt::Key_Left:
        translate(-20,0);
        break;
    case Qt::Key_Right:
        translate(20,0);
        break;
    case Qt::Key_ZoomIn:
    case Qt::Key_VolumeUp:
    case Qt::Key_Plus:
        scale(1.2,1.2);
        break;
    case Qt::Key_ZoomOut:
    case Qt::Key_VolumeDown:
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
    qreal s = pow(2.0, -event->delta() / 240.0);
    scale(s,s);
}

//void Canvas::resizeEvent (QResizeEvent *) {
//    if (t)
//        if (! (t->beams().empty()))
//            fitInView(t,Qt::KeepAspectRatio);

//}

//void Canvas::mouseReleaseEvent(QMouseEvent *event) {
void Canvas::    mousePressEvent(QMouseEvent *event) {
    QGraphicsItem *clicked = itemAt(event->pos());
    QGraphicsItem *focused ;
    if (clicked) focused = clicked;
    else /* no object at click point ; let's focus the entire truss */
        focused = t;

    // The animation from current rectangle to those of the clicked element is currently way too slow. Perhaps beam.paint is suboptimal.
    // Commenting it out and sticking to
    fitInView(focused, Qt::KeepAspectRatio);
    //animation.setStartValue(sceneRect()); // Start animation from current rectangle show
    // The animation will end showing focused in a way similar
    // to a call to fitInView(focused, Qt::KeepAspectRatio);
    //    animation.setEndValue(focused->mapRectToScene(focused->boundingRect()));
    //    animation.start();
}

void Canvas::mouseDoubleClickEvent ( QMouseEvent * event ) {
    hide();
    dialog.show();
    show();
}
