/*
 * chewing-editor: Chewing userphrase editor
 * Copyright (C) 2014 ChangZhuo Chen

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

#include "ChewingEditor.h"
#include "ui_ChewingEditor.h"

#include <QtGui>
#include <QDebug>

ChewingEditor::ChewingEditor(QWidget *parent)
    :QMainWindow(parent)
    ,ui_(new Ui::ChewingEditor)
    ,model_(new UserphraseModel(this))
    ,proxyModel_(new UserphraseSortFilterProxyModel(this))
    ,addNewPhraseDialog_(new AddNewPhraseDialog(this))
{
    ui_.get()->setupUi(this);

    proxyModel_->setSourceModel(model_);
    ui_.get()->userphraseView->setModel(proxyModel_);

    setupConnect();
}

ChewingEditor::~ChewingEditor()
{
}

void ChewingEditor::addNewPhrase(int result)
{
    if (result != QDialog::Accepted) { return; }

    qDebug() << FUNC_NAME << "accepted";
}

void ChewingEditor::setupConnect()
{
    connect(
        ui_.get()->removeButton, SIGNAL(pressed()),
        ui_.get()->userphraseView, SLOT(remove())
    );

    connect(
        ui_.get()->userphraseFilter, SIGNAL(textEdited(const QString&)),
        ui_.get()->userphraseView, SLOT(setFilterString(const QString&))
    );

    connect(
        ui_.get()->refreshButton, SIGNAL(pressed()),
        model_, SLOT(refresh())
    );

    connect(
        ui_.get()->addButton, SIGNAL(pressed()),
        addNewPhraseDialog_, SLOT(exec())
    );

    connect(
        addNewPhraseDialog_, SIGNAL(finished(int)),
        this, SLOT(addNewPhrase(int))
    );
}
