#include "chewingeditor.h"
#include "ui_chewingeditor.h"

ChewingEditor::ChewingEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChewingEditor)
{
    ui->setupUi(this);
}

ChewingEditor::~ChewingEditor()
{
    delete ui;
}
