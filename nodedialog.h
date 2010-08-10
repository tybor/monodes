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


#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QtGui>
#include "ui_nodedialog.h"
//#include "node.h"

class Node; // Forward declaration

class NodeDialog : public QDialog, public Ui::NodeDialog
{
    Q_OBJECT
public:
    explicit NodeDialog(Node &a_node, QWidget *parent = 0);

protected:
    Node &node;
    void changeEvent(QEvent *e);

private:

signals:

public slots:

};

#endif // NODEDIALOG_H
