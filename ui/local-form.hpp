#ifndef LOCALFORM_H
#define LOCALFORM_H

#include <QWidget>

namespace Ui {
class LocalForm;
}

class LocalForm : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit LocalForm(QWidget *parent = 0);
    ~LocalForm();

private:
    Ui::LocalForm *ui;
};

#endif // LOCALFORM_H
