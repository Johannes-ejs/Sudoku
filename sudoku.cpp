#include "game.h"

using namespace std;

Game::Game(int level, int num_players) : level(level), num_players(num_players)
{
    // aqui precisamos gerar um jogo possivel de se vencer
    srand(time(nullptr));
    client = false;
    if (level == 0)
    {
        times_remove = 1;
    }
    if (level == 1)
    {
        times_remove = 3;
    }
    if (level == 2)
    {
        times_remove = 10;
    }
    int M[9][9];
    reset_data(M);
    solve(0, 0, M, true);
    //    test_sol(); até aqui funciona
    int i, j;
    for (int m = 0; m < 9; m++)
        for (int n = 0; n < 9; n++)
            M[m][n] = soluction[m][n];

    while (times_remove)
    {
        do
        {
            do
            {
                i = rand() % 9;
                j = rand() % 9;
            } while (M[i][j] == 0);
            M[i][j] = 0;
        } while (solve(0, 0, M, false) == 1);
        M[i][j] = soluction[i][j];
        times_remove--;
    }
    int num_zeros = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (M[i][j] == 0)
                num_zeros++;
    if (level == 0)
    {
        times_add = num_zeros / 10;
    }
    if (level == 1)
    {
        times_add = num_zeros / 20;
    }
    if (level == 2)
    {
        times_add = 0;
    }
    while (times_add--)
    {
        do
        {
            i = rand() % 9;
            j = rand() % 9;
        } while (M[i][j] != 0);
        M[i][j] = soluction[i][j];
    }
    for (int i = 0; i < num_players; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int m = 0; m < 9; m++)
            {
                data_game[i][j][m] = M[j][m];
            }
        }
    }
}

Game::Game(const Game &outro) : level(outro.level), num_players(outro.num_players)
{
    client = outro.client;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int l = 0; l < num_players; l++)
            {
                this->data_game[l][i][j] = outro.data_game[l][i][j];
            }
            soluction[i][j] = outro.soluction[i][j];
        }
    }
}

Game::Game()
{
    client = true;
}

Game::~Game()
{
    //    for (int i = 0; i < 9; i++) {
    //        delete[] soluction[i];  // Libera cada linha da matriz
    //    }
    //    delete[] soluction;
}

// void Game::test_sol(){
//     for(int i=0;i<9;i++){
//         qDebug("\n");
//         for(int j=0;j<9;j++){
//             if(soluction[i][j]==0) qDebug("0");
//             if(soluction[i][j]==1) qDebug("1");
//             if(soluction[i][j]==2) qDebug("2");
//             if(soluction[i][j]==3) qDebug("3");
//             if(soluction[i][j]==4) qDebug("4");
//             if(soluction[i][j]==5) qDebug("5");
//             if(soluction[i][j]==6) qDebug("6");
//             if(soluction[i][j]==7) qDebug("7");
//             if(soluction[i][j]==8) qDebug("8");
//             if(soluction[i][j]==9) qDebug("9");
//         }
//     }
// }

int Game::play(int num_player, int i, int j, int x)
{
    int key = 1;
    if (soluction[i][j] == x)
    {
        data_game[num_player][i][j] = x;
        for (int m = 0; m < 9 && key; m++)
        {
            for (int n = 0; n < 9 && key; n++)
            {
                if (data_game[num_player][m][n] == 0)
                    key = 0;
            }
        }
        if (key)
        {
            return 2;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int Game::solution_value(int i, int j)
{
    return soluction[i][j];
}

int Game::initial_data_value(int num_player, int i, int j)
{
    return data_game[num_player][i][j];
}

void Game::reset_data(int (&M)[9][9])
{
    for (int i = 0; i < 9; i++)
        for (int k = 0; k < 9; k++)
            M[i][k] = 0;
}
bool Game::can(int x, int i, int j, int (&M)[9][9])
{
    if (M[i][j] != 0)
        return false;
    for (int k = 0; k < 9; k++)
    {
        if (M[k][j] == x)
            return false;
        if (M[i][k] == x)
            return false;
    }
    int i_bloco = i / 3, j_bloco = j / 3;
    for (int m = 0; m < 3; m++)
    {
        for (int n = 0; n < 3; n++)
        {
            if (M[m + i_bloco * 3][n + j_bloco * 3] == x)
                return false;
        }
    }
    return true;
}
int Game::solve(int i, int j, int (&M)[9][9], bool first)
{
    // ideia:só buscar uma solução qualquer
    int num_sol = 0;
    // se falso significa que chegou ao fim
    if (next(i, j, M) == false)
    {
        if (first)
            for (int a = 0; a < 9; a++)
                for (int b = 0; b < 9; b++)
                    soluction[a][b] = M[a][b];
        return 1;
    }
    vector<int> opcoes;
    for (int k = 0; k < 9; k++)
    {
        opcoes.push_back(k + 1);
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gerador(seed);
    shuffle(opcoes.begin(), opcoes.end(), gerador);
    for (int k = 0; k < 9; k++)
    {
        if (can(opcoes[k], i, j, M) == 1)
        {
            M[i][j] = opcoes[k];
            num_sol += solve(i, j, M, first);
            M[i][j] = 0;
            if (num_sol > 1)
                return 2;
        }
    }
    return num_sol;
}
bool Game::next(int &x, int &y, int (&M)[9][9])
{
    while (true)
    {
        if (M[x][y] == 0)
        {
            return true;
        }
        if (x == 8 && y == 8)
            return false;
        if (x == 8)
        {
            x = 0;
            y++;
        }
        else
            x++;
    }
}

int main(int argc, char **argv)
{
    Game jogo(argv[1][0] - '0', 1);
    for (int k = 0; k < 2; k++){
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                if (k == 0)
                    cout << jogo.initial_data_value(0, i, j);
                else
                    cout << jogo.solution_value(i, j);
            }
        }
        cout << endl;
    }
    return 0;
}