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


#include "section.h"

Section::Section(qreal a_width, qreal an_height)
{
    b=a_width;
    h=an_height;
}

qreal Section::area() const {return b*h;}
qreal Section::moment_of_inertia() const {
    // Avoiding pow to be faster
    return b*h*h*h/12.0;
}
qreal Section::height() const {return h;};
qreal Section::shear_factor() const {
    // Rectangular sections have a shear factor of 6/5
    return 1.2;
}
