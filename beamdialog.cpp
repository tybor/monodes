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


#include "beamdialog.h"
#include "beam.h"
#include <QtGui>

BeamDialog::BeamDialog(Beam &a_beam,QWidget *parent) :
    QDialog(parent),
    beam(a_beam)
{
    QFormLayout *layout = new QFormLayout;
    setLayout(layout);
    QDoubleSpinBox *x_spin = new QDoubleSpinBox;
    QDoubleSpinBox *y_spin = new QDoubleSpinBox;
    x_spin->setValue(beam.x());
    y_spin->setValue(beam.y());
    layout->addRow("x:", x_spin);
    layout->addRow("y:", y_spin);
}
