#include "ranking.h"
#include "ui_ranking.h"

Ranking::Ranking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ranking)
{
    ui->setupUi(this);
}

Ranking::~Ranking()
{
    delete ui;
}
