#ifndef INPUT_H
#define INPUT_H
#include <cstddef>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include <windows.h>
#include <commdlg.h>
#include <string>
#include<conio.h> 
using namespace std;

class compressor;
class Input {
    friend class compressor;
    private: 
        string filename;
        string extension;  
    public:
        inline Input(const string& filepath); //Save Filepath 
        inline bool isvalid();// Checks for file type 
        inline vector<string>Chunks();// Divide File into Fixed chucks(Parts)
        inline string getfilename();
        inline size_t getsize();
        inline string Openfile();
        string getextension();
        
};


Input::Input(const string& filepath) : filename(filepath) {

    ifstream file(filepath, ios::binary );
    if (!file) {
        cerr << "Error opening file: " << filepath << endl;
        throw runtime_error("File not found");
    }
}

bool Input::isvalid() {
    vector<string> validExt{
    ".txt", ".json", ".csv", ".xml", ".log", ".html", ".ini", ".yaml", ".pdf", 
    ".xlsx", ".xls", ".docx", ".exe", ".bin", ".png", ".jpg",".huff"
};

    string ext = filename.substr(filename.find_last_of('.'));
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return find(validExt.begin(), validExt.end(), ext) != validExt.end();
}   

vector<string> Input::Chunks(){
    vector<string> chunks;
    ifstream file(filename,ios::binary);
    if(!file){
        cerr<<"File Not Open\n";
        return chunks;
    }

     size_t Chunk_size = 16384; //size 16KB
    size_t fileSize = getsize();


    if (fileSize > 10 * 1024 * 1024)       // >10 MB → large file
        Chunk_size = 131072;               // 128 KB
    else if (fileSize > 5 * 1024 * 1024)   // 5–10 MB
        Chunk_size = 65536;                // 64 KB
    else if (fileSize > 1 * 1024 * 1024)   // 1–5 MB
        Chunk_size = 32768;                 // 32 KB
                 
    while (!file.eof()) {
         string buffer(Chunk_size, '\0');
        file.read(&buffer[0], Chunk_size);
        streamsize bytesRead = file.gcount(); 
        if (bytesRead > 0) {
            buffer.resize(bytesRead);
            chunks.push_back(buffer.substr(0, bytesRead));
        }
    }
    return chunks;
}

size_t Input::getsize(){
    ifstream file(filename, ios::binary |ios::ate);    
    return static_cast<size_t>(file.tellg());
}

string Input::getfilename(){
    size_t pos =filename.find_last_of("/\\");
    string name_ext=(pos!=string::npos) ? filename.substr(pos+1): filename;

    size_t dotpos =name_ext.find_last_of(".");
    return(dotpos!=string::npos) ? name_ext.substr(0,dotpos) : name_ext;
}

inline string Input::getextension(){
    size_t pos =filename.find_last_of("/\\");
    string name_ext=(pos!=string::npos) ? filename.substr(pos+1): filename;
    size_t dotpos =name_ext.find_last_of(".");
    return(dotpos!=string::npos) ? name_ext.substr(dotpos+1) : "";
}

#endif 