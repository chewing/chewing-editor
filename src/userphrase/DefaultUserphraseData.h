#pragma once

#include <memory>

#include <chewing.h>

#include "UserphraseData.h"

class DefaultUserphraseData final : public UserphraseData {
public:
    explicit DefaultUserphraseData(const char *path = nullptr);
    DefaultUserphraseData(const DefaultUserphraseData&) = delete;
    DefaultUserphraseData& operator=(const DefaultUserphraseData&) = delete;
    virtual ~DefaultUserphraseData() = default;

private:
    virtual void refreshImpl() override;
    virtual void saveImpl() override;
    virtual bool removeImpl(size_t index) override;

    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
};
