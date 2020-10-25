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
	std::vector<Town*> connectedTowns;
	std::vector<int> townTravelCost;

	int townNum = 0;

	Town() 
	{ 
		//std::cout << "created town\n"; 
	};

	~Town() 
	{
		//std::cout << "destroyed town\n";
	};
};

class BlockEnemy
{
public:
	std::vector<Town> towns;

	BlockEnemy() {};

	void parseTowns(std::vector<std::string> roads, int N)
	{
		towns.resize(N);

		for (int i = 0; i < roads.size() + 1; i++)
		{
			Town temp;
			towns.push_back(temp);
		}

		std::string a = "";
		std::string b = "";
		std::string c = "";

		int next = 0;

		int aInt = 0;
		int bInt = 0;
		int cInt = 0;

		for (int i = 0; i < roads.size(); i++)
		{
			next = 0;

			a = "";
			b = "";
			c = "";

			for (int j = 0; j < roads[i].size(); j++)
			{
				if (roads[i].at(j) == ' ')
				{
					next++;
				}
				else
				{
					if (next == 0)
					{
						a.push_back(roads[i].at(j));

					}
					if (next == 1)
					{
						b.push_back(roads[i].at(j));

					}
					if (next == 2)
					{
						c.push_back(roads[i].at(j));
					}
				}
			}
			
			aInt = std::stoi(a);
			bInt = std::stoi(b);
			cInt = std::stoi(c);

			//std::cout << "town " << bInt << " stored into town " << aInt << "\n";
			//std::cout << "town " << aInt << " stored into town " << bInt << "\n";

			towns[aInt].townNum = aInt;
			towns[bInt].townNum = bInt;

			towns[aInt].townTravelCost.push_back(cInt);
			towns[bInt].townTravelCost.push_back(cInt);

			towns[aInt].connectedTowns.push_back(&towns[bInt]);
			towns[bInt].connectedTowns.push_back(&towns[aInt]);
		}
	}

	int minEffort(int N, std::vector<std::string> roads, std::vector<int> occupiedTowns)
	{
		towns.clear();
		parseTowns(roads, N);
		
		for (int i = 0; i < towns.size(); i++)
		{
			//std::cout << "Town " << i << " is connected to town(s): ";
			for (int j = 0; j < towns[i].connectedTowns.size(); j++)
			{
				//std::cout << towns[i].connectedTowns[j]->townNum << " (cost " << towns[i].townTravelCost[j] << ") ";
			}
			//std::cout << "\n";
		}
		
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
				//std::cout << "towns in the stack: ";

				while (stack.size() > 0)
				{
					//std::cout << stack.top().townNum << " ";
					stack.pop();
				}
				*/

				//disconnecting the path

				if (found)
				{
					int lowestPartner;
					int lowest;
					
					int lastTown;

					int cost = 0;
					int disconnectPath = 0;

					lowestPartner = stack.top();
					lastTown = stack.top();

					stack.pop();

					for (int j = 0; j < towns[stack.top()].connectedTowns.size(); j++)
					{
						if (towns[stack.top()].connectedTowns[j]->townNum == towns[lastTown].townNum)
						{
							lowest = stack.top();

							//std::cout << "lastTown: " << towns[lastTown].townNum << "\n";
							//std::cout << "currentTown: " << towns[stack.top()].townNum << "\n";
							//std::cout << "current lowest cost: " << towns[stack.top()].townTravelCost[j] << "\n";

							cost = towns[stack.top()].townTravelCost[j];
							disconnectPath = j;
							
							break;
						}
					}

					lastTown = stack.top();
					stack.pop();

					while (stack.size() > 0)
					{
						for (int j = 0; j < towns[stack.top()].connectedTowns.size(); j++)
						{
							if (towns[stack.top()].connectedTowns[j]->townNum == towns[lastTown].townNum)
							{
								
								//std::cout << "lastTown: " << towns[lastTown].townNum << "\n";
								//std::cout << "currentTown: " << towns[stack.top()].townNum << "\n";
								//std::cout << "is cost lower than lowest? " << towns[stack.top()].townTravelCost[j] << "\n";

								if (towns[stack.top()].townTravelCost[j] < cost)
								{
									//std::cout << "yes!\n";
									lowest = stack.top();
									lowestPartner = lastTown;

									cost = towns[stack.top()].townTravelCost[j];
									disconnectPath = j;
									break;
								}
							}
						}
						lastTown = stack.top();
						stack.pop();
					}

					//std::cout << "lowest cost was: " << cost << "\n";
					totalCost += cost;

					//std::cout << "removing town: " << towns[lowest].connectedTowns[disconnectPath]->townNum << " from town: " << towns[lowest].townNum << "\n";

					towns[lowest].connectedTowns.erase(towns[lowest].connectedTowns.begin() + disconnectPath);
					towns[lowest].townTravelCost.erase(towns[lowest].townTravelCost.begin() + disconnectPath);

					for (int j = 0; j < towns[lowestPartner].connectedTowns.size(); j++)
					{
						if (towns[lowestPartner].connectedTowns[j]->townNum == towns[lowest].townNum)
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
		//std::cout << "visiting: " << towns[lastVisit].connectedTowns[currentVisit]->townNum << " with " << towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].connectedTowns.size() << " paths.\n";
		stack->push(towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].townNum);

		if (towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].townNum == goal)
		{
			//std::cout << "found the goal!\n";
			return true;
		}

		if (towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].connectedTowns.size() == 1)
		{
			//std::cout << "found a dead end.\n";
			stack->pop();
			return false;
		}

