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
	
	/*
	Constraints
-	N will be between 2 and 50, inclusive.
-	roads will contain between 1 and 50 elements, inclusive.
-	Each element of roads will contain between 5 and 50 characters, inclusive.
-	Each element of roads will be in the form "a b e" (quotes for clarity) where a and b are distinct integers between 0 and N-1, inclusive, and e is an integer between 1 and 1,000,000, inclusive.
-	The integers in roads will not contain extra leading zeroes.
-	There will be exactly one way to get from one town to any other, by following one or more roads.
-	occupiedTowns will contain between 1 and 50 elements, inclusive.
-	Each element of occupiedTowns will be between 0 and N-1, inclusive.
-	The elements of occupiedTowns will be distinct.
	*/
	
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

int main()
{
	BlockEnemy test;
	
	std::cout << test.minEffort(5, { "1 0 1", "1 2 2", "0 3 3", "4 0 4" }, { 3, 2, 4 }) << "\n";
	std::cout << "Expected return: 4\n";
	//To make the communication between town 2 and the other occupied towns impossible, we must destroy one of the first two roads. 
	//We choose the first one as it requires less effort. Similarly, we choose between the last two roads in order to disconnect towns 3 and 4. 
	//The total cost is therefore 4.

	std::cout << test.minEffort(12, { "0 1 3", "2 0 5", "1 3 1", "1 4 8", "1 5 4", "2 6 2", "4 7 11", "4 8 10", "6 9 7", "6 10 9", "6 11 6" }, { 1, 2, 6, 8 }) << "\n";
	std::cout << "Expected return 13\n";
	//Towns 1 and 2 are on the path from 6 to 8. We have to destroy the sixth road to disconnect town 6 from 2. 
	//We must destroy one of the first two roads to disconnect towns 1 and 2 and we pick the first one. 
	//Also, we should destroy one of the roads on the path from 1 to 8 and we choose the fourth road. 
	//The total cost is 2 + 3 + 8 = 13.
	
	std::cout << test.minEffort(12, { "0 1 3", "2 0 5", "1 3 1", "1 4 8", "1 5 4", "2 6 2", "4 7 11", "4 8 10", "6 9 7", "6 10 9", "6 11 6" }, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }) << "\n";
	std::cout << "Expected return 66\n";
	//We have to destroy all roads this time.
	
	return 0;
}
