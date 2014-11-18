#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

class Pathfinder
{
	private:
		string current_maze;
		vector<string> solved_path;
		int maze[5][5][5]; // maze[x][y][z]
		stack<string> flip_stack;
	
	public:
		Pathfinder();
		~Pathfinder();

		//Part 1-----------------------------------------------------------------------------------
		string getMaze();
		void createRandomMaze();
		
		//Part 2-----------------------------------------------------------------------------------
		bool importMaze(string file_name);

		//Part 3-----------------------------------------------------------------------------------
		vector<string> solveMaze();
		void mazeConverter();
		bool recursiveExplore(int x, int y, int z);
		void clearPath();
};
