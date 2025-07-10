#include "File_picker.h"
#include "input.h"
#include "compressor.h"
#include "decompressor.h"
#include "log.h"
#include "display.h"
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;

int main(){
    while(true){
        system("cls");
        displayheader();
        int ch=choice();
        if(ch==1){
            cout<<"Exiting...";
            break;
        }
        system("cls");
         displayheader();
        //Selection Of file
        printCentered("Select Input File:Press Space To Open File ");
        string path;
        if(_getch()==' ' || _getch()=='\n')
            path=open();
        if (path.empty()) {
            cerr << "❌ File selection cancelled.\n";
            cout << "Returning to main menu...\n";
            _getch(); // Pause before returning
            continue; // Jump back to top of loop
        }

        try{
            Input input(path);
            if (!input.isvalid()) {
             cerr << " Unsupported file type.\n";
             cerr << " Supported types: .txt, .json, .csv, .xml, .log, .html, .ini, .yaml, .pdf, .xlsx, .xls, .docx, .exe, .bin, .png, .jpg\n";
            }
            Mode Mode=chooseMode();
            if(Mode==Compress){
                Compressor compressor;
                vector<string> chunks =input.Chunks();
                string allChunks ;
                for(int i=0;i<chunks.size();i++){
                    allChunks+=chunks[i];
                }
                unordered_map<char, string> codeMap=compressor.getCodeTable(allChunks);
                vector<string> compressedChunks=compressor.compressChunksParallel(chunks,codeMap);
                progressbar("Commpressing!");
                string outputPath;
                string baseName = sanitizeBaseName(input.getfilename());
                string suggestedName = baseName + "_compressed.huff";
                if (menu()) {
                    outputPath = save(suggestedName);  // pass smart suggestion
                    if (outputPath.empty()) {
                    cerr << " Save cancelled or failed.\n";
                return 1;
                    }
                }   
                else {
                    cerr<<" Save cancelled or failed.\n";        // fallback name used directly
                    continue;
                }

                if (compressor.saveToFile(outputPath, compressedChunks, input.getextension(), codeMap)){
                    cout << "Compressed file saved as: " << outputPath + "_compressed.huff" << "\n";
                    size_t totalBits = 0;
                    for (const auto& chunk : compressedChunks)
                        totalBits += chunk.size();

                    Logger logger;// Log successfull compression
                    logger.logCompressionStats(input.getsize(), totalBits, input.getextension(), "Success");

                    _getch();
                }    
                else
                cerr << " Failed to save compressed file.\n";
            }  
            else if (Mode == Decompress) {
                Decompressor decompressor;

                string baseName = input.getfilename();
                string ext      = input.getextension();
                string suggestedName = baseName;

                string outputPath;

                if (menu()) {
                outputPath = save(suggestedName); // 👈 let user save as: <name>_decompressed.<ext>
                if (outputPath.empty()) {
                    cerr << " Save cancelled or failed.\n";
                    continue;
                }
                }
                else {
                    cerr << " Process cancelled or failed.\n";
                    continue;
                }

                progressbar(" Decompressing!");

                if (decompressor.decompress(path, outputPath)){
                    cout << " Decompression complete.\n";
                    _getch();

                }
                else{
                    cerr << " Failed to decompress file.\n";
                    _getch();
                }
                }
        }
        catch (const exception& e) {
            cerr << " Exception: " << e.what() << "\n";
            _getch();
        }
       
    }
     return 0;
}