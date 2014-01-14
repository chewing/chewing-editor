#include "ChewingEditor.h"
#include "ui_ChewingEditor.h"

#include <QtGui>
#include <QDebug>

ChewingEditor::ChewingEditor(QWidget *parent)
    :QMainWindow(parent)
    ,ui_(new Ui::ChewingEditor)
    ,model_(UserphraseModelFactory())
    ,removeUserphraseDialog_(new RemoveUserphraseDialog(parent))
    ,selectedIndex_(-1)
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
    if (result == QDialog::Accepted) {
        model_.get()->remove(selectedIndex_);
        selectedIndex_ = -1;
    }

}

void ChewingEditor::setupConnect()
{
    connect(
        // Cannot use SIGNAL here due to lambda.
        ui_.get()->userphraseView, &QListView::pressed,
        [this](const QModelIndex &index) {
            selectedIndex_ = index.row();
            removeUserphraseDialog_.get()->exec();
        }
    );

    connect(
        removeUserphraseDialog_.get(), SIGNAL(finished(int)),
        this, SLOT(finishRemoveUserphraseDialog(int))
    );
}
