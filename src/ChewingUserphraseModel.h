#pragma once

#include <memory>

#include <chewing.h>

#include "UserphraseModel.h"

class ChewingUserphraseModel : public UserphraseModel
{
    Q_OBJECT

public:
    ChewingUserphraseModel(const char* path = nullptr);
    ChewingUserphraseModel(const ChewingUserphraseModel& x) = delete;
    ChewingUserphraseModel& operator=(const ChewingUserphraseModel& x) = delete;
    virtual ~ChewingUserphraseModel() = default;

    virtual void refresh();
    virtual void save();

private:
    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
};
