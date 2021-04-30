#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <memory>
#include <sstream>

//	Author Ugur Buyukdurak
// 	Finished at 29.04.2021 22:18

#define DEBUG
#undef DEBUG

using namespace std;

enum COLOR{
	WHITE,
	BLACK
};

const static map<char, float> stones {
	{'p', 1},
	{'a', 3},
	{'f', 3},
	{'k', 5},
	{'v', 9},
	{'s', 100}
};

// Helper struct for horse moves
struct Position{
	int x;
	int y;
};

// Possible moves of a horse in a chess board
vector<Position> horseMoves = {
	{1,2},
	{3,1},
	{2,-1},
	{1,-2},
	{-1,-2},
	{-2,-1},
	{-2,1},
	{-1, 2}
};

// A class showing a stone's status in a chessboard. 
struct Stone{

	float point;
	char stoneType;
	bool black;
	bool underThreat = false;

	Stone(const string & str){
		this->stoneType = str[0];
		this->black = str[1] == 's' ? true : false;
		this->point = stones.at(str[0]);
	}

	friend ostream & operator<<(ostream& out, const Stone & stone);
};

ostream & operator << (ostream &out, const Stone & stone)
{
    return out << stone.stoneType << (stone.black ? 's' : 'b') << (stone.underThreat ? 'Y' : 'N');
}

// Represents a chessboard
static map<int, map<int, unique_ptr<Stone>>> chessTable;

// Function to fill chess table
static void fillChessTable(ifstream & file){
	string line;
	string area1;
	string area2;
	string area3;
	string area4;
	string area5;
	string area6;
	string area7;
	string area8;

	int i = 8;
	int j = 1;

	while(getline(file, line)){
		unique_ptr<Stone> stone = nullptr;
		if(line == ""){
			continue;
		}
		istringstream ss(line);
		ss >> area1 >> area2 >> area3 >> area4 >> area5 >> area6 >> area7 >> area8;
#ifdef DEBUG
		cout << area1 << " " << area2 << " " << area3 << " " << area4 << " " << area5 << " " << area6 << " " << area7 << " " << area8 << endl;
#endif
		// if area is empty (if there is no stone), then assign null pointer
		chessTable[i][j++] = (area1 != "xx" ? std::make_unique<Stone>(Stone(area1)) : nullptr);
		chessTable[i][j++] = (area2 != "xx" ? std::make_unique<Stone>(Stone(area2)) : nullptr); 
		chessTable[i][j++] = (area3 != "xx" ? std::make_unique<Stone>(Stone(area3)) : nullptr); 
		chessTable[i][j++] = (area4 != "xx" ? std::make_unique<Stone>(Stone(area4)) : nullptr); 
		chessTable[i][j++] = (area5 != "xx" ? std::make_unique<Stone>(Stone(area5)) : nullptr); 
		chessTable[i][j++] = (area6 != "xx" ? std::make_unique<Stone>(Stone(area6)) : nullptr); 
		chessTable[i][j++] = (area7 != "xx" ? std::make_unique<Stone>(Stone(area7)) : nullptr); 
		chessTable[i][j++] = (area8 != "xx" ? std::make_unique<Stone>(Stone(area8)) : nullptr); 
		i--;
		j=1;
	}
}

static void printhelp(){
	cout << "Please enter relative or absolute file path to input file" << endl;
	cout << "example: ./prog.exe -h or ./prog.exe -p ./input.txt" << endl;
}

// Function to read text file into chess table
static void readTable(const string & path){
	ifstream file(path);
	if(!file.is_open()){
		cerr << "File cannot be opened please try again !" << endl;
		exit(EXIT_FAILURE);
	}

	fillChessTable(file);
	file.close();
}

// Function to mark stones threatened by a horse
static inline void calculatePointHorse(int i, int j, bool black){

	// if the horse is black
	if(black){
		for(const auto & position : horseMoves){ // Loop over pre-defined moves for a horse
			int newJ = j + position.x;
			int newI = i + position.y;
			if(newJ < 1 || newJ > 8) continue;
			if(newI < 1 || newI > 8) continue;
			if(chessTable[newI][newJ] == nullptr) continue;
			if((*chessTable[newI][newJ]).black) continue;
			if(!(*chessTable[newI][newJ]).underThreat){
				(*chessTable[newI][newJ]).point /= 2;
				(*chessTable[newI][newJ]).underThreat = true;
			}
		}
	}else{ // if the horse is white
		for(const auto & position : horseMoves){ // Loop over pre-defined moves for a horse
			int newJ = j + position.x;
			int newI = i + position.y;
			if(newJ < 1 || newJ > 8) continue;
			if(newI < 1 || newI > 8) continue;
			if(chessTable[newI][newJ] == nullptr) continue;
			if(!(*chessTable[newI][newJ]).black) continue;
			if(!(*chessTable[newI][newJ]).underThreat){
				(*chessTable[newI][newJ]).point /= 2;
				(*chessTable[newI][newJ]).underThreat = true;
			}
		}
	}
}

