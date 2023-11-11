#ifndef GAME_H
#define GAME_H
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include<chrono>
#define MAX_PLAYERS 7


class Game{
    int data_game[MAX_PLAYERS][9][9];
    int soluction[9][9];
    int level;
    int times_remove;
    int times_add;
    int num_players;
    bool client;
public:
    Game(int level,int num_players);
    Game(const Game& outro);
    Game();
    int play(int num_player, int i, int j,int x);
    //uma função para pegar os valores da matriz
    int initial_data_value(int num_player,int i, int j);
    ~Game();
    void test_sol();
    //uma função para modificar de cá pra lá ->play
private:
    void reset_data(int (&M)[9][9]);
    bool can(int x,int i,int j,int (&M)[9][9]);
    int solve(int i,int j,int (&M)[9][9],bool first);
    bool next(int &x, int &y,int (&M)[9][9]);
};


#endif // GAME_H
