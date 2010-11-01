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

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <math.h>

#include "beam.h"
#include "beamdialog.h"
#include "node.h"
#include "truss.h"

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

Beam::Beam(Node &a_node, Node &another_node, Truss &a_truss, Section &a_section, Material &a_material) :
        truss(a_truss),
        first_node(a_node),
        second_node(another_node),
        section(a_section),
        material(a_material)
{
    //assert( a_node==another_node));
    setParentItem(&truss);

    /// TODO: remove length caching!
    setAcceptHoverEvents (true);

    // Computing length and lentgh²
    QPointF d = second().pos()-first().pos();
    stored_length2 = d.x()*d.x() + d.y()*d.y();
    stored_length = sqrt(stored_length2);

    cosa = d.x() / length(); // Cos(alpha)
    sina = d.y() / length(); // Sin(alpha)

    setTransform(QTransform(
            cosa, -sina,
            sina, cosa,
            first_node.x(), first_node.y()));
    stiffness_computed = false;
    member_end_forces_computed = false;
    max_deflection = 0.0;
    // Allocate polygons for deformed, axial, shear and moment, pre-allocating enough points to draw deformated beam in order to avoid unnecessary reallocations.
    deformed = QPolygonF(); deformed.reserve(deformed_points_count);
    axial = QPolygonF(); axial.reserve(deformed_points_count);
    shear= QPolygonF(); shear.reserve(deformed_points_count);
    moment = QPolygonF(); moment.reserve(deformed_points_count);

    // Update topology of the model.
    first_node.add_beam(this);
    second_node.add_beam(this);
    truss.add_beam(*this);
}

// How difficoult to get read-only fields, that in Eiffel are for free!!! Symmetrically setters are always free and you can't get rid of them even when you would like to, so you must make it private, writing both the setter and the getter. So in conclusion Eiffel's approach - the default is fields writable only from inside its object and read-only from outside is what saves efforts.
Node &Beam::first() const { return first_node; }
Node &Beam::second() const { return second_node; }

//void Beam::set_section(Section &a_section) {
//    s = &a_section;
//    stiffness_computed=false; // so the next time stiffness() will be invoked it will be recomputed
//}

//Section &Beam::section() const {
//    assert (s!=NULL);
//    return *s;
//}

//void Beam::set_material (Material &a_material) {
//    m=&a_material;
//    stiffness_computed=false; // so the next time stiffness() will be invoked it will be recomputed
//}

//Material &Beam::material() const {
//    assert (m!=NULL);
//    return *m;
//}

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

Matrix<qreal, 6, 1> &Beam::fixed_end_forces() {
    if (!stiffness_computed) compute_stiffness();
    return gf;
}

Matrix<qreal, 6, 1> &Beam::member_end_forces() {
    if (!member_end_forces_computed) {
        Matrix<qreal,6,1> local_displacements,global_displacements;
        global_displacements <<
                first().u(), first().v(), first().fi(),
                second().u(), second().v(), second().fi();
        local_displacements = transformation().transpose() * global_displacements;
        mef = transformation()*(stiffness() * local_displacements) ;
        member_end_forces_computed = true;
        //std::cout<<"Mef:"<<mef<<" gf:"<<gf<<std::endl;
    }
    return mef;
}

qreal Beam::constant_load() const {
    if (load) return load->constant();
    else return 0.0;
}

void Beam::set_load(qreal an_amount) {
    load = new Load(*this,an_amount);
}

