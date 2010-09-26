#include "load.h"
#include "beam.h"
#include "truss.h"

Load::Load(Beam &a_parent_beam, qreal a_constant_load) :
        beam(a_parent_beam)
{
    setParentItem(&a_parent_beam);
}

QRectF Load::boundingRect() const {
    return QRectF(0,0, beam.length(), amount);
}

void Load::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //    Draw the load, assuming horizontal beams. TODO: remove this assumption; if
    //    we always draw the beam from (0,0) to (length,0) and then rototranslate it
    //    to its actual position we can continue draw this horizontally
    // Leave a little space (a line width) between load and beam
    // painter->setViewTransformEnabled(false);
    load_rect = QRectF (0, 0, beam.length(), amount);
    load_rect.adjust(0.0, -amount*beam.truss.load_scale, 0.0, 0.0);

    painter->setTransform(beam.transform());
    painter->setPen(QPen(Qt::red));
    painter->setBrush(QBrush(QColor(255,96,96,128)));
    QString label = QString("%1 kg/m").arg(amount);
    QFont font;
    /* Pick the size that fits the load rectangle better */
    QRectF text_rect(painter->boundingRect(load_rect,label)); // The size we would occupy
    font.setPointSizeF( font.pointSizeF() * fmin(
            load_rect.width() / text_rect.width(),
            load_rect.height() / text_rect.height()
            ));
    painter->setFont(font);
    painter->drawRect(load_rect);
    painter->drawText(load_rect, Qt::AlignCenter, label);
}
