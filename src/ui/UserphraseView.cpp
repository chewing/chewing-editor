#include "UserphraseView.h"
#include "UserphraseModel.h"

void UserphraseView::remove()
{
    auto selection = selectionModel();
    auto m = dynamic_cast<UserphraseModel*>(model());

    foreach(auto selected, selection->selectedIndexes()) {
        if (m->remove(selected.row())) {
        }
    }

    selection->reset();
}
