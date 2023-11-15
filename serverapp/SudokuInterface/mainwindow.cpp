#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(int (&game_data)[9][9], int (&solution_data)[9][9], int num_round,int num_round_now, QString player_name,int time, bool is_GM, int level,int num_players, int code, QVector<pair<QString,int>> ranking, Game game,QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    num_round(num_round),
    num_round_now(num_round_now),
    player_name(player_name),
    time(time),
    is_GM(is_GM),
    level(level),
    num_players(num_players),
    code(code),
    ranking(ranking),
    pontuation(0)
{
    num_player=0;

    qDebug()<<"iin11";
    for(int m=0;m<9;m++) for(int l=0;l<9;l++) data[m][l]=game_data[m][l];
    for(int m=0;m<9;m++) for(int l=0;l<9;l++) solution[m][l]=solution_data[m][l];
    ui->setupUi(this);
    for(int i=0;i<9;i++){
        for (int j = 0; j < 9; j++) {
            QString buttonName = QString("button_%1%2").arg(i).arg(j);
            buttons[i][j] = findChild<QPushButton*>(buttonName);

            if (buttons[i][j]) {
                connect(buttons[i][j], SIGNAL(clicked()), this, SLOT(choose_position()));
                buttons[i][j]->setStyleSheet("QPushButton { background-color: white; border: 1px solid lightgray; border-radius: 5px; color: black; }");
                QFont buttonFont = buttons[i][j]->font();
                buttonFont.setPointSize(20);
                buttons[i][j]->setFont(buttonFont);
            }
        }
    }
    keyToButtonMap[Qt::Key_1] = ui->number_1;
    keyToButtonMap[Qt::Key_2] = ui->number_2;
    keyToButtonMap[Qt::Key_3] = ui->number_3;
    keyToButtonMap[Qt::Key_4] = ui->number_4;
    keyToButtonMap[Qt::Key_5] = ui->number_5;
    keyToButtonMap[Qt::Key_6] = ui->number_6;
    keyToButtonMap[Qt::Key_7] = ui->number_7;
    keyToButtonMap[Qt::Key_8] = ui->number_8;
    keyToButtonMap[Qt::Key_9] = ui->number_9;
    keyToButtonMap[Qt::Key_Return] = ui->enter_box;
    keyToButtonMap[Qt::Key_Enter] = ui->enter_box;
    keyToButtonMap[Qt::Key_Backspace] = ui->delete_box;
    for(int l=1;l<=9;l++){
        QString buttonName = QString("number_%1").arg(l);
        number[l-1] = findChild<QPushButton*>(buttonName);
        if(number[l-1]){
            connect(number[l-1], SIGNAL(clicked()), this, SLOT(select_number()));
        }
    }
    connect(ui->delete_box, SIGNAL(clicked()), this, SLOT(delete_select()));
    connect(ui->enter_box,SIGNAL(clicked()),this,SLOT(enter_select()));

    for(int a=0;a<9;a++){
        for(int b=0;b<9;b++){
            if(data[a][b]!=0){
                buttons[a][b]->setStyleSheet("QPushButton { background-color: white; border: 1px solid gray; border-radius: 5px; color: black; }");
                buttons[a][b]->setText(QString::number(data[a][b]));
            }
        }
    }
    tempo = new QTimer(this);
    connect(tempo,SIGNAL(timeout()),this,SLOT(new_time()));
    endTime = QTime::currentTime().addSecs(time);
    tempo->start(1000);
}


void MainWindow::choose_position() {
    QString number_button;
    if(select) number_button=buttons[i][j]->text();
    if(select_view){
        modify_collor(view_number,true);
        select_view=false;
    }
    if((!select)|| (select && number_button==QString(""))){
        if(select){
            buttons[i][j]->setStyleSheet("QPushButton { background-color: white; border: 1px solid lightgray; border-radius: 5px; color: black; }");
        }
        QPushButton* button = qobject_cast<QPushButton*>(sender());

        //ideia:pegar do nome o i e j, fazer o bool.
        //aperta o botao "x"->muda em (i,j) ainda pressionada e trava
        if (button) {
            QString buttonName = button->objectName();
            QStringList parts = buttonName.split("_");
            int aux = parts[1].toInt();
            if(data[aux/10][aux%10]!=0){
                view_number=data[aux/10][aux%10];
                select_view=true;
                modify_collor(view_number,false);
                select=false;
                return;
            }
            i=aux/10;
            j=aux%10;
            select=true;
            button->setStyleSheet("QPushButton { background-color: white; border: 2px solid black; border-radius: 5px; color: black; }");
        }
    }

}

