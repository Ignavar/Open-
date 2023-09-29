#include"Way.h"

Way::Way() { weight = 1; }

Way::Way(string id, int num)
{
    weight = 0;
    this->id = id;
    NumofNodes = num;
}

Way::~Way()
{
}

void Way::setWeight(int w) { weight = w; }

bool Way::is_node_linked(Node node)
{
    for (int i = 0; i < NumofNodes; i++)
    {
        if (node.getId() == nodes[i].id) return true;
    }
    return false;
}

void Way::AddNode(NodeData N) { nodes.push_back(N); }

string Way::get_id() { return id; }

vector<NodeData> Way::getNodes() { return nodes; }

int Way::getNumofNodes() { return NumofNodes; }

int Way::getWeight() { return weight; }

int Way::get_Nodepos(Node N)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].id == N.getId()) return i;
    }
}
