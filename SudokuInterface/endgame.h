#ifndef ENDGAME_H
#define ENDGAME_H

#include <QDialog>

namespace Ui {
class EndGame;
}

class EndGame : public QDialog
{
    Q_OBJECT

public:
    explicit EndGame(QWidget *parent = nullptr);
    ~EndGame();

private:
    Ui::EndGame *ui;
};

#endif // ENDGAME_H
