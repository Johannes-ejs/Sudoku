#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#ifdef _WIN32
char POSIX_CHAR = '\\';
#else 
char POSIX_CHAR = '/';
#endif

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

    string exec(const char *cmd){
        array<char, 128> buffer;
        string result;
        unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
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

    string join(string nickname, string ip){
        return exec(string("python serverapp") + POSIX_CHAR + "client" + POSIX_CHAR + "clientV2.py" + " " + ip + " " + nickname);
    }

    string create(int max_num){
        return exec(string("python serverapp")+ POSIX_CHAR +"server"+ POSIX_CHAR +"serverV2.py" + " " + to_string(max_num));
    }

};
// start button -> criar msg padronizada -> escrever a msg em '.server/cpp_to_python.txt' == write_file

int main(){
    AlgumaCoisa a("aaa", "sknfjds");
    cout << a.get_ip() << endl;
}