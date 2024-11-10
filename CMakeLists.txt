cmake_minimum_required(VERSION 3.29)
project(FSM)

set(CMAKE_CXX_STANDARD 20)

add_executable(FSM main.cpp
        FSM.cpp
        FSM.h
        CanReceiver.cpp
        CanReceiver.h
        Logger.cpp
        Logger.h
        Statics.cpp
        Statics.h
        fake_receiver.c
        fake_receiver.h)
