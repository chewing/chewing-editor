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

#include "UserphraseModel.h"
#include "UserphraseView.h"

#include <QDebug>

UserphraseView::UserphraseView(QWidget *parent)
    :QListView{parent}
    ,UserphraseDialog_{new UserphraseDialog{this}}
    ,UserphraseDeleteDialog_{new UserphraseDeleteDialog{this}}
    ,menu_(new UserphraseViewMenu{this})
{
    setupContextMenu();
    setupAddUserphraseDialog();
    // setup double click
    connect(
        this, SIGNAL(doubleClicked(const QModelIndex &)),
        this, SLOT(showModifyUserphraseDialog())
    );
}

void UserphraseView::showAddUserphraseDialog()
{
    dialogType_ = DIALOG_ADD;
    UserphraseDialog_->setText("", "");
    UserphraseDialog_->setWindowTitle(tr("Add new phrase"));
    emit UserphraseDialog_->exec();
}

void UserphraseView::showModifyUserphraseDialog()
{
    if (selectionModel()->selectedIndexes().size() != 1) {
        return;
    }

    dialogType_ = DIALOG_MODIFY;

    auto userphrase = model()->getUserphrase(selectionModel()->selectedIndexes().first());

    UserphraseDialog_->setText(userphrase->phrase_, userphrase->bopomofo_);
    UserphraseDialog_->setWindowTitle(tr("Modify phrase"));
    emit UserphraseDialog_->exec();
}

void UserphraseView::showDeleteUserphraseDialog()
{
  dialogType_ = DIALOG_ADD;
  UserphraseDeleteDialog_->setWindowTitle(tr("Delete phrase"));
  emit UserphraseDeleteDialog_->exec();
}

void UserphraseView::addPhrase(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    std::shared_ptr<QString> phrase{new QString(UserphraseDialog_->getPhrase())};
    std::shared_ptr<QString> bopomofo{new QString(UserphraseDialog_->getBopomofo())};

    qDebug() << "Add" << *phrase.get() << "(" << *bopomofo.get() << ")";

    if (dialogType_ == DIALOG_MODIFY) {
        remove(QDialog::Accepted);
    }

    emit model()->add(phrase, bopomofo);
}

void UserphraseView::remove(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    auto selection = selectionModel();
    emit model()->remove(selection->selectedIndexes());
    selection->reset();
}

void UserphraseView::setFilterString(const QString& text)
{
    model()->setFilterWildcard(text);
}

void UserphraseView::setupContextMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(
        this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&))
    );

    connect(
        menu_->getActionRemovePhrase(), SIGNAL(triggered()),
        this, SLOT(showDeleteUserphraseDialog())
    );
}

void UserphraseView::showContextMenu(const QPoint& point)
{
    emit menu_->execWithSelectInfo(mapToGlobal(point), selectionModel()->selectedIndexes().size());
}

void UserphraseView::setupAddUserphraseDialog()
{
    connect(
        menu_->getActionAddPhrase(), SIGNAL(triggered()),
        this, SLOT(showAddUserphraseDialog())
    );

    connect(
        menu_->getActionModifyPhrase(), SIGNAL(triggered()),
        this, SLOT(showModifyUserphraseDialog())
    );

    connect(
        UserphraseDialog_, SIGNAL(finished(int)),
        this, SLOT(addPhrase(int))
    );

    connect(
        UserphraseDeleteDialog_, SIGNAL(finished(int)),
        this, SLOT(remove(int))
    );
}
