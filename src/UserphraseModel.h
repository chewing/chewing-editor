#pragma once

#include <QAbstractListModel>

class UserphraseModel: public QAbstractListModel {
    Q_OBJECT

public:
    UserphraseModel() = default;
    UserphraseModel(const UserphraseModel& userphrase);
    UserphraseModel& operator=(const UserphraseModel& userphrase);
    virtual ~UserphraseModel() = default;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;

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
