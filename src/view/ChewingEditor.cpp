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

#include "ChewingEditor.h"
#include "ui_ChewingEditor.h"

#include <QtGui>
#include <QDebug>

#include "ChewingImporter.h"
#include "ChewingExporter.h"

ChewingEditor::ChewingEditor(QWidget *parent)
    :QMainWindow{parent}
    ,ui_{new Ui::ChewingEditor}
    ,model_{new UserphraseModel{this}}
    ,proxyModel_{new UserphraseSortFilterProxyModel{this}}
    ,importDialog_{new QFileDialog{this}}
    ,exportDialog_{new QFileDialog{this}}
{
    ui_.get()->setupUi(this);

    proxyModel_->setSourceModel(model_);
    ui_.get()->userphraseView->setModel(proxyModel_);

    setupImport();
    setupExport();
    setupAdd();
    setupRemove();
    setupRefresh();
    setupFilter();
    setupAboutWidget();
}

ChewingEditor::~ChewingEditor()
{
}


void ChewingEditor::importUserphrase(const QString& file)
{
    emit model_->importUserphrase(createUserphraseImporter(file));
}

void ChewingEditor::exportUserphrase(const QString& file)
{
    // TODO: Find a suitable exporter
    std::shared_ptr<ChewingExporter> exporter(new ChewingExporter{file});
    emit model_->exportUserphrase(exporter);
}

void ChewingEditor::setupImport()
{
    importDialog_->setAcceptMode(QFileDialog::AcceptOpen);
    exportDialog_->setFileMode(QFileDialog::ExistingFile);

    connect(
        ui_.get()->actionImport, SIGNAL(triggered()),
        importDialog_, SLOT(exec())
    );

    connect(
        importDialog_, SIGNAL(fileSelected(const QString&)),
        this, SLOT(importUserphrase(const QString&))
    );

    connect(
        model_, SIGNAL(importCompleted(bool, const QString&, size_t, size_t)),
        ui_.get()->notification, SLOT(notifyImportCompleted(bool, const QString&, size_t, size_t))
    );
}

void ChewingEditor::setupExport()
{
    exportDialog_->setAcceptMode(QFileDialog::AcceptSave);
    exportDialog_->setFileMode(QFileDialog::AnyFile);
    exportDialog_->setConfirmOverwrite(true);

    connect(
        ui_.get()->actionExport, SIGNAL(triggered()),
        exportDialog_, SLOT(exec())
    );

    connect(
        exportDialog_, SIGNAL(fileSelected(const QString&)),
        this, SLOT(exportUserphrase(const QString&))
    );

    connect(
        model_, SIGNAL(exportCompleted(bool, const QString&, size_t)),
        ui_.get()->notification, SLOT(notifyExportCompleted(bool, const QString&, size_t))
    );
}

void ChewingEditor::setupAdd()
{
    connect(
        ui_.get()->actionAddPhrase, SIGNAL(triggered()),
        ui_.get()->userphraseView, SLOT(showAddUserphraseDialog())
    );

    connect(
        model_, SIGNAL(addNewPhraseCompleted(const Userphrase&)),
        ui_.get()->notification, SLOT(notifyAddNewPhraseCompleted(const Userphrase&))
    );
}

void ChewingEditor::setupRemove()
{
    connect(
        ui_.get()->actionRemovePhrase, SIGNAL(triggered()),
        ui_.get()->userphraseView, SLOT(remove())
    );

    connect(
        model_, SIGNAL(removePhraseCompleted(size_t)),
        ui_.get()->notification, SLOT(notifyRemovePhraseCompleted(size_t))
    );
}

void ChewingEditor::setupRefresh()
{
    connect(
        ui_.get()->actionRefresh, SIGNAL(triggered()),
        model_, SLOT(refresh())
    );

    connect(
        model_, SIGNAL(refreshCompleted(size_t)),
        ui_.get()->notification, SLOT(notifyRefreshCompleted(size_t))
    );

    emit model_->refresh();
}

void ChewingEditor::setupFilter()
{
    connect(
        ui_.get()->userphraseFilter, SIGNAL(textEdited(const QString&)),
        ui_.get()->userphraseView, SLOT(setFilterString(const QString&))
    );
}

void ChewingEditor::setupAboutWidget()
{
   connect(
        ui_.get()->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt())
   );
}
