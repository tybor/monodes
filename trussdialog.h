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


#ifndef TRUSSDIALOG_H
#define TRUSSDIALOG_H

#include <QtGui>

namespace Ui {
    class TrussDialog;
}

class TrussDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrussDialog(QWidget *parent = 0);
    ~TrussDialog();

    //see http://wiki.forum.nokia.com/index.php/Handle_change_in_screen_orientation_in_Qt_for_Symbian
    //resizeEvent() method get called when user change screen mode.
    void resizeEvent (QResizeEvent* event);
signals:

public slots:
    void update_spans(int new_spans);
private:
    Ui::TrussDialog *ui;
};

#endif // TRUSSDIALOG_H
