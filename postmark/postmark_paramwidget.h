/*postmark测试的参数面板
    成员函数getParamData返回一个结构体 ，
    该结构体包含了参数的信息，供postmark_thread使用
    该结构体使用完后需要手动delete
*/


#ifndef POSTMARK_PARAMWIDGET
#define POSTMARK_PARAMWIDGET

#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

struct postmark_param_struct{
    int file_size_min;
    int file_size_max;
    int read_block_size;
    int write_block_size;
    int number;
    int transactions;

    bool use_buffer_checked;

    QString file_posi;
};

class postmarkParamWidget : public QWidget
{
    Q_OBJECT
public:
    postmarkParamWidget(QWidget *parent = 0);
    struct postmark_param_struct * getParamData();

private:
    QLabel * file_size_min_label;
    QSpinBox * file_size_min_qspinbox;
    QSlider * file_size_min_qslider;

    QLabel * file_size_max_label;
    QSpinBox * file_size_max_qspinbox;
    QSlider * file_size_max_qslider;

    QLabel * read_block_size_label;
    QSpinBox * read_block_size_qspinbox;
    QSlider * read_block_size_qslider;

    QLabel * write_block_size_label;
    QSpinBox * write_block_size_qspinbox;
    QSlider * write_block_size_qslider;

    QLabel * number_label;
    QSpinBox * number_qspinbox;
    QSlider * number_qslider;

    QLabel * transactions_label;
    QSpinBox * transactions_qspinbox;
    QSlider * transactions_qslider;



    QCheckBox * isbuffered;

    QLabel * file_posi_label;
    QLineEdit * file_posi_qlineedit;
    QPushButton * file_posi_select_button;
};

#endif // POSTMARK_PARAMWIDGET

