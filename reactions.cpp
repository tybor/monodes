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


#include "reactions.h"
#include "node.h"

Reactions::Reactions(Node &parent) :
        node(parent)
{
    setParentItem(&parent);
}

void Reactions::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
// drawing a vertical arrow
painter->setPen(heavy);
painter->drawLine(0.0, bigger, 0.0, 4.0*bigger);
//painter->setPen(QPen(QColor(Qt::red)));
painter->drawLine(QPointF(0.0, bigger), QPointF(-bigger/2, 2*bigger));
painter->drawLine(QPointF(0.0, bigger), QPointF(bigger/2, 2*bigger));
// Draw reaction.void Beam::compute_deformed() {
QString label = QString("%1").arg(node.vertical);
QFont font;
/* Pick the size that fits the load rectangle better */
QRectF text_rect(painter->boundingRect(load_rect,label)); // The size we would occupy
font.setPointSizeF( font.pointSizeF() * fmin(
        load_rect.width() / text_rect.width(),
        load_rect.height() / text_rect.height()
        ));
painter->setFont(font);
QFont font; font.setPointSizeF(bigger); painter->setFont(font);
painter->drawText(QRectF(bigger,bigger,2*bigger,2*bigger),
                  QString("%1").arg(node.vertical));
}

QRectF Reactions::boundingRect() const
{
    return QRectF(-bigger, 0, 2.0*bigger, 3.0*bigger);
}
