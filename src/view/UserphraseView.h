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

#pragma once

#include <QListView>

#include "AddNewPhraseDialog.h"
#include "UserphraseSortFilterProxyModel.h"
#include "UserphraseViewMenu.h"

class UserphraseView final : public QListView {
    Q_OBJECT

public:
    explicit UserphraseView(QWidget *parent = 0);
    UserphraseView(const UserphraseView&) = delete;
    UserphraseView& operator=(const UserphraseView&) = delete;
    virtual ~UserphraseView() = default;
    virtual void setModel(UserphraseSortFilterProxyModel *model) {
        QListView::setModel(model);
    }

public slots:
    void showAddUserphraseDialog();
    void addNewPhrase(int result);
    void remove();
    void setFilterString(const QString& text);
    void showContextMenu(const QPoint& point);


protected:
    UserphraseSortFilterProxyModel* model() const {
        return dynamic_cast<UserphraseSortFilterProxyModel*>(QListView::model());
    }

private:
    void setupContextMenu();
    void setupAddUserphraseDialog();

    AddNewPhraseDialog *addNewPhraseDialog_;
    UserphraseViewMenu *menu_;
};
