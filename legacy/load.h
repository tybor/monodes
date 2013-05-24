#ifndef LOAD_H
#define LOAD_H

#include <QGraphicsItem>
class Beam; // Forward declaration

class Load : public QGraphicsItem
        /// A load applied on a beam.

        /// Currently only constant loads are supported.
{
public:
    Load(Beam &a_parent_beam, qreal constant_load=0.0);

    qreal constant() const {return amount;};
    void set (qreal an_amount) {amount = an_amount;};

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    Beam &beam; /// The beam the load is referring to.
    qreal amount;
    QRectF load_rect;
};

#endif // LOAD_H
