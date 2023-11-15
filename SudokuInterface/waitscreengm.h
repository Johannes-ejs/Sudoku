#ifndef WAITSCREENGM_H
#define WAITSCREENGM_H
#include <QString>
#include "game.h"
#include <QDialog>
#include<QTime>
#include <QVector>
#include <QString>
#include <map>
#include <QVector>
#include <QString>
#include <map>

namespace Ui {
class WaitScreenGm;
}
using namespace std;

class WaitScreenGm : public QDialog
{
    Q_OBJECT
    QVector<pair<QString,int>> ranking;
    int num_players;
    int num_round;
    int num_round_now;
    QString player_name;
    int time;
    int code;
    int game_data[9][9];
    int solution_data[9][9];
    int level;
    int num_players_now;
    Game game;
    QTimer *tempo;

public:
    explicit WaitScreenGm(QVector<pair<QString,int>> ranking,int num_players,int num_round, int num_round_now,int time,int code ,int level,QWidget *parent=nullptr);
    ~WaitScreenGm();

private slots:
    void on_pushButton_clicked();
    void new_time();

private:
    Ui::WaitScreenGm *ui;
};

#endif // WAITSCREENGM_H
