#pragma once

#include <memory>
#include <vector>

#include <QAbstractListModel>

class UserphraseModel: public QAbstractListModel {
    Q_OBJECT

public:
    UserphraseModel() = default;
    UserphraseModel(const UserphraseModel& x);
    UserphraseModel& operator=(const UserphraseModel& x);
    virtual ~UserphraseModel() = default;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual void refresh() = 0;
    virtual void save() = 0;

protected:
    struct Userphrase {
        std::vector<char> phrase;
        std::vector<char> bopomofo;
        QString display;
    };

    std::vector<Userphrase> userphrase_;
};

std::unique_ptr<UserphraseModel> UserphraseModelFactory(const char* path = nullptr);
