#define OLC_PGE_APPLICATION
#include"olcPixelGamingEngine.h"
#include"Map.h"
#include"Vehicle.h"
#include"Traffic_Lights.h"

class OneLoneCoder_PathFinding : public olc::PixelGameEngine
{
public:
	OneLoneCoder_PathFinding()
	{
		sAppName = "Path Finding ALgorithm";
		for (int i = 0; i < m1.totalNodes; i++)
		{
			NodePos.push_back(int(CalculateXPosition(m1.Node_arr[i]) + CalculateYPosition(m1.Node_arr[i])));
		}
		fill1 = false;
		fill2 = false;
	}

	float CalculateXPosition(Node N)
	{
		return ((( N.lon- min_lon) /(max_lon -min_lon)) * ScreenWidth()/2);
	}
	float CalculateYPosition(Node N)
	{
		return (((N.lat - min_lat) /(max_lat - min_lat)) *ScreenHeight()/2);
	}
	float CalculateXPositionZ(Node N)
	{
		return ((((N.lon - min_lon) / (max_lon - min_lon)) * ScreenWidth())-GetMouseX());
	}
	float CalculateYPositionZ(Node N)
	{
		return ((((N.lat - min_lat) / (max_lat - min_lat)) * ScreenHeight())-GetMouseY());
	}
private:
	int count = 0;
	bool fill1,fill2;
	int pos1, pos2;
	Map m1;
	vector<int> NodePos;
	vector<Bus> BusArr;
	vector<Signal> SignalArr;
	vector<Train> TrainArr;
	vector<Taxi> TaxiArr;

	double max_lat = m1.max_lat;
	double min_lat = m1.min_lat;
	double max_lon = m1.max_lon;
	double min_lon = m1.min_lon;

	Node* nodeStartB = nullptr;
	Node* nodeEndB = nullptr;

	Node* nodeStartT = nullptr;
	Node* nodeEndT = nullptr;

	Node* nodeStartTax = nullptr;
	Node* nodeEndTax = nullptr;

	Node* Light;

