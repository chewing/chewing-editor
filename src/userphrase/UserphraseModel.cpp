#include "UserphraseModel.h"

#include <QDebug>

UserphraseModel::UserphraseModel(UserphraseData *data)
:data_(data)
{
}

int UserphraseModel::rowCount(const QModelIndex &parent) const
{
    return data_.get()->size();
}

QVariant UserphraseModel::data(const QModelIndex &index, int role) const
{
    const auto& phrase = data_.get()->get(index.row());

    // FIXME: QString shall be cached.
    QString str = QString("%1 (%2)")
            .arg(QString::fromStdString(phrase.phrase_))
            .arg(QString::fromStdString(phrase.bopomofo_));

    switch (role) {
    case Qt::DisplayRole:
        return str;
    case Qt::WhatsThisRole: // FIXME: Provide "What's This?"
        break;
    default:
        break;
    }

    return QVariant();
}

bool UserphraseModel::remove(const std::vector<size_t> &index)
{
    return data_.get()->remove(index);
}
