#include "RemoveUserphraseDialog.h"
#include "ui_RemoveUserphraseDialog.h"

#include <QDebug>

RemoveUserphraseDialog::RemoveUserphraseDialog(QWidget *parent)
    :QDialog(parent)
    ,ui_(new Ui::RemoveUserphraseDialog)
{
    ui_.get()->setupUi(this);
}

RemoveUserphraseDialog::~RemoveUserphraseDialog()
{
}
