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

#include <QFormLayout>
#include <QPushButton>
#include <QFormLayout>

//NodeDialog::NodeDialog(QWidget *parent) :
//    QDialogButtonBox(parent)
//{
//}

NodeDialog::NodeDialog(Node &a_node) :
        QDialog(),
        node(a_node)
    /// Present the user the precise location of the node which may be edited and a way to change its support conditions
{
	//connect(this, SIGNAL(accepted()), this, SLOT(accept()));
    //connect(this, SIGNAL(rejected()), this, SLOT(reject()));

//    std::cout<<"premuto ("<<x()<<","<<y()<<")"
//	    <<std::endl<<std::flush;
//    QDialog dialog; //= new QDialog;
    QPushButton *support = new QPushButton(QIcon(":/icone/carrello.png"),"trailer");
    QPushButton *hinge= new QPushButton(QIcon(":/icone/cerniera.png"),"hinge");
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout *vertical = new QVBoxLayout();
    vertical->addWidget(support);
    vertical->addWidget(hinge);
    vertical->addWidget(buttons);
    setLayout(vertical);
    //    int res = dialog.exec();
    //    //QGraphicsProxyWidget *dialog_item = scene()->addWidget(dialog);
}
