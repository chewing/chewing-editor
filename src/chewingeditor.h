#ifndef CHEWINGEDITOR_H
#define CHEWINGEDITOR_H

#include <QMainWindow>

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
    Ui::ChewingEditor *ui;
};

#endif // CHEWINGEDITOR_H
