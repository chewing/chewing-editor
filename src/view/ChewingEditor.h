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

#include <memory>

#include <QFileDialog>
#include <QMainWindow>

#include "UserphraseDialog.h"
#include "Notification.h"
#include "UserphraseSortFilterProxyModel.h"

namespace Ui {
    class ChewingEditor;
}

class ChewingEditor final: public QMainWindow
{
    Q_OBJECT

public:
    explicit ChewingEditor(QWidget *parent = 0);
    ChewingEditor(const ChewingEditor&) = delete;
    ChewingEditor& operator=(const ChewingEditor&) = delete;
    ~ChewingEditor(); // = default;

public slots:
    void importUserphrase(const QString& file);
    void exportUserphrase(const QString& file);

private:
    void setupImport();
    void setupExport();
    void setupAdd();
    void setupRemove();
    void setupRefresh();
    void setupFilter();
    void setupAboutWidget();

    std::unique_ptr<Ui::ChewingEditor> ui_;

    UserphraseModel *model_;
    UserphraseSortFilterProxyModel *proxyModel_;

    QFileDialog *importDialog_;
    QFileDialog *exportDialog_;
};
