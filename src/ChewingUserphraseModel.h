#pragma once

#include <memory>
#include <vector>

#include <QAbstractListModel>

#include <chewing.h>

class ChewingUserphraseModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ChewingUserphraseModel();
    ~ChewingUserphraseModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void refresh();

private:
    struct ChewingUserphrase {
        std::vector<char> phrase;
        std::vector<char> bopomofo;
        QString display;
    };

    void update_userphrase();

    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
    std::vector<ChewingUserphrase> userphrase_;
};
