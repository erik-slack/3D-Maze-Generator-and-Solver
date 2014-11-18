#include "Pathfinder.h"
#include <fstream>
#include <sstream>
#include <time.h>
#include <cstdlib>
using namespace std;


Pathfinder::Pathfinder(){srand(time(0));}
Pathfinder::~Pathfinder(){}

//Part 1-----------------------------------------------------------------------------------
/* 
 * getMaze
 * 
 * Returns a string representation of the current maze. Returns a maze of all 1s if no maze
 * has yet been generated or imported.
 * 
 * A valid string representation of a maze consists only of 125 1s and 0s (each separated 
 * by spaces) arranged in five 5x5 grids (each separated by newlines). A valid maze must 
 * also have 1s in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).
 *
 * Cell (0, 0, 0) is represented by the first number in the string representation of the 
 * maze. Increasing in x means moving toward the east, meaning cell (1, 0, 0) is the second
 * number. Increasing in y means moving toward the south, meaning cell (0, 1, 0) is the 
 * sixth number. Increasing in z means moving downward to a new grid, meaning cell 
 * (0, 0, 1) is the twenty-sixth number. Cell (4, 4, 4) is represented by the last number.
 *
 * Returns:		string
 *				A single string representing the current maze
 */
string Pathfinder::getMaze(){	
	bool is_valid = true;
	stringstream ss;
	
	bool has_maze = false;
	bool all_one = false;
	
	if (current_maze.length() != 0){
		has_maze = true;
	} else {
	}
	
	if (all_one == true || has_maze == false){
		is_valid = false;
	}
	
	if (is_valid == true){
		//cout << current_maze;
		return current_maze;
	} else {
		string invalid_response = " 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1\n 1 1 1 1 1";
		//cout << invalid_response;
		return invalid_response;
	}
}

/* 
 * createRandomMaze
 * 
 * Generates a random maze and stores it as the current maze.
 * 
 * The generated maze must contain a roughly equal number of 1s and 0s and must have a 1 
 * in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be 
 * solvable or unsolvable, and this method should be able to produce both kinds of mazes.
 */
void Pathfinder::createRandomMaze(){
	int one_count = 0;
	int zero_count = 0;
	
	int random_maze_space = -1;
	
	stringstream ss;
	
	for (int i = 1; i < 126; i++){
		if ((i == 125 || i == 1)){
			random_maze_space = 1;
		} else {
			if (one_count > 1 + zero_count){
				random_maze_space = 0;
			} else if (zero_count > 1 + one_count){
				random_maze_space = 1;
			} else {
				random_maze_space = rand() % 2;
			}
		}
			
		if (random_maze_space == 0){
			zero_count++;
		} else if (random_maze_space == 1){
			one_count++;
		}
		
		ss << " " << random_maze_space;
		if (i < 125){
			if (i % 5 == 0){
				ss << endl;
			}
			
			if (i % 25 == 0){
				ss << endl;
			}
		}
		
	}
	current_maze = ss.str();
}
//-----------------------------------------------------------------------------------------

//Part 2-----------------------------------------------------------------------------------
/* 
 * importMaze
 * 
 * Reads in a maze from a file with the given file name and stores it as the current maze.  
 * Does nothing if the file does not exist or if the file's data does not represent a valid
 * maze.
 * 
 * The file's contents must be of the format described above to be considered valid.
 * 
 * Parameter:	file_name
 *				The name of the file containing a maze
 * Returns:		bool
 *				True if the maze is imported correctly; false otherwise
 */
bool Pathfinder::importMaze(string file_name){
	bool is_valid = true;
	bool all_one = true;
	
	stringstream ss;
	ifstream in_file;
	
	const char* maze_choice = file_name.c_str();
	//cout << "File Name = " << file_name << endl;
	in_file.open(maze_choice);
	
	if(in_file.is_open()){
		int count = 1;
		
		string new_maze_space;

		while (in_file >> new_maze_space){
			//cout << count;
			if (((count == 1 || count == 125) && new_maze_space != "1") || count > 125){
				is_valid = false;
			}
			
			if (new_maze_space == "0" || new_maze_space == "1") {
				if (new_maze_space == "0"){
					all_one = false;
				}
				if ((count - 1) % 5 != 0){
					ss << " ";
				}
				ss << new_maze_space;
			} else {
				ss << "Maze space " << new_maze_space << "format is invalid." << endl;
				is_valid = false;
			}
			
			if (count % 5 == 0 && count != 125){
				ss << endl;
			}
			if (count % 25 == 0 && count != 125){ss << endl;}
			count++;
		}
		
		if (count < 125 || count > 126){
			is_valid = false;
		}
		
		//cout << endl << "---" << endl << ss.str() << endl;
	} else {
		is_valid = false;
	}
	
	if (is_valid == true){
		current_maze = ss.str();
	}
	
	return is_valid;
}
//-----------------------------------------------------------------------------------------

