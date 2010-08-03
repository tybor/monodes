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


#include "matrix.h"
#include <iostream>
#include <string.h> // to get memcpy
#include "assert.h"

Matrix::Matrix () {
    rows_count = 0;
    cols_count = 0;
    coefficients = NULL;
}

Matrix::Matrix(unsigned int a_rows_n, unsigned int a_cols_n)
{
    rows_count = a_rows_n;
    cols_count = a_cols_n;
    coefficients = new qreal[rows()*cols()];
}

Matrix::Matrix(unsigned int a_rows_n, unsigned int a_cols_n, qreal some_coefficients[]) {
	// I would have liked to write 
	// unsigned int count =  sizeof some_coefficients / sizeof *some_coefficients;
	// assert (count== a_rows_n*a_cols_n);
	// to check if some_coefficients has the correct size but for C++ this is a pointer to a black hole and the size of the array it points to is lost. Shame on you!
    rows_count = a_rows_n;
    cols_count = a_cols_n;

    coefficients = new qreal[rows()*cols()];
    memcpy(coefficients, some_coefficients, sizeof some_coefficients);
//    for(unsigned int i=1; i<=rows();i++)
//        for(unsigned int j=1; i<=cols();j++)
//            set(some_coefficients[(i-1)*cols()+j-1],i,j);
}

Matrix::Matrix (const Matrix &another) {
    rows_count = another.rows();
    cols_count = another.cols();
    // We may provide something better than that:
    coefficients = new qreal[rows()*cols()];
    for(unsigned int i=1; i<=rows();i++)
	for(unsigned int j=1; j<=cols();j++)
            set(another.at(i,j),i,j);
    // perhaps something like coeffients = memcpy(another.coefficients,
}

Matrix::~Matrix() {
    std::cout<<"Deleting a "<<
	    rows_count<<"x"<<cols_count<<
	    " matrix..."<<std::flush;
    delete coefficients;
    std::cout<<"deleted.\n"<<std::flush;
}

qreal Matrix::at(unsigned int a_row, unsigned int a_col) const {
    assert(is_valid_row(a_row));
    assert(is_valid_col(a_col));
    return coefficients[(a_row-1)*cols() + (a_col-1) ];
}

void Matrix::set(qreal a_value,unsigned int a_row, unsigned int a_col) {
    assert(is_valid_row(a_row));
    assert(is_valid_col(a_col));
   coefficients[(a_row-1)*cols() + (a_col-1)] = a_value;
}

void Matrix::add(qreal a_value,unsigned int a_row, unsigned int a_col) {
    assert(is_valid_row(a_row));
    assert(is_valid_col(a_col));
    coefficients[(a_row-1)*cols() + (a_col-1)]+=a_value;
}

Matrix Matrix::transposed() {
    // Copy current matrix
    Matrix result(cols(),rows());
    for (unsigned int r=1; r<=rows();r++) {
        // Copy the diagonal element
        result.set(at(r,r), r,r);
        // Copy non-diagonal elements
        for (unsigned int c=r+1; c<=cols();c++)
            result.set(at(r,c),c,r);
    }
    return result;
}

Matrix Matrix::operator* (Matrix another) {
    assert(cols()==another.rows());
    Matrix result(rows(),another.cols());
    for (unsigned int i=1; i<=result.rows();i++)
	for (unsigned int j=1; j<=result.cols();j++) {
	    result.set(0.0,i,j);
	    for (unsigned int k=1; k<=cols();k++)
		result.add(at(i,k)*another.at(k,j),i,j);
        };
       return result;
}

bool Matrix::is_valid_col(unsigned int a_col) const {
    return ((1<=a_col) && (a_col<=cols()));
}

bool Matrix::is_valid_row(unsigned int a_row) const {
    return ((1<=a_row) && (a_row<=rows()));
}


/// Outputting
std::ostream& operator<<(std::ostream& os, const ::Matrix& m) {
    os<<"[";
    for(unsigned int i = 1; i <= m.rows(); i++) {
        os<<"[";
        for(unsigned int j = 1; j <= m.rows(); j++) {
            os << m.at(i,j);
            os << ", ";
        };
        os << "]\n"; //endl don't like me << endl;
    };
    os << "]\n";// << endl;
    return os;
}
