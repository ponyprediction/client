#include "connection-form.hpp"
#include "ui_connection-form.h"

ConnectionForm::ConnectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionForm)
{
    ui->setupUi(this);
    ui->lineEditUsername->setPlaceholderText("Username");
    ui->lineEditPassword->setPlaceholderText("Password");
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->buttonDisconnect->setVisible(false);
}

ConnectionForm::~ConnectionForm()
{
    delete ui;
}
