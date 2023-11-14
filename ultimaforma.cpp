#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace fs=std::filesystem;

fs::path get_path(string dir, string file){
    return fs::path(dir).append(file);
}

bool exists(fs::path Path){
    return fs::exists(Path);
}

string read_file(fs::path Path){
    ifstream inputFile(Path);
    string ret;

    if (inputFile.is_open()){
        string line;
        while (getline(inputFile, line)){
            ret += line + '\n';
        }
        inputFile.close();
        fs::remove(Path);
    }
    else{
        cerr << "Unable to open file for reading." << endl;
    }
    return ret;
}

void write_file(fs::path Path, string msg){
    ofstream inputFile(Path);

    if (inputFile.is_open()){
        inputFile << msg;
        inputFile.close();
    }
    else{
        cerr << "Unable to open file for reading." << endl;
    }
}