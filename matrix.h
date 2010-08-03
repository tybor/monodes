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


#ifndef MATRIX_H
#define MATRIX_H

#include <QtCore>

class Matrix
        /// A two dimensional array dynamically
{

    // TODO: implement a copy-on-write design!
public:
    Matrix(); // Undefined matrix
    Matrix(unsigned int a_rows_n, unsigned int a_cols_c);
    Matrix(unsigned int a_rows_n, unsigned int a_cols_c, qreal coefficients[]);
    Matrix(const Matrix &another); // Copy constructor
    ~Matrix();

    // Element access and modifying
    qreal at(unsigned int rows_n, unsigned int cols_c) const;
    void set(qreal a_value,unsigned int rows_n, unsigned int cols_c);
    void add(qreal a_value,unsigned int rows_n, unsigned int cols_c);

    // Operation
    Matrix transposed();
    Matrix operator* (Matrix another);

    // Queries; all leaves matrix constant
    unsigned int rows() const {return rows_count;};
    unsigned int cols() const {return cols_count;};

    bool is_valid_col (unsigned int a_col) const;
    bool is_valid_row (unsigned int a_row) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
private:
    unsigned int rows_count;
    unsigned int cols_count;

    qreal    *coefficients;
};

#endif // MATRIX_H
