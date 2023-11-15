#ifndef HOMEGM_H
#define HOMEGM_H
#include <QDialog>
#include <QString>

namespace Ui {
class HomeGm;
}

class HomeGm : public QDialog
{
    Q_OBJECT
    int code;
    int level;
    bool level_select;

public:
    explicit HomeGm(QWidget *parent = nullptr);
    ~HomeGm();
    void generate_code();

private slots:
    void on_criarsala_clicked();

    void on_level_0_clicked();

    void on_level_1_clicked();

    void on_level_2_clicked();

private:
    Ui::HomeGm *ui;
};

#endif // HOMEGM_H
