#include "homeclient.h"
#include "ui_homeclient.h"
#include "waitscreen.h"

HomeClient::HomeClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeClient)
{
    ui->setupUi(this);
}

HomeClient::~HomeClient()
{
    delete ui;
}

void HomeClient::on_criarsala_clicked()
{
    player_name = (ui->player_name->toPlainText());
    code=(ui->code->toPlainText());


    //    create_game(A);
    //interessante: definir aqui a matriz!!!
    //    MainWindow *w=new(MainWindow(num_players,num_round,player_name,time,code));
    initial_conection();
}

int HomeClient::initial_conection(){
    //if retorno é 0 não encontrou servidor
    //if retorno é 1 encontrou o servidor e conectou
    //if retorno é 2 encontrou e não conseguiu conectar
    if(1){
        WaitScreen *w=new WaitScreen(0,player_name,code.toInt());
        w->show();
        close();
    }
}

