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
    ,menu_(new UserphraseViewMenu{this})
{
    setupContextMenu();
    setupAddUserphraseDialog();
}

void UserphraseView::showAddUserphraseDialog()
{
    dialogType_ = DIALOG_ADD;
    UserphraseDialog_->setText("", "");
    emit UserphraseDialog_->exec();
}

void UserphraseView::showModifyUserphraseDialog()
{
    dialogType_ = DIALOG_MODIFY;

    Q_ASSERT(selectionModel()->selectedIndexes().size() == 1);

    auto userphrase = model()->getUserphrase(*selectionModel()->selectedIndexes().begin());

    UserphraseDialog_->setText(userphrase->phrase_, userphrase->bopomofo_);

    emit UserphraseDialog_->exec();
}

void UserphraseView::addNewPhrase(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    std::shared_ptr<QString> phrase{new QString(UserphraseDialog_->getPhrase())};
    std::shared_ptr<QString> bopomofo{new QString(UserphraseDialog_->getBopomofo())};

    qDebug() << "Add" << *phrase.get() << "(" << *bopomofo.get() << ")";

    if (dialogType_ == DIALOG_MODIFY) {
        remove();
    }

    emit model()->add(phrase, bopomofo);
}

void UserphraseView::remove()
{
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
        this, SLOT(remove())
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
        this, SLOT(addNewPhrase(int))
    );
}
