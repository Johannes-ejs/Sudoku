#include "rank.h"
#include "ui_rank.h"
#include "endgame.h"
#include "waitscreengm.h"
#include "waitscreen.h"
#include <QVector>
#include <QString>
#include <map>
#include <QDebug>

Rank::Rank (QVector<pair<QString,int>> ranking,int num_round, int num_round_now, bool is_GM, int time,int level,int code, QString player_name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rank),
    ranking(ranking),
    num_round(num_round),
    num_round_now(num_round_now),
    is_GM(is_GM),
    time(time),
    level(level),
    code(code),
    player_name(player_name)
{
//    for (int i = 0; i < 7; i++) {
//        players[i] = nullptr;
//        pontuations[i] = nullptr;
//    }
    qDebug("innn");
    ranking.push_back({"asfora",500});
    ranking.push_back({"asford",5200});
    ranking.push_back({"asfa",5200});
    ranking.push_back({"asfo",200});
    ui->setupUi(this);
    qDebug("innn2");
    for(int i=1;i<=ranking.size();i++){
        QString playerLabel = QString("player_%1").arg(i);
        QLabel *player_label = findChild<QLabel*>(playerLabel);
        if (player_label) {
            player_label->setText(ranking[i-1].first);
            player_label->setStyleSheet("background-color: white; border: 1px solid black; border-radius: 5px; color: black;");
        }
        QString pontuationLabel = QString("pontuation_%1").arg(i);
        QLabel *pontuation_label = findChild<QLabel*>(pontuationLabel);
        if (pontuation_label) {
            pontuation_label->setText(QString::number(ranking[i-1].second));
            pontuation_label->setStyleSheet("background-color: white; border: 1px solid black; border-radius: 5px; color: black;");
        }
        else{
        qDebug("innn3");
        }
    }
}

Rank::~Rank()
{
    delete ui;
}

void Rank::on_continue_2_clicked()
{
    if(num_round_now==num_round){
        EndGame *endgame=new(EndGame);
        endgame->show();
        close();
    }
    else{
        num_round_now++;
        qDebug("aaaa1");
        if(is_GM){
            qDebug()<<ranking.size();
            WaitScreenGm *waitGM = new WaitScreenGm(ranking,ranking.size(),num_round,num_round_now,time,code,level);
            waitGM->show();
            close();
        }
        else{
            WaitScreen *wait = new WaitScreen(num_round,num_round_now,player_name,code);
            wait->show();
            close();
        }
    }

}