		bool result = false;

		for (int j = 0; j < towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].connectedTowns.size(); j++)
		{
			if (towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].connectedTowns[j]->townNum != towns[lastVisit].townNum)
			{
				result = search(goal, towns[towns[lastVisit].connectedTowns[currentVisit]->townNum].townNum, j, stack);

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
	
	//std::cout << test.minEffort(5, { "1 0 1", "1 2 2", "0 3 3", "4 0 4" }, { 3, 2, 4 }) << "\n";
	//std::cout << test.minEffort(5, { "1 0 1", "1 2 2", "0 3 3", "4 0 4" }, { 3 }) << "\n";
	//std::cout << test.minEffort(12, { "0 1 3", "2 0 5", "1 3 1", "1 4 8", "1 5 4", "2 6 2", "4 7 11", "4 8 10", "6 9 7", "6 10 9", "6 11 6" }, { 1, 2, 6, 8 }) << "\n";
	//std::cout << test.minEffort(12, { "0 1 3", "2 0 5", "1 3 1", "1 4 8", "1 5 4", "2 6 2", "4 7 11", "4 8 10", "6 9 7", "6 10 9", "6 11 6" }, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }) << "\n";
	//std::cout << test.minEffort(4, { "1 3 95", "2 0 71", "2 3 78" }, { 0, 3, 2 }) << "\n";
	//std::cout << test.minEffort(12, { "9 5 681", "11 1 398", "6 1 559", "10 3 48", "5 0 978", "0 1 301", "0 2 727", "7 1 609", "3 9 894", "8 10 122", "4 3 127" }, { 11, 8, 1, 6, 4, 7, 9 }) << "\n";
	//std::cout << test.minEffort(50, { "24 14 30585", "14 2 17913", "2 1 7185", "14 32 3524", "2 3 23893", "14 42 32358", "1 46 15102", "42 29 20974", "29 11 24873", "2 23 17418", "2 31 3231", "29 12 14447", "11 9 1113", "2 0 8315", "46 34 19524", "32 33 25625", "14 17 30017", "42 18 20966", "9 47 11888", "33 35 23186", "33 13 31662", "2 20 7048", "0 7 13660", "29 21 31953", "13 44 13667", "0 38 26165", "17 27 21876", "9 28 20388", "11 49 8853", "14 10 12501", "7 45 27097", "14 39 5165", "33 16 622", "42 4 20706", "34 36 30941", "28 22 12384", "33 15 32654", "13 19 748", "19 25 18571", "4 40 26503", "10 37 11397", "9 8 19221", "24 43 17180", "0 5 25278", "1 6 20571", "28 48 9621", "2 26 11138", "16 41 2423", "46 30 777" }, { 21, 49, 34, 16, 40, 22, 6, 26, 39, 5, 33, 19, 44, 48, 9, 0, 3, 17, 45, 47 });
	//std::cout << test.minEffort(50, {"11 49 705350", "45 49 708106", "48 49 557507", "16 49 206030" , "46 49 681085",  "42 49 592851", "29 49 955456", "36 49 644055", "14 49 998371", "25 49 243780", "38 49 676165", "19 49 295695", "37 49 85890", "34 49 772321", "31 49 495278", "35 49 881683", "32 49 509966", "13 49 573322", "41 49 953193", "28 49 684833", "8 49 338711", "22 49 7607", "20 49 988937", "15 49 774045", "12 49 749572", "23 49 197707", "18 49 143647", "30 49 913598", "24 49 784195", "27 49 589280", "17 49 864709", "6 49 689371", "9 49 287833", "21 49 93643", "3 49 628920", "4 49 280025", "7 49 877184", "26 49 161234", "5 49 273188", "0 49 543685", "1 49 960792", "40 49 174364", "33 49 157069", "39 49 268121", "10 49 177457", "43 49 576795", "44 49 498155", "2 49 285189", "47 49 155944" }, { 0, 10, 20, 30, 40, 47 }) << "\n";

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
