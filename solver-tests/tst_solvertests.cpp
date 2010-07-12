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


#include <QtCore/QString>
#include <QtTest/QtTest>
#include "matrix.h"

class SolverTests : public QObject
{
    Q_OBJECT

public:
    SolverTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

SolverTests::SolverTests()
{
    Matrix m(5);

    m[1][1] = 10.0;
    m[3][4] =  5.0;
    Matrix n(m);
}

void SolverTests::initTestCase()
{
}

void SolverTests::cleanupTestCase()
{
}

void SolverTests::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SolverTests);

#include "tst_solvertests.moc"
