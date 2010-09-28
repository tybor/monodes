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

Reactions::Reactions(Node &parent) :
        node(parent)
{
    setParentItem(&parent);
}

void Reactions::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
// drawing a vertical arrow
painter->setPen(heavy);
painter->drawLine(0.0, bigger, 0.0, 4.0*bigger);
//painter->setPen(QPen(QColor(Qt::red)));
painter->drawLine(QPointF(0.0, bigger), QPointF(-bigger/2, 2*bigger));
painter->drawLine(QPointF(0.0, bigger), QPointF(bigger/2, 2*bigger));
// Draw reaction.void
painter->rotate(90);
painter->translate(bigger/2,bigger);
QRectF label_rect(QPointF(0.0, 0.0), QPointF(bigger, 3*bigger));

painter->setPen(Qt::green); painter->drawRect(label_rect);; /// debug
QString label = QString("N %1 kg").arg(node.vertical);
QFont font;
///* Pick the size that fits the load rectangle better */
text_rect = painter->boundingRect(label_rect,label); // The size we would occupy
qreal new_size = font.pointSizeF() * fmin(
        label_rect.width() / text_rect.width(),
        label_rect.height() / text_rect.height());
std::cout<<"Reaction label size:"<<new_size<<std::endl;
assert(new_size>0.0);
font.setPointSizeF(new_size);
painter->setFont(font);
painter->drawText(text_rect, label);
painter->setPen(Qt::red);
painter->drawRect(text_rect);
}

QRectF Reactions::boundingRect() const
{
    return text_rect.adjusted(0.0, 0.0, bigger, bigger/2); // text_rect;
}
