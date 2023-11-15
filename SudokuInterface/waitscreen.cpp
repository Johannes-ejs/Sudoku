#include "waitscreen.h"
#include "ui_waitscreen.h"
#include "mainwindow.h"
#include <QVector>
#include <QString>
#include <map>

WaitScreen::WaitScreen(int num_round,int num_round_now,QString player_name,int code,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitScreen),
    num_round(num_round),
    num_round_now(num_round_now),
    player_name(player_name),
    code(code)
{
    ui->setupUi(this);
    tempo = new QTimer(this);
    connect(tempo,SIGNAL(timeout()),this,SLOT(new_time()));
    tempo->start(600);
}

WaitScreen::~WaitScreen()
{
    delete ui;
}



void WaitScreen:: new_time(){
    static int num_points=1;
    switch (num_points) {
    case 1:
        ui->wait_points->setText(".");
        num_points++;
        break;
    case 2:
        ui->wait_points->setText(". .");
        num_points++;
        break;
    case 3:
        ui->wait_points->setText(". . .");
        num_points++;
        break;
    case 4:
        ui->wait_points->setText(". . . .");
        num_points++;
        break;
    case 5:
        ui->wait_points->setText(". . . . .");
        num_points=1;
        break;
    }
    if(define_player_game()){
        //não precisa mais do code, num_player, time, num_player
        qDebug() << "iinnn";
        qDebug()<<num_round<<"-"<<num_round_now<<"-"<<player_name<<"-"<<time;
        MainWindow *w=new MainWindow(game_data,solution_data,num_round,num_round_now,player_name,time,false);
        w->show();
        qDebug() << "iinnn";
        tempo->stop();
        close();
    }
}

bool WaitScreen::define_player_game(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            game_data[i][j]=0;
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            solution_data[i][j]=i;
        }
    }
    time=10;
    //nessa função ele precisa obter o game_data,time, solution_data,e se o jogo deve ser iniciado que é o que ele retorna

    //em cada conexão ele pega todas essas informações
    return true;
}

