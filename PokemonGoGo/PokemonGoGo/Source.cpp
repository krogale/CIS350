/*
Pokemon Go Go
Part 2
Reads in input for program 1
Outputs the shortest route to visit all unique stops
followed by the distance of this route

Bruce Elenbogen

*/

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

struct Stop {
	int row;
	int col;
	string name;
};

int findIndex(int index, const vector<int> & uniqueNames) {
	/*

	post:
	Given a vector of ints, and a target int (index)
	returns the location where index is found
	returns -1 if the index is not found.
	*/
	for (unsigned int i = 0; i < uniqueNames.size(); i++)
		if (index == uniqueNames[i])
			return i;
	return -1;
}

bool nextOneOfEachName(vector<int> & lastPermute, vector<vector<int> > &uniqueNames) {
	/*
	pre: lastPermuate.size() == uniqueNames.size()
	lastPermute[i] == uniqueNames[i][j] for some j
	post: lastPermute is incremented to the next possible set of integers such that
	lastPermute[i]== uniqueNames[i][j] for some j (where next is determined by the order
	the names appear in input).
	*/
	bool done = false;
	int index;
	for (unsigned int i = 1; i < lastPermute.size() && !done; i++) {
		index = findIndex(lastPermute[i], uniqueNames[i]);
		if (index <uniqueNames[i].size() - 1) {
			lastPermute[i] = uniqueNames[i][++index];
			return true;
		}
		else if (index == uniqueNames[i].size() - 1)
		{
			lastPermute[i] = uniqueNames[i][0];
		}
	}
	return done;
}

void setUpUniqueNames(const vector<Stop> & locations, vector<vector<int> > & indexUniqueNames) {
	/*
	post: locations[indexUniqueNames[i][j]].name == location[indexUnqueNames[i][k]] for all j, k
	locations[indexUniqueNames[i][j]].name == the ith unique name entered in input
	*/
	vector<int> temp(1, 0);
	bool placed;
	string name;
	indexUniqueNames.push_back(temp);  // the empty name is added to make set 1, 2, ...
	for (unsigned int i = 1; i < locations.size(); i++) {
		name = locations[i].name;
		placed = false;
		for (unsigned int j = 0; j < indexUniqueNames.size() && !placed; j++)
			if (name == locations[indexUniqueNames[j][0]].name) {
				placed = true;
				indexUniqueNames[j].push_back(i);
			}
		if (!placed) {
			temp[0] = i;
			indexUniqueNames.push_back(temp);
		}
	}
}

int getDistance(int x1, int y1, int x2, int y2)
{
	return (abs(x2 - x1) + abs(y2 - y1));
}

int getRouteDistance(string route, vector<Stop> & locations)
{
	int distanceSum = 0;
	distanceSum += getDistance(0, 0, locations[route[0]].col, locations[route[0]].row);
	for (int i = 1; i < route.size() - 1; i++)
	{
		distanceSum += getDistance(locations[route[i-1]].col, locations[route[i-1]].row, locations[route[i]].col, locations[route[i]].row);
	}
	distanceSum += getDistance(locations[route.back()].col, locations[route.back()].row, 0, 0);
	return distanceSum;
}

int main() {
	int numStops;
	vector<Stop> locations;
	vector<int> permute;
	vector<vector<int> > uniqueNames;
	vector<string> routes;
	vector<int> distances;
	string tempRoute = "";
	int index = 0;
	int minDistance = INT32_MAX;

	cin >> numStops;
	locations.resize(numStops + 1);
	locations[0].row = 0;
	locations[0].col = 0;
	locations[0].name = "";
	for (int i = 1; i < numStops + 1; i++)
		cin >> locations[i].row >> locations[i].col >> locations[i].name;
	setUpUniqueNames(locations, uniqueNames);
	for (unsigned int i = 0; i < uniqueNames.size(); i++) {
		permute.push_back(uniqueNames[i][0]);
	}

	do {
		for (unsigned int i = 1; i < permute.size(); i++)
		{
			tempRoute += permute[i];
		}
		do {
			routes.push_back(tempRoute);
		} while (next_permutation(tempRoute.begin(), tempRoute.end()));
		tempRoute = "";
	} while (nextOneOfEachName(permute, uniqueNames));

	for (int i = 0; i < routes.size(); i++)
	{
		distances.push_back(getRouteDistance(routes[i], locations));

		if (distances.back() < minDistance)
		{
			minDistance = distances.back();
			index = i;
		}
		else if (distances.back() == minDistance)
		{
			if (routes[i] < routes[index])
			{
				minDistance = distances.back();
				index = i;
			}
		}
	}

	for (int i = 0; i < routes[index].size(); i++)
	{
		cout << routes[index][i] << " ";
	}
	cout << minDistance << endl;
	


	//DEBUG
	for (int i = 0; i < routes.size(); i++)
		cout << routes[i] << endl;
	
	system("Pause");
	return 0;
}

/*

5
5 9 Eevee
50 50 Flareon
1 1 Flareon
1 8 Joteon
2 8 Umbreon


8
1 2 a
2 3 b
3 4 c
4 5 a
5 6 b
6 7 c
7 8 a
8 9 b

*/
