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


#include "nodedialog.h"
#include "node.h"
#include "beam.h"

NodeDialog::NodeDialog(Node &a_node, QWidget *parent) :
	QDialog(parent),
        node(a_node)
    /// Present the user the precise location of the node which may be edited and a way to change its support conditions
{
    setupUi(this);
    /// TODO: Reactions:
    h_value->setText(QString("%1").arg(a_node.horizonal));
    n_value->setText(QString("%1").arg(a_node.vertical));
    m_value->setText(QString("%1").arg(a_node.moment));
    /// Displacements
    u_value->setText(QString("%1").arg(a_node.u()));
    v_value->setText(QString("%1").arg(a_node.v()));
    fi_value->setText(QString("%1").arg(a_node.fi()));

#if defined(Q_WS_S60)
    showMaximized();
#endif
//    if (node.beams().count()==1) {
//	// This node is an extreme, it may be free, hinged or fixed.
//	Beam *ab=node.beams().first();
//    } else { // We are in the middle of the beam. Hinged assumed

//    }
}

void NodeDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
	retranslateUi(this);
	break;
    default:
	break;
    }
}
