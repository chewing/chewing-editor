#include "ChewingUserphraseModel.h"

ChewingUserphraseModel::ChewingUserphraseModel() :
    ctx_(chewing_new(), chewing_delete)
{
    // FIXME:: Handle chewing_new() fail here. Popup might be a good idea.
}

ChewingUserphraseModel::~ChewingUserphraseModel()
{
}
