#pragma once
#ifndef SIGNAL_H
#define SIGNAL_H
#include"Node.h"

    class Signal :public Node {

        Node start;
        int light_timing[3]; // red : 0, yellow : 1, green : 2
        int Delay;

    public:
        Signal(Node&);
        Signal(Node, int, int, int, int);

        void setTiming();

        void setRedTime(int);
        void setYellowTime(int);
        void setGreenTime(int);

        int getRedTime() const;
        int getYellowTime() const;
        int getGreenTime() const;

        Node* getPos();
    };
#endif SIGNAL_H

