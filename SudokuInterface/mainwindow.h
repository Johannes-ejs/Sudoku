#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "rank.h"
#include "game.h"
#include <QPushButton>
#include <QString>
#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>
#include <map>
#include<QTime>

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int i;
    int j;
    int x;
    int pontuation;
    int num_time;
    int view_number;
    int num_players;
    int num_player;
    int code;
    int num_round;
    int num_round_now;
    int time;
    int level;
    bool select_view =false;
    bool select=false;
    bool red=false;
    bool client;
    bool is_GM;
    int data[9][9];
    int solution[9][9];
    QVector<pair<QString,int>>ranking;
    QTime endTime;
    QString player_name;
    Game game;
    QPushButton *buttons[9][9];
    QPushButton *number[9];
    QTimer *tempo;
    QMap<int, QPushButton*> keyToButtonMap;

public:
    MainWindow(int (&game_data)[9][9],int (&solution_data)[9][9],int num_round,int num_round_now,QString player_name,int time, bool is_GM,int level=0,int num_players=0, int code=0,QVector<pair<QString,int>>ranking=QVector<pair<QString,int>>(), Game game=Game(),QWidget *parent = nullptr);
    ~MainWindow();
    void modify_collor(int value,bool white);
    void call_rank();
    void to_red();
    void finished();
    void finished_time();
    void is_finished();
    int play(int i, int j,int x);
    void get_rank();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void choose_position();
    void select_number();
    void delete_select();
    void enter_select();
    void new_time();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
