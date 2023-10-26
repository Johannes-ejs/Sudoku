#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include<chrono>
#define MAX_PLAYERS 10

using namespace std;


enum level{
    easy,medium,hard
};

class sudoku{
    int data[MAX_PLAYERS][9][9];
    int soluction[9][9];
    int life[MAX_PLAYERS];
    enum level level;
    int times_remove;
    int times_add;
    int num_players;
    public:
    sudoku(enum level level,int life_all,int num_players): level(level), num_players(num_players){
        //aqui precisamos gerar um jogo possivel de se vencer
        if(level == easy) {times_remove=1;}
        if(level == medium){times_remove=3;}
        if(level == hard){times_remove=10;}
        for(int i=0;i<num_players;i++) life[i]=life_all;
        int M[9][9];
        zerar_data(M);
        solve(0,0,M,true);
        int i,j;
        for(int i=0;i<9;i++) for(int j=0;j<9;j++)M[i][j]=soluction[i][j];
        while(times_remove){
            do{
                do{
                    i=rand()%9;
                    j=rand()%9;
                }while(M[i][j]==0);
                M[i][j]=0;
            }while(solve(0,0,M)==1);
            M[i][j]=soluction[i][j];
            times_remove--;
        }
        int num_zeros=0;
        for(int i=0;i<9;i++) for(int j=0;j<9;j++)if(M[i][j]==0) num_zeros++;
        if(level == easy) {times_add=num_zeros/10;}
        if(level == medium){times_add=num_zeros/20;}
        if(level == hard){times_add=0;}
        while(times_add--){
            do{
                    i=rand()%9;
                    j=rand()%9;
            }while(data[i][j]!=0);
            M[i][j]=soluction[i][j];
        }
        for(int i=0;i<num_players;i++){
            for(int j=0;j<9;j++)for(int m=0;m<9;m++) data[i][j][m]=M[j][m];
        }
    }
    ~sudoku(){};
    void zerar_data(int (&M)[9][9]){
        for(int i=0;i<9;i++) for(int k=0;k<9;k++){
            M[i][k]=0;
        }
    }
    bool can(int x,int i,int j,int (&M)[9][9]){
        if(M[i][j]!=0) return false;
        for(int k=0;k<9;k++){
            if(M[k][j]==x) return false;
            if(M[i][k]==x) return false;
        }
        int i_bloco=i/3,j_bloco=j/3;
        for(int m=0;m<3;m++){
            for(int n=0;n<3;n++){
                if(M[m+i_bloco*3][n+j_bloco*3]==x) return false;
            }
        }
        return true;
    }

    int solve(int i,int j,int (&M)[9][9],bool first=false){
        //ideia:só buscar uma solução qualquer
        int num_sol=0;
        //se falso significa que chegou ao fim
        if(next(i,j,M)==false){
            if(first) for(int a=0;a<9;a++) for(int b=0;b<9;b++) soluction[a][b]=M[a][b];
            return 1;
        }
        vector <int> opcoes;
        for(int k=0;k<9;k++){
            opcoes.push_back(k+1);
        }
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine gerador(seed);
        shuffle(opcoes.begin(),opcoes.end(),gerador);
        for(int k=0;k<9;k++){
            if(can(opcoes[k],i,j,M)==1){
                M[i][j]=opcoes[k];
                num_sol+=solve(i,j,M,first);
                M[i][j]=0;
                if(num_sol>1)return 2;
            }
        }
        return num_sol; 
    }


    bool next(int &x, int &y,int (&M)[9][9]){
        while(true){
            if(M[x][y]==0){
                return true;
            }
            if(x==8 and y==8) return false;
            if(x==8){
                x=0;
                y++;
            }
            else x++;
        }
    }
    void print(int num_player){
        cout<<endl<<"NUMERO DE VIDAS: ";
        for(int i=0;i<life[num_player-1];i++)
            cout<<(char)3<<" ";
        cout<<endl<<endl<<"    1    2    3    4    5    6    7    8    9"<<endl;
        // cout<<"   ___________________________________________"<<endl;
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(j==0){
                    if(data[num_player-1][i][j]!=0)cout<<i+1<<"  |"<<data[num_player-1][i][j]<<"|_";
                    else cout<<i+1<<"  |"<<(char)254<<"|_";
                }
                else if(j==8){
                    if(data[num_player-1][i][j]!=0)cout<<"_|"<<data[num_player-1][i][j]<<"|";
                    else cout<<"_|"<<(char)254<<"|";
                }
                else{
                    if(data[num_player-1][i][j]!=0)cout<<"_|"<<data[num_player-1][i][j]<<"|_";
                    else cout<<"_|"<<(char)254<<"|_";
                }
            }
            cout<<endl;
        }
    }
    void print_sol(){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                cout<<"   "<<soluction[i][j]<<"   ";
            }
            cout<<endl;
        }
    }
    void play(int num_player, int i, int j,int x){
        if(data[num_player-1][i-1][j-1]!=0){
            cout<<endl<<endl<<"ESSE LUGAR JA ESTAVA OCUPADO"<<endl<<endl;
            return;
        }
        if(soluction[i-1][j-1]==x){
            data[num_player-1][i-1][j-1]=x;
            cout<<endl<<endl<<"PREENCHIDO!!!"<<endl<<endl;
            int key=1;
            for(int i=0;i<9 && key;i++) for(int j=0;j<9 && key;j++) if(data[num_player-1][i][j]==0) key=0;
            if(key){
                life[num_player-1]=0;
                cout<<"VOCE VENCEU, PARABENS"<<endl;
            }
        }
        else{
            life[num_player-1]--;
            if(life==0){
                cout<<endl<<endl<<"FIM DE JOGO, VOCE PERDEU"<<endl<<endl;
            }
            else cout<<endl<<endl<<"UMA VIDA A MENOS"<<endl<<endl;
        }
    }
};
//ideia, sempre que quiser jogar o sudoku instancia um novo objeto

