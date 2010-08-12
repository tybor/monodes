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


#include "sectiondialog.h"
#include <assert.h>

SectionDialog::SectionDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    // Create  the scene holding the preview of the section
    QGraphicsScene *scene =  new QGraphicsScene(this);
    sectionPreview->setScene(scene);
    redraw();

    // Update the preview when values change.
    connect(widthSpin,SIGNAL(valueChanged(double)),this,SLOT(redraw()));
    connect(heightSpin,SIGNAL(valueChanged(double)),this,SLOT(redraw()));
}

void SectionDialog::redraw() {
    QGraphicsScene *scene = sectionPreview->scene();
    assert(scene!=NULL);
    scene->clear();
    qreal w = widthSpin->value();
    qreal h = heightSpin->value();
    scene->addRect(0,0,w,h,QPen(Qt::black,1), QBrush(Qt::red,Qt::BDiagPattern));
}

void SectionDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
