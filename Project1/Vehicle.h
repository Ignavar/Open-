#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H
#include <cmath>
#include "Node.h"

using namespace std;

// Abstract base class
class Vehicle
{
    int Speed, Time;
    double distance;
public:
    int getSpeed();
    void setspeed(int);
    double getDistance();
    void setDistance(double);
    int getTime();
    void setTime(int);
    virtual void CalculateDistanceOfJourney() = 0;
    virtual void CalculateTimeOfJourney() = 0;  // Pure virtual function
};

// Derived class Bus
class Bus : public Vehicle, public Node {
    Node EndPoint,StartPoint;
public:
    Bus(Node&, Node&, int);
    virtual void CalculateTimeOfJourney() override;
    virtual void CalculateDistanceOfJourney()  override;
    Node* getStartNode();
    Node* getEndNode();

};

// Derived class Train
class Train : public Vehicle, public Node {
    Node StartPoint,EndPoint;
public:
    Train(Node&, Node&, int);
    virtual void CalculateTimeOfJourney() override;
    virtual void CalculateDistanceOfJourney()  override;
    Node* getStartNode();
    Node* getEndNode();
};

// Derived class Car
class Taxi : public Vehicle, public Node {
    Node StartPoint,EndPoint;
public:
    Taxi(Node&, Node&, int);
    virtual void CalculateTimeOfJourney() override;
    virtual void CalculateDistanceOfJourney()  override;
    Node* getStartNode();
    Node* getEndNode();
};

Node* Train::getEndNode() { return &EndPoint; }

Node* Train::getStartNode() { return &StartPoint;}

Node* Taxi::getEndNode() { return &EndPoint; }

Node* Taxi::getStartNode() { return &StartPoint; }

Node* Bus::getEndNode() { return &EndPoint; }

Node* Bus::getStartNode() { return &StartPoint; }

int Vehicle::getSpeed() { return Speed; }

void Vehicle::setspeed(int speed) { Speed = speed; }

double Vehicle::getDistance() { return distance; }

void Vehicle::setDistance(double d) { distance = d; }

int Vehicle::getTime() { return Time; }

void Vehicle::setTime(int t) { Time = t; }

void Bus::CalculateDistanceOfJourney()
{
    setDistance(sqrtf(((get_lat()) - (EndPoint.get_lat()) * (get_lat()) - (EndPoint.get_lat())) + ((get_lon() - EndPoint.get_lon()) * (get_lon() - EndPoint.get_lon()))));
}

void Bus::CalculateTimeOfJourney()
{
    setTime(getDistance() / getSpeed());
}

Bus::Bus(Node& start, Node& end, int speed)
{
    StartPoint = start;
    this->setspeed(speed);
    this->set_id(start.getId());
    this->set_Index(start.get_Index());
    cout << start.get_Index();
    this->set_lat(start.get_lat());
    this->set_lon(start.get_lon());
    EndPoint = end;
}

void Train::CalculateDistanceOfJourney()
{
    setDistance(sqrtf(((get_lat()) - (EndPoint.get_lat()) * (get_lat()) - (EndPoint.get_lat())) + ((get_lon() - EndPoint.get_lon()) * (get_lon() - EndPoint.get_lon()))));
}

void Train::CalculateTimeOfJourney()
{
    setTime(getDistance() / getSpeed());
}

Train::Train(Node& start, Node& end, int speed)
{
    StartPoint = start;
    setspeed(speed);
    set_id(start.getId());
    set_Index(start.get_Index());
    set_lat(start.get_lat());
    set_lon(start.get_lon());
    EndPoint = end;
}

void Taxi::CalculateDistanceOfJourney()
{
    setDistance(sqrtf(((get_lat()) - (EndPoint.get_lat()) * (get_lat()) - (EndPoint.get_lat())) + ((get_lon() - EndPoint.get_lon()) * (get_lon() - EndPoint.get_lon()))));
}

void Taxi::CalculateTimeOfJourney()
{
    setTime(getDistance() / getSpeed());
}

Taxi::Taxi(Node& start, Node& end, int speed)
{
    StartPoint = start;
    setspeed(speed);
    set_id(start.getId());
    set_Index(start.get_Index());
    set_lat(start.get_lat());
    set_lon(start.get_lon());
    EndPoint = end;
}


#endif






