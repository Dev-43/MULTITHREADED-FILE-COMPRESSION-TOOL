#ifndef FILEPICKER_H
#define FILEPICKER_H
#include <string>
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
using namespace std;

inline  string open() {
    char filename[MAX_PATH] = "";

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetForegroundWindow();  // 👈 Ensures dialog gets focus
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return string(filename);
    }
    return "";
}

inline string save(const string& suggestedFilename) {
    char filename[MAX_PATH] = "";

    // Pre-fill filename for the save dialog
    strncpy_s(filename, suggestedFilename.c_str(), MAX_PATH - 1);
    filename[MAX_PATH - 1] = '\0';  // Ensure null-termination

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize  = sizeof(ofn);
    ofn.hwndOwner    = GetForegroundWindow();
    ofn.lpstrFilter  = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile    = filename;
    ofn.nMaxFile     = MAX_PATH;
    ofn.Flags        = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle   = "Save Output File As";

    if (GetSaveFileNameA(&ofn)) {
        return string(filename);
    }

    return ""; // User cancelled or failed
}
#endif
