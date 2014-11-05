/*
 * chewing-editor: Chewing userphrase editor
 * Copyright (C) 2014 Chewing Development Team

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "UserphraseViewMenu.h"
#include "ui_UserphraseViewMenu.h"

UserphraseViewMenu::UserphraseViewMenu(QWidget *parent)
    :QMenu{parent}
    ,ui_{new Ui::UserphraseViewMenu}
{
    ui_.get()->setupUi(this);
}

UserphraseViewMenu::~UserphraseViewMenu()
{
}

void UserphraseViewMenu::execWithSelectInfo(const QPoint &point, int selected)
{
    switch (selected) {
    case 0:
        ui_.get()->actionRemovePhrase->setEnabled(false);
        break;

    default:
        ui_.get()->actionRemovePhrase->setEnabled(true);
        break;
    }

    exec(point);
}

QAction *UserphraseViewMenu::getActionAddPhrase()
{
    return ui_.get()->actionAddPhrase;
}


QAction *UserphraseViewMenu::getActionRemovePhrase()
{
    return ui_.get()->actionRemovePhrase;
}
