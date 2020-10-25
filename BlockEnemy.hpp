// BlockEnemy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <stack>

class Town
{
public:
	Town() {};
	
	int townNum = 0;
	std::vector<Town> connectedTowns;
	std::vector<int> townTravelCost;

	~Town() {};
};

class BlockEnemy
{
public:
	std::vector<Town> towns;

	BlockEnemy() {};

	void parseTowns(std::vector<std::string> roads)
	{
		for (int i = 0; i < roads.size() + 1; i++)
		{
			Town temp;
			towns.push_back(temp);
		}

		for (int i = 0; i < roads.size(); i++)
		{
			int next = 0;

			std::string a = "";
			std::string b = "";
			std::string c = "";

			for (int j = 0; j < roads[i].size(); j++)
			{
				if (roads[i].at(j) == ' ')
				{
					next++;
				}
				else
				{
					switch (next)
					{
						case 0:
						{
							a.push_back(roads[i].at(j));
							break;
						}
						case 1:
						{
							b.push_back(roads[i].at(j));
							break;
						}
						case 2:
						{
							c.push_back(roads[i].at(j));
							break;
						}
						default:
						{
							break;
						}
					}
				}
			}
			
			//std::cout << "town " << std::stoi(b) << " stored into town " << std::stoi(a) << "\n";
			//std::cout << "town " << std::stoi(a) << " stored into town " << std::stoi(b) << "\n";

			towns[std::stoi(a)].townNum = std::stoi(a);
			towns[std::stoi(b)].townNum = std::stoi(b);

			towns[std::stoi(a)].townTravelCost.push_back(std::stoi(c));
			towns[std::stoi(b)].townTravelCost.push_back(std::stoi(c));

			towns[std::stoi(a)].connectedTowns.push_back(towns[std::stoi(b)]);
			towns[std::stoi(b)].connectedTowns.push_back(towns[std::stoi(a)]);
		}
	}

	int minEffort(int N, std::vector<std::string> roads, std::vector<int> occupiedTowns)
	{
		towns.clear();
		parseTowns(roads);
		/*
		for (int i = 0; i < towns.size(); i++)
		{
			std::cout << "Town " << i << " is connected to town(s): ";
			for (int j = 0; j < towns[i].connectedTowns.size(); j++)
			{
				std::cout << towns[i].connectedTowns[j].townNum << " (cost " << towns[i].townTravelCost[j] << ") ";
			}
			std::cout << "\n";
		}
		*/
		std::stack<int> stack;
		int totalCost = 0;

		for (int i = 0; i < occupiedTowns.size() - 1; i++)
		{
			//finding the path
			for (int k = i + 1; k < occupiedTowns.size(); k++)
			{
				bool found = false;

				for (int j = 0; j < towns[occupiedTowns[i]].connectedTowns.size(); j++)
				{
					//std::cout << "starting from town: " << occupiedTowns[i] << "\n";
					stack.push(towns[occupiedTowns[i]].townNum);

					if (search(occupiedTowns[k], towns[occupiedTowns[i]].townNum, j, &stack))
					{
						found = true;
						break;
					}
					stack.pop();
				}

				//std::cout << "disconnecting path\n";

				/*
				std::cout << "towns in the stack: ";

				while (stack.size() > 0)
				{
					std::cout << stack.top().townNum << " ";
					stack.pop();
				}
				*/

				//disconnecting the path

				if (found)
				{
					int lowest;
					int lowestPartner;
					int lastTown;

					int cost = 0;
					int disconnectPath = 0;

					lowestPartner = stack.top();
					stack.pop();

					for (int j = 0; j < towns[stack.top()].connectedTowns.size(); j++)
					{
						if (towns[stack.top()].connectedTowns[j].townNum == towns[lowestPartner].townNum)
						{
							lowest = stack.top();
							//std::cout << "current lowest cost: " << towns[stack.top()].townTravelCost[j] << "\n";
							cost = towns[stack.top()].townTravelCost[j];
							disconnectPath = j;

							lastTown = stack.top();
							break;
						}
					}

					stack.pop();

					while (stack.size() > 0)
					{
						for (int j = 0; j < towns[stack.top()].connectedTowns.size(); j++)
						{
							//std::cout << "is cost lower than lowest? " << towns[stack.top()].townTravelCost[j] << "\n";

							if (towns[stack.top()].townTravelCost[j] < cost)
							{
								//std::cout << "yes!\n";
								lowest = stack.top();
								lowestPartner = lastTown;

								cost = towns[stack.top()].townTravelCost[j];
								disconnectPath = j;

								lastTown = stack.top();
								break;
							}
						}

						stack.pop();
					}

					//std::cout << "lowest cost was: " << cost << "\n";
					totalCost += cost;

					//std::cout << "removing town: " << towns[lowest].connectedTowns[disconnectPath].townNum << " from town: " << towns[lowest].townNum << "\n";

					towns[lowest].connectedTowns.erase(towns[lowest].connectedTowns.begin() + disconnectPath);
					towns[lowest].townTravelCost.erase(towns[lowest].townTravelCost.begin() + disconnectPath);

					for (int j = 0; j < towns[lowestPartner].connectedTowns.size(); j++)
					{
						if (towns[lowestPartner].connectedTowns[j].townNum == towns[lowest].townNum)
						{
							towns[lowestPartner].connectedTowns.erase(towns[lowestPartner].connectedTowns.begin() + j);
							towns[lowestPartner].townTravelCost.erase(towns[lowestPartner].townTravelCost.begin() + j);
							break;
						}
					}
				}
			}
		}
		
		return totalCost;
	}

