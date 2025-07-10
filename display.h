#ifndef DISPLAY_H
#define DISPLAY_H
#include<iostream>
#include<thread>
#include<string>
#include<conio.h>
#include <windows.h>
#include<vector>
using namespace std;

enum Mode {Compress ,Decompress};

inline int getconsolewidth(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80; // Fallback width

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}

inline void printCentered(const string& text) {
    int width = getconsolewidth();
    int padding = max(0, (width - static_cast<int>(text.length())) / 2);
    cout << string(padding, ' ') << text << "\n";
}

inline void displayheader() {
     printCentered(" *******************Huff Compression Tool******************* ");
     cout<<"\n";
}    

inline Mode  chooseMode(){
    const string option[2]={"COMPRESS","DECOMPRESS"};
    int choice=0;


    while (true){
        system("cls");
        displayheader();

        for(int i=0;i<2;i++){
            string ch=+(i==choice?">":"  ")+option[i]+"\n";
            printCentered(ch);
        }
        int arrowkey=_getch();
        if(arrowkey==0 || arrowkey==224){
            arrowkey=_getch();
            if(arrowkey==72){
                choice=(choice-1+2)%2;
            }
            else if(arrowkey==80){
                choice=(choice+1)%2;
            }
        }
        else if(arrowkey==13){
            return static_cast<Mode>(choice);
        }
    }
}

inline void progressbar(const string task, int durationMs = 1200) {
    int width = getconsolewidth();
    int barWidth = 40;

    for (int i = 0; i <= barWidth; ++i) {
        int percent = (100 * i) / barWidth;
        string bar = string(i, (char)219) + string(barWidth - i, (char)176);
        string line = task + bar + " " + to_string(percent) + "%";

        int padding = max(0, (width - static_cast<int>(line.length())) / 2);
        cout << "\r" << string(padding, ' ') << line << flush;
        this_thread::sleep_for(chrono::milliseconds(durationMs / barWidth));
    }
    cout << "\n";
}

inline int choice(){
    const string option[2]={"Process File","Exit"};
    int choice=0;


    while (true){
        system("cls");
        displayheader();

        for(int i=0;i<2;i++){
           string ch=+(i==choice?">":"  ")+option[i]+"\n";
            printCentered(ch);
        }
        int arrowkey=_getch();
        if(arrowkey==0 || arrowkey==224){
            arrowkey=_getch();
            if(arrowkey==72){
                choice=(choice-1+2)%2;
            }
            else if(arrowkey==80){
                choice=(choice+1)%2;
            }
        }
        else if(arrowkey==13){
            return choice;
        }
    }
}
inline std::string sanitizeBaseName(const std::string& path) {
    // Get filename part only
    size_t lastSlash = path.find_last_of("/\\");
    std::string name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;

    // Remove extension
    size_t dotPos = name.find_last_of(".");
    if (dotPos != std::string::npos)
        name = name.substr(0, dotPos);

    // Remove redundant suffixes
    const std::vector<std::string> suffixes = { "_compressed", "_decompressed" };
    for (const auto& suffix : suffixes) {
        if (name.length() >= suffix.length() &&
            name.substr(name.length() - suffix.length()) == suffix) {
            name = name.substr(0, name.length() - suffix.length());
        }
    }

    return name;
}
inline bool menu(){

    printCentered("Press Enter To Download\n)");
    int key=_getch();
    if(key==13 || key==32){
        return true;
    }
    return false;
}
#endif