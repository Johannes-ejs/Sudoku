#include "homescreen.h"
#include "ui_homescreen.h"
#include "homegm.h"
#include "homeclient.h"

HomeScreen::HomeScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeScreen)
{
    ui->setupUi(this);
}

HomeScreen::~HomeScreen()
{
    delete ui;
}

void HomeScreen::on_pushButton_clicked()
{
    HomeGm *home=new(HomeGm);
    home->show();
    close();
}


void HomeScreen::on_pushButton_2_clicked()
{
    HomeClient *home=new(HomeClient);
    home->show();
    close();
}

