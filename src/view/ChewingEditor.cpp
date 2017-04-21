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
#include "config.h"

#include <QtGui>
#include <QDebug>
#include <QMessageBox>

#include "ChewingImporter.h"
#include "ChewingExporter.h"

static int import_count;

ChewingEditor::ChewingEditor(QWidget *parent)
    :QMainWindow{parent}
    ,ui_{new Ui::ChewingEditor}
    ,model_{new UserphraseModel{this}}
    ,proxyModel_{new UserphraseSortFilterProxyModel{this}}
    ,fileDialog_{new QFileDialog{this}}
{
    ui_.get()->setupUi(this);

    proxyModel_->setSourceModel(model_);
    ui_.get()->userphraseView->setModel(proxyModel_);

    setupFileSelection();
    setupImport();
    setupExport();
    setupAdd();
    setupRemove();
    setupRefresh();
    setupUndo();
    setupFilter();
    setupAboutWidget();

    // exit action
    shortcut_exit_ = new QShortcut(QKeySequence::Quit, this);
    connect(
        shortcut_exit_, SIGNAL(activated()),
        SLOT(close())
    );
    connect(
        ui_.get()->actionExit, SIGNAL(triggered()),
        SLOT(close())
    );

    // modify action
    shortcut_modify_ = new QShortcut(Qt::Key_Return, this);
    connect(
        shortcut_modify_, SIGNAL(activated()),
        ui_.get()->userphraseView, SLOT(showModifyUserphraseDialog())
    );
}

ChewingEditor::~ChewingEditor()
{
}

void ChewingEditor::execFileDialog(DialogType type)
{
    dialogType_ = type;

    qDebug() << "dialogType_ = " << dialogType_;

    switch (dialogType_) {
    case DIALOG_IMPORT:
        fileDialog_->setWindowTitle(tr("Import"));
        fileDialog_->setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog_->setFileMode(QFileDialog::ExistingFile);
        fileDialog_->setConfirmOverwrite(false);
        fileDialog_->selectFile("");
        break;

    case DIALOG_EXPORT:
        fileDialog_->setWindowTitle(tr("Export"));
        fileDialog_->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog_->setFileMode(QFileDialog::AnyFile);
        fileDialog_->setConfirmOverwrite(true);
        fileDialog_->selectFile("chewing.json");
        break;

    default:
        Q_ASSERT(!"Unknown DialogType");
    }

    emit fileDialog_->exec();
}

void ChewingEditor::selectImportFile()
{
    // reset import_count
    import_count = 0;
    qDebug() << "import_count = " << import_count;
    execFileDialog(DIALOG_IMPORT);
}

void ChewingEditor::selectExportFile()
{
    execFileDialog(DIALOG_EXPORT);
}

