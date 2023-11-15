#ifndef RANK_H
#define RANK_H
#include<QLabel>
#include <QVector>
#include<map>
#include <QDialog>
#include <QString>

using namespace std;
namespace Ui {
class Rank;
}

using namespace std;

class Rank : public QDialog
{
    Q_OBJECT
    int num_round;
    int num_round_now;
    bool is_GM;
    int time;
    int level;
    int code;
    QString player_name;
//    QLabel *players[7];
//    QLabel *pontuations[7];
    QVector <pair<QString,int>> ranking;

public:
    explicit Rank(QVector<pair<QString,int>> ranking,int num_round, int num_round_now,bool is_GM,int time, int level,int code, QString player_name,QWidget *parent = nullptr);
    ~Rank();

private slots:
    void on_continue_2_clicked();

private:
    Ui::Rank *ui;
};

#endif // RANK_H
