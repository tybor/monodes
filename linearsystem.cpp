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


#include "linearsystem.h"
#include "assert.h"

LinearSystem::LinearSystem (const Matrix<qreal, Dynamic, Dynamic> &a_stiffness,
                            const Matrix<qreal, Dynamic, Dynamic> &some_loads) {
    //assert(/* square stiffness */ a_stiffness.rows()==a_stiffness.cols());
    //assert(/* coherent matrices */ a_stiffness.rows() == some_loads.rows());

    //stiffness = Matrix(a_stiffness);
    //loads = Matrix(some_loads);
}

void LinearSystem::run()
{    
    //displacements = Matrix(stiffness.rows(), loads.cols());

}

const Matrix<qreal, Dynamic, Dynamic> &LinearSystem::solutions()
{
    //return displacements;
}
