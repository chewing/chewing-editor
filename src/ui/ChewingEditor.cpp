#include "ChewingEditor.h"
#include "ui_ChewingEditor.h"

#include <QtGui>
#include <QDebug>

ChewingEditor::ChewingEditor(QWidget *parent)
    :QMainWindow(parent)
    ,ui_(new Ui::ChewingEditor)
    ,model_(UserphraseModelFactory())
{
    ui_.get()->setupUi(this);
    ui_.get()->userphraseView->setModel(model_.get());

    setupConnect();
}

ChewingEditor::~ChewingEditor()
{
}

void ChewingEditor::remove()
{
    auto selectionModel = ui_.get()->userphraseView->selectionModel();

    if (selectionModel->hasSelection()) {
        bool success = true;

        foreach (auto index, selectionModel->selectedIndexes()) {
            if (!model_->remove(index.row())) {
                success = false;
            }
        }
        selectionModel->reset();

        if (!success) {
            // TODO: notify user about failure
        }
    }
}

void ChewingEditor::setupConnect()
{
    connect(
        ui_.get()->removeButton, SIGNAL(pressed()),
        this, SLOT(remove())
    );
}
