#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <windows.h>

class Logger {
private:
    std::ofstream file;

    inline std::string timestamp() {
        time_t now = time(nullptr);
        tm* local = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
        return buffer;
    }

    inline std::string getExecutableDir() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        std::string fullPath(buffer);
        size_t slash = fullPath.find_last_of("\\/");
        return (slash != std::string::npos) ? fullPath.substr(0, slash + 1) : "";
    }

public:
    Logger() {
        std::string logPath = getExecutableDir() + "log.log";
        file.open(logPath, std::ios::app);
    }

    ~Logger() {
        if (file.is_open()) file.close();
    }

    void logCompressionStats(size_t originalSizeBytes, size_t compressedBits,
                             const std::string& ext, const std::string& status) {
        if (!file.is_open()) return;
        double bitRatio = static_cast<double>(compressedBits) / (originalSizeBytes * 8);
        double reduction = 100.0 * (1.0 - bitRatio);

        file << "[" << timestamp() << "] Compression\n";
        file << "Extension   : " << ext << "\n";
        file << "Original    : " << originalSizeBytes << " bytes\n";
        file << "Compressed  : " << compressedBits << " bits\n";
        file << std::fixed << std::setprecision(2);
        file << "Ratio       : " << bitRatio << "\n";
        file << "Reduction   : " << reduction << "%\n";
        file << "Status      : " << status << "\n";
        file << "------------------------------\n";
    }

    void logDecompressionStats(size_t rawBytesRead,
                               const std::string& ext, const std::string& status) {
        if (!file.is_open()) return;
        file << "[" << timestamp() << "] Decompression\n";
        file << "Extension   : " << ext << "\n";
        file << "Raw Bytes   : " << rawBytesRead << " bytes\n";
        file << "Status      : " << status << "\n";
        file << "------------------------------\n";
    }
};

#endif
