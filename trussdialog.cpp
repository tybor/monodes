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

enum bearing {
    unconstrained=0,
    hinged,
    fixed_joint
};

TrussDialog::TrussDialog(QWidget *parent) :
    QDialog(parent),
    // The following constructors and their "expanded" objects may be converted into references and created in the body of this constructor. Oh, my  how foolish is C++
    lengths(),
    loads(),
    // Initialization of sub-dialogs
    material_dialog(this),
    section_dialog(this)

{
    setupUi(this);
    // Lenghts e spans are collections of QDoubleSpinBox; suggesting their size.

    lengths.reserve(maximum_spans);
    loads.reserve(maximum_spans);
    // This is clumsy but it will do for now
    lengths.append(span1);
    lengths.append(span2);
    lengths.append(span3);
    lengths.append(span4);
    lengths.append(span5);
    loads.append(load1);
    loads.append(load2);
    loads.append(load3);
    loads.append(load4);
    loads.append(load5);
#if defined(Q_WS_S60)
    showMaximized();
#endif

   update_spans(spans_count->value());
//   We currently allows only free, hinge
//   and fixed bearing conditions; when we will support the others (trailers and
//   shoes) we shall set the allowed one going throught the model of the combo
//   then setting the flags of the items, and unset Qt::ItemIsSelectable
    //   Show/hide
   connect(spans_count, SIGNAL(valueChanged(int)), this,SLOT(update_spans(int)));
   connect(material_button, SIGNAL(clicked()),&material_dialog,SLOT(show()));
   connect(section_button, SIGNAL(clicked()),&section_dialog, SLOT(show()));
}


//resizeEvent() method get called when user change screen mode.
void TrussDialog::resizeEvent (QResizeEvent* event)
{
    QSize widgetSize = event->size();
	//Resize your custom control according to new size.
    //QMainWindow::
    //resizeEvent(event);
}

enum Constrain contrain(int index){
    switch (index) {
    case 0: return uncostrained ; break;
    case 1: return hinge; break;
    case 2: return restrained; break;
    default:
        assert(0 /* never reached */);
        return uncostrained;
    }
}

enum Constrain TrussDialog::left_constrain() {
    return contrain(left_bearing->currentIndex());
}

enum Constrain TrussDialog::right_constrain() {
    return contrain(right_bearing->currentIndex());
}

void TrussDialog::update_spans(int spans) {
    QComboBox *left=left_bearing, *right=right_bearing;
    switch (spans) {
    case 1: // A free extreme node require a fixed bearing on the other side
	switch (right->currentIndex()) {
	case unconstrained: // right must be fixed
	    right->setCurrentIndex(fixed_joint);
	    break;
	case hinged: // right cannot be uncostrained
	    if (right->currentIndex()==unconstrained) {
		// Change it to something more sensible
		right->setCurrentIndex(hinged);
	    }
	    break;
	case fixed_joint: // Any right bearing is allowed
	    break;
	};
	break;
    case 2: // A free extreme node require the other not to be free either
	break;
    default: // Everything else is allowed.
	break;
    };

    int span; int col;
    // Shows all span length and load for rows from 2 to spans
    for (span=2; span<=spans;++span)
	for (col=0; col<=2; ++col)
            gridLayout->itemAtPosition(span,col)->widget()->show();
    //assert(span==spans);
    // Hides all span length and load for rows from 2 to spans
    for (/*unnecessary ?*/span=spans+1 ; span<=maximum_spans;++span)
	for (col=0; col<=2; ++col)
           gridLayout->itemAtPosition(span,col)->widget()->hide();

}
