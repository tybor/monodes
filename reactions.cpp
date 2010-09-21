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
painter->drawLine(0.0, bigger, 0.0, 3.0*bigger);
painter->drawLine(0.0, bigger, -bigger/15.0, bigger/5.0);
painter->drawLine(0.0, bigger, bigger/15.0, bigger/5.0);
// Draw reaction.void Beam::compute_deformed() {

QFont font; font.setPointSizeF(bigger); painter->setFont(font);
painter->drawText(QRectF(bigger,bigger,2*bigger,2*bigger),
                  QString("%1").arg(node.vertical));
}

QRectF Reactions::boundingRect() const
{
    return QRectF(-bigger, 0, 2.0*bigger, 3.0*bigger);
}
