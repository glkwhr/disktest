#ifndef IOZONE_PARAMWIDGET_H
#define IOZONE_PARAMWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

struct iozone_param_struct{
    int file_size;
    bool write_checked;
    bool read_checked;
    bool random_checked;
    QString file_posi;
};

class iozoneParamWidget : public QWidget
{
    Q_OBJECT
public:
    iozoneParamWidget(QWidget *parent = 0);
    struct iozone_param_struct * getParamData();

private:
    QLabel * file_size_label;
    QSpinBox * file_size_qspinbox;
    QSlider * file_size_qslider;

    QGroupBox * op_select;
    QCheckBox * write_qcheckbox;
    QCheckBox * read_qcheckbox;
    QCheckBox * random_qcheckbox;

    QLabel * file_posi_label;
    QLineEdit * file_posi_qlineedit;
    QPushButton * file_posi_select_button;
};

#endif // IOZONE_PARAMWIDGET_H

