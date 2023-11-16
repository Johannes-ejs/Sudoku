#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include "sudoku.cpp"

#ifdef _WIN32
char POSIX_CHAR = '\\';
#else 
char POSIX_CHAR = '/';
#endif

using namespace std;


class AlgumaCoisa {
    string dir;
    string file;
    string ESCAPE_TOKEN = "\n";

    void config(){
        // TODO: RECEBE RODADAS, TEMPO, JOGO INCOMPLETO, SOLUCAO
    }

    void rank(){
        // TODO: PREENCHER QVECTOR DE RANKING NO C++ 
    }

    public:

    bool exists(){
        ifstream f(dir + POSIX_CHAR + file);
        return f.good();
    }

    void remove(string path){
        system(string("rm " + path).c_str());
    }

    vector<string> read_file()
    {
        ifstream inputFile(dir + POSIX_CHAR + file);
        vector<string> ret;
        while(!exists()){}
        if (inputFile.is_open())
        {
            string line;
            while (getline(inputFile, line)){
                ret.push_back(line);
            }
            inputFile.close();
            remove(dir + POSIX_CHAR + file);
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
        if (ret[0] == "<CONFIG>")
            config();
        else if (ret[0] == "<RANK>")
            rank();
        return ret;
    }

    void write_file(string msg)
    {
        ofstream inputFile(dir + POSIX_CHAR + file);

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

    string exec(string cmd){
        array<char, 128> buffer;
        string result;
        unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe)
        {
            throw runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        return result;
    }

    string get_ip(){
        return exec(string("python serverapp")+ POSIX_CHAR +"server"+ POSIX_CHAR +"get_ip.py");
    }

    string join(string ip, string nickname){
        return exec(string("python serverapp") + POSIX_CHAR + "client" + POSIX_CHAR + "client.py" + " " + ip + " " + nickname);
    }

    string create(int max_num){
        return exec(string("python serverapp") + POSIX_CHAR + "server" + POSIX_CHAR + "server.py" + " " + to_string(max_num));
    }

};