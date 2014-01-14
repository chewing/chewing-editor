#include "ChewingEditor.h"
#include "ui_ChewingEditor.h"

#include <QtGui>
#include <QDebug>

ChewingEditor::ChewingEditor(QWidget *parent)
    :QMainWindow(parent)
    ,ui_(new Ui::ChewingEditor)
    ,model_(UserphraseModelFactory())
    ,removeUserphraseDialog_(new RemoveUserphraseDialog(parent))
{
    ui_.get()->setupUi(this);
    ui_.get()->userphraseView->setModel(model_.get());

    setupConnect();
}

ChewingEditor::~ChewingEditor()
{
}

void ChewingEditor::finishRemoveUserphraseDialog(int result)
{
    qDebug() << __func__ << "result = " << result;

    auto selectionModel = ui_.get()->userphraseView->selectionModel();

    if (result == QDialog::Accepted && selectionModel->hasSelection()) {
        // TODO: Start to remove item.
        selectionModel->reset();
    }
}

void ChewingEditor::setupConnect()
{
    connect(
        ui_.get()->removeButton, SIGNAL(pressed()),
        removeUserphraseDialog_.get(), SLOT(exec())
    );

    connect(
        removeUserphraseDialog_.get(), SIGNAL(finished(int)),
        this, SLOT(finishRemoveUserphraseDialog(int))
    );
}
