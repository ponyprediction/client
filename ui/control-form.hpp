#ifndef CONTROLFORM_HPP
#define CONTROLFORM_HPP

#include <QWidget>

namespace Ui {
class ControlForm;
}

class ControlForm : public QWidget
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit ControlForm(QWidget *parent = 0);
    ~ControlForm();

private:
    Ui::ControlForm *ui;
};

#endif // CONTROLFORM_HPP