void ChewingEditor::finishFileSelection(const QString& file)
{
    qDebug() << "dialogType_ = " << dialogType_;

    switch (dialogType_) {
    case DIALOG_IMPORT:
        // Do nothing after the first trigger
        if (import_count == 0) {
            importUserphrase(file);
            import_count++;
        }

        qDebug() << "import_count = " << import_count;

        break;

    case DIALOG_EXPORT:
        exportUserphrase(file);
        break;

    default:
        Q_ASSERT(!"Unknown DialogType");
    }
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

void ChewingEditor::showAbout()
{
    QString caption = QString("<h3>%1</h3><p>%2</p>")
        .arg(tr("About Chewing Editor"))
#ifdef PROJECT_GIT_VERSION
        .arg(tr("Version %1 (%2).").arg(PROJECT_VERSION).arg(PROJECT_GIT_VERSION));
#else
        .arg(tr("Version %1.").arg(PROJECT_VERSION));
#endif
    QString text = QString("<p style=\"white-space: pre-wrap;\">%1\n\n%2\n\n%3\n\n%4\n\n%5\n\n%6</p>")
        .arg("Chewing editor is a cross platform Chewing user phrase editor. "
        "It provides a easy way to manage user phrase. With it, user can "
        "customize their user phrase to increase input performance.")
        .arg("Chewing editor is licensed under the GNU GPL version 2.0 or later version.")
        .arg("Please see <a href=\"https://github.com/chewing/chewing-editor/blob/master/COPYING\">COPYING</a> "
        "for detail of Chewing editor licensing.")
        .arg("Copyright&copy; 2004-2016 by Chewing Core Team and contributors on "
        "<a href=\"https://github.com/chewing/chewing-editor/graphs/contributors\">GitHub</a>.")
        .arg("Chewing editor is developed as an open source project on "
        "<a href=\"https://github.com/chewing/chewing-editor\">https://github.com/chewing/chewing-editor</a>.")
        .arg("Any suggestions are welcomed on "
        "<a href=\"https://github.com/chewing/chewing-editor/issues\">https://github.com/chewing/chewing-editor/issues</a>");
    QMessageBox aboutBox(this);
    aboutBox.setWindowTitle(tr("About Chewing Editor"));
    aboutBox.setText(caption);
    aboutBox.setInformativeText(text);
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.setDefaultButton(QMessageBox::Ok);

    QPixmap pm(QLatin1String(":/chewing-editor.png"));
    if (!pm.isNull())
        aboutBox.setIconPixmap(pm);

    aboutBox.exec();
}

void ChewingEditor::showDeleteConfirmWindow()
{
    if ( !model_->rowCount() ) return;
    
    QString text = tr("Do you want to delete this phrase?");

    QMessageBox deleteBox(this);
    deleteBox.setWindowTitle(tr("Delete phrase"));
    deleteBox.setText(text);
    deleteBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    deleteBox.setDefaultButton(QMessageBox::No);

    connect(
        deleteBox.button(QMessageBox::Yes), SIGNAL(clicked()),
        ui_.get()->userphraseView, SLOT(remove())
    );

    deleteBox.exec();
}

void ChewingEditor::setupFileSelection()
{
    fileDialog_->setNameFilter("*.json");

    connect(
        fileDialog_, SIGNAL(fileSelected(const QString&)),
        this, SLOT(finishFileSelection(const QString &))
    );
}

void ChewingEditor::setupImport()
{
    shortcut_import_ = new QShortcut(Qt::CTRL + Qt::Key_I, this);
    connect(
        shortcut_import_, SIGNAL(activated()),
        this, SLOT(selectImportFile())
    );

    connect(
        ui_.get()->actionImport, SIGNAL(triggered()),
        this, SLOT(selectImportFile())
    );

    connect(
        model_, SIGNAL(importCompleted(bool, const QString&, size_t, size_t)),
        ui_.get()->notification, SLOT(notifyImportCompleted(bool, const QString&, size_t, size_t))
    );

    // initialize import_count
    import_count = 0;
    qDebug() << "import_count = " << import_count;
}

void ChewingEditor::setupExport()
{
    shortcut_export_ = new QShortcut(Qt::CTRL + Qt::Key_E, this);
    connect(
        shortcut_export_, SIGNAL(activated()),
        this, SLOT(selectExportFile())
    );

    connect(
        ui_.get()->actionExport, SIGNAL(triggered()),
        this, SLOT(selectExportFile())
    );

    connect(
        model_, SIGNAL(exportCompleted(bool, const QString&, size_t)),
        ui_.get()->notification, SLOT(notifyExportCompleted(bool, const QString&, size_t))
    );
}

void ChewingEditor::setupAdd()
{
    shortcut_add_ = new QShortcut(QKeySequence::New, this);
    connect(
        shortcut_add_, SIGNAL(activated()),
        ui_.get()->userphraseView, SLOT(showAddUserphraseDialog())
    );

    connect(
        ui_.get()->addButton, SIGNAL(pressed()),
        ui_.get()->userphraseView, SLOT(showAddUserphraseDialog())
    );

    connect(
        model_, SIGNAL(addNewPhraseCompleted(const Userphrase&)),
        ui_.get()->notification, SLOT(notifyAddNewPhraseCompleted(const Userphrase&))
    );

    connect(
        model_, SIGNAL(addNewPhraseFailed()),
        ui_.get()->notification, SLOT(notifyAddNewPhraseFailed())
    );
}

void ChewingEditor::setupRemove()
{
    shortcut_remove_ = new QShortcut(QKeySequence::Delete, this);
    connect(
        shortcut_remove_, SIGNAL(activated()),
        this, SLOT(showDeleteConfirmWindow())
    );

    connect(
        ui_.get()->removeButton, SIGNAL(pressed()),
        this, SLOT(showDeleteConfirmWindow())
    );

    connect(
        model_, SIGNAL(removePhraseCompleted(size_t)),
        ui_.get()->notification, SLOT(notifyRemovePhraseCompleted(size_t))
    );
}

void ChewingEditor::setupRefresh()
{
    shortcut_refresh_ = new QShortcut(Qt::CTRL + Qt::Key_R, this);
    connect(
        shortcut_refresh_, SIGNAL(activated()),
        model_, SLOT(refresh())
    );

    connect(
        ui_.get()->refreshButton, SIGNAL(pressed()),
        model_, SLOT(refresh())
    );

    connect(
        model_, SIGNAL(refreshCompleted(size_t)),
        ui_.get()->notification, SLOT(notifyRefreshCompleted(size_t))
    );

    emit model_->refresh();
}

void ChewingEditor::setupUndo()
{
    connect(
        ui_.get()->undoButton, SIGNAL(pressed()),
        model_, SLOT(undo())
    );

    connect(
        model_, SIGNAL(undoCompleted(const QString&)),
        ui_.get()->notification, SLOT(notifyUndoCompleted(const QString&))
    );
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

   connect(
        ui_.get()->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout())
   );
}
