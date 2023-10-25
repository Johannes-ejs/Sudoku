#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

class sudoku{
    int data[9][9];
    int soluction[9][9];
    int vida;
    public:
    sudoku(int minimum): vida(3){
        //aqui precisamos gerar um jogo possivel de se vencer
        //ideia1: ir adicionando números aleatórios até que tenha solução única. após isso adicionar alguns elementos
        zerar_data(data);
        vector <int> opcoes;
        for(int k=0;k<9;k++){
            opcoes.push_back(k+1);
        }
        int i,j;
        int vezes=0;
        do{
            //i e j são os index aleatórios
            random_device rd;
            mt19937 gerador(rd());
            do{
                i=rand()%9;
                j=rand()%9;
            }while(data[i][j]!=0);
            shuffle(opcoes.begin(),opcoes.end(),gerador);
            for(int l=0;l<9;l++){
                int x=opcoes[l];
                if(pode(x,i,j,data)){
                    data[i][j]=x;
                    break;
                }
                else if(l==8){
                    zerar_data(data);
                    break;
                }
            }
        } while(solve(0,0,data,true,true)!=1 && vezes++<minimum);

        do{
            do{
                i=rand()%9;
                j=rand()%9;
            }while(data[i][j]!=0);
            data[i][j]=soluction[i][j];
        }while(solve(0,0,data,false,true)!=1);
    }
    ~sudoku(){};
    void zerar_data(int (&M)[9][9]){
        for(int i=0;i<9;i++) for(int k=0;k<9;k++){
            M[i][k]=0;
        }
    }
    bool pode(int x,int i,int j,int (&M)[9][9]){
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
    int solve(int i,int j,int (&M)[9][9],bool flag=true,bool first=false){
        //já temos até i/j preenchido
        //<j já foi
        //if(next==false=>full)
        static int num_sol=0;
        if(first==true) num_sol=0;
        if(num_sol>1) return num_sol;
        if(next(i,j,M)==false){
            num_sol++;
            if(flag) for(int a=0;a<9;a++) for(int b=0;b<9;b++) soluction[a][b]=M[a][b];
            return num_sol;
        }
        //x,y são os proximos indices a se preencher
        vector <int> opcoes;
        for(int k=0;k<9;k++){
            opcoes.push_back(k+1);
        }
        random_device rd;
        mt19937 gerador(rd());
        shuffle(opcoes.begin(),opcoes.end(),gerador);
        for(int k=0;k<9;k++){
            if(pode(opcoes[k],i,j,M)==1){
                M[i][j]=opcoes[k];
                solve(i,j,M,flag);
                M[i][j]=0;
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
    void print(){
        cout<<endl<<"NUMERO DE VIDAS: ";
        for(int i=0;i<vida;i++)
            cout<<(char)3<<" ";
        cout<<endl<<endl<<"    1    2    3    4    5    6    7    8    9"<<endl;
        // cout<<"   ___________________________________________"<<endl;
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(j==0){
                    if(data[i][j]!=0)cout<<i+1<<"  |"<<data[i][j]<<"|_";
                    else cout<<i+1<<"  |"<<(char)254<<"|_";
                }
                else if(j==8){
                    if(data[i][j]!=0)cout<<"_|"<<data[i][j]<<"|";
                    else cout<<"_|"<<(char)254<<"|";
                }
                else{
                    if(data[i][j]!=0)cout<<"_|"<<data[i][j]<<"|_";
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
    void jogar(int i, int j,int x){
        if(data[i-1][j-1]!=0){
            cout<<endl<<endl<<"ESSE LUGAR JA ESTAVA OCUPADO"<<endl<<endl;
            return;
        }
        if(soluction[i-1][j-1]==x){
            data[i-1][j-1]=x;
            cout<<endl<<endl<<"PREENCHIDO!!!"<<endl<<endl;
            int key=1;
            for(int i=0;i<9 && key;i++) for(int j=0;j<9 && key;j++) if(data[i][j]==0) key=0;
            if(key){
                vida=0;
                cout<<"VOCE VENCEU, PARABENS"<<endl;
            }
        }
        else{
            vida--;
            if(vida==0){
                cout<<endl<<endl<<"FIM DE JOGO, VOCE PERDEU"<<endl<<endl;
            }
            else cout<<endl<<endl<<"UMA VIDA A MENOS"<<endl<<endl;
        }
    }
    operator bool(){
        return vida;
    }
};
//ideia, sempre que quiser jogar o sudoku instancia um novo objeto

int main(){
    srand(time(nullptr));
    sudoku jogo(10);
    do{
        jogo.print();
        cout<<endl<<"Escolha a linha (1 a 9), a coluna (1 a 9) e o valor que voce quer preencher: "<<endl;
        int i, j,x;
        cin>>i>>j>>x;
        if(i==0)jogo.print_sol();
        jogo.jogar(i,j,x);
    }while(jogo);
    return 0;
}
