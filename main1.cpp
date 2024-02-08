#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

struct Sequence {
    vector<string> tokens;
    int reward;
};

struct Position {
    int x, y;
};

void explorePaths(const vector<vector<string>>& matrix, Position pos, vector<string>& path, vector<vector<string>>& allPaths, vector<vector<bool>>& visited, int bufferSize, bool moveVertical) {
    if (pos.x < 0 || pos.x >= matrix[0].size() || pos.y < 0 || pos.y >= matrix.size() || visited[pos.y][pos.x]) {
        // Out of bounds or already visited
        return;
    }

    // Mark the current position as visited
    visited[pos.y][pos.x] = true;

    if (path.empty() && !(pos.x == -1 && pos.y == -1)) {
        path.push_back(matrix[pos.y][pos.x]);
    }

    if (path.size() == bufferSize) {
        allPaths.push_back(path);
        // Debugging output
        for (const string& token : path){
            cout << token << " ";
        }
        cout <<endl;
        // Unmark the current position before backtracking
        visited[pos.y][pos.x] = false;
        return;
    }

    vector<Position> nextPositions;
    if (moveVertical) {
        for (int newY = pos.y + 1; newY < matrix.size(); ++newY) {
            nextPositions.push_back({pos.x, newY});
        }
        for (int newY = pos.y - 1; newY >= 0; --newY) {
            nextPositions.push_back({pos.x, newY});
        }
    } else {
        for (int newX = pos.x + 1; newX < matrix[0].size(); ++newX) {
            nextPositions.push_back({newX, pos.y});
        }
        for (int newX = pos.x - 1; newX >= 0; --newX) {
            nextPositions.push_back({newX, pos.y});
        }
    }

    for (const Position& nextPos : nextPositions) {
        if (!visited[nextPos.y][nextPos.x]) {
            path.push_back(matrix[nextPos.y][nextPos.x]);
            explorePaths(matrix, nextPos, path, allPaths, visited, bufferSize, !moveVertical); // Alternate direction
            path.pop_back(); // Backtrack
        }
    }

    // Unmark the current position before backtracking
    visited[pos.y][pos.x] = false;
}


int main() {
    ifstream file("token.txt"); // Replace with your file path

    if (!file) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    int bufferSize;
    int matrixWidth, matrixHeight;
    file >> bufferSize;
    file >> matrixWidth >> matrixHeight;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    // Read the matrix
    vector<vector<string>> matrix(matrixHeight, vector<string>(matrixWidth));
    for (int i = 0; i < matrixHeight; ++i) {
        for (int j = 0; j < matrixWidth; ++j) {
            file >> matrix[i][j];
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int numberOfSequences;
    file >> numberOfSequences;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    // Read the sequences and rewards
    vector<Sequence> sequences(numberOfSequences);
    for (int i = 0; i < numberOfSequences; ++i) {
        string line;
        getline(file, line); // Read the line of tokens
        cout <<line<<" kwkwkw"<<endl;
        stringstream sequence(line); // Create a stringstream from the line
        string token;
        while (getline(sequence, token, ' ')) { // Use getline on stringstream to split by space
            if (!token.empty()) { // Check to avoid empty tokens due to spaces
                sequences[i].tokens.push_back(token);
            }
        }

        file >> sequences[i].reward;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    file.close();

    cout<<bufferSize<<endl;
    cout<<matrixHeight<<endl;
    cout<<matrixWidth<<endl;
    
    for (int i = 0; i < matrixHeight; ++i) {
        for (int j = 0; j < matrixWidth; ++j) {
            cout<<matrix[i][j];
            cout<<" ";

        }
        cout<<endl;
    }
    cout<<numberOfSequences<<endl;

    for (int i = 0; i < sequences.size(); ++i) {
        cout << "Sequence " << i + 1 << ": ";
        for (const string& token : sequences[i].tokens) {
            cout << token << " ";
        }
        cout << "- Reward: " << sequences[i].reward << endl;
    }
    vector<string> path;
    vector<vector<string>> allPaths;
    vector<vector<bool>> visited(matrixHeight, vector<bool>(matrixWidth, false));
    explorePaths(matrix,{0,0},path,allPaths,visited,bufferSize,true);

    
    return 0;
}
