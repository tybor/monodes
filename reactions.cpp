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

#include <assert.h>
#include <iostream>

#include "reactions.h"
#include "node.h"
#include "nodedialog.h"
#include "truss.h"

Reactions::Reactions(Node &parent) :
        node(parent)
{
    setParentItem(&parent);
}

void Reactions::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    /// TODO: Currently drawing only the horizontal reaction
    // Drawing a little below the node
    painter->translate(0.0, node.boundingRect().bottom()*1.2);
    // Drawing a vertical arrow
    painter->setPen(heavy);
    Truss *truss = reinterpret_cast<Truss*>(node.parentItem()); /// This won't be necessary in Eiffel since parent_object would be redefined as TRUSS.
    assert (truss!=NULL);
    qreal l =node.vertical*truss->action_scale, w = l/8;
    std::cout<<"freccia "<<l<<"x"<<w<<" scala "<<truss->action_scale<<std::endl;
    painter->drawLine(0.0, 0.0, 0.0, l);
    //painter->setPen(QPen(QColor(Qt::red)));
    painter->drawLine(QPointF(), QPointF(-w, w));
    painter->drawLine(QPointF(), QPointF( w, w));
    // Please note that those lines should be drawed providing QPointF objects; if you pass their actual coordinates they get casted into integers and you won't get what you intend.

    // Draw reaction
    painter->rotate(90);
    painter->translate(bigger/2,bigger);
    painter->setPen(Qt::green);

    //QRectF label_rect(QPointF(0.0, 0.0), QPointF(bigger, 3*bigger));
    //painter->drawRect(label_rect);; /// debug
    QString label = QString("N %1 kg").arg(node.vertical);

    ///* Pick the size that fits the load rectangle better */
    //    QFont font = QFont("sans");
    //  text_rect = painter->boundingRect(QrectF(),label); // The size we would occupy
    //    qreal new_size = font.pointSizeF() * fmin(
    //            label_rect.width() / text_rect.width(),
    //            label_rect.height() / text_rect.height());
    //    std::cout<<"Reaction label size:"<<new_size<<std::endl;
    //    assert(new_size>0.0);
    //    font.setPointSizeF(new_size);
    //    painter->setFont(font);
    painter->drawText(text_rect, label);
    painter->setPen(Qt::red);
    painter->drawRect(text_rect);
}

QRectF Reactions::boundingRect() const
{
    qreal displacement = node.boundingRect().bottom()*1.2;
    return node.boundingRect().adjusted
            (-bigger/2,  displacement,
              bigger/2, displacement+4*bigger);
}

void Reactions::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    NodeDialog dialog(node); // A dialog for current node
    //dialog.show();
    /* unused int res = */ dialog.exec();
    update();
    //QGraphicsItem::mousePressEvent(event);
}
