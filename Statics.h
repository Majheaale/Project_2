//
// Created by gurha on 04/11/2024.
//

#ifndef STATICS_H
#define STATICS_H
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <iostream>


using namespace std;
class Statics {
    unordered_map<uint16_t, vector<::chrono::time_point<::chrono::system_clock>>> timestamps; // timestamps dei messaggi per ID
    unordered_map<uint16_t, int> messageCount; // conteggio dei messaggi per ID



    double calculateMeanTime(std::vector<chrono::time_point<chrono::system_clock>>& timePoints) {
        if(timePoints.size() < 2)
            return 0.0;
        double totalInterval = 0.0;
        for(size_t i = 1; i < timePoints.size(); i++) {
            auto interval = chrono :: duration_cast<chrono::microseconds>(timePoints[i] - timePoints[i - 1]).count();
            totalInterval += interval;
        }
        return totalInterval / (timePoints.size() - 1);
    }
    public:
    Statics(){};

    void recordMessage(uint16_t id) {
        auto now = chrono :: system_clock::now();
        timestamps[id].push_back(now); //aggiunge il timestamp alla mappa per l'id dato
        messageCount[id]++; //incrementa il conteggio dei mesaggi per l'id
    }
    void saveStatics() {
        string filePath = generateFileName();
        ofstream outFile(filePath, ios_base::out);
        if(!outFile.is_open()) {
            cerr << "Failed to open file " << filePath << endl;
            return;
        }
        outFile << "ID,number_of_messages,mean_time_ms" << endl;
        for(auto& entry : timestamps) {
            uint16_t id = entry.first;
            auto & timePoints = entry.second;

            int numMessages = messageCount.at(id);
            double meanTime = calculateMeanTime(timePoints);

            outFile << hex << uppercase << id << dec << "," << numMessages << "," << meanTime << ","
                    << timePoints.size() << ","
                    <<fixed << setprecision(2) << meanTime
                    << endl;
        }
        outFile.close();
    }

private:
    string generateFileName() {
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << "statistics_" << put_time(localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".csv";
        return ss.str();
    }

};



#endif //STATICS_H
