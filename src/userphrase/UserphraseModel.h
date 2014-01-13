#pragma once

#include <memory>

#include <QAbstractListModel>

#include "UserphraseData.h"

class UserphraseModel: public QAbstractListModel {
    Q_OBJECT

public:
    explicit UserphraseModel(UserphraseData *data);
    UserphraseModel(const UserphraseModel& x) = delete;
    UserphraseModel& operator=(const UserphraseModel& x) = delete;
    virtual ~UserphraseModel() = default;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    std::unique_ptr<UserphraseData> data_;
};
