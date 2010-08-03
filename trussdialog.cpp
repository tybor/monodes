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


#include "trussdialog.h"
#include "ui_trussdialog.h"
#include <assert.h>

static int maximum_spans = 5;

TrussDialog::TrussDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrussDialog)
{
    ui->setupUi(this);
    // Show/hide
    connect(ui->spans_count, SIGNAL(valueChanged(int)),
			 this, SLOT(update_spans(int)));
}

TrussDialog::~TrussDialog()
{
    delete ui;
}

//resizeEvent() method get called when user change screen mode.
void TrussDialog::resizeEvent (QResizeEvent* event)
{
    QSize widgetSize = event->size();
	//Resize your custom control according to new size.
    //QMainWindow::
    //resizeEvent(event);
}

void TrussDialog::update_spans(int spans) {
    int span; int col;
    // Shows all span length and load for rows from 2 to spans
    for (span=2; span<=spans;++span)
	for (col=0; col<=2; ++col)
	    ui->gridLayout->itemAtPosition(span,col)->widget()->show();
    //assert(span==spans);
    // Hides all span length and load for rows from 2 to spans
    for (/*unnecessary ?*/span=spans+1 ; span<=maximum_spans;++span)
	for (col=0; col<=2; ++col)
	   ui->gridLayout->itemAtPosition(span,col)->widget()->hide();
}
