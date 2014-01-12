#pragma once

#include <memory>

#include <QMainWindow>

#include "UserphraseModel.h"

namespace Ui {
    class ChewingEditor;
}

class ChewingEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChewingEditor(QWidget *parent = 0);
    ChewingEditor(const ChewingEditor&) = delete;
    ChewingEditor& operator=(const ChewingEditor&) = delete;
    ~ChewingEditor();

private:
    std::unique_ptr<Ui::ChewingEditor> ui_;
    std::unique_ptr<UserphraseModel> model_;
};
