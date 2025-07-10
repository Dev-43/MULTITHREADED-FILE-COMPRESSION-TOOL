#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <cstddef>
#include <cstdint>
#include <queue>
#include <thread>
#include<iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <future> 
#include<fstream>
using namespace std;

struct HuffNode { // Huffman Tree Node
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;
    HuffNode(char c, int f){
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    };
};


struct Compare { // Comparison logic for priority queue
    bool operator()(HuffNode* a, HuffNode* b);
};

class Compressor {
    public:

    string compress(const string& chunk);
    vector<string> compressChunksParallel(const vector<string>& chunks,const unordered_map<char, string>& codeMap);
    vector<uint8_t> bitstobytes(const string& chunks);//Converts the 0s 1s string to bytes
    unordered_map<char, string> getCodeTable(const string& data); // reuse Huffman map

    bool saveToFile(const string& outputPath, const vector<string>& compressedChunks,string ext,unordered_map<char, string>& codeMap);
    void logCompressionStats(const string& originalFile, const vector<string>& chunks, const vector<string>& compressedChunks);
    void getcodetable(HuffNode* root, string code, unordered_map<char, string>& codeMap);
    string compressWithMap(const string& chunk, const unordered_map<char,string>& codeMap);
};

inline bool Compare::operator()(HuffNode* a, HuffNode* b) {
    return a->freq > b->freq;
}

inline bool Compressor::saveToFile(const string& outputPath, const vector<string>& compressedChunks,string ext,unordered_map<char, string>& codeMap) {
    ofstream file(outputPath, ios::binary);
    if (!file) {
        return false;
    }
    size_t totalBits=0;
    for(auto& chunks:compressedChunks){
        totalBits+=chunks.size();
    }
    file << "EXT:" <<ext<< "\n";
    file<<"MAP_ENTRIES:"<<codeMap.size()<<"\n";
    file<<"BITCOUNT:"<<totalBits<<"\n";
    for (unordered_map<char, string>::iterator it = codeMap.begin(); it != codeMap.end(); ++it) {
        file << static_cast<int>(it->first) << " " << it->second << "\n";
    }
    file.flush();
    for (const string& chunk : compressedChunks) {
        vector<uint8_t> chunksbytes = bitstobytes(chunk);
        file.write(reinterpret_cast<const char*>(chunksbytes.data()), chunksbytes.size());
    }
    file.close();
    return true;
}
inline unordered_map<char,string> Compressor::getCodeTable(const string& data) { // Gets the Huffman code table
    unordered_map<char,int>freq;
    for(char ch :data)
        freq[ch]++;
    //Gets the Prirority Queue
    priority_queue<HuffNode*, vector<HuffNode*>, Compare> queue;
    for (unordered_map<char, int>::iterator it = freq.begin(); it != freq.end(); ++it)
        queue.push(new HuffNode(it->first, it->second));

     //Build the Huffman Tree
    while (queue.size() > 1) {
        HuffNode* left = queue.top();
        queue.pop();
        HuffNode* right = queue.top();
        queue.pop();
        HuffNode* parent = new HuffNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        queue.push(parent);
    }
    //Get the code table
    unordered_map<char, string> codeMap;
    HuffNode* root = queue.top();
    string code = "";
    getcodetable(root, code, codeMap); // codeMap is a global variable
    
    return codeMap;
}
inline vector<uint8_t> Compressor::bitstobytes(const string& bits) {
    vector<uint8_t> bytes;
    uint8_t currentbyte = 0;
    int bitsCount = 0;
    for (char bit : bits) {
        currentbyte = (currentbyte << 1) | (bit - '0');
        bitsCount++;
        if (bitsCount == 8) {
            bytes.push_back(currentbyte);
            currentbyte = 0;
            bitsCount = 0;
        }
    }
    if (bitsCount > 0) {
        currentbyte <<= (8 - bitsCount);
        bytes.push_back(currentbyte);
    }
    return bytes;
    
}
inline void Compressor::getcodetable(HuffNode* root, string code, unordered_map<char, string>& codeMap) {
    if (root->left) {
        getcodetable(root->left, code + "0", codeMap);
    }
    if (root->right) {
        getcodetable(root->right, code + "1", codeMap);
    }
    if (!root->left && !root->right) {
        codeMap[root->ch] = code;
    }
}

inline string Compressor::compress(const string& chunk) {
    unordered_map<char, string> codeMap = getCodeTable(chunk);
    string compressed = "";
    for (char ch : chunk) {
        compressed += codeMap[ch]; //Replace the character with its Huffman code 
    }
    return compressed;
}

inline string Compressor::compressWithMap(const string& chunk,const unordered_map<char,string>& codeMap) {
    string compressed;
    compressed.reserve(chunk.size() * 2);
    for (char c : chunk) {
        compressed += codeMap.at(c);
    }
    return compressed;
}
inline vector<string> Compressor::compressChunksParallel( const vector<string>& chunks,const unordered_map<char,string>& codeMap) {
    vector<future<string>> futures;
    futures.reserve(chunks.size());

    for (const string& chunk : chunks) {
        futures.push_back(async(
            launch::async,
            &Compressor::compressWithMap,
            this,
            chunk,
            ref(codeMap)
        ));
    }

    vector<string> compressed;
    compressed.reserve(chunks.size());
    for (auto& f : futures) {
        compressed.push_back(f.get());
    }
    return compressed;
}


inline void Compressor::logCompressionStats(const string& originalFile, const vector<string>& chunks, const vector<string>& compressedChunks) {
    size_t originalSize = 0, compressedSize = 0;
    for (const auto& c : chunks) originalSize += c.size();
    for (const auto& c : compressedChunks) compressedSize += c.size();
    double bitRatio = (double)compressedSize / (originalSize * 8);
    double percentReduction = 100.0 * (1.0 - bitRatio);
    cout<<"Compression ratio: "<<bitRatio<<" Compression reduction: "<<percentReduction<<"%";

}
#endif
