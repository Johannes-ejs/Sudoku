#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

namespace fs=std::filesystem;

class AlgumaCoisa {
    string dir;
    string file;
    string ESCAPE_TOKEN = "\n";

    void config(){
        // TODO: RECEBE RODADAS, TEMPO, JOGO INCOMPLETO, SOLUCAO
    }

    void endgame(){
        // TODO: PARA O JOGO E MANDA <POINTS>  
    }

    void rank(){
        // TODO: PREENCHER QVECTOR DE RANKING NO C++ 
    }

    public:

    fs::path get_path()
    {
        return fs::path(dir).append(file);
    }

    bool exists()
    {
        return fs::exists(get_path());
    }

    vector<string> read_file()
    {
        ifstream inputFile(get_path());
        vector<string> ret;

        if (inputFile.is_open())
        {
            string line;
            while (getline(inputFile, line))
            {
                ret.push_back(line);
            }
            inputFile.close();
            fs::remove(get_path());
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
        if (ret[0] == "<CONFIG>")
            config();
        else if (ret[0] == "<ENDGAME>")
            endgame();
        else if (ret[0] == "<RANK>")
            rank();
        return ret;
    }

    void write_file(string msg)
    {
        ofstream inputFile(get_path());

        if (inputFile.is_open())
        {
            inputFile << msg;
            inputFile.close();
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
    }

    string padronizar_msg(string tipo, string msg_bruta){
        string msg = "<" + tipo + ">" + ESCAPE_TOKEN;
        msg += msg_bruta;
        return msg;
    }

    AlgumaCoisa(string dir, string name){
        this->dir = dir;
        this->file = name;
    }

};

// start button -> criar msg padronizada -> escrever a msg em '.server/cpp_to_python.txt' == write_file