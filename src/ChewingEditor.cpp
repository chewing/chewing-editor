#include "ChewingEditor.h"
#include "ui_ChewingEditor.h"

#include <QtGui>

ChewingEditor::ChewingEditor(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::ChewingEditor),
    model_(new ChewingUserphraseModel)
{
    ui_.get()->setupUi(this);
}

ChewingEditor::~ChewingEditor()
{
}
