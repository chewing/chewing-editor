#include "chewingeditor.h"
#include "ui_chewingeditor.h"

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
