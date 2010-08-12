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
#include <stdio.h>

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

    // Add zoom buttons
    // the following is clumsy; "-" overrides "+" button.

//    QPushButton *zoomin_button = new QPushButton ("+");
//    QPushButton *zoomout_button = new QPushButton ("-");
//    QGraphicsProxyWidget *proxy2 = scene->addWidget (zoomout_button);

    setMinimumSize(150, 100);
    setWindowTitle(tr("Mobile nodes"));
    // Show a nice squared paper background
    setBackgroundBrush(QBrush(QImage(":/images/paper4.jpg")));
    // Trasformation to view the model as the engineers expects, i.e. y growing upwards.
    setTransform(QTransform
                (1.0,  0.0, 0.0,
                 0.0, -1.0, 0.0,
                 0.0,  0.0, 1.0), false);

//    // Makes a continous beam with 1 extra_bearing, in addition to the left and the right. Left and right are hinges. The beam is rectangular and made of concrete. A uniform load is added to the right span.
//    // a--b--c
//    // ^  ^  ^
//    // Left bearing
//    Node *a = new Node(0.0, 300.0,uncostrained); add_node(*a);
//    // Middle bearing
//    Node *b = new Node(300.0,0.0,horizontal_trailer); add_node(*b);
//    // Right bearing
//    Node *c = new Node(800.0,0.0, hinge); add_node(*c);

//    a->setToolTip("A");
//    b->setToolTip("B");
//    c->setToolTip("C");

//    // Adding load on bc, the right span.
//    // Testing rendering of nodes
//    add_node(*(new Node(0.0,-100.0, uncostrained)));
//    add_node(*(new Node(50.0,-100.0, vertical_trailer)));
//    add_node(*(new Node(100.0,-100.0, horizontal_trailer)));
//    add_node(*(new Node(150.0,-100.0, hinge)));
//    add_node(*(new Node(200.0,-100.0, vertical_shoe)));
//    add_node(*(new Node(250.0,-100.0, horizontal_shoe)));
//    add_node(*(new Node(300.0,-100.0, restrained)));

//    //QGraphicsSvgItem foo("")
//    Beam *left = new Beam(a,b); add_beam(*left);
//    Beam *right = new Beam(b,c); add_beam(*right);

    material = new Material(210000.0,0.9,1.4e-6); // Steel
    section = new Section(8.0, 15.0);
//    left->set_material(*stee/l); left->set_section(*section);
//    right->set_material(*steel); right; right->set_section(*section);
    dialog.show();

    zoom_to_fit(); // instead of  scene->setSceneRect(-50, -50, 150, 50);
}

void Canvas::dialog_closed(int res) {
    std::cout<<"Dialog closed\n";
    qreal x = 0.0;
    Node *left = new Node(x,0.0, dialog.left_constrain());
    add_node(*left);
    for (int span=1, int spans=dialog.spans_count->value(); span<=spans; ++span){
        x += dialog.lengths[span];
        Node *middle(x,0.0, hinge);
        add_node(*middle);
    }
}

void Canvas::add_node(Node &a_node) {
    scene()->addItem(&a_node);
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
    qreal x1=0.0,x2=0.0,y1=0.0,y2=0.0;
    QList<Node*>::iterator i;
    // Find the smallest and biggest coordinate.
    for (i=nodes_list.begin(); i!=nodes_list.end(); ++i) {
        Node &n=**i;
        if (n.x()<x1) x1=n.x();
        if (n.x()>x2) x2=n.x();
        if (n.y()<y1) y1=n.y();
        if (n.y()>y2) y2=n.y();
    }
    qreal dx = x2-x1; qreal dy = y2-y1;
    qreal border = ( (dx>dy) ? dx : dy ) / 20;
    scene()->setSceneRect(x1-border,y1-border,dx+2*border,dy+2*border);
}
void Canvas::zoom_in(){

}
void Canvas::zoom_out(){

}

unsigned int Canvas::support_conditions_count() const {
    assert(/*not implemented*/ NULL);
    unsigned int i, size=nodes().size(), result=0;
    for (i=size; i>=0; i--) result+=nodes().at(i)->support_conditions_count();
    return result;
}

void Canvas::solve() {
    unsigned int dofs=nodes().size()*3 -support_conditions_count() ; // degrees of freedom
    unsigned int lcc=1; // load cases count

    // TODO: this shall be a SymmetricSquareMatrix
    Matrix m(dofs,dofs);
    // Sum all the stiffness
}
