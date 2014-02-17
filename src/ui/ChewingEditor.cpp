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

void ChewingEditor::setupConnect()
{
    connect(
        ui_.get()->removeButton, SIGNAL(pressed()),
        ui_.get()->userphraseView, SLOT(remove())
    );
}
