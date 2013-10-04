#include "ChewingUserphraseModel.h"

#include <cstdio>

#include <QDebug>

static void logger(void *data, int level, const char *fmt, ...)
{
    va_list list;
    int len;

    va_start(list, fmt);
    len = std::vsnprintf(NULL, 0, fmt, list);
    va_end(list);

    std::vector<char> buf(len);

    va_start(list, fmt);
    len = std::vsnprintf(&buf[0], buf.size(), fmt, list);
    va_end(list);

    qDebug() << QString::fromUtf8(&buf[0]);
}

ChewingUserphraseModel::ChewingUserphraseModel() :
    ctx_(chewing_new(), chewing_delete)
{
    // FIXME:: Handle chewing_new() fail here. Popup might be a good idea.
    chewing_set_logger(ctx_.get(), logger, nullptr);
    update_userphrase();
}

ChewingUserphraseModel::~ChewingUserphraseModel()
{
}

int ChewingUserphraseModel::rowCount(const QModelIndex &parent) const
{
    return userphrase_.size();
}

QVariant ChewingUserphraseModel::data(const QModelIndex &index, int role) const
{
    auto& current_userphrase = userphrase_[index.row()];

    switch (role) {
    case Qt::DisplayRole:
        return current_userphrase.display;
        break;
    case Qt::WhatsThisRole: // FIXME: Provide "What's This?"
        break;
    default:
        break;
    }

    return QVariant{};
}

void ChewingUserphraseModel::refresh()
{
    update_userphrase();
    // FIXME: notify view
}

void ChewingUserphraseModel::update_userphrase()
{
    std::vector<ChewingUserphrase> new_userphrase;
    unsigned int phrase_len;
    unsigned int bopomofo_len;

    chewing_userphrase_enumerate(ctx_.get());
    while (chewing_userphrase_has_next(ctx_.get(), &phrase_len, &bopomofo_len)) {
        ChewingUserphrase current_userphrase;
        current_userphrase.phrase.resize(phrase_len);
        current_userphrase.bopomofo.resize(bopomofo_len);

        int ret = chewing_userphrase_get(ctx_.get(),
            &current_userphrase.phrase[0], current_userphrase.phrase.size(),
            &current_userphrase.bopomofo[0], current_userphrase.bopomofo.size());
        if (ret == -1) {
            qDebug() << "chewing_userphrase_get() returns " << ret;
            continue;
        }

        current_userphrase.display = QString("%1 (%2)")
            .arg(QString::fromUtf8(&current_userphrase.phrase[0]))
            .arg(QString::fromUtf8(&current_userphrase.bopomofo[0]));

        new_userphrase.push_back(std::move(current_userphrase));
    }

    userphrase_.swap(new_userphrase);
}
