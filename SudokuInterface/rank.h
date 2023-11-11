#ifndef RANK_H
#define RANK_H
#include<QLabel>
#include <QVector>
#include<map>
#include <QDialog>

using namespace std;
namespace Ui {
class Rank;
}

class Rank : public QDialog
{
    Q_OBJECT
    int num_round;
    int num_round_now;
//    QLabel *players[7];
//    QLabel *pontuations[7];
    QVector <pair<QString,int>> ranking;

public:
    explicit Rank(QVector<pair<QString,int>> ranking,int num_round, int num_round_now,QWidget *parent = nullptr);
    ~Rank();

private:
    Ui::Rank *ui;
};

#endif // RANK_H
