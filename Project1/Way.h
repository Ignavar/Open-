#ifndef WAY_H
#define WAY_H

#include"Node.h"



using namespace std;



class Way //A way is basically an edge between multiple Nodes and will act as a line on our Map
{
private:
	string id; //Uniuqe id of every Way 
	vector<NodeData> nodes; //Stores all of the Nodes that the way connects
	int NumofNodes, weight; //Length refers to how many nodes are connected and weight refers to amount of traffic on Way
public:
	friend class Map; //So that Map can access all data members of Way
	Way();
	Way(string, int);
	~Way();
	void AddNode(NodeData);
	void RemoveNodes();
	void setWeight(int); //Used to set weight
	int getNumofNodes(); //Returns Number of Nodes connected to Way
	int getWeight(); //Returns weight associated with way
	vector<NodeData> getNodes(); //Returns first address of NodeIds vector
	string get_id(); //returns id of way
	bool is_node_linked(Node); //Checks if A node is linked with a particular Way
	int get_Nodepos(Node); //returns position of Node in vector
};
#endif