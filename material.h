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


#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtCore>

class Material
        // An indefinite elastic material
{
public:
    Material(); // Default material
    Material(qreal an_E, qreal a_nu, qreal an_alfa);

    // TODO: turn into a QObject heir with slots for changed values.
    qreal young_modulus() const;
    qreal poisson_ratio() const;
    qreal thermal_expansion_coefficient() const;
    qreal weight() const;
private:
    qreal E; // Young modulus
    qreal nu; // Poisson ratio
    qreal alfa; // coefficient of thermal expansion
    qreal density; // density
};

#endif // MATERIAL_H
