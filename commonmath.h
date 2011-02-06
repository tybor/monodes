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
#include <valarray>
#include <cstdlib>
#include <stdlib.h>

#if defined (__SYMBIAN32__)
// Defining some math functions that seems to be missing from Symbian.
// Note: this is a horrible hack.
namespace std {
inline int abs(int &x) { return std::abs(x); }
inline double abs(long double &x) {return std::abs((double) x); }
inline double sqrt(long double &x) {return std::sqrt(x);}
inline double exp(long double &x) {return std::exp(x);}
inline double log(long double &x) {return std::log(x);}
inline double sin(long double &x) {return std::sin(x);}
inline double cos(long double &x) {return std::cos(x);}
inline double atan(long double &x) {return std::atan(x);}
inline double atan2(long double &x, long double &y) {return std::atan2(x,y);}
inline double pow(long double &x, long double &y) {return std::pow(x,y);}
};
#endif

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
