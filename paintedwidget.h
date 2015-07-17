#ifndef PAINTEDWIDGET
#define PAINTEDWIDGET

#include <QTextEdit>

class PaintedWidget : public QTextEdit
{
    Q_OBJECT
public:
    PaintedWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // PAINTEDWIDGET

