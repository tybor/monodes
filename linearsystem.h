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


#ifndef LINEARSYSTEM_H
#define LINEARSYSTEM_H

#include <QThread>
#include <Eigen/Core>
// import most common Eigen types
USING_PART_OF_NAMESPACE_EIGEN
using Eigen::Dynamic;
// To avoid alignment issues. See http://eigen.tuxfamily.org/dox/UnalignedArrayAssert.html
#define EIGEN_DONT_VECTORIZE 1
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1

class LinearSystem : public QThread
        // A linear system running as a separate thread, specialized in solving the systems resulting from the
        // stiffness method of structural analysis.
{
public:
        LinearSystem (const Matrix<qreal, Dynamic, Dynamic> &stiffness,
                      const Matrix<qreal, Dynamic, Dynamic> &loads);

        void run();
        const Matrix<qreal, Dynamic, Dynamic> &solutions();

 private:
     unsigned int dof; /// Degrees of freedom
     unsigned int lcc; /// Load cases count

     Matrix<qreal, Dynamic, Dynamic> stiffness;
     Matrix<qreal, Dynamic, Dynamic> displacements;
     Matrix<qreal, Dynamic, Dynamic> loads;

signals:

public slots:

};

#endif // LINEARSYSTEM_H
