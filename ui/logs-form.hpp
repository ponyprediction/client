#ifndef LOGSFORM_HPP
#define LOGSFORM_HPP

#include <QWidget>

namespace Ui {
  class LogsForm;
}

class LogsForm : public QWidget
{
    friend class MainWindow;
    Q_OBJECT
  public:
    explicit LogsForm(QWidget *parent = 0);
    ~LogsForm();
  private:
    Ui::LogsForm * ui;
};

#endif // LOGSFORM_HPP
