#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>

using namespace std;

struct NodeData
{
    string id;
    int position;
};

class OneLoneCoder_PathFinding;

using namespace std;

class Node //Node will act as a point on our Map with x and y coordinates
{
    friend class OneLoneCoder_PathFinding;
private:
    bool obstacle; //Used in path finding algorithm to see if Node can be traversed or not
    bool visited;  //Used in path finding algorithm to see if Node is visited or not
    float globalgoal; //Used in path finding algorithm for storing distance to goal so far
    float localgoal; //Used in path finding algorithm for storing distance to goal if alternate route is used
    Node* parent; //Used in path finding algorithm
    vector<Node> ConnectedNodes; //This contains all the Nodes that are connected with this Node
    int index; //Stores vector index position 
    string id; //Stores the unique id for each node
    double lat, lon; //Stores the x and y position of each node
    bool connected; //Lets us know if the node is connected to a Way or not
public:
    Node(string, double, double, int);
    Node();
    ~Node();
    void set_id(string); //Used to set id
    void set_lat(double); //Used to set latitiude 
    void set_lon(double); //Used to set longitude 
    string getId(); //Returns id
    double get_lat(); //Returns Longitude
    double get_lon(); //Returns Latitude
    bool is_connected() const; //Lets us know if connected to a way or not returns true if connected
    int get_Index(); //returns index position of where node stored in array
    void set_Index(int); //sets index
    void set(); // Sets connected to true
    void unset(); //Sets connected to false
    void AddConnectedNodes(Node);
    vector<Node> getConnectedNodes();
};

#endif