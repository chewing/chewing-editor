#include "chewingeditor.h"
#include "ui_chewingeditor.h"

ChewingEditor::ChewingEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChewingEditor),
    chewing_context_(chewing_new(), chewing_delete)
{
    ui->setupUi(this);
}

ChewingEditor::~ChewingEditor()
{
    delete ui;
}
