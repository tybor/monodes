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

//#include <QFormLayout>
//#include <QGraphicsScene>
//#include <QGraphicsSceneMouseEvent>
//#include <QPainter>
//#include <QStyleOption>
//#include <QDialog>
//#include <QDoubleSpinBox>

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <math.h>

#include "beam.h"
#include "beamdialog.h"
#include "node.h"
#include "truss.h"

/* Hermite functions */
//C		PLOT
//      SUBROUTINE PLOT
//      INCLUDE 'common.h'

//      REAL*8 QS(6), QSL(6), FNE(6), QF(6)
//      REAL*8 NCODE(6), T(6,6)
//      CHARACTER FILENAME*255
//      LC = 0
// 1    LC = LC + 1
//C     COMPONGO IL NOME DEL FILE DA APRIRE COME LCASE.NUMERO_CONDIZIONE
//C     DI_CARICO
//      WRITE(FILENAME,'(A6,I3.3)') 'LCASE.', LC
//c      WRITE(*,*) 'Filename is:', FILENAME
//C     APRO IL FILE
//      OPEN(33,FILE=FILENAME ,STATUS='UNKNOWN')

//      WRITE(33,*) 1
//      WRITE(33,*) NNODI,NELE
//C     STAMPO LE COORDINATE DEI NODI
//      DO 3 I = 1, NNODI
//         WRITE(33,110) I,(COORD(I,J),J=1,2)
// 110     FORMAT(2X,I5,3(2X,E12.6))
// 3    CONTINUE
//C     STAMPO LA MATRICE DI CONNETTIVITA'
//      DO 5 I = 1, NELE
//         WRITE(33,100) I, (IN(I,J), J=1,2)
// 100     FORMAT(2X,10(I5,2X))
// 5    CONTINUE
//C     RIAVVOLGO IL FILE 9 (TEMPORANEO PER ST, T, FNE)
//      REWIND(9)
//C     CALCOLO SPOSTAMENTI E SOLECITAZIONI AGLI ESTREMI DELL'ELEMENTO
//      DO 90 NE = 1, NELE
//         N1 = IN(NE,1)
//         N2 = IN(NE,2)
//         DO 10 I = 1,3
//            NCODE(I)   = IGL(N1,I)
//            NCODE(I+3) = IGL(N2,I)
// 10      CONTINUE
//C     LETTURA MATRICE DI RIGIDEZZA DEL SINGOLO ELEMENTO NEL RIFERIMENTO
//C     GLOBALE
//         DO 21 I = 1,6
//            DO 20 J = 1,6
//               READ(9,*) ST(I,J)
// 20         CONTINUE
// 21      CONTINUE
//C     LETTURA MATRICE DI TRASFORMAZIONE
//         DO 31 I = 1,6
//            DO 30 J = 1,6
//               READ(9,*) T(I,J)
// 30         CONTINUE
// 31      CONTINUE
//C     LETTURA FORZE NODALI EQUIVALENTI NEL RIFERIMENTO LOCALE
//         DO 41 L = 1,NCOND
//            DO 40 I = 1,6
//               READ(9,*) FORCE
//               IF (L.EQ.LC) FNE(I) = FORCE
// 40         CONTINUE
// 41      CONTINUE
//C     SPOSTAMENTI DEL SINGOLO ELEMENTO NEL RIFERIMENTO GLOBALE
//         DO 50 I = 1, 6
//            GDL = NCODE(I)
//            IF (GDL.LT.0) THEN
//               QS(I) = 0.0
//            ELSE
//               QS(I) = VQ(GDL,LC)
//            ENDIF
// 50      CONTINUE
//C     SPOSTAMENTI DEL SINGOLO ELEMENTO NEL RIFERIMENTO LOCALE
//         DO 65 I = 1, 6
//            QSL(I) = 0.0
//            DO 60 J = 1, 6
//               QSL(I) = QSL(I) + T(J,I)*QS(J)
// 60         CONTINUE
// 65      CONTINUE
//C     SOLLECITAZIONI AGLI ESTREMI DELL'ELEMENTO NEL RIFERIMENTO LOCALE
//         DO 80 I= 1,6
//            QF(I) = -FNE(I)
//            DO 70 J = 1,6
//               QF(I) = QF(I) + ST(I,J)*QSL(J)
// 70         CONTINUE
// 80      CONTINUE
//C     SPOSTAMENTI E SOLLECITAZIONI LUNGO L'ASTA E FINE STAMPA FILE 33.
//         DX = COORD(N2,1) - COORD(N1,1)
//         DY = COORD(N2,2) - COORD(N1,2)
//         AL = DSQRT( DX**2.0 + DY**2.0 )
//         CA = DX / AL
//         SA = DY / AL
//         WRITE(33,1010) NE,AL,CA,SA
// 1010    FORMAT(2X,I5,3(2X,E12.6))
//         PX = CARD(NE,1,LC)
//         PY1 = CARD(NE,2,LC)
//         PY2 = CARD(NE,3,LC)
//         IS = ISEZ(NE)
//         IM = IMAT(NE)
//         AA = CSEZ(IS,1)
//         PESO = CMAT(IM,5)
//         PG = AA*PESO
//         PX = PX - PG*SA
//         PY1 = PY1 - PG*CA
//         PY2 = PY2 - PG*CA
//         DO 85 I = 0,20
//            X = AL*DBLE(I)/DBLE(20.0)
//            UU = U(QSL,X,AL)
//            VV = V(QSL,X,AL)
//            PY = PY1 + (PY2-PY1)*X/AL
//            SSN = -QF(1) - PX*X
//            SST = -QF(2) - (PY1+PY)*X/2.0
//            SSM = -QF(3) + QF(2)*X + PY1*X**2.0/2.0+(PY-PY1)*X**2.0/6.0
//            WRITE(33,120) X,UU,VV,SSN,-SST,-SSM
// 120        FORMAT(2X,6(E12.6,1X))
// 85      CONTINUE
// 90   CONTINUE
//      CLOSE(33)
//      IF (LC.LT.NCOND) GOTO 1
//      RETURN
//      END

