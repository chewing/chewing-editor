#pragma once

#include <memory>

#include <QtGui>

#include <chewing.h>

class ChewingUserphraseModel : public QStandardItemModel
{
    Q_OBJECT

public:
    ChewingUserphraseModel();
    ~ChewingUserphraseModel();

private:
    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
};
