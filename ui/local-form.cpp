#include "local-form.hpp"
#include "ui_local-form.h"

LocalForm::LocalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalForm)
{
    ui->setupUi(this);
}

LocalForm::~LocalForm()
{
    delete ui;
}
