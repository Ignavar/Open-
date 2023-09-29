#include "Node.h"

Node::~Node() {}
Node::Node() { connected = false; }
Node::Node(string id, double lat, double lon, int index) :id(id), lat(lat), lon(lon), index(index) { connected = false;}
string Node::getId() { return id; }
double Node::get_lat() { return lat; }
double Node::get_lon() { return lon; }
bool Node::is_connected() const { return connected; }
vector<Node> Node::getConnectedNodes() { return ConnectedNodes; }
void Node::set() { connected = true; }
void Node::set_id(string id) { this->id = id; }
void Node::set_lat(double lat) { this->lat = lat; }
void Node::set_lon(double lon) { this->lon = lon; }
void Node::AddConnectedNodes(Node N) {ConnectedNodes.push_back(N);}
int Node::get_Index() { return index; }
void Node::set_Index(int index) { this->index = index; }