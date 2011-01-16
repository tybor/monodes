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


#ifndef COMMONMATH_H
#define COMMONMATH_H

/// Shared access to all math related headers, mainly Eigen2 matrix library

#include <cmath>
#include <cstdlib>
#include <stdlib.h>

// Avoiding any optimization that requires specific alignment of data to avoid building issues on symbian and windows. TODO: re-enable this on linux
#define EIGEN_DONT_ALIGN 1
// I used to define EIGEN_DONT_VECTORIZE  and EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT to avoid alignment issues as http://eigen.tuxfamily.org/dox/UnalignedArrayAssert.html
// #define EIGEN_DONT_VECTORIZE 1
// #define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Array> // To compare matrices

// import most common Eigen types
USING_PART_OF_NAMESPACE_EIGEN
using Eigen::Dynamic;

#endif // COMMONMATH_H
