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
#include "matrix.h"

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
//    QPushButton *zoomin_button = new QPushButton ("+");
//    QPushButton *zoomout_button = new QPushButton ("-");
//    QGraphicsProxyWidget *proxy2 = scene->addWidget (zoomout_button);

    setMinimumSize(150, 100);
    setWindowTitle(tr("Mobile nodes"));
    // Show a nice squared paper background
    // setBackgroundBrush(QBrush(QImage(":/images/paper4.jpg")));
    // Trasformation to view the model as the engineers expects, i.e. y growing upwards.
    setTransform(QTransform
                (1.0,  0.0, 0.0,
                 0.0, -1.0, 0.0,
                 0.0,  0.0, 1.0), false);

    material = new Material(210000.0,0.9,1.4e-6); // Steel
    section = new Section(8.0, 15.0);
//    left->set_material(*stee/l); left->set_section(*section);
//    right->set_material(*steel); right; right->set_section(*section);
    dialog.show();
}

void Canvas::dialog_closed(int res) {
    //std::cout<<"Dialog closed\n";
#   if defined(Q_WS_S60)
    showMaximized();
#   else
    show();
#   endif
    qreal x = 0.0;
    Node *left = new Node(x,0.0, hinge);
    add_node(*left);
    int spans=dialog.spans_count->value(); // Caching the number of spans
    for (int span=0; span<spans; ++span){
        QDoubleSpinBox *spinbox = dialog.lengths[span];
        x += spinbox->value();
        Node *right = new Node(x,0.0, hinge);
        add_node(*right);
        Beam *b = new Beam(left,right);
        add_beam(*b);
        left = right; // The left node of the next span is the right of current
    }
    zoom_to_fit();
}

void Canvas::add_node(Node &a_node) {
    scene()->addItem(&a_node);
    //std::cout<<"Adding node"<<a_node<<"\n"<<std::flush;
    nodes_list.append(&a_node);
}

void Canvas::add_beam(Beam &a_beam) {
    scene()->addItem(&a_beam);
    beams_list.append(&a_beam);
}

QList<Node*> Canvas::nodes() const {
    return nodes_list;
}

QList<Beam*> Canvas::beams() const {
    return beams_list;
}

void Canvas::zoom_to_fit() {
    assert(/*non empty canvas*/ !beams().empty());
    // Compute the right scale for bearings and loads.

    QList<Beam*>::iterator i=beams_list.begin();
    qreal shortest_beam = (**i).length();
    qreal longest_beam = shortest_beam;
    qreal highest_load = (**i).load;
    for (++i; i != beams_list.end(); ++i) {
        assert(/* Beam not NULL */ *i != NULL);
        Beam &b = **i;
        if (b.load>highest_load) highest_load = b.load;
        if (b.length()<shortest_beam) shortest_beam = b.length();
    }
    load_scale = highest_load;
    bearing_size = shortest_beam / 20.0;

    // Find the smallest and the biggest coordinate.
    qreal x1=0.0, x2=0.0, y1=0.0, y2=0.0;
    for (QList<Node*>::iterator i=nodes_list.begin(); i != nodes_list.end(); ++i) {
        Node &n=**i;
        //std::cout<<"("<<n.x() <<","<<n.y()<<"), struture in ("<<x1<<","<<y1<<") a ("<<x2<<","<<y2<<")\n";
        if (n.x()<x1) x1=n.x();
        if (n.x()>x2) x2=n.x();
        if (n.y()<y1) y1=n.y();
        if (n.y()>y2) y2=n.y();
    }

    // std::cout<<nodes_list.count()<<" node structure in ("<<x1<<","<<y1<<") a ("<<x2<<","<<y2<<") \n";
    qreal dx = x2-x1; qreal dy = y2-y1;
    qreal border = ( (dx>dy) ? dx : dy ) / 20;
   scene()->setSceneRect(x1-border,y1-border,dx+2*border,dy+2*border);
   QRectF r = scene()->sceneRect();
   fitInView(r,Qt::KeepAspectRatio);
    std::cout<<nodes_list.count()<<" node structure in ("<<r.x()<<","<<r.y()<<") + ("<<r.width()<<","<<r.height()<<std::endl<<std::flush;
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

unsigned int Canvas::support_conditions_count() const {
    assert(/*unimplemented*/ NULL);
    unsigned int i, size=nodes().size(), result=0;
    for (i=size; i>=0; i--)
        result+=nodes().at(i)->support_conditions_count();
    return result;
}

void Canvas::solve() {
    unsigned int dofs=nodes().size()*3 -support_conditions_count() ; // degrees of freedom
    unsigned int lcc=1; // load cases count; currently fixed at 1.

    // Degrees of freedom numbering
    unsigned int dof = 0;
    for (QList<Node*>::iterator i=nodes_list.begin(); i!=nodes_list.end(); ++i) {
        assert (/* non null node*/ *i != NULL);
        Node &n = **i;
        switch (n.constrain()) {
        case uncostrained:
            n.dof_x = ++dof;
            n.dof_y = ++dof;
            n.dof_tetha = ++dof;
            break;
            // TODO:	case vertical_trailer: break;
            // TODO:    case horizontal_trailer: break;
        case hinge:
            n.dof_tetha = ++dof;
            break;
            // TODO: case vertical_shoe:break;
            // TODO: case horizontal_shoe:break;
        case restrained:
            n.dof_x = -1;
            n.dof_y = -1;
            n.dof_tetha = -1;
            break;
        }
    }
    // TODO: this shall be a SymmetricSquareMatrix
    Matrix m(dofs,dofs);
    // Sum all the stiffness

}
