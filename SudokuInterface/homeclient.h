#ifndef HOMECLIENT_H
#define HOMECLIENT_H

#include <QDialog>

namespace Ui {
class HomeClient;
}

class HomeClient : public QDialog
{
    Q_OBJECT
    QString player_name;
    QString code;

public:
    explicit HomeClient(QWidget *parent = nullptr);
    ~HomeClient();
    int initial_conection();

private slots:
    void on_criarsala_clicked();

private:
    Ui::HomeClient *ui;
};

#endif // HOMECLIENT_H