// Hermite functions
qreal f1 (qreal csi) {
    assert((0.0<=csi) && (csi<=1.0));
    return 1.0 - csi;
}
qreal f2 (qreal csi) {
    assert((0.0<=csi) && (csi<=1.0));
    qreal csi2 = csi*csi;
    qreal csi3 = csi2*csi;
    return 1.0-3.0* csi2+2.0*csi3;
}
qreal f3 (qreal csi) {
    assert((0.0<=csi) && (csi<=1.0));
    qreal csi2 = csi*csi;
    qreal csi3 = csi2*csi;
    return csi - 2.0*csi2 + csi3;
}
qreal f4 (qreal csi) {
    assert((0.0<=csi) && (csi<=1.0));
    return csi;
}
qreal f5 (qreal csi) {
    assert((0.0<=csi) && (csi<=1.0));
    qreal csi2 = csi*csi;
    qreal csi3 = csi2*csi;
    return 3.0*csi2-2.0*csi3;
}
qreal f6 (qreal csi) {
    assert((0.0<=csi) && (csi<=1.0));
    qreal csi2 = csi*csi;
    qreal csi3 = csi2*csi;
    return csi3-csi2;
}


qreal Beam::u(qreal csi) {
    return f1(csi)*first().u()+
	    f3(csi)*first().u()*length()+
	    f4(csi)*second().u();
}
qreal Beam::v(qreal csi) {
    return
	    f2(csi)*first().v() + f3(csi)*first().fi()*length() +
	    f5(csi)*second().v() + f6(csi)*second().fi()*length();
}

Beam::Beam(Node *a_node, Node *another_node)
{
    assert(a_node!=NULL);
    assert(another_node!=NULL);
    assert(a_node!=another_node);
    // assert(*a_node != *another_node);

    setAcceptHoverEvents (true);

    first_node = a_node;
    second_node = another_node;
    beam_length = first().distance(second());
    first_node->add_beam(this);
    second_node->add_beam(this);
    load = 0.0;
#ifdef DEBUG
    std::cout<<"New beam "<<first()<<"--"<<second()<<" (length="<<length()<<")\n"<<std::flush;
#endif

}

// How difficoult to get read-only fields, that in Eiffel are for free!!! Symmetrically setters are always free and you can't get rid of them even when you would like to, so you must make it private, writing both the setter and the getter. So in conclusion Eiffel's approach - the default is fields writable only from inside its object and read-only from outside is what saves efforts.
Node &Beam::first() const { return *first_node; }
Node &Beam::second() const { return *second_node; }
qreal Beam::length() const { return beam_length;}

void Beam::set_section(Section &a_section) {
    s = &a_section;
    stiffness_computed=false; // so the next time stiffness() will be invoked it will be recomputed
}

Section &Beam::section() const {
    assert (s!=NULL);
    return *s;
}

void Beam::set_material (Material &a_material) {
    m=&a_material;
    stiffness_computed=false; // so the next time stiffness() will be invoked it will be recomputed
}

Material &Beam::material() const {
    assert (m!=NULL);
    return *m;
}

Matrix<qreal, 6, 6> &Beam::stiffness() {
    // Computes the stiffness matrix in local coordinates, exploiting modulus equality similarities of various terms.
    if (!stiffness_computed) compute_stiffness();
    return st;
}

Matrix<qreal, 6, 6> &Beam::local_stiffness() {
    if (!stiffness_computed) compute_stiffness();
    return local_st;
}

