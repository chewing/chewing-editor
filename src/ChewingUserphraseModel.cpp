#include "ChewingUserphraseModel.h"

ChewingUserphraseModel::ChewingUserphraseModel() :
    ctx_(chewing_new(), chewing_delete)
{
    // FIXME:: Handle chewing_new() fail here. Popup might be a good idea.
    refresh();
}

ChewingUserphraseModel::~ChewingUserphraseModel()
{
}

int ChewingUserphraseModel::rowCount(const QModelIndex &parent) const
{
    return data_.size();
}

int ChewingUserphraseModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant ChewingUserphraseModel::data(const QModelIndex &index, int role) const
{
}

void ChewingUserphraseModel::refresh()
{
    std::vector<ChewingUserphrase> new_data;
    unsigned int phrase_len;
    unsigned int bopomofo_len;
    int ret;

    chewing_userphrase_enumerate(ctx_.get());
    while (chewing_userphrase_has_next(ctx_.get(), &phrase_len, &bopomofo_len)) {
        std::vector<char> phrase_buf(phrase_len);
        std::vector<char> bopomofo_buf(bopomofo_len);

        ret = chewing_userphrase_get(ctx_.get(),
            &phrase_buf[0], phrase_buf.size(),
            &bopomofo_buf[0], bopomofo_buf.size());
        if (ret == -1) {
            // FIXME: log error here.
            continue;
        }

        new_data.push_back(ChewingUserphrase{
            std::move(phrase_buf),
            std::move(bopomofo_buf)
        });
    }

    data_.swap(new_data);
}
