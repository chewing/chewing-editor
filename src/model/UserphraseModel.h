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

#include <QAbstractListModel>
#include <QString>

#include <chewing.h>

#include "Userphrase.h"
#include "UserphraseExporter.h"
#include "UserphraseImporter.h"
#include "UserphraseSet.h"

class UserphraseModel final: public QAbstractListModel {
    Q_OBJECT

public:
    UserphraseModel(QObject * parent = 0, const char *path = nullptr);
    UserphraseModel(const UserphraseModel& x) = delete;
    UserphraseModel& operator=(const UserphraseModel& x) = delete;
    virtual ~UserphraseModel() = default;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const Userphrase *getUserphrase(const QModelIndex& idx);

    int getAddResult() const {return addresult_;}
signals:
    void importCompleted(
        bool result,
        const QString& file,
        size_t imported,
        size_t total);
    void exportCompleted(
        bool result,
        const QString& file,
        size_t exported);
    void addNewPhraseCompleted(const Userphrase& userphrase);
    void removePhraseCompleted(size_t count);
    void refreshCompleted(size_t count);
    void addNewPhraseFailed();    
    void undoCompleted(const QString &phrase);

public slots:
    void refresh();
    void add(std::shared_ptr<QString> phrase, std::shared_ptr<QString> bopomofo);
    void importUserphrase(std::shared_ptr<UserphraseImporter> importer);
    void exportUserphrase(std::shared_ptr<UserphraseExporter> exporter);
    void remove(QModelIndexList indexList);
    void undo();

private:
    void add(const QString &phrase, const QString &bopomofo);
    QString checkBopomofo(const QString &bopomofo) const;

    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
    UserphraseSet userphrase_;
    std::vector<Userphrase> removerecord_;
    int addresult_;
};