Matrix<qreal, 6, 6> &Beam::transformation() {
    if (!stiffness_computed) compute_stiffness();
    return tr;
}

Matrix<qreal, 6, 1> &Beam::nodal_forces() {
    if (!stiffness_computed) compute_stiffness();
    return gf;
}

Truss &Beam::truss() const {
    return static_cast<Truss&> (*parentItem());
};

void Beam::compute_stiffness() {
    // Geometric
    qreal dx = first().x() - second().y();
    qreal dy = first().y() - second().y();
    qreal l = length();
    qreal l2= l*l; // It may also be dx*dx + dy*dy;
    qreal l3 = l * l2;
    qreal ca = dx / l; // Cos(alpha)
    qreal sa = dy / l; // Sin(alpha)

    // Material
    qreal E=material().young_modulus();
    qreal nu = material().poisson_ratio();
    qreal alfa = material().thermal_expansion_coefficient();
    qreal G = E / (2.0*(1+nu));

    // Section
    qreal A=section().area();
    qreal J=section().moment_of_inertia();
    qreal h=section().height();
    qreal fi = section().shear_factor();

    // Axial deformability
    qreal axial = E*A/l;
    // Flexional and shear deformability
    qreal flex = 12.0* E* J / (l3 * (1+fi));
    qreal shear = (4+fi) * E * J / (l * (1+fi));
    qreal fl_sh = 6*E*J/(l2*(1+fi));
    qreal sh_fl = (2-fi)*E*J/(l*(1+fi));

    local_st <<
            axial,  0.0,    0.0,    -axial, 0.0,    0.0,
            0.0,    flex,   fl_sh,  0.0,    -flex,  fl_sh,
            0.0,    fl_sh,  shear,  0.0,    -fl_sh, sh_fl,
            -axial, 0.0,    0.0,    axial,  0.0,    0.0,
            0.0,    -flex,  -fl_sh, 0.0,    flex,   -fl_sh,
            0.0,    fl_sh,  sh_fl,  0.0,    -fl_sh, shear;

    assert(local_st==local_st.transpose());

    // Transforming stiffness matrix from local coordinates into global one
    tr <<
            ca, -sa, 0.0, 0.0, 0.0, 0.0,
            sa, ca, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, ca, -sa, 0.0,
            0.0, 0.0, 0.0, sa, ca, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 1.0;

    assert (tr==tr.transpose());
    st = (tr * local_st)*(tr.transpose());
    assert (st == st.transpose());

//    C     VETTORE FORZE NODALI EQUIVALENTI PER TUTTE LE CONDIZIONI DI CARICO.
//    C     INIZIALIZZAZIONE DEL CICLO REALIZZATO CON IF E GOTO
//          LC = 0
//    C     INCREMENTO DI CONTATORE DEL CICLO SULLE CONDIZIONI DI CARICO
//     90   LC = LC + 1
//          DO 100 I = 1, 6           ! Azzero il vettore dei carichi
//             FNE(I) = 0.0           ! in coord. locali
//     100  CONTINUE

    // Initializing nodal forces vector
    f = Matrix<qreal,6,1>(6,1);
    f.setZero(); /// Nodal loads initialization
    // Loads
    qreal px = 0.0; /// Constant axial distribuited load
    qreal py1 = load; /// Transversal linear load: value on first vertex
    qreal py2 = load; /// Transversal linear load: value on second vertex
    qreal utd = 0.0; /// Upper thermal delta
    qreal ltd = 0.0; /// Lower thermal delta

    qreal weight = section().area() * material().weight(); /// Self weight
    /// Updating loads with self weight
    px -= weight*sa;
    py1 -= weight*ca;
    py2 -= weight*ca;

    /// Nodal forces corresponding to distributed loads
    qreal fh = px * length()/2.0;
    qreal pp = py1, qq = py2 -py1;

    /// To compute shear deformation of distributed loads
    qreal aa = length() / (12.0 * E * J);
    qreal bb = section().shear_factor() / (length()* G * A);
    qreal cc = bb / (aa+bb);

    f(0) += fh; /// Horizontal force on first node
    f(1) += (0.5 * pp + 0.15*qq*(1.0+cc/9.0))*length(); /// Vertical force on first node
    f(2) += (pp/12.0 + qq*(1.0+cc/4.0)/30.0)*pow(length(),2); /// Moment on first node

    f(3) += fh; /// Horizontal force on second node
    f(4) += (0.5*pp + 0.35*qq*(1.0-cc/21.0))*length(); /// Vertical force on second node
    f(5) += - (pp/12.0 + qq*(1.0-cc/6.0)/20.0)*pow(length(),2); /// Moment on second node

    // Nodal forces corresponding  to thermal changes
    qreal atd = alfa * E * A * (utd+ltd)/2; // Average thermal delta
    qreal fm = alfa * E * J * (utd-ltd)/h;
    f(0) -= atd;
    f(2) += fm;
    f(3) += atd;
    f(5) -= fm;

    /// Computing nodal forces in absolute reference system
    gf = tr * f; // Local vector is overwritten.
//    C     TORNO INDIETRO PER ELABORARE LA PROSSIMA CONDIZIONE DI CARICO
//          IF (LC.LE.NCOND) GOTO 90
//          RETURN
//          END
    stiffness_computed = true;
}

