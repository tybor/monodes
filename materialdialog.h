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


#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include "ui_materialdialog.h"

class MaterialDialog : public QDialog, public Ui::MaterialDialog
{
    Q_OBJECT

public:
    explicit MaterialDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // MATERIALDIALOG_H
