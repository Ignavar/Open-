#pragma once
#ifndef MAP_H
#define MAP_H

#include"Way.h"
#include<fstream>


class OneLoneCoder_PathFinding;

class Map //This class stores all information of nodes and ways
{ 
	friend class OneLoneCoder_PathFinding;
private:
	string filename; //The name of the TEXT file which contains open street maps data
	fstream file; //File object used by memeber functions to access TEXT file
	Node* Node_arr; //Stores all of the Nodes seen on the Map
	vector<Way> Way_arr; //Stores all of the Ways seen on the Map

	double max_lon, max_lat, min_lon, min_lat; //Stores the Maximum and minimum latitude and longitude helps construct the dimensions of the Map
	int totalNodes, totalWays; //Stores total nodes/ways seen on the map
	int TotalNodes(); //Returns Total Nodes
	int TotalWays(); //Returns Total Ways
	void setMaxMin_lon(); //Sets Maximum and minimum longitude 
	void setMaxMin_lat(); //Sets Maximum and minimum latitude
	void InitializeNode_arr(); //Used to extract all node data from osm TEXT file
	void InitializeWay_arr(); //Used to extract all Ways data from osm TEXT file
	int SetConnectedNode(string); //Sets given node to connected
	//void DeleteNodes(); //Deletes All unnecessary Nodes from osm TEXT file
	void SetNodeConnections(); //Interconnects all Nodes
public:
	Map();
	int get_totalnodes(); //returns total number of nodes
	Node* get_NodeArr(); //Returns the first memeory address of the Node array can be used to acces Nodes
	vector<Way> get_WayArr(); //Returns the firsy memeory address of the Way array can be used to acces Ways
	int get_totalWays(); //Returns total number of ways
};


#endif