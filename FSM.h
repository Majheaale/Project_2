#ifndef FSM_H
#define FSM_H

#include <cmath>

#include "CanReceiver.h"
#include "Logger.h"
#include "Statics.h"
#include <string>

using namespace std;
class FSM {
    enum class State {Idle, Run};
    State currentState;
    CanReceiver* receiver;
    Logger logger;
    Statics statics;
public:
    FSM(CanReceiver* receiver){
        this->receiver = receiver;
        this->logger = Logger();
        this->statics = Statics();
        this->currentState = State::Idle;
    }

    void update() {
        string message;
        if(receiver->getNextMessage(message)) {
            handleCanMessage(message);
        }
    }

    void handleCanMessage(string message) {
        if(currentState == State::Idle) {
            if(message.compare("0A0#660") == 0 || message.compare("0A0#FF01") == 0) {
                transitionTo(State :: Run);
            }
        }
        else if(currentState == State::Run) {
            uint16_t id = parsing(message);
            logger.logMessageWithTimeStamp(message);
            statics.recordMessage(id);

            //transition to Idle
            if(message.compare("0A0#66FF") == 0) {
                transitionTo(State::Idle);
            }
        }
    }

    void transitionTo(State newState) {
        currentState = newState;
        if(currentState == State::Idle) {
            statics.saveStatics();
            logger.endSession();
        }
        else if(currentState == State::Run)
            logger.startNewSession();
    }

private:
    uint16_t parsing(string message) {

        size_t delimiterPos = message.find('#');
        // Parsing ID
        string idStr = message.substr(0, delimiterPos);  // ID in formato stringa
        uint16_t id;
        try {
            id = static_cast<uint16_t>(stoi(idStr, nullptr, 16));  // Conversione da esadecimale
            return id;
        } catch (const invalid_argument& e) {
            cerr << "Error parsing ID: " << e.what() << endl;
        }
    }
};



#endif //FSM_H