void Beam::compute_stiffness() {
    // length, lentgh2, sine and cosine directors have been compuetued during beam creation.
    qreal l=length();
    qreal l2=length2();
    qreal l3=l*l2;

    // Material
    qreal E=material.young_modulus();
    //1qreal nu = material.poisson_ratio();
    qreal alfa = material.thermal_expansion_coefficient();
    //qreal G = E / (2.0*(1+nu));

    // Section
    qreal A=section.area();
    qreal J=section.moment_of_inertia();
    qreal h=section.height();
    //qreal fi = section.shear_factor();

    // Axial deformability
    qreal axial = E*A/l;
    // Flexional and shear deformability
    // Temporary commenting out influence of shear deformability
    qreal flex = 12.0* E* J / (l3); // (l3 * (1+fi));
    qreal shear = 4*E*J/l; //  (4+fi) * E * J / (l * (1+fi));
    qreal fl_sh = 6*E*J/l2; // 6*E*J/(l2*(1+fi));
    qreal sh_fl = 2*E*J/l; //(2-fi)*E*J/(l*(1+fi));

    local_st <<
            axial,  0.0,    0.0,    -axial, 0.0,    0.0,
            0.0,    flex,   -fl_sh,  0.0,    -flex,  -fl_sh,
            0.0,    -fl_sh,  shear,  0.0,    fl_sh, sh_fl,
            -axial, 0.0,    0.0,    axial,  0.0,    0.0,
            0.0,    -flex,  fl_sh, 0.0,    flex,   -fl_sh,
            0.0,    -fl_sh,  sh_fl,  0.0,    -fl_sh, shear;

    assert(local_st==local_st.transpose());

    // Transforming stiffness matrix from local coordinates into global one
    tr <<
            cosa, -sina, 0.0, 0.0, 0.0, 0.0,
            sina, cosa, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, cosa, -sina, 0.0,
            0.0, 0.0, 0.0, sina, cosa, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 1.0;

    assert (tr==tr.transpose());
    st = (tr * local_st)*(tr.transpose());
    assert (st == st.transpose());

    // Initializing nodal forces vector
    f = Matrix<qreal,6,1>(6,1);
    f.setZero(); /// Nodal loads initialization
    // Loads
    qreal px = 0.0; /// Constant axial distribuited load
    qreal py1 = -constant_load(); /// Transversal linear load: value on first vertex
    qreal py2 = -constant_load(); /// Transversal linear load: value on second vertex
    qreal utd = 0.0; /// Upper thermal delta
    qreal ltd = 0.0; /// Lower thermal delta

    qreal weight = section.area() * material.weight(); /// Self weight
    /// Updating loads with self weight
    px -= weight*sina;
    py1 -= weight*cosa;
    py2 -= weight*cosa;

    /// Nodal forces corresponding to distributed loads
    qreal fh = px * length()/2.0;
    qreal pp = py1, qq = py2 -py1;

    /// To compute shear deformation of distributed loads
    //qreal aa = length() / (12.0 * E * J);
    //qreal bb = section.shear_factor() / (length()* G * A);
    //qreal cc = bb / (aa+bb);

    f(0) += fh; /// Horizontal force on first node
    f(1) += (pp*0.5+ 0.15*qq)*length();
    // f(1) += (0.5 * pp + 0.15*qq*(1.0+cc/9.0))*length(); /// Vertical force on first node
    f(2) +=  (pp/12.0 + qq/30.0)*length2() ;
    //(pp/12.0 + qq*(1.0+cc/4.0)/30.0)*length2(); /// Moment on first node

    f(3) += fh; /// Horizontal force on second node
    f(4) += (pp/2.0+0.35*qq)*length();
    //(0.5*pp + 0.35*qq*(1.0-cc/21.0))*length(); /// Vertical force on second node
    f(5) += (-pp/12.0 -qq/20.0)*length2();
    //- (pp/12.0 + qq*(1.0-cc/6.0)/20.0)*pow(length(),2); /// Moment on second node

    // Nodal forces corresponding  to thermal changes
    qreal atd = alfa * E * A * (utd+ltd)/2; // Average thermal delta
    qreal fm = alfa * E * J * (utd-ltd)/h;
    f(0) -= atd;
    f(2) += fm;
    f(3) += atd;
    f(5) -= fm;

    /// Computing nodal forces in absolute reference system
    gf = tr * f;
    stiffness_computed = true;
}

qreal Beam::maximum_deflection()  const { return max_deflection; }
qreal Beam::maximum_axial()  const { return max_axial; }
qreal Beam::maximum_shear()  const { return max_shear; }
qreal Beam::maximum_moment()  const { return max_moment; }

QRectF Beam::boundingRect() const
{
    qreal line_width = section.height();
    //qreal load_scale = truss().load_scale;
    QRectF result = QRectF(0,0, length(),0);
    // Accounting the load and some space between the beam and the load.
    result.adjust(0.0, -line_width, 0.0, +line_width);
    result.united(scaled_deformed.boundingRect());
    return result.normalized();
}