// Function to mark stones threatened by a elephant
static inline void calculatePointElephant(int i, int j, bool black){
	// if the elephant is black
	if(black){
		//Each for-loop represents one of the four diagonal paths.
		for(int k = i-1, z = j-1; k >= 1 && z >= 1; k--, z--){
			if(chessTable[k][z] == nullptr) continue;
			if((*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}

		for(int k = i+1, z = j+1; k <= 8 && z <= 8; k++, z++){
			if(chessTable[k][z] == nullptr) continue;
			if((*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}

		for(int k = i+1, z = j-1; k <= 8 && z >= 1; k++, z--){
			if(chessTable[k][z] == nullptr) continue;
			if((*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}

		for(int k = i-1, z = j+1; k >= 1 && z <= 8; k--, z++){
			if(chessTable[k][z] == nullptr) continue;
			if((*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}
	}else{ // if the elephant is white
		for(int k = i-1, z = j-1; k >= 1 && z >= 1; k--, z--){
			if(chessTable[k][z] == nullptr) continue;
			if(!(*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}

		for(int k = i+1, z = j+1; k <= 8 && z <= 8; k++, z++){
			if(chessTable[k][z] == nullptr) continue;
			if(!(*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}

		for(int k = i+1, z = j-1; k <= 8 && z >= 1; k++, z--){
			if(chessTable[k][z] == nullptr) continue;
			if(!(*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}

		for(int k = i-1, z = j+1; k >= 1 && z <= 8; k--, z++){
			if(chessTable[k][z] == nullptr) continue;
			if(!(*chessTable[k][z]).black) break;
			if(!(*chessTable[k][z]).underThreat){ 
				(*chessTable[k][z]).point /= 2;
				(*chessTable[k][z]).underThreat = true;
			}
			break;
		}
	}
}

// Point calculation is made here
static map<COLOR, float> calculatePoints(){
	map<COLOR, float> result;

	// Traverse the table
	// Calculate final point score based on table's stone layout.
	for(int i = 8; i >= 1; i--){
		for(int j = 1; j <= 8 ;j++){
			if(chessTable[i][j] == nullptr)
				continue;
			switch((*chessTable[i][j]).stoneType){
				case 'f': // if the stone is elephant, then find all other stones threatened by this stone.
					calculatePointElephant(i, j, (*chessTable[i][j]).black);
					break;

				case 'a':  // if the stone is horse, then find all other stones threatened by this stone.
					calculatePointHorse(i, j, (*chessTable[i][j]).black);
					break;
			}
		}
	}

	float whiteSum = 0;
	float blackSum = 0;

	// After status update of all the stones in chessTable, calculate result points
	for(int i = 1; i <= 8; i++){
		for(int j = 1; j <= 8; j++){
			if(chessTable[i][j] != nullptr){
				if((*chessTable[i][j]).black){
					blackSum += (*chessTable[i][j]).point;
				}else{
					whiteSum += (*chessTable[i][j]).point;
				}
			}
		}
	}

	result[BLACK] = blackSum;
	result[WHITE] = whiteSum;

	return result;
}

int main(int argc, char * argv[]){
	string filepath;
	int c;

	if(argc == 1){
		cout << "Please provide a command line argument first" << endl;
		cout << "Call the program with -h command line option for help" << endl;
		exit(EXIT_FAILURE);
	}

	while ((c = getopt (argc, argv, "hp:")) != -1){
		switch(c){
			case 'h':
				printhelp();
				exit(EXIT_FAILURE);
				break;
			case 'p':
				filepath = string(optarg);
				break;
			default:
				printhelp();
				exit(EXIT_FAILURE);
				break;
		}
	}

	// Read table into chessTable
	readTable(filepath);

	auto result = calculatePoints();

#ifdef DEBUG
	cout << "----------------------" << endl;
	for(int i = 8; i >= 1; i--){
		for(int j = 1; j <= 8 ;j++){
			if(chessTable[i][j] != nullptr){
				cout << *(chessTable[i][j]) << " ";
			}else{
				cout << "xx" << " ";
			}
		}
		cout << endl;
	}
#endif

	cout << "Siyah : " << result[BLACK] << ", " << "Beyaz : " << result[WHITE] << endl;

	return 0;
}