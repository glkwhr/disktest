#include "iozone_paramwidget.h"


#include <QHBoxLayout>
#include <QVBoxLayout>

iozoneParamWidget::iozoneParamWidget(QWidget *parent):QWidget(parent)
{
    file_size_label = new QLabel(this);
    file_size_label->setText("file size(M)");
    file_size_qspinbox = new QSpinBox(this);
    file_size_qslider = new QSlider(Qt::Horizontal, this);
    file_size_qspinbox->setRange(0, 100);
    file_size_qslider->setRange(0, 100);
    connect(file_size_qslider, SIGNAL(valueChanged(int)), file_size_qspinbox, SLOT(setValue(int)));
    connect(file_size_qspinbox, SIGNAL(valueChanged(int)), file_size_qslider, SLOT(setValue(int)));
    file_size_qspinbox->setValue(16);

    op_select = new QGroupBox("op", this);
    write_qcheckbox = new QCheckBox("write && rewrite", this);
    write_qcheckbox->setCheckState(Qt::Checked);
    read_qcheckbox = new QCheckBox("read && reread", this);
    read_qcheckbox->setCheckState(Qt::Checked);
    random_qcheckbox = new QCheckBox("random read && write", this);
    random_qcheckbox->setCheckState(Qt::Checked);

    file_posi_label = new QLabel(this);
    file_posi_label->setText("file position:");
    file_posi_qlineedit = new QLineEdit(this);
    file_posi_qlineedit->setText("./tmp");
    file_posi_select_button = new QPushButton("brower", this);

    QHBoxLayout * filesizelayout = new QHBoxLayout;
    filesizelayout->addWidget(file_size_label);
    filesizelayout->addWidget(file_size_qspinbox);
    filesizelayout->addWidget(file_size_qslider);

    QVBoxLayout * oplayout = new QVBoxLayout;
    oplayout->addWidget(op_select);
    oplayout->addWidget(write_qcheckbox);
    oplayout->addWidget(read_qcheckbox);
    oplayout->addWidget(random_qcheckbox);

    QHBoxLayout * fileposilayout = new QHBoxLayout;
    fileposilayout->addWidget(file_posi_label);
    fileposilayout->addWidget(file_posi_qlineedit);
    fileposilayout->addWidget(file_posi_select_button);

    QVBoxLayout * mainlayout = new QVBoxLayout;
    mainlayout->addLayout(filesizelayout);
    mainlayout->addLayout(oplayout);
    mainlayout->addLayout(fileposilayout);

    this->setLayout(mainlayout);


}


struct iozone_param_struct * iozoneParamWidget::getParamData()
{
    struct iozone_param_struct *res = new iozone_param_struct;
    res->file_size = file_size_qslider->value();
    res->write_checked = write_qcheckbox->isChecked();
    res->read_checked = read_qcheckbox->isChecked();
    res->random_checked = random_qcheckbox->isChecked();

    res->file_posi = file_posi_qlineedit->text();

    return res;
}
