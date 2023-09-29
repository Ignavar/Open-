#include"Map.h"
#include<string>
#include <algorithm>

Node* Map::get_NodeArr() { return Node_arr; }

vector<Way> Map::get_WayArr() { return Way_arr; }

int Map::TotalNodes()
{
    int total = 0;
    file.open(filename, ios::in);
    string line;
    while (getline(file, line))
    {
        if (line.substr(2, 4) == "node")
            total++;
        if (line.substr(2, 3) == "way")
            break;
    }
    file.close();
    return total;
}

int Map::TotalWays()
{
    string line;
    int total = 0;
    file.open(filename, ios::in);
    while ((line.find("way id")) == -1)
        getline(file, line);
    total++;
    while (getline(file, line))
    {
        if (line.find("way id") != -1)
            total++;
        if ((line.find("relation") != -1) || (line.find("osm") != -1))
            break;
    }
    file.close();
    return total;
}

void Map::setMaxMin_lat()
{
    int index, i;
    string line;
    file.open(filename, ios::in);
    while (getline(file, line))
        if (line.find("minlat") != -1)
            break;
    index = line.find("minlat");
    i = 0;
    while (line[i + index + 8] != '"')
        i++;
    min_lat = (stod(line.substr(index + 8, i)));
    index = line.find("maxlat");
    i = 0;
    while (line[i + index + 8] != '"')
        i++;
    max_lat = stod(line.substr(index+8, i));
    file.close();
    cout << max_lat << endl << min_lat << endl;
}

void Map::setMaxMin_lon()
{
    int index, i;
    string line;
    file.open(filename, ios::in);
    while (getline(file, line))
        if (line.find("minlon") != -1)
            break; 
    index = line.find("minlon");
    i = 0;
    while (line[i + index + 8] != '"')
        i++;
    min_lon = (stod(line.substr(index + 8, i)));
    index = line.find("maxlon");
    i = 0;
    while (line[i + index + 8] != '"')
        i++;
    max_lon = stod(line.substr(index + 8, i));
    file.close();
    file.close();
}

void Map::InitializeNode_arr()
{
    Node_arr = new Node[totalNodes];
    string line;
    file.open(filename, ios::in);
    int postion = 0, index, count = 0;
    while (getline(file, line))
    {
        if (line.find("way id") != -1)
            break;
        index = line.find("node id");
        if (index != -1)
        {
            int i = 0;
            while (line[index + 9 + i] != '"')
                i++;
            Node_arr[count].set_id(line.substr(index + 9, i));
            index = line.find("lat");
            i = 0;
            while (line[i + index + 5] != '"')
                i++;
            Node_arr[count].set_lat(stod(line.substr(index + 5, i)));
            index = line.find("lon");
            i = 0;
            while (line[i + index + 5] != '"')
                i++;
            Node_arr[count].set_lon(stod(line.substr(index + 5, i)));
            count++;
        }
    }
    file.close();
}

int::Map::SetConnectedNode(string t)
{
    for (int i = 0; i < totalNodes; i++)
    {
        if (Node_arr[i].getId() == t)
        {
            Node_arr[i].set();
            return i;
        }
    }
    return 0;
}

void Map::InitializeWay_arr()
{
    Way temp;
    NodeData temps;
    temps.position = 0;
    string line;
    file.open(filename, ios::in);
    int index, count = 0;
    getline(file, line);
    while ((index = line.find("way id")) == -1)
        getline(file, line);
    int i = 0;
    while (line[i + index + 8] != '"')
        i++;
    temp.id = line.substr(index + 8, i);
    Way_arr.push_back(temp);
    while (getline(file, line))
    {
        if (index = line.find("nd ref") != -1)
        {
            i = 0;
            while (line[i + index + 10] != '"')
                i++;
            temps.id = line.substr(index + 10, i);
            temps.position = SetConnectedNode(line.substr(index + 10, i));
            Way_arr[count].AddNode(temps);
        }
        if (index = line.find("tag k") != -1)
        {
            Way_arr[count].NumofNodes = Way_arr[count].nodes.size();
            i = 0;
            while (line[i + index + 9] != '"')
                i++;
            if (line.substr(index + 9, i) != "highway")
            {
                Way_arr.pop_back();
                count--;
            }
            while ((index = line.find("way id")) == -1)
            {
                getline(file, line);
                if ((line.find("relation") != -1) || (line.find("</osm>") != -1))
                    goto SKIP;
            }
            count++;
            i = 0;
            while (line[i + index + 8] != '"')
                i++;
            temp.id = line.substr(index + 8, i);
            Way_arr.push_back(temp);
        }
    }
SKIP:
    totalWays = count + 1;
    Way_arr[count].NumofNodes = Way_arr[count].nodes.size();
    temp.~Way();
    file.close();
}

void Map::SetNodeConnections()
{
    for (int i = 0; i < Way_arr.size(); i++)
    {
        vector<NodeData> temp = Way_arr[i].getNodes();
        for (int i = 0; i < temp.size() - 1; i++)
        {
            Node_arr[temp[i].position].AddConnectedNodes(Node_arr[temp[i + 1].position]);
            Node_arr[temp[i + 1].position].AddConnectedNodes(Node_arr[temp[i].position]);
        }
    }
}

Map::Map()
{
    filename = "map4.txt";
    totalNodes = TotalNodes();
    totalWays = TotalWays();
    setMaxMin_lat();
    setMaxMin_lon();
    InitializeNode_arr();
    InitializeWay_arr();
    //DeleteNodes();
    SetNodeConnections();
}

int Map::get_totalnodes()
{
    return totalNodes;
}

int Map::get_totalWays() { return totalWays; }


/*int main()
{
    Map m1;
    int count = m1.get_totalnodes();
    Node* temp = m1.get_NodeArr();
    vector<Node> temp1;
    for (int i = 0; i < count; i++)
    {
        cout << endl << temp[i].getId() << endl << temp[i].get_lat() << endl<< temp[i].get_lon();
        temp1 = temp[i].getConnectedNodes();
      
    }
    vector<Way> temp3 = m1.get_WayArr();
    vector<NodeData> temp4;
    for (int i = 0; i < temp3.size(); i++)
    {
        cout << endl;
        cout << temp3[i].get_id() << endl;
        temp4 = temp3[i].getNodes();
        for (int x = 0; x < temp4.size(); x++)
            cout << temp4[x].id << endl << temp4[x].position << endl;
    }
}*/



