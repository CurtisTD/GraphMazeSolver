/* Project 3 - Curtis Davis - COT 4400 FA19*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/range.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>

using std::string;

struct Vertex {

	int northTravel;
	int eastTravel;
	int southTravel;
	int westTravel;
	int upTravel;
	int downTravel;
};

struct Edge {
	string label;
};

using Graph_t = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, Vertex, Edge>;
using Vertex_t = Graph_t::vertex_descriptor;

/* Function: readInFile
This code is to open the file and read-in the information */
void spiderMaze() {
	std::ifstream inputFile("input.txt");
	std::ofstream outputFile("output.txt");
	std::string line;
	std::string value;

	if (inputFile.is_open()) {
		int numberOfMazes;

		int levelsOfMaze, rowsOfMaze, colsOfMaze;		
		int levelStart, rowStart, colStart;	
		int levelGoal, rowGoal, colGoal; //Maze attributes
		
		inputFile >> numberOfMazes; //Gets frst int, the number of mazes in file

		for (int numMaze = 0; numMaze < numberOfMazes; numMaze++) {
			Graph_t mazeGraph; //Creates an adjacency list called 'graph'
			std::vector<boost::adjacency_list<>::vertex_descriptor> vectorOfVerts;

			inputFile >> levelsOfMaze >> rowsOfMaze >> colsOfMaze; //Gets new graph attributes
			inputFile >> levelStart >> rowStart >> colStart; //Gets Maze start
			inputFile >> levelGoal >> rowGoal >> colGoal; //Gets Maze goal
			int numberOfCells = levelsOfMaze * rowsOfMaze*colsOfMaze;

			std::string temp;
			int id = 0;
			for (int l = 0; l < levelsOfMaze; l++) {
				for (int r = 0; r < rowsOfMaze; r++) {
					for (int c = 0; c < colsOfMaze; c++) {
						inputFile >> temp;

						Vertex vTemp;

						vTemp.northTravel= temp[0] - '0';
						vTemp.eastTravel = temp[1] - '0';
						vTemp.southTravel = temp[2] - '0';
						vTemp.westTravel =  temp[3] - '0';
						vTemp.upTravel = temp[4] - '0';
						vTemp.downTravel = temp[5] - '0';
												
						auto u = boost::add_vertex(vTemp, mazeGraph); //Add all vertices to the graph
						vectorOfVerts.push_back(u); //Pushed vertex into vector of vertices
					}
				}
			}

			//Add edges for adjacency list
			for (int i = 0; i < numberOfCells; i++) {
				//If it can travel in given direction, add an edge for the cells
				if (mazeGraph[i].northTravel) {
					boost::add_edge(vectorOfVerts[i], vectorOfVerts[i - colsOfMaze], Edge{ "N" }, mazeGraph); //Connects it to respective vertex
				}
				if (mazeGraph[i].eastTravel) {
					boost::add_edge(vectorOfVerts[i], vectorOfVerts[i + 1], Edge{ "E" }, mazeGraph); //? +1 or -1
				}
				if (mazeGraph[i].southTravel) {
					boost::add_edge(vectorOfVerts[i], vectorOfVerts[i + colsOfMaze], Edge{ "S" }, mazeGraph);
				}
				if (mazeGraph[i].westTravel) {
					boost::add_edge(vectorOfVerts[i], vectorOfVerts[i - 1], Edge{ "W" }, mazeGraph);
				}
				if (mazeGraph[i].upTravel) {
					boost::add_edge(vectorOfVerts[i], vectorOfVerts[i + (rowsOfMaze*colsOfMaze)], Edge{ "U" }, mazeGraph);
				}
				if (mazeGraph[i].downTravel) {
					boost::add_edge(vectorOfVerts[i], vectorOfVerts[i - (rowsOfMaze*colsOfMaze)], Edge{ "D" }, mazeGraph);
				}
			}

			int levelGoalID = (levelGoal * (rowsOfMaze*colsOfMaze)) + (rowGoal * rowsOfMaze) + (colGoal); //Gets id of goal vertex
			int levelStartID = (levelStart * (rowsOfMaze*colsOfMaze)) + (rowStart * rowsOfMaze) + (colStart); //Gets id of goal vertex
			
			//Find levelGoalID vertex with DFS starting at levelStartID
			std::vector<Graph_t::vertex_descriptor> pred(num_vertices(mazeGraph), Graph_t::null_vertex());

			//Preform DFS at a starting vertex while recording the predecessors on every tree edge which forms the search tree
			boost::depth_first_search(mazeGraph, visitor(
				boost::make_dfs_visitor(
					boost::record_predecessors(pred.data(), boost::on_tree_edge{})) 
						).root_vertex(levelStartID));
			
			std::vector<string> directionsTraveled;
			int newTrackID = levelGoalID; //Initialization
			while (newTrackID != levelStartID) {
				for (auto v : boost::make_iterator_range(vertices(mazeGraph))) {
					if (v == newTrackID) {
						//Pushes the edges of the dfs traversal, starting from goal (backwards)
						if (v == (pred.at(v) - colsOfMaze)) {
							directionsTraveled.push_back("N");
						}
						if (v == (pred.at(v) + 1)) {
							directionsTraveled.push_back("E");
						}
						if (v == (pred.at(v) + colsOfMaze)) {
							directionsTraveled.push_back("S");
						}
						if (v == (pred.at(v) - 1)) {
							directionsTraveled.push_back("W");
						}						
						if (v == (pred.at(v) + (colsOfMaze*rowsOfMaze)) ) {
							directionsTraveled.push_back("U");
						}
						if (v == (pred.at(v) - (colsOfMaze*rowsOfMaze)) ) {
							directionsTraveled.push_back("D");
						}

						newTrackID = pred.at(v); //Go to next predecessor
						if (newTrackID == levelStartID) {
							break;
						}
					}
				}
			}
			
			std::cout << "Outputting a maze...\n";
			//Print out the directions traveled through the maze (in reverse)
			for (std::vector<string>::reverse_iterator iter = directionsTraveled.rbegin(); iter != directionsTraveled.rend(); ++iter) {
				outputFile << *iter << " ";
			}
			outputFile << std::endl;
			mazeGraph.clear(); //Clears graph
		}
	}
	inputFile.close();
	outputFile.close();
}

int main (){
	spiderMaze(); //Gets input and produces directions for output

	return EXIT_SUCCESS;
}