void MainWindow::select_number() {
    if(select){
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString buttonName = button->objectName();
            QStringList parts = buttonName.split("_");
            buttons[i][j]->setText(parts[1]);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (keyToButtonMap.contains(event->key())) {
        QPushButton* button = keyToButtonMap[event->key()];
        button->click();
    }
}


void MainWindow::delete_select()
{
    if(select){
        buttons[i][j]->setText("");
        buttons[i][j]->setStyleSheet("QPushButton { background-color: white; border: 1px solid lightgray; border-radius: 5px; color: black; }");
        select=false;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::enter_select()
{
    if(select_view){
        modify_collor(view_number,true);
        select_view=false;
    }
    QString num_of_box = buttons[i][j]->text();
    if(select && num_of_box!=QString("")){
        if(!client){
            int num_play=play(i,j,num_of_box.toInt());
            if(num_play==0){
                pontuation-=10;
                ui->pontuation->setText(QString::number(pontuation));
                select=false;
                buttons[i][j]->setStyleSheet("QPushButton { background-color: red; border: 1px solid gray; border-radius: 5px; color: black; }");
                red=true;
                return;
            }
            if(num_play==2){
                finished();
                call_rank();

            }
            buttons[i][j]->setStyleSheet("QPushButton { background-color: white; border: 1px solid gray; border-radius: 5px; color: black; }");
            select=false;
            data[i][j]=num_of_box.toInt();
            pontuation+=10;
            ui->pontuation->setText(QString::number(pontuation));
        }
    }
}

void MainWindow::finished(){
    //#nessa função ele envia para o servidor  a informação de que ele acabou

    //fica travado no get_rank até obter o rank
    get_rank();
}

void MainWindow::is_finished(){
    //#nessa função ele verifica se finalizou o jogo com o servidor (recebe a informação de termino com o servidor)
    //caso tenha finalizado ele envia para o servidor a sua pontuação
}

void MainWindow::finished_time(){
    //função evocada quando acaba o tempo, envia a pontuação para o servidor
    get_rank();
}

void MainWindow::get_rank(){
    ranking[0].second=pontuation;
    //função evocada ao se finalizar a rodada para obter o ranking geral do servidor para a próxima tela
}

void MainWindow::new_time(){
     // 3 minutos a partir do tempo atual
    QTime currentTime = QTime::currentTime();
    int remainingSeconds = currentTime.secsTo(endTime);
    if (remainingSeconds <= 0) {
        finished_time();
        qDebug()<<11;
        call_rank();

    } else {
        QString timeText = QString("%1:%2")
            .arg((remainingSeconds % 3600) / 60, 2, 10, QChar('0')) // Minutos
            .arg(remainingSeconds % 60, 2, 10, QChar('0')); // Segundos
        ui->timer1->setText(timeText);
    }
}

void MainWindow::modify_collor(int value,bool white){
    for(int m=0;m<9;m++){
        for(int n=0;n<9;n++){
            if(data[m][n]==value){
                if(white) buttons[m][n]->setStyleSheet("QPushButton { background-color: white; border: 1px solid gray; border-radius: 5px; color: black; }");
                else buttons[m][n]->setStyleSheet("QPushButton { background-color: blue; border: 1px solid gray; border-radius: 5px; color: white; }");
            }
        }
    }
}

void MainWindow:: call_rank(){
    Rank *w=new Rank(ranking,num_round,num_round_now,is_GM, time, level,code,player_name);
    w->show();
    tempo->stop();
    close();
}

int MainWindow::play(int m,int n, int x){
    if(solution[m][n]!=x)return 0;
    else{
        data[m][n]=x;
        for(int l=0;l<9;l++){
            for(int p=0;p<9;p++){
                if(data[l][p]==0){
                    return 1;
                }
            }
        }
        return 2;
    }
}

