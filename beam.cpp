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

#include "beam.h"
#include "beamdialog.h"
#include "node.h"
#include "assert.h"

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

    first_node = a_node;
    second_node = another_node;
    beam_length = first().distance(second());
    first_node->add_beam(this);
    second_node->add_beam(this);
    std::cout<<"New beam "<<first()<<"--"<<second()<<" (length="<<length()<<")\n"<<std::flush;
    load = 10.0;
}

// How difficoult to get read-only fields, that in Eiffel are for free!!!
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

Matrix &Beam::stiffness() {
    // Computes the stiffness matrix in local coordinates, exploiting modulus equality similarities of various terms.
    if (!stiffness_computed) compute_stiffness();
    return st;
}

Matrix &Beam::local_stiffness() {
    if (!stiffness_computed) compute_stiffness();
    return local_st;
}

Matrix &Beam::transformation() {
    if (!stiffness_computed) compute_stiffness();
    return tr;
}

void Beam::compute_stiffness() {
    // Geometric
    qreal dx = first().x() - second().y();
    qreal dy = first().y() - second().y();
    qreal l2= dx*dx + dy*dy;
    qreal l = sqrt(l2);
    qreal l3 = l * l2;
    qreal ca = dx / l; // Cos(alpha)
    qreal sa = dy / l; // Sin(alpha)

    // Material
    qreal E=material().young_modulus();
    qreal nu = material().poisson_ratio();
    //qreal alfa = material().coefficient of thermal expansion();

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

    qreal m[36] = {
	axial,  0,    0,    -axial, 0,    0,
	0,    flex,   fl_sh,  0,    -flex,  fl_sh,
	0,    fl_sh,  shear,  0,    -fl_sh, sh_fl,
	-axial, 0,    0,    axial,  0,    0,
	0,    -flex,  -fl_sh, 0,    flex,   -fl_sh,
	0,    fl_sh,  sh_fl,  0,    -fl_sh, shear
    };
    /*Matrix */
    local_st = Matrix(6,6,m);

    // Transforming stiffness matrix from local coordinates into global one
    // TODO: cache the transformation matrix
    qreal t[36] = {
	ca, -sa, 0.0, 0.0, 0.0, 0.0,
	sa, ca, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, ca, -sa, 0.0,
	0.0, 0.0, 0.0, sa, ca, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    tr = Matrix(6,6,t);

    st = (tr * local_st)*(tr.transposed());
    // Seeing all those zeros in the transformation matrix makes tempting to provide an ad-hoc funtions to compute this product
    stiffness_computed = true;
}

QRectF Beam::boundingRect() const
{
    qreal extra=length()/30.0;
    QRectF result = QRectF(first_node->pos(),  second_node->pos()).normalized();
    result.adjust(-extra, -extra, extra, extra);
    return result;
}

void Beam::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // A beam is simply a line
    painter->setPen(QPen(Qt::black, length()/50.0) );
    painter->drawLine(first_node->pos(),second_node->pos());
    // Draw the loads, written with the assumption that all beams are horizontal. TODO: remove this assumption.
    QRect load_rect(first().pos().x(), first().pos().y(),
                    second().pos().x(), second().pos().y()+load);
    painter->drawText(load_rect, Qt::AlignCenter,
                      QString("%1 kg/m").arg(load));
    painter->setPen(QPen(Qt::red,1));
    painter->setBrush(QBrush(Qt::red,Qt::VerPattern));
    painter->drawRect(load_rect);

    // TODO: Draw axial stress, shear stress and moment

//    // Draw deformated beam.
//    painter->setPen(Qt::blue);
//    // painter->drawLine(first().deformed_pos(),second().deformed_pos());
//    // It would be nice to draw it using splines, but it seems that it is not that easy. Let's draw it as always did, by points; so wow many points shall we draw? Let's naively say currently 32.
//    QPolygonF deformed;

//    const int steps=32;
//    qreal step=1.0/steps; // Beware that 1 is NOT 1.0. If you write it 1/steps, step will be exactly zero.
//    const int ampl=1.0; // TODO make it user defined, or even better computed for best view.
//    qreal csi=0;
//    QPointF step_vector((second().pos() - first().pos())/steps);
//    QPointF current(first().pos());
//    for (int i=0; i<=steps; ++i) {
//	QPointF delta(u(csi)*ampl,v(csi)*ampl);
//	deformed<< current+delta;
//	csi +=step;
//	current+=step_vector;
//    }
//     painter->drawPolyline(deformed);
}

void Beam::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Present the user the precise location of the node which may be edited and a way to change its support conditions

    std::cout<<"beam pressed at "
            <<event->pos().x()<<","<<event->pos().y()
            <<std::endl<<std::flush;
    BeamDialog dialog(*this);
    int res = dialog.exec();
    update();
    //QGraphicsItem::mousePressEvent(event);
}

void Beam::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout<<"rilasciata asta"<<std::endl<<std::flush;
    update();
    //QGraphicsItem::mouseReleaseEvent(event);
}
