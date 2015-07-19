#include "postmark_paramwidget.h"


#include <QGridLayout>

postmarkParamWidget::postmarkParamWidget(QWidget *parent):QWidget(parent)
{
    file_size_min_label = new QLabel(this);
    file_size_min_label->setText("min file size");
    file_size_min_qspinbox = new QSpinBox(this);
    file_size_min_qslider = new QSlider(Qt::Horizontal, this);
    file_size_min_qspinbox->setRange(0, 50000);
    file_size_min_qslider->setRange(0, 50000);
    connect(file_size_min_qslider, SIGNAL(valueChanged(int)), file_size_min_qspinbox, SLOT(setValue(int)));
    connect(file_size_min_qspinbox, SIGNAL(valueChanged(int)), file_size_min_qslider, SLOT(setValue(int)));
    file_size_min_qspinbox->setValue(50);


    file_size_max_label = new QLabel(this);
    file_size_max_label->setText("max file size");
    file_size_max_qspinbox = new QSpinBox(this);
    file_size_max_qslider = new QSlider(Qt::Horizontal, this);
    file_size_max_qspinbox->setRange(0, 50000);
    file_size_max_qslider->setRange(0, 50000);
    connect(file_size_max_qslider, SIGNAL(valueChanged(int)), file_size_max_qspinbox, SLOT(setValue(int)));
    connect(file_size_max_qspinbox, SIGNAL(valueChanged(int)), file_size_max_qslider, SLOT(setValue(int)));
    file_size_max_qspinbox->setValue(10000);


    read_block_size_label = new QLabel(this);
    read_block_size_label->setText("read block size");
    read_block_size_qspinbox = new QSpinBox(this);
    read_block_size_qslider = new QSlider(Qt::Horizontal, this);
    read_block_size_qspinbox->setRange(0, 5000);
    read_block_size_qslider->setRange(0, 5000);
    connect(read_block_size_qslider, SIGNAL(valueChanged(int)), read_block_size_qspinbox, SLOT(setValue(int)));
    connect(read_block_size_qspinbox, SIGNAL(valueChanged(int)), read_block_size_qslider, SLOT(setValue(int)));
    read_block_size_qspinbox->setValue(50);


    write_block_size_label = new QLabel(this);
    write_block_size_label->setText("write block size");
    write_block_size_qspinbox = new QSpinBox(this);
    write_block_size_qslider = new QSlider(Qt::Horizontal, this);
    write_block_size_qspinbox->setRange(0, 5000);
    write_block_size_qslider->setRange(0, 5000);
    connect(write_block_size_qslider, SIGNAL(valueChanged(int)), write_block_size_qspinbox, SLOT(setValue(int)));
    connect(write_block_size_qspinbox, SIGNAL(valueChanged(int)), write_block_size_qslider, SLOT(setValue(int)));
    write_block_size_qspinbox->setValue(50);


    number_label = new QLabel(this);
    number_label->setText("set number");
    number_qspinbox = new QSpinBox(this);
    number_qslider = new QSlider(Qt::Horizontal, this);
    number_qspinbox->setRange(0, 50000);
    number_qslider->setRange(0, 50000);
    connect(number_qslider, SIGNAL(valueChanged(int)), number_qspinbox, SLOT(setValue(int)));
    connect(number_qspinbox, SIGNAL(valueChanged(int)), number_qslider, SLOT(setValue(int)));
    number_qspinbox->setValue(1000);

    transactions_label = new QLabel(this);
    transactions_label->setText("set transactions");
    transactions_qspinbox = new QSpinBox(this);
    transactions_qslider = new QSlider(Qt::Horizontal, this);
    transactions_qspinbox->setRange(0, 100000);
    transactions_qslider->setRange(0, 100000);
    connect(transactions_qslider, SIGNAL(valueChanged(int)), transactions_qspinbox, SLOT(setValue(int)));
    connect(transactions_qspinbox, SIGNAL(valueChanged(int)), transactions_qslider, SLOT(setValue(int)));
    transactions_qspinbox->setValue(50000);



    isbuffered = new QCheckBox("use buffer io", this);
    isbuffered->setCheckState(Qt::Unchecked);


    file_posi_label = new QLabel(this);
    file_posi_label->setText("file position:");
    //file_posi_qlineedit = new QLineEdit(this);
    //file_posi_qlineedit->setText("/home/yalewoo");
    //file_posi_select_button = new QPushButton("brower", this);


    QGridLayout *layout = new QGridLayout;

    layout->addWidget(file_size_min_label, 0, 0);
    layout->addWidget(file_size_min_qspinbox, 0, 1);
    layout->addWidget(file_size_min_qslider, 0, 2);

    layout->addWidget(file_size_max_label, 1, 0);
    layout->addWidget(file_size_max_qspinbox, 1, 1);
    layout->addWidget(file_size_max_qslider, 1, 2);

    layout->addWidget(read_block_size_label, 2, 0);
    layout->addWidget(read_block_size_qspinbox, 2, 1);
    layout->addWidget(read_block_size_qslider, 2, 2);

    layout->addWidget(write_block_size_label, 3, 0);
    layout->addWidget(write_block_size_qspinbox, 3, 1);
    layout->addWidget(write_block_size_qslider, 3, 2);

    layout->addWidget(number_label, 4, 0);
    layout->addWidget(number_qspinbox, 4, 1);
    layout->addWidget(number_qslider, 4, 2);

    layout->addWidget(transactions_label, 5, 0);
    layout->addWidget(transactions_qspinbox, 5, 1);
    layout->addWidget(transactions_qslider, 5, 2);

    layout->addWidget(isbuffered, 6, 0);

    //layout->addWidget(file_posi_label, 7, 0);
    //layout->addWidget(file_posi_qlineedit, 7, 1);
    //layout->addWidget(file_posi_select_button, 7, 2);

    this->setLayout(layout);
}


struct postmark_param_struct * postmarkParamWidget::getParamData()
{
    struct postmark_param_struct *res = new postmark_param_struct;
    res->file_size_min = file_size_min_qslider->value();
    res->file_size_max = file_size_max_qslider->value();
    res->read_block_size = read_block_size_qslider->value();
    res->write_block_size = write_block_size_qslider->value();
    res->number = number_qslider->value();
    res->transactions = transactions_qslider->value();



    res->use_buffer_checked = isbuffered->isChecked();


    //res->file_posi = file_posi_qlineedit->text();

    return res;
}

