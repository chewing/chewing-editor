#include "UserphraseModelFactory.h"

#include "DefaultUserphraseData.h"

UserphraseModel *UserphraseModelFactory() {
    return new UserphraseModel(new DefaultUserphraseData());
}
