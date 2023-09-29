#include "Traffic_Lights.h"
#include"Node.h"

Signal::Signal(Node& n)
{
    start = n;
    set_id(n.getId());
    set_Index(n.get_Index());
    set_lat(n.get_lat());
    set_lon(n.get_lon());
    setTiming();
}
Signal::Signal(Node n, int red_time, int yellow_time, int green_time, int Delay)
{
    set_id(n.getId());
    set_Index(n.get_Index());
    set_lat(n.get_lat());
    set_lon(n.get_lon());
    this->Delay = Delay;
    setRedTime(red_time);
    setYellowTime(yellow_time);
    setGreenTime(green_time);
}

Node* Signal::getPos() { return &start; }
void Signal::setTiming()
{
    for (int i = 0; i < 3; i++)
        light_timing[i] = 5;
    Delay = 5;
}

void Signal::setRedTime(int time)
{
    light_timing[0] = time;
}
void Signal::setYellowTime(int time)
{
    light_timing[1] = time;
}
void Signal::setGreenTime(int time) {
    light_timing[2] = time;
}
int Signal::getRedTime() const {
    return light_timing[0];
}
int Signal::getYellowTime() const {
    return light_timing[1];
}
int Signal::getGreenTime() const {
    return light_timing[2];
}