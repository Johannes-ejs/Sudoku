#ifndef WAITSCREENGM_H
#define WAITSCREENGM_H
#include <QString>
#include "game.h"
#include <QDialog>
#include<QTime>

namespace Ui {
class WaitScreenGm;
}

class WaitScreenGm : public QDialog
{
    Q_OBJECT
    int num_players;
    int num_round;
    int num_round_now;
    QString player_name;
    int time;
    int code;
    int game_data[9][9];
    int num_player=0;
    int level;
    int num_players_now;
    Game game;
    QTimer *tempo;

public:
    explicit WaitScreenGm(int num_players,int num_round,int num_round_now,QString player_name,int time,int code,int level,QWidget *parent=nullptr);
    ~WaitScreenGm();

private slots:
    void on_pushButton_clicked();
    void new_time();

private:
    Ui::WaitScreenGm *ui;
};

#endif // WAITSCREENGM_H
