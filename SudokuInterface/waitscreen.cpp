#include "waitscreen.h"
#include "ui_waitscreen.h"
#include "mainwindow.h"

WaitScreen::WaitScreen(int num_round_now,QString player_name,int code,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitScreen),
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

        MainWindow *w=new MainWindow(game_data,num_players,num_round,num_round_now,player_name,time,0,num_player);
        w->show();
        close();
    }
}

bool WaitScreen::define_player_game(){
    //nessa função ele precisa obter o num_player,o game_data, num_players(final),num_round,time,level e se o jogo deve ser iniciado
    //em cada conexão ele pega todas essas informações
    //a ideia é ele buscar essas informações a cada clock (execução da função new_time())
    //retorna 1 se ele recebeu a informação de que pode prosseguir
}

