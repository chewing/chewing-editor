#pragma once

#include <memory>

#include <QMainWindow>

#include "UserphraseModelFactory.h"
#include "RemoveUserphraseDialog.h"

namespace Ui {
    class ChewingEditor;
}

class ChewingEditor final: public QMainWindow
{
    Q_OBJECT

public:
    explicit ChewingEditor(QWidget *parent = 0);
    ChewingEditor(const ChewingEditor&) = delete;
    ChewingEditor& operator=(const ChewingEditor&) = delete;
    ~ChewingEditor(); // = default;

public slots:
    void finishRemoveUserphraseDialog(int result);

private:
    void setupConnect();

    std::unique_ptr<Ui::ChewingEditor> ui_;
    std::unique_ptr<UserphraseModel> model_;
    std::unique_ptr<RemoveUserphraseDialog> removeUserphraseDialog_;

    int selectedIndex_;
};
