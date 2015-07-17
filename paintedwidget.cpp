#include "paintedwidget.h"

#include <QPainter>

PaintedWidget::PaintedWidget(QWidget *parent):QTextEdit(parent)
{

}

void PaintedWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this->viewport());
    painter.drawLine(80, 100, 650, 500);
}