	int nMapWidth;
	int nMapHeight;

public:
	bool Solve_AStar(Node* nodeStart, Node* nodeEnd)
	{
		if (nodeEnd == nullptr)
			return false;

		// Reset Navigation Graph - default all node states
		for (int x = 0; x < m1.get_totalnodes(); x++)
		{
			m1.Node_arr[x].visited = false;
			m1.Node_arr[x].globalgoal = INFINITY;
			m1.Node_arr[x].localgoal = INFINITY;
			m1.Node_arr[x].parent = nullptr;	// No parents
		}

		auto distance = [](Node* a, Node* b) // For convenience
		{
			return (a->get_lat() - b->get_lat()) * (a->get_lat() - b->get_lat()) + (a->get_lon() - b->get_lon()) * (a->get_lon() - b->get_lon());
		};

		auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		Node* nodeCurrent = nodeStart;
		nodeStart->localgoal = 0.0f;
		nodeStart->globalgoal = heuristic(nodeStart, nodeEnd);

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		list<Node*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);
		while (!listNotTestedNodes.empty())// && nodeCurrent != nodeEnd)
		{
			cout << "succcccccccccccccccccccccccccccccccccccccccccccccccccccc" << endl;
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->globalgoal < rhs->globalgoal; });
			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
				listNotTestedNodes.pop_front();
			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->visited = true; // We only explore a node once

			Node* nodeNeighbour = nullptr;
			vector<Node> temp1;
			// Check each of this node's neighbours...
			for (int i = 0; i < m1.get_totalnodes(); i++)
			{
				cout << i << endl;
					temp1 = m1.Node_arr[i].getConnectedNodes();
				
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				for(int i = 0;i<temp1.size();i++)
				{ 
					nodeNeighbour = &(temp1[i]);
					if (nodeNeighbour == nodeCurrent)
						continue;
				if (!nodeNeighbour->visited && nodeNeighbour->obstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->localgoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour->localgoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->localgoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->globalgoal = nodeNeighbour->localgoal + heuristic(nodeNeighbour, nodeEnd);
				}
				}
			}
		}
		return true;
	}

	protected:

			int clostVal(int K)
			{
				// Stores the closest
				// value to K
				int res = NodePos[0];
				int position = 0;

				// Traverse the array
				for (int i = 1; i < NodePos.size();i++) 
				{
					if ( K == NodePos[i])
						return i;
					// If absolute difference
					// of K and res exceeds
					// absolute difference of K
					// and current element
					if (abs(K - res) >= abs(K - NodePos[i])) 
					{
						res = NodePos[i];
						position = i;
					}
				}

				// Return the closest
				// array element
				return position;
			}
		
	virtual bool OnUserCreate()
	{
		return true;
	}
	virtual bool OnUserUpdate(float fElsapedtime)
	{
		Clear(olc::WHITE);

		if (GetKey(olc::B).bHeld)
		{
				if (GetKey(olc::SHIFT).bHeld)
				{
					int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
					nodeStartB = &m1.Node_arr[pos];
					pos1 = pos;
					fill1 = true;
					FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::BLUE);
				}
				FillCircle(CalculateXPosition(m1.Node_arr[pos1]), CalculateYPosition(m1.Node_arr[pos1]), 8, olc::BLUE);
				if (GetKey(olc::CTRL).bHeld)
				{
					int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
					nodeEndB = &m1.Node_arr[pos];
					nodeStartB = &m1.Node_arr[pos1];
					pos2 = pos;
					FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::DARK_BLUE);
					if (GetKey(olc::ENTER).bHeld)
					{
						Bus temp(*nodeStartB, *nodeEndB, 20);
						BusArr.push_back(temp);
					}
				}
				FillCircle(CalculateXPosition(m1.Node_arr[pos2]), CalculateYPosition(m1.Node_arr[pos2]), 8, olc::DARK_BLUE);
				cout << "succcccccccccccccccccccccccccccccccccccccccccccccccccccc" << endl;
				Solve_AStar(nodeStartB,nodeEndB);
				if (nodeEndB != nullptr)
				{
					Node* p = nodeEndB;
					while (p->parent != nullptr)
					{
						if (!GetKey(olc::Z).bHeld)
						{
							DrawLine(CalculateXPosition(*p), CalculateYPosition(*p), CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)), olc::GREEN);
							DrawLine(CalculateXPosition(*p), CalculateYPosition(*p) + 1, CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)) + 1, olc::GREEN);
							DrawLine(CalculateXPosition(*p) - 1, CalculateYPosition(*p), CalculateXPosition(*(p->parent)) - 1, CalculateYPosition(*(p->parent)), olc::GREEN);
						}
						if (GetKey(olc::Z).bHeld)
						{
							DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)), olc::GREEN);
							DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p) + 1, CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)) + 1, olc::GREEN);
							DrawLine(CalculateXPositionZ(*p) - 1, CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)) - 1, CalculateYPositionZ(*(p->parent)), olc::GREEN);

						}
						// Set next node to this node's parent
						p = p->parent;
					}
				}
		}

		for (int i = 0; i < BusArr.size(); i++)
		{
			nodeStartB = BusArr[i].getStartNode();
			nodeEndB = BusArr[i].getEndNode();
			FillCircle(CalculateXPosition(*nodeStartB), CalculateYPosition(*nodeStartB), 8, olc::BLUE);
			FillCircle(CalculateXPosition(*nodeEndB), CalculateYPosition(*nodeEndB), 8, olc::DARK_BLUE);
			Solve_AStar(nodeStartB,nodeEndB);
			if (nodeEndB != nullptr)
			{
				Node* p = nodeEndB;
				while (p->parent != nullptr)
				{
					if (!GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p), CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p) + 1, CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPosition(*p) - 1, CalculateYPosition(*p), CalculateXPosition(*(p->parent)) - 1, CalculateYPosition(*(p->parent)), olc::GREEN);
					}
					if (GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p) + 1, CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPositionZ(*p) - 1, CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)) - 1, CalculateYPositionZ(*(p->parent)), olc::GREEN);

					}
					// Set next node to this node's parent
					p = p->parent;
				}
			}
		}

		if (GetKey(olc::T).bHeld)
		{
			if (GetKey(olc::SHIFT).bHeld)
			{
				int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
				nodeStartT = &m1.Node_arr[pos];
				pos1 = pos;
				fill1 = true;
				FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::BLACK);
			}
			FillCircle(CalculateXPosition(m1.Node_arr[pos1]), CalculateYPosition(m1.Node_arr[pos1]), 8, olc::BLACK);
			if (GetKey(olc::CTRL).bHeld)
			{
				int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
				nodeEndT = &m1.Node_arr[pos];
				nodeStartT = &m1.Node_arr[pos1];
				pos2 = pos;
				FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::DARK_GREEN);
				if (GetKey(olc::ENTER).bHeld)
				{
					Train temp(*nodeStartT, *nodeEndT, 20);
					TrainArr.push_back(temp);
				}
				if (GetKey(olc::Q).bPressed)
					if (TrainArr.size() != 0)
						TrainArr.pop_back();
			}
			FillCircle(CalculateXPosition(m1.Node_arr[pos2]), CalculateYPosition(m1.Node_arr[pos2]), 8, olc::DARK_GREEN);
			Solve_AStar(nodeStartT, nodeEndT);
			if (nodeEndT != nullptr)
			{
				Node* p = nodeEndT;
				while (p->parent != nullptr)
				{
					if (!GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p), CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p) + 1, CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPosition(*p) - 1, CalculateYPosition(*p), CalculateXPosition(*(p->parent)) - 1, CalculateYPosition(*(p->parent)), olc::GREEN);
					}
					if (GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p) + 1, CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPositionZ(*p) - 1, CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)) - 1, CalculateYPositionZ(*(p->parent)), olc::GREEN);

					}
					// Set next node to this node's parent
					p = p->parent;
				}
			}
		}
		for (int i = 0; i < TrainArr.size(); i++)
		{
			nodeStartT =TrainArr[i].getStartNode();
			nodeEndT = TrainArr[i].getEndNode();
			FillCircle(CalculateXPosition(*nodeStartT), CalculateYPosition(*nodeStartT), 8, olc::BLACK);
			FillCircle(CalculateXPosition(*nodeEndT), CalculateYPosition(*nodeEndT), 8, olc::DARK_GREEN);
			Solve_AStar(nodeStartT, nodeEndT);
			if (nodeEndT != nullptr)
			{
				Node* p = nodeEndT;
				while (p->parent != nullptr)
				{
					if (!GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p), CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p) + 1, CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPosition(*p) - 1, CalculateYPosition(*p), CalculateXPosition(*(p->parent)) - 1, CalculateYPosition(*(p->parent)), olc::GREEN);
					}
					if (GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p) + 1, CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPositionZ(*p) - 1, CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)) - 1, CalculateYPositionZ(*(p->parent)), olc::GREEN);

					}
					// Set next node to this node's parent
					p = p->parent;
				}
			}
		}

		if (GetKey(olc::A).bHeld)
		{
			if (GetKey(olc::SHIFT).bHeld)
			{
				int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
				nodeStartTax = &m1.Node_arr[pos];
				pos1 = pos;
				fill1 = true;
				FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::YELLOW);
			}
			FillCircle(CalculateXPosition(m1.Node_arr[pos1]), CalculateYPosition(m1.Node_arr[pos1]), 8, olc::YELLOW);
			if (GetKey(olc::CTRL).bHeld)
				if (GetKey(olc::CTRL).bHeld)
				{
					int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
					nodeEndTax = &m1.Node_arr[pos];
					nodeStartTax = &m1.Node_arr[pos1];
					pos2 = pos;
					FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::DARK_GREEN);
					if (GetKey(olc::ENTER).bHeld)
					{
						Taxi temp(*nodeStartT, *nodeEndT, 20);
						TaxiArr.push_back(temp);
					}

				}
			FillCircle(CalculateXPosition(m1.Node_arr[pos2]), CalculateYPosition(m1.Node_arr[pos2]), 8, olc::GREEN);
			Solve_AStar(nodeStartTax, nodeEndTax);
			if (nodeEndTax != nullptr)
			{
				Node* p = nodeEndT;
				while (p->parent != nullptr)
				{
					if (!GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p), CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p) + 1, CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPosition(*p) - 1, CalculateYPosition(*p), CalculateXPosition(*(p->parent)) - 1, CalculateYPosition(*(p->parent)), olc::GREEN);
					}
					if (GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p) + 1, CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPositionZ(*p) - 1, CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)) - 1, CalculateYPositionZ(*(p->parent)), olc::GREEN);

					}
					// Set next node to this node's parent
					p = p->parent;
				}
			}
		}
		for (int i = 0; i < TaxiArr.size(); i++)
		{
			nodeStartTax = TaxiArr[i].getStartNode();
			nodeEndTax = TaxiArr[i].getEndNode();
			FillCircle(CalculateXPosition(*nodeStartTax), CalculateYPosition(*nodeStartTax), 8, olc::BLACK);
			FillCircle(CalculateXPosition(*nodeEndTax), CalculateYPosition(*nodeEndTax), 8, olc::DARK_GREEN);
			Solve_AStar(nodeStartTax, nodeEndTax);
			if (nodeEndTax != nullptr)
			{
				Node* p = nodeEndTax;
				while (p->parent != nullptr)
				{
					if (!GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p), CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPosition(*p), CalculateYPosition(*p) + 1, CalculateXPosition(*(p->parent)), CalculateYPosition(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPosition(*p) - 1, CalculateYPosition(*p), CalculateXPosition(*(p->parent)) - 1, CalculateYPosition(*(p->parent)), olc::GREEN);
					}
					if (GetKey(olc::Z).bHeld)
					{
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)), olc::GREEN);
						DrawLine(CalculateXPositionZ(*p), CalculateYPositionZ(*p) + 1, CalculateXPositionZ(*(p->parent)), CalculateYPositionZ(*(p->parent)) + 1, olc::GREEN);
						DrawLine(CalculateXPositionZ(*p) - 1, CalculateYPositionZ(*p), CalculateXPositionZ(*(p->parent)) - 1, CalculateYPositionZ(*(p->parent)), olc::GREEN);

					}
					// Set next node to this node's parent
					p = p->parent;
				}
			}
		}

		if (GetKey(olc::X).bHeld)
		{
			if (GetKey(olc::SHIFT).bHeld)
			{
				int pos = clostVal((GetMouseX() / 5.8) + (GetMouseY() / 5.8));
				Light = &m1.Node_arr[NodePos[pos]];
				FillCircle(CalculateXPosition(m1.Node_arr[pos]), CalculateYPosition(m1.Node_arr[pos]), 8, olc::GREY);
				if (GetKey(olc::ENTER).bHeld)
				{
					Signal temp(*Light);
					SignalArr.push_back(temp);
				}
				if (GetKey(olc::Q).bHeld)
					if (SignalArr.size() != 0)
						SignalArr.pop_back();
			}
		}
		for (int i = 0; i < SignalArr.size(); i++)
		{
			if (count < 100)
				FillCircle(CalculateXPosition(*SignalArr[i].getPos()), CalculateYPosition(*SignalArr[i].getPos()), 8, olc::RED);
			if (count > 100 && count < 150)
				FillCircle(CalculateXPosition(*SignalArr[i].getPos()), CalculateYPosition(*SignalArr[i].getPos()), 8, olc::YELLOW);
			if (count > 150)
			{
				FillCircle(CalculateXPosition(*SignalArr[i].getPos()), CalculateYPosition(*SignalArr[i].getPos()), 8, olc::GREEN);
				if(count>250)
				count = 0;
			}
			count++;
		}
		vector<Node> temp;
		for (int i = 0; i < m1.totalNodes; i++)
		{
			temp = m1.Node_arr[i].getConnectedNodes();
			for (int x = 0; x < temp.size(); x++)
			{
					DrawLine(CalculateXPosition(m1.Node_arr[i]), CalculateYPosition(m1.Node_arr[i]), CalculateXPosition(temp[x]), CalculateYPosition(temp[x]), olc::BLACK);
					/*DrawLine(CalculateXPosition(m1.Node_arr[i]) + 1, CalculateYPosition(m1.Node_arr[i]), CalculateXPosition(temp[x]) + 1, CalculateYPosition(temp[x]), olc::BLACK);
					DrawLine(CalculateXPosition(m1.Node_arr[i]), CalculateYPosition(m1.Node_arr[i]) + 1, CalculateXPosition(temp[x]), CalculateYPosition(temp[x]) + 1, olc::BLACK);
				*/
			}
		}
		/*vector<NodeData> temp;
		for (int i = 0; i < m1.totalWays; i++)
		{
			temp = m1.Way_arr[i].getNodes();
			for (int i = 0; i < temp.size() - 1; i++)
			{
				DrawLine(CalculateXPosition(m1.Node_arr[temp[i].position]), CalculateYPosition(m1.Node_arr[temp[i].position]), CalculateXPosition(m1.Node_arr[temp[i + 1].position]), CalculateYPosition(m1.Node_arr[temp[i + 1].position]), olc::BLACK);
				DrawLine(CalculateXPosition(m1.Node_arr[temp[i].position]), CalculateYPosition(m1.Node_arr[temp[i].position]) + 1, CalculateXPosition(m1.Node_arr[temp[i + 1].position]), CalculateYPosition(m1.Node_arr[temp[i + 1].position]) + 1, olc::BLACK);
				DrawLine(CalculateXPosition(m1.Node_arr[temp[i].position]) + 1, CalculateYPosition(m1.Node_arr[temp[i].position]), CalculateXPosition(m1.Node_arr[temp[i + 1].position]) + 1, CalculateYPosition(m1.Node_arr[temp[i + 1].position]), olc::BLACK);
			}
		}
		if (GetKey(olc::Z).bHeld)
		{
			Clear(olc::GREY);
			for (int i = 0; i < m1.totalWays; i++)
			{
				temp = m1.Way_arr[i].getNodes();
				for (int i = 0; i < temp.size() - 1; i++)
				{
					
					DrawLine(CalculateXPositionZ(m1.Node_arr[temp[i].position]), CalculateYPositionZ(m1.Node_arr[temp[i].position]), CalculateXPositionZ(m1.Node_arr[temp[i + 1].position]), CalculateYPositionZ(m1.Node_arr[temp[i + 1].position]), olc::BLACK);
					DrawLine(CalculateXPositionZ(m1.Node_arr[temp[i].position]), CalculateYPositionZ(m1.Node_arr[temp[i].position]) + 1, CalculateXPositionZ(m1.Node_arr[temp[i + 1].position]), CalculateYPositionZ(m1.Node_arr[temp[i + 1].position]) + 1, olc::BLACK);
					DrawLine(CalculateXPositionZ(m1.Node_arr[temp[i].position]) + 1, CalculateYPositionZ(m1.Node_arr[temp[i].position]), CalculateXPositionZ(m1.Node_arr[temp[i + 1].position]) + 1, CalculateYPositionZ(m1.Node_arr[temp[i + 1].position]), olc::BLACK);
				}
			}
		}*/
			return true;
		}
	};
int main()
{
	OneLoneCoder_PathFinding app;
	app.Construct(1280, 720, 1, 1);
	app.Start();
}