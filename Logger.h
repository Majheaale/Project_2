//
// Created by gurha on 04/11/2024.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;
class Logger {
    ofstream logFile;


    string generateFileName() {
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now); //ottengo il tempo corrente in formato time_t
        tm* localTime = localtime(&now_c); //converte il tempo in una struttura che ha yy mm dd h m s

        ostringstream oss;

        oss << "log_"
        << (1900 + localTime->tm_year) << "_"
        << (1 + localTime->tm_mon) << "_"
        << localTime->tm_mday << "_"
        << localTime->tm_hour << "_"
        << localTime->tm_min << "_"
        << localTime->tm_sec
        << ".txt";
        return oss.str();
    }

public:
    Logger() {};

    void startNewSession()
    {
        endSession();
        string filename = generateFileName();
        logFile.open(filename, ios::out); //apro il file in modalità apertura
        if(!logFile.is_open())
            cerr<< "Errore: impossibile aprire il file di log." << endl;
    }
    void endSession() {
        if (logFile.is_open())
            logFile.close();
    }

    void logMessageWithTimeStamp(string& message) {
        if(logFile.is_open()) {
            auto now = chrono::system_clock::now();
            auto nowMs = chrono::time_point_cast<chrono::milliseconds>(now);
            auto epoch = nowMs.time_since_epoch();
            auto timestamp = chrono::duration_cast<chrono::milliseconds>(epoch).count();
            logFile << timestamp << " " << message << endl;
        }
    }

};



#endif //LOGGER_H
