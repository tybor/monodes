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

#include <Eigen/LU>
#include <assert.h>
#include <stdio.h>

#include "linearsystem.h"

LinearSystem::LinearSystem (const Matrix<qreal, Dynamic, Dynamic> &a_stiffness,
                            const Matrix<qreal, Dynamic, Dynamic> &some_loads) {
    assert(/* square stiffness */ a_stiffness.rows()==a_stiffness.cols());
    assert(/* coherent matrices */ a_stiffness.cols() == some_loads.rows());

    /// Everything got copied here. Avoid it if possible.
    stiffness = Matrix<qreal, Dynamic, Dynamic>(a_stiffness);
    loads = Matrix<qreal, Dynamic, Dynamic>(some_loads);
    displacements = Matrix<qreal, Dynamic, Dynamic>(some_loads.rows(),some_loads.rows());
}

void LinearSystem::run()
{    
    displacements = Matrix<qreal, Dynamic, Dynamic>(stiffness.rows(), loads.cols());
    stiffness.lu().solve(loads, &displacements);
    std::cout<<"Displacement found:"<<displacements<<std::endl;
}

const Matrix<qreal, Dynamic, Dynamic> &LinearSystem::solutions()
{
    return displacements;
}
