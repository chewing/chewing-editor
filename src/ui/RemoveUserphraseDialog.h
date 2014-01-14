#pragma once

#include <memory>

#include <QDialog>

namespace Ui {
    class RemoveUserphraseDialog;
}

class RemoveUserphraseDialog final: public QDialog
{
    Q_OBJECT

public:
    explicit RemoveUserphraseDialog(QWidget *parent = 0);
    RemoveUserphraseDialog(const RemoveUserphraseDialog&) = delete;
    const RemoveUserphraseDialog& operator=(const RemoveUserphraseDialog&) = delete;
    ~RemoveUserphraseDialog(); // = default;

private:
    std::unique_ptr<Ui::RemoveUserphraseDialog> ui_;
    int index_;
};
