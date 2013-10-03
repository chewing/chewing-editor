#pragma once

#include <memory>
#include <vector>

#include <QAbstractTableModel>

#include <chewing.h>

class ChewingUserphraseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ChewingUserphraseModel();
    ~ChewingUserphraseModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void refresh();

private:
    struct ChewingUserphrase {
        std::vector<char> phrase;
        std::vector<char> bopomofo;
    };

    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
    std::vector<ChewingUserphrase> data_;
};