void Beam::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    /// std::cout<<" painting beam"<<std::endl<<std::flush;
    qreal line_width = section.height();
    // A beam is simply a line
    painter->setPen(QPen(Qt::black, line_width));
    painter->drawLine(QPointF(0,0),QPointF(length(),0));

    // TODO: Draw axial stress, shear stress and moment; better make them subitems to be drawn
    //painter->drawPolyline(moment);

    // Draw deformated beam.
    painter->setPen(QPen(QColor(0, 0, 255, 128),line_width));
    //std::cout<<"Deformation scale "<<truss.deformation_scale<<std::endl;
    painter->drawPolyline(scaled_deformed);

//#ifdef DEBUG
    // Draw boundingRect with a thin green line
    painter->setPen(QPen(Qt::green, 3, Qt::DotLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());
//#endif
}

void Beam::compute_deformed() {
    deformed.clear();
    axial.clear();
    shear.clear();
    moment.clear();

    qreal step=1.0/deformed_points_count; // Beware that 1 is NOT 1.0. If you write it 1/steps, step will be exactly zero.
    QPointF step_vector = QPointF(length(),0.0) / deformed_points_count;
    QPointF current(0,0);
    QPointF current_axial(0,0);
    QPointF current_shear(0,0);
    QPointF current_moment(0,0);
    max_deflection = 0.0;
    max_axial = 0.0;
    max_shear = 0.0;
    max_moment = 0.0;
    qreal weight = section.area()*material.weight();

    qreal px =  member_end_forces()[0] - weight * sina;
    qreal py1 = member_end_forces()[1] - weight * cosa;
    qreal py2 = member_end_forces()[2] - weight * cosa;

    qreal csi=0;
    for (int i=0; i<=deformed_points_count; ++i) {
        QPointF delta(u(csi),v(csi));
        qreal py = py1 + (py2-py1)*csi;
        deformed << current+delta;
        axial << QPointF(csi, -fixed_end_forces()[0] - px * current.x());
        shear << QPointF(csi, -fixed_end_forces()[1] - (py1+py)*current.x()/2.0 );
        qreal x2 = current.x()*current.x(); // pow(current.x() , 2.0);
        moment << QPointF(csi, -fixed_end_forces()[2] + fixed_end_forces()[1]*current.x() +
                          py1 * x2/2.0 + (py+py1)*x2/6.0);
        max_deflection = fmax(fabs(delta.y()),max_deflection);
        max_axial = fmax(fabs(axial.last().y()),max_axial);
        max_shear = fmax(fabs(shear.last().y()),max_shear);
        max_moment = fmax(fabs(moment.last().y()),max_moment);
        csi +=step;
        current+=step_vector;
    }
    /// Close axial, shear and moment plot by re-adding the first point.
    axial<<axial.first();
    shear<<shear.first();
    moment<<moment.first();
    //std::cout<<*this<<" max deflection "<<max_deflection<<std::endl;
}

void Beam::update_plots() {
    scaled_deformed.clear(); scaled_deformed.reserve(deformed_points_count);
    scaled_axial.clear(); scaled_axial.reserve(deformed_points_count);
    scaled_shear.clear(); scaled_shear.reserve(deformed_points_count);
    scaled_moment.clear(); scaled_moment.reserve(deformed_points_count);

    foreach (QPointF point, deformed)  scaled_deformed<< QPointF(point.x(), point.y()*truss.deformation_scale);
    foreach (QPointF point, axial) scaled_axial<< QPointF(point.x(), point.y()*truss.axial_scale);
    foreach (QPointF point, shear) scaled_shear<< QPointF(point.x(), point.y()*truss.shear_scale);
    foreach (QPointF point, shear) scaled_shear<< QPointF(point.x(), point.y()*truss.moment_scale);
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

void Beam::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
#ifdef DEBUG
    std::cout<<"rilasciata asta"<<std::endl<<std::flush;
#endif
    BeamDialog dialog(*this);
    /* unused int res = */ dialog.exec();
    // to need to redraw so no need to update();
    //QGraphicsItem::mouseReleaseEvent(event);
}


std::ostream &operator<<(std::ostream &s, Beam &a_beam) {
    s<<a_beam.first()<<"-"<<a_beam.second();
    return s;
}

