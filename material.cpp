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


#include "material.h"

Material::Material()
{
    Material(1.0,1.0,1.0);
    density = 0.0;
}

Material::Material(qreal an_E, qreal a_nu, qreal an_alfa) {
    E=an_E;
    nu=a_nu;
    alfa=an_alfa;
    density = 0.0;
}

qreal Material::young_modulus() const {return E;}
qreal Material::poisson_ratio() const {return nu; }
qreal Material::thermal_expansion_coefficient() const {return alfa;}
qreal Material::weight() const { return density; }
