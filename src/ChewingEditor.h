#pragma once

#include <memory>

#include <QMainWindow>

#include "ChewingUserphraseModel.h"

namespace Ui {
    class ChewingEditor;
}

class ChewingEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChewingEditor(QWidget *parent = 0);
    ~ChewingEditor();

private:
    std::unique_ptr<Ui::ChewingEditor> ui_;
    std::unique_ptr<ChewingUserphraseModel> model_;
};
