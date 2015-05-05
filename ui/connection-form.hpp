#ifndef CONNECTIONFORM_HPP
#define CONNECTIONFORM_HPP

#include <QWidget>

namespace Ui {
  class ConnectionForm;
}

class ConnectionForm : public QWidget
{
    friend class MainWindow;
    Q_OBJECT
  public:
    explicit ConnectionForm(QWidget *parent = 0);
    ~ConnectionForm();

  private:
    Ui::ConnectionForm *ui;
};

#endif // CONNECTIONFORM_HPP
