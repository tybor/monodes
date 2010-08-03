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
#include "matrix.h"

class LinearSystem : public QThread
        // A linear system running as a separate thread, specialized in solving the systems resulting from the
        // stiffness method of structural analysis.
{
public:
        LinearSystem (Matrix stiffness, Matrix loads);
        // a new solving linear system for a structure with `a_dofs' degrees of freedom with `a_lcc' load cases
        //add_stiffness (unsigned int i, unsigned int j, qreal stiffness); /// Add stiffness at index i,j
        //add_load (unsigned int dof, unsigned int lc, qreal load);

        Matrix solutions();
protected:
     void run();
 private:
     unsigned int dof; /// Degrees of freedom
     unsigned int lcc; /// Load cases count

     Matrix stiffness;
     Matrix displacements;
     Matrix loads;

signals:

public slots:

};

#endif // LINEARSYSTEM_H
