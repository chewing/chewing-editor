#ifndef CHEWINGEDITOR_H
#define CHEWINGEDITOR_H

#include <memory>

#include <QMainWindow>

#include <chewing.h>

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
    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> chewing_context_;
};

#endif // CHEWINGEDITOR_H
