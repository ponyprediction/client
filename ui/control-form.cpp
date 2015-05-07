#include "control-form.hpp"
#include "ui_control-form.h"
#include "core/util.hpp"

ControlForm::ControlForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlForm)
{
    ui->setupUi(this);
    ui->doubleSpinBoxMutationFrequency->setValue(0.0f);
    ui->doubleSpinBoxMutationIntensity->setValue(0.0f);
}

ControlForm::~ControlForm()
{
    delete ui;
}
