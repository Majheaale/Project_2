//
// Created by gurha on 04/11/2024.
//

#ifndef CANRECEIVER_H
#define CANRECEIVER_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include "fake_receiver.h"

using namespace std;
class CanReceiver {
    queue<string> messageQueue;
    mutex queueMutex;
    thread receiveThread;
    condition_variable messageAvaible; //notifica la presenza di nuovi messaggi
    bool stopFlag;


public:
    CanReceiver() {
        stopFlag = false;
    }

    ~CanReceiver() {
        stopReceiving();
    }

    void startReceiving(string& filepath) {
        if(open_can(filepath.c_str()) == 0) { //avvio la connessione
            stopFlag = false;
            receiveThread = thread(&receiveLoop, this); //lancio la funzione di ricevere messaggi in un nuovo thread separato dallo principlale
        }
    }

    void stopReceiving() {
        stopFlag = true;
        if(receiveThread.joinable()) //verifica se il thread è stato avviato ma non terminato
            receiveThread.join();
        close_can(); //chiudo la connessione
    }

    void receiveLoop() {
        while(!stopFlag) {
            char message[MAX_CAN_MESSAGE_SIZE] = {0};
            int bytesReceived = can_receive(message);

            if(bytesReceived > 0) {
                lock_guard<mutex> lock(queueMutex);
                messageQueue.push(string(message));
                messageAvaible.notify_one(); // Notifica che è disponibile un nuovo messaggio
            }
        }
    }

    bool getNextMessage(string& message) {
        unique_lock<mutex> lock(queueMutex);
        if(messageQueue.empty())
            return false;

        message = messageQueue.front();
        messageQueue.pop();
        return true;
    }
};



#endif //CANRECEIVER_H
