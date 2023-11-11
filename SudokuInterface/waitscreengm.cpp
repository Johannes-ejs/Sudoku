#include "waitscreengm.h"
#include "ui_waitscreengm.h"
#include "mainwindow.h"
#include <QDebug>

using namespace std;

WaitScreenGm::WaitScreenGm(int num_players,int num_round,int num_round_now,QString player_name,int time,int code,int level,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitScreenGm),
    num_players(num_players),
    num_round(num_round),
    num_round_now(num_round_now),
    player_name(player_name),
    time(time),
    code(code),
    level(level),
    num_player(0),
    game(level,num_players),
    num_players_now(1)
{
    for(int i=0;i<9;i++) for(int j=0;j<9;j++) game_data[i][j]=game.initial_data_value(num_player,i,j);
    tempo = new QTimer(this);
    connect(tempo,SIGNAL(timeout()),this,SLOT(new_time()));
    tempo->start(600);
    ui->setupUi(this);
    //create_game();
    //dentro da função que muda os pontinhos ele aceita solicitação de outros e envia a matriz e os dados
}

WaitScreenGm::~WaitScreenGm()
{
    delete ui;
}

void WaitScreenGm::on_pushButton_clicked()
{
    num_players=num_players_now;
    MainWindow *w=new MainWindow(game_data,num_players,num_round,num_round_now,player_name,time,level,num_player,game);
    w->show();
    tempo->stop();
    close();
}

void WaitScreenGm:: new_time(){
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
    if(num_players_now==num_players){
        //alem disso a condição do booleano de ter passado corretamente as informações da matriz data para os clientes
        ui->pushButton->click();
    }
    ui->num_atual_players_2->setText(QString::number(num_players_now));
}

