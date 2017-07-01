#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // getopt
#include <vector>

#include "helpers.cpp"
#include "Node.h"

// Show usage
void usage(char *programName)
{
	cerr << "Usage: " << programName << " [options]\n"
	     << "\nOptions:\n"
	     << "  -f filename\t\tRead input from `filename`. (default: read from stdin)\n"
		 << "  -h\t\t\tShow this help message and exit program\n"
	     << "  -i number\t\tSet entry node to node `number` (default: 1)\n"
	     << "  -o number\t\tSet target node to node `number` (default: 30)\n"
	     << "  -s {BFS,DIJKSTRA}\tSet search strategy to use `strat` (default: DIJKSTRA)\n";
}

void parseArgs(int argc, char *argv[], size_t &startNode, size_t &endNode, string &filename, SEARCH_STRATEGY &strategy)
{
	int c = 0;
	while ((c = getopt(argc, argv, "f:hi:o:s:")) != -1)
	{
		switch (c)
		{
			case 'f':
			{
				filename = optarg;
				break;
			}
			case 'h':
			{
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			}
			case 'i':
			{
				startNode = atoi(optarg);
				break;
			}
			// End (target) node
			case 'o':
			{
				endNode = atoi(optarg);
				break;
			}
			case 's':
			{
				string opt = optarg;
				strategy = (opt == "BFS") ? BFS : DIJKSTRA;
				break;
			}
			default:
			{
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	size_t startNode = 0;
	size_t endNode = 29;
	string filename = "";
	SEARCH_STRATEGY strategy = DIJKSTRA;

	parseArgs(argc, argv, startNode, endNode, filename, strategy);
	vector<vector<size_t>> adjMat = readFileIntoAdjMat(filename);

	// Try to avoid pointless segfaults
	// Ensure {start,end}Node in range [0,n]
	clampBetween(0, startNode, adjMat.size());
	clampBetween(0, endNode, adjMat.size());

	Node* result = doSearch(adjMat, startNode, endNode, strategy);
	printPathAndCost(result);
}