	bool search(int goal, int lastVisit, int currentVisit, std::stack<int> * stack)
	{
		//std::cout << "visiting: " << towns[lastVisit].connectedTowns[currentVisit].townNum << " with " << towns[towns[lastVisit].connectedTowns[currentVisit].townNum].connectedTowns.size() << " paths.\n";
		stack->push(towns[towns[lastVisit].connectedTowns[currentVisit].townNum].townNum);

		if (towns[towns[lastVisit].connectedTowns[currentVisit].townNum].townNum == goal)
		{
			//std::cout << "found the goal!\n";
			return true;
		}

		if (towns[towns[lastVisit].connectedTowns[currentVisit].townNum].connectedTowns.size() == 1)
		{
			//std::cout << "found a dead end.\n";
			stack->pop();
			return false;
		}

		bool result = false;

		for (int j = 0; j < towns[towns[lastVisit].connectedTowns[currentVisit].townNum].connectedTowns.size(); j++)
		{
			if (towns[towns[lastVisit].connectedTowns[currentVisit].townNum].connectedTowns[j].townNum != towns[lastVisit].townNum)
			{
				result = search(goal, towns[towns[lastVisit].connectedTowns[currentVisit].townNum].townNum, j, stack);

				if (result == true)
				{
					break;
				}
			}
		}

		if (result == false)
		{
			//std::cout << "dead branch.\n";
			stack->pop();
		}

		return result;
	}

	~BlockEnemy() {};
};
/*
int main()
{
	BlockEnemy test;
	
	std::cout << test.minEffort(5, { "1 0 1", "1 2 2", "0 3 3", "4 0 4" }, { 3, 2, 4 }) << "\n";
	std::cout << test.minEffort(5, { "1 0 1", "1 2 2", "0 3 3", "4 0 4" }, { 3 }) << "\n";
	std::cout << test.minEffort(12, { "0 1 3", "2 0 5", "1 3 1", "1 4 8", "1 5 4", "2 6 2", "4 7 11", "4 8 10", "6 9 7", "6 10 9", "6 11 6" }, { 1, 2, 6, 8 }) << "\n";
	std::cout << test.minEffort(12, { "0 1 3", "2 0 5", "1 3 1", "1 4 8", "1 5 4", "2 6 2", "4 7 11", "4 8 10", "6 9 7", "6 10 9", "6 11 6" }, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }) << "\n";
	std::cout << test.minEffort(4, { "1 3 95", "2 0 71", "2 3 78" }, { 0, 3, 2 }) << "\n";
	std::cout << test.minEffort(12, { "9 5 681", "11 1 398", "6 1 559", "10 3 48", "5 0 978", "0 1 301", "0 2 727", "7 1 609", "3 9 894", "8 10 122", "4 3 127" }, { 11, 8, 1, 6, 4, 7, 9 }) << "\n";
	

	return 0;
}
*/
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
