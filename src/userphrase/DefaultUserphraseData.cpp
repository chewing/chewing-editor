#include "DefaultUserphraseData.h"

#include <cstdio>

#include <QDebug>

static void logger(void *data, int level, const char *fmt, ...)
{
    va_list list;
    int len;

    va_start(list, fmt);
    len = vsnprintf(NULL, 0, fmt, list);
    va_end(list);

    std::vector<char> buf(len);

    va_start(list, fmt);
    len = vsnprintf(&buf[0], buf.size(), fmt, list);
    va_end(list);

    qDebug() << QString::fromUtf8(&buf[0]);
}

DefaultUserphraseData::DefaultUserphraseData(const char* path)
:ctx_(chewing_new2(nullptr, path, logger, nullptr), chewing_delete)
{
    if (!ctx_) {
        // FIXME: Report error here
        qDebug() << "chewing_new2 fails";
    }
    refresh();
}


void DefaultUserphraseData::refreshImpl()
{
    std::vector<Userphrase> userphrase;

    std::vector<char> phrase;
    unsigned int phrase_len;

    std::vector<char> bopomofo;
    unsigned int bopomofo_len;

    chewing_userphrase_enumerate(ctx_.get());
    while (chewing_userphrase_has_next(ctx_.get(), &phrase_len, &bopomofo_len)) {
        phrase.resize(phrase_len);
        bopomofo.resize(bopomofo_len);

        int ret = chewing_userphrase_get(ctx_.get(),
            &phrase[0], phrase.size(),
            &bopomofo[0], bopomofo.size());
        if (ret == -1) {
            qDebug() << "chewing_userphrase_get() returns " << ret;
            continue;
        }

        userphrase.push_back(Userphrase{
            std::string(&phrase[0]),
            std::string(&bopomofo[0])
        });

    }

    userphrase_.swap(userphrase);
}

void DefaultUserphraseData::saveImpl()
{
}

bool DefaultUserphraseData::removeImpl(const std::vector<size_t> &index)
{
    bool success = true;
    for (auto i: index) {
        auto item = userphrase_.begin() + i;

        int ret = chewing_userphrase_remove(
            ctx_.get(),
            item->phrase_.c_str(),
            item->bopomofo_.c_str());
        qDebug() << "chewing_userphrase_remove returns = " << ret;

        if (ret == -1) {
            success = false;
        }
    }
    refresh();

    return success;
}
