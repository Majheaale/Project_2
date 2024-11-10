#include <stdio.h>
#include <iostream>

#include "FSM.h"
using namespace std;

extern "C"{
    #include "fake_receiver.h"
}
volatile sig_atomic_t stopFlag = false;
void signalHandler(int signum) {
    stopFlag = true;
}
int main(void){

    CanReceiver canReceiver = CanReceiver();
    FSM fsm = FSM(&canReceiver);
    string filepath = "candump.log";
    canReceiver.startReceiving(filepath);

    signal(SIGINT, signalHandler);

    while (!stopFlag) {
        fsm.update();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // ferma la ricezione e chiudi la sessione
    canReceiver.stopReceiving();

    return 0;
}