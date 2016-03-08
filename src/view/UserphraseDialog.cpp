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

#include "UserphraseDialog.h"
#include "ui_UserphraseDialog.h"

UserphraseDialog::UserphraseDialog(QWidget *parent)
    :QDialog{parent}
    ,ui_{new Ui::UserphraseDialog}
{
    ui_.get()->setupUi(this);
    setupConnect();
}

UserphraseDialog::~UserphraseDialog()
{
}

void UserphraseDialog::setupConnect()
{
    connect(
        ui_.get()->buttonBox, SIGNAL(accepted()),
        this, SLOT(accept())
    );

    connect(
        ui_.get()->buttonBox, SIGNAL(rejected()),
        this, SLOT(reject())
    );
}

QString UserphraseDialog::getPhrase() const
{
    return ui_.get()->phrase->text().trimmed();
}

QString UserphraseDialog::getBopomofo() const
{
    return ui_.get()->bopomofo->text().trimmed();
}

void UserphraseDialog::setText(const QString& phrase, const QString& bopomofo)
{
    ui_.get()->phrase->setText(phrase);
    ui_.get()->bopomofo->setText(bopomofo);
}