int main(){
    srand(time(nullptr));
    sudoku jogo(hard, 5, 3);
    do{
        // jogo.print_sol();
        jogo.print(1);
        cout<<endl<<"Escolha a linha (1 a 9), a coluna (1 a 9) e o valor que voce quer preencher: "<<endl;
        int i, j,x;
        cin>>i>>j>>x;
        if(i==0)jogo.print_sol();

        jogo.play(1,i,j,x);
    }while(1);
    return 0;
}










    // void prt(int (&M)[9][9]){
    //     cout<<endl<<endl<<"    1    2    3    4    5    6    7    8    9"<<endl;
    //     // cout<<"   ___________________________________________"<<endl;
    //     for(int i=0;i<9;i++){
    //         for(int j=0;j<9;j++){
    //             if(j==0){
    //                 cout<<i+1<<"  |"<<M[i][j]<<"|_";
    //             }
    //             else if(j==8){
    //                 cout<<"_|"<<M[i][j]<<"|";
    //             }
    //             else{
    //                 cout<<"_|"<<M[i][j]<<"|_";
    //             }
    //         }
    //         cout<<endl;
    //     }
    // }



    //nova solve::: gera uma resposta que vai ser salva em soluction e em outra função retira caras aleatorios
    // int new_solve(int (&M)[9][9], int(&S)[9][9], int(&Q)[9][9]){
    //     int n_soluctions=0;
    //     bool remove=false;
    //     int final_i=0,final_j=0,n_sol=0,final_x=0;
    //     //escolhe indice a ser preenchido
    //     int i,j,k;
    //     int flag=0;
    //     for(int i=0;i<9;i++)for(int j=0;j<9;j++)if(M[i][j]==0){flag++;}
    //     if(flag<10){
    //         prt(M);
    //         cout<<flag<<endl<<endl;
    //     }
    //     if(flag=0){
    //         for(int m=0;m<9;m++) for(int l=0;l<9;l++) S[m][l]=M[m][l];
    //         return 1;
    //     }
    //     do{
    //         i=rand()%9;
    //         j=rand()%9;
    //     }while(M[i][j]!=0);
    //     vector <int> opcoes;
    //     for(int k=0;k<9;k++){
    //         opcoes.push_back(k+1);
    //     }
    //     shuffle(opcoes.begin(),opcoes.end(),gerador);
    //     for(int l=0;l<9;l++){
    //         int x=opcoes[l];
    //         if(pode(x,i,j,M)){
    //             M[i][j]=x;
    //             // prt(M);
    //             // if(!remove){
    //             //     P.erase(P.begin()+k);
    //             //     remove=true;
    //             // }
    //             // else P.erase(P.end());
    //             n_sol=new_solve(M,S,Q);
    //             // P.push_back({i,j});
    //             M[i][j]=0;
    //             if(n_sol==1){
    //                 final_i=i;
    //                 final_j=j;
    //                 final_x=x;
    //             }
    //             n_soluctions+=n_sol;
    //             if(n_soluctions>1 && final_i!=0){
    //                 for(int m=0;m<9;m++) for(int l=0;l<9;l++){
    //                     Q[m][l]=M[m][l];
    //                 }
    //                 Q[final_i][final_j]=final_x;
    //                 return 2;
    //             }
    //             if(n_soluctions>1) return 2;
    //         }
    //         if(l==8){
    //             return n_soluctions;
    //         }
    //     }
    // }


    // int solve(int i,int j,int (&M)[9][9],bool flag=true,bool first=false){
    //     //já temos até i/j preenchido
    //     //<j já foi
    //     //if(next==false=>full)
    //     static int num_sol=0;
    //     if(first==true) num_sol=0;
    //     if(num_sol>1) return num_sol;
    //     if(next(i,j,M)==false){
    //         num_sol++;
    //         if(flag) for(int a=0;a<9;a++) for(int b=0;b<9;b++) soluction[a][b]=M[a][b];
    //         return num_sol;
    //     }
    //     //x,y são os proximos indices a se preencher
    //     vector <int> opcoes;
    //     for(int k=0;k<9;k++){
    //         opcoes.push_back(k+1);
    //     }
    //     random_device rd;
    //     mt19937 gerador(rd());
    //     shuffle(opcoes.begin(),opcoes.end(),gerador);
    //     for(int k=0;k<9;k++){
    //         if(pode(opcoes[k],i,j,M)==1){
    //             M[i][j]=opcoes[k];
    //             solve(i,j,M,flag);
    //             M[i][j]=0;
    //         }
    //     }
    //     return num_sol; 
    // }
