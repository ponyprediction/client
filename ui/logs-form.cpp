#include "logs-form.hpp"
#include "ui_logs-form.h"

LogsForm::LogsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogsForm)
{
    ui->setupUi(this);
    ui->plainTextEditLogs->setMaximumBlockCount(1000);
}

LogsForm::~LogsForm()
{
    delete ui;
}
