#include "load.h"
#include "beam.h"
#include "truss.h"
#include <math.h>

Load::Load(Beam &a_parent_beam, qreal a_constant_load) :
        beam(a_parent_beam)
{
    setParentItem(&a_parent_beam);
    amount = a_constant_load;
}

QRectF Load::boundingRect() const {
    return QRectF(0.0,0.0, beam.length(), amount);
}

void Load::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    //    Draw the load, assuming horizontal beams. TODO: remove this assumption; if
    //    we always draw the beam from (0,0) to (length,0) and then rototranslate it
    //    to its actual position we can continue draw this horizontally

    /// Todo: remove the current assumption that everything is a deadload
    qreal actual_height = fabs(amount) * beam.truss.load_scale;
    load_rect = QRectF (0.0, -actual_height, beam.length(), actual_height);
    painter->translate(0.0, -beam.section.height()*2); // Leave a little gap between load and beam
    painter->setPen(QPen(Qt::red));
    painter->setBrush(QBrush(QColor(255,96,96,128))); // Half-transparent orange
    QString label = QString("%1 kg/m").arg(fabs(amount));
    /* Pick the font size that fits the load rectangle better */
    QFont font;
    QRectF text_rect = painter->boundingRect(load_rect,label); // The size the label would occupy in the load rectangle.
    qreal new_size = font.pointSizeF() * fmin(
            load_rect.width() / text_rect.width(),
            load_rect.height() / text_rect.height()
            );
    assert (new_size>0.0);
    font.setPointSizeF(new_size);
    painter->setFont(font);
    painter->drawRect(load_rect);
    painter->drawText(load_rect, Qt::AlignCenter, label);
}
