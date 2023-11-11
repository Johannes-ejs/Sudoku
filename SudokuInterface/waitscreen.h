#ifndef WAITSCREEN_H
#define WAITSCREEN_H

#include <QDialog>

namespace Ui {
class WaitScreen;
}

class WaitScreen : public QDialog
{
    Q_OBJECT
    int num_players;
    int game_data[9][9];
    int num_round;
    int num_round_now;
    QString player_name;
    int time;
    int code;
    int num_player;
    QTimer *tempo;

public:
    explicit WaitScreen(int num_round_now,QString player_name,int code,QWidget *parent = nullptr);
    ~WaitScreen();
    bool define_player_game();
private slots:
    void new_time();

private:
    Ui::WaitScreen *ui;
};

#endif // WAITSCREEN_H
