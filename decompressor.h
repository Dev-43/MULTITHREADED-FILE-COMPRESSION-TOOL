#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H
#include <cstddef>
#include <cstdint>
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<fstream>
#include "display.h"
#include "log.h"
#include "input.h"
using namespace std;

class Decompressor{
    public:
    bool decompress(const string& compressedPath, const string& outputPath);

    private:
    string bytesToBits(const vector<uint8_t>& rawBytes);
    string decodeBitstream(const string& bitstream, const unordered_map<string, char>& codeMap);
    bool saveToFile(const string& outputPath, const string& decodedData,string ext);
}; 

inline bool Decompressor::decompress(const string& compressedPath, const string& outputPath) {
    ifstream file(compressedPath, ios::binary);
    if(!file.is_open()){
        cout<<"Failed to open compressed file."<<endl;
        return false;
    }
    
    string headerline;
    getline(file,headerline);
    string ext="txt"; //default extension
    if(headerline.rfind("EXT:",0)==0 && headerline.size()>4){
        ext=headerline.substr(4);//gets the extentions
    }
    else 
        cout<<"No Extension found.Defaulting to .txt"<<endl;

    getline(file,headerline);
    int mapcount=0;
    if(headerline.rfind("MAP_ENTRIES:",0)==0 ){
        mapcount=stoi(headerline.substr(12));
    }
    else{
        cerr<<"Invalid Map_Entries header."<<endl;
        return false;
    }
    getline(file,headerline);
    size_t bitcount=0;
    if(headerline.rfind("BITCOUNT:",0)==0){
        bitcount=stoull(headerline.substr(9));
    }
    else{
        cerr<<"Invalid or Missing Bitcount header."<<endl;
        return false;
    }

    unordered_map<string, char> codeMap ;
    for(int i=0;i<mapcount;i++){
        string bits;
        int asc;
        file>>asc>>bits;
        codeMap[bits]=static_cast<char>(asc);
    }
    file.get();
    vector<uint8_t> rawBytes((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); //Reads the file into a vector buffer
    file.close();

    string bitstream=bytesToBits(rawBytes); //Converts the bytes into 0s and 1s
    if(bitstream.size()>bitcount)//Check the bitstream size so no extra bits are read
        bitstream.resize(bitcount);

    string decoded=decodeBitstream(bitstream,codeMap); //Decodes the bitstream using the code map
    return saveToFile(outputPath,decoded,ext);
}

inline string Decompressor::bytesToBits(const vector<uint8_t>& rawBytes) {
    string bits;
    for (uint8_t byte : rawBytes) {
        for(int i=7;i>=0;i--)
            bits+=((byte>>i) & 1)?'1':'0';
    }
    return bits;
}

inline bool Decompressor::saveToFile(const std::string& outputPath, const std::string& decoded, std::string ext) {
    string base = sanitizeBaseName(outputPath); // fix here
    string finalpath = base + "_decompressed." + ext;
    Logger logger;
    ofstream outfile(finalpath, std::ios::binary);
    if (!outfile.is_open()) {
        cerr << " Failed to create output file: " << finalpath << "\n";
        Logger logger;
        logger.logDecompressionStats(0,  ext, "Failed");

        return false;
    }

    outfile.write(decoded.data(), decoded.size());
    outfile.close();

    cout << " Decompressed file saved to: " << finalpath << "\n";
    logger.logDecompressionStats(decoded.size() , ext, "Success");

    return true;
}


inline string Decompressor::decodeBitstream(const string& bitstream, const unordered_map<string, char>& codeMap) {
    string decoded,buffer;
    for (char bit : bitstream) {
        buffer += bit;
        auto it=codeMap.find(buffer);
        if (it != codeMap.end()) {
            decoded += it->second;
            buffer.clear();
        }
    }
    return decoded;
}

#endif