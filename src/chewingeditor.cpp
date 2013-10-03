#include "chewingeditor.h"
#include "ui_chewingeditor.h"

ChewingEditor::ChewingEditor(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::ChewingEditor),
    ctx_(chewing_new(), chewing_delete)
{
    ui_.get()->setupUi(this);
    // FIXME:: Handle chewing_new() fail here. Popup might be a good idea.
}

ChewingEditor::~ChewingEditor()
{
}
