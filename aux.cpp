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
    ifstream inputFile(filePath);
    string ret;

    if (inputFile.is_open()){
        string line;
        while (getline(inputFile, line)){
            res + line + '\n';
        }
        inputFile.close();
        fs::remove(Path);
    }
    else{
        cerr << "Unable to open file for reading." << endl;
    }
    return res;
}

void write_file(fs::path Path, string msg){
    ofstream inputFile(filePath);

    if (inputFile.is_open()){
        inputFile.write(msg);
        inputFile.close();
    }
    else{
        cerr << "Unable to open file for reading." << endl;
    }
}