//Part 3-----------------------------------------------------------------------------------
/* 
 * solveMaze
 * 
 * Attempts to solve the current maze and returns a solution if one is found.
 * 
 * A solution to a maze is a list of coordinates for the path from the entrance to the exit
 * (or an empty vector if no solution is found). This list cannot contain duplicates, and 
 * any two consecutive coordinates in the list can only differ by 1 for only one 
 * coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included 
 * in the solution. Each string in the solution vector must be of the format "(x, y, z)", 
 * where x, y, and z are the integer coordinates of a cell.
 *
 * Understand that most mazes will contain multiple solutions
 * 
 * Returns:		vector<string>
 *				A solution to the current maze, or an empty vector if none exists
 */
vector<string> Pathfinder::solveMaze(){
	mazeConverter();
	
	recursiveExplore(0, 0, 0);
	
	//for (int i = 0; i < solved_path.size(); i++){
		//cout << solved_path[i] << endl;
	//}
	
	clearPath();
	
	while(flip_stack.size() > 0){
		solved_path.push_back(flip_stack.top());
		flip_stack.pop();
	}
	
	return solved_path;
}

void Pathfinder::mazeConverter(){
	stringstream ss(current_maze);
	int token = 0;
	int count = 0; // FOR DEBUGGINS COSMETICS ONLY
	
	int x = 0;
	int y = 0;
	int z = 0;
	
	while (ss >> token){
		count++;
		if (x > 4){x = 0; y++;}
		if (y > 4){y = 0; z++;}
		if (z > 4){cout << "ERROR";}
		
		//cout << "x = " << x << " y = " << y << " z = " << z << endl;
		
		maze[x][y][z] = token;
		
		//cout << maze[x][y][z];
		//cout << token;
		//if (count % 5 == 0){cout << endl;}
		//if (count % 25 == 0){cout << endl;}
		//if (count % 125 == 0){cout << endl;}
		
		x++;
	}
}

bool Pathfinder::recursiveExplore(int x, int y, int z){
	stringstream ss;
	if (x == 4 && y == 4 && z == 4){ // BASE CASE
		ss << "(" << x << ", " << y << ", " << z << ")";
		flip_stack.push(ss.str()); 
		return true;
	}
	else if (x < 0 || x > 4 || y < 0 || y > 4 || z < 0 || z > 4){return false;} // OUT OF BOUNDS
	else if (maze[x][y][z] == 0){return false;} // IMPASSABLE
	else if (maze[x][y][z] == 2){return false;} // PAINTED, CAN'T BACKTRACK
	
	//cout << maze[x][y][z];
	
	maze[x][y][z] = 2;
	
	ss << "(" << x << ", " << y << ", " << z << ")";
	
	if (recursiveExplore(x + 1, y, z) || recursiveExplore(x, y + 1, z) || recursiveExplore(x, y, z + 1) || recursiveExplore(x - 1, y, z) || recursiveExplore(x, y - 1, z) || recursiveExplore(x, y, z - 1)){
		flip_stack.push(ss.str());
		return true;
	}
	
	return false;
}

void Pathfinder::clearPath(){
	solved_path.clear();
}
//-----------------------------------------------------------------------------------------


/* NOTES FROM HELP SESSION 11 AM Friday
 X bool recursion(int x, int y){
 X Base Case
 X OUT OF BOUNDS?
 X VALID SPOT?
 X if ( // only one of these needs to return true
	 * Recursion(x+1, y) ||
	 * Recursion(x, y+1) ||
	 * Recursion(x, y-1) ||
	 * Recursion(x-1, y)
	 * }
 X ){return true;}
 X else {return false;}
 * avoid infinite loop back and forth (think of boggle); it's called painting: where have I been before?
 * change each one you've been to, to 2 (maze[x][y] = 2;)
 * then at the end, revert back to 1
 * 
 * for vector use insert(index, pos) on everytime after the first on which you use pushback
 * index = myVector.begin();
 * Don't forget to empty your vector out
 * */