QRectF Beam::boundingRect() const
{
    qreal line_width = section().height();
    qreal load_scale = truss().load_scale;
    QRectF result = QRectF(first_node->pos(),  second_node->pos());
    // Accounting the load and some space between the beam and the load.
    result.adjust(0.0, -load*load_scale-line_width,
                  0.0,                 +line_width);

    //result.adjust(-extra, -extra, extra, extra);
    return result.normalized();
}

void Beam::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    /// std::cout<<" painting beam"<<std::endl<<std::flush;
    Truss &t = truss();
    qreal line_width = section().height();
    // A beam is simply a line
    painter->setPen(QPen(Qt::black, line_width));
    painter->drawLine(first_node->pos(),second_node->pos());
    // Draw the load, assuming horizontal beams. TODO: remove this assumption.
    // Leave a little space (a line width) between load and beam
    painter->setViewTransformEnabled(false);
    QRectF load_rect(first().pos(),second().pos());
    load_rect.adjust(0.0, -load*t.load_scale-line_width,
                     0.0,                   -line_width );
    //std::cout<<"beam load "<<load_rect.x()<<","<<load_rect.y()<<" "<<load_rect.width()<<","<<load_rect.height()<<std::endl<<std::flush;
    painter->setPen(QPen(Qt::red));
    painter->setBrush(QBrush(QColor(255,96,96,128)));
    QString label("%1 kg/m");
    label = label.arg(load);
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

    // TODO: Draw axial stress, shear stress and moment

    // Draw deformated beam.
    painter->setPen(QPen(QColor(0, 0, 255, 128),section().height()));
    // It would be nice to draw it using splines, but it seems that it is not that easy. Let's draw it as always did, by points; so how many points shall we draw? Let's naively say currently 32.
    QPolygonF deformed;

    const int steps=32;
    qreal step=1.0/steps; // Beware that 1 is NOT 1.0. If you write it 1/steps, step will be exactly zero.
    const int ampl=1000.0; // TODO make it user defined, or even better computed for best view.
    qreal csi=0;
    QPointF step_vector((second().deformed_pos() - first().deformed_pos())/steps);
    QPointF current(first().deformed_pos());
    for (int i=0; i<=steps; ++i) {
        QPointF delta(u(csi)*ampl,v(csi)*ampl);
        deformed<< current+delta;
        csi +=step;
        current+=step_vector;
    }
    painter->drawPolyline(deformed);

#ifdef DEBUG
    // Draw boundingRect with a thin green line
    painter->setPen(QPen(Qt::green, 3, Qt::DotLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());
#endif
}

void Beam::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Present the user the precise location of the node which may be edited and a way to change its support conditions
#ifdef DEBUG
    std::cout<<"beam pressed at "
            <<event->pos().x()<<","<<event->pos().y()
            <<std::endl<<std::flush;
#endif
    QPointF pos = event->pos();
    setToolTip(QString("Beam pressed at (%1,%2)").arg(pos.x()).arg(pos.y()));
   // update();
    //QGraphicsItem::mousePressEvent(event);
}
void Beam::hoverMoveEvent ( QGraphicsSceneHoverEvent * event ) {
    QPointF pos = event->pos();
    // Find the neareast point of the beam
    QLineF me = QLineF(first().pos(),second().pos());
    QLineF normal = me.normalVector();
    normal.translate(pos - first().pos());
    QPointF nearest;
    QLineF::IntersectType intersect_type = normal.intersect(me, &nearest);
    assert(intersect_type != QLineF::NoIntersection);
    QString msg = QString("Beam at (%1,%2)").arg(nearest.x()).arg(nearest.y());
#ifdef DEBUG
    std::cout<<msg.toStdString()<<std::endl;
#endif
    setToolTip(msg);
}

void Beam::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
#ifdef DEBUG
    std::cout<<"rilasciata asta"<<std::endl<<std::flush;
#endif
    BeamDialog dialog(*this);
    int res = dialog.exec();
    // to need to redraw so no need to update();
    //QGraphicsItem::mouseReleaseEvent(event);
}


std::ostream &operator<<(std::ostream &s, Beam &a_beam) {
    s<<a_beam.first()<<"-"<<a_beam.second();
    return s;
}

