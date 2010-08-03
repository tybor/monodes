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


#include "materialdialog.h"
#include "material.h"
#include <QtGui>

MaterialDialog::MaterialDialog(Material &a_material, QWidget *parent) :
    QDialog(parent),
    material(a_material)
{
    layout = new QFormLayout(this);
    setLayout(layout);
    // E, Young modulus
    E_spin = new QDoubleSpinBox(this);
    E_spin->setValue(material.young_modulus());
    layout->addRow("Young modulus",E_spin);
    // nu, Poisson ratio
    nu_spin = new QDoubleSpinBox(this);
    nu_spin->setValue(material.poisson_ratio());
    layout->addRow("Poisson ratio",nu_spin);
     // alpha, coefficient of thermal expansion
    alpha_spin = new QDoubleSpinBox(this);
    alpha_spin->setValue(material.thermal_expansion_coefficient());
    layout->addRow("Thermal expansion coefficient",alpha_spin);
    // density
    density_spin = new QDoubleSpinBox(this);
    density_spin->setValue(material.young_modulus());
    layout->addRow("Density",density_spin);
}

//This should not be necessary setting widget parents
//MaterialDialog::~MaterialDialog() {
//    delete E_spin;
//    delete nu_spin;
//    delete alpha_spin;
//    delete density_spin;
//}
