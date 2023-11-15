#include "endgame.h"
#include "ui_endgame.h"

EndGame::EndGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EndGame)
{
    ui->setupUi(this);
}

EndGame::~EndGame()
{
    delete ui;
}
