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

struct TokenInfo {
    string value;
    Position pos;
};

struct InputData {
    int bufferSize;
    int matrixWidth, matrixHeight;
    vector<vector<string>> matrix;
    vector<Sequence> sequences;
};


InputData readFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error opening file" << endl;
        exit(1); // or handle the error in a way appropriate for your application
    }

    InputData data;
    file >> data.bufferSize;
    file >> data.matrixWidth >> data.matrixHeight;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    // Read the matrix
    data.matrix.resize(data.matrixHeight, vector<string>(data.matrixWidth));
    for (int i = 0; i < data.matrixHeight; ++i) {
        for (int j = 0; j < data.matrixWidth; ++j) {
            file >> data.matrix[i][j];
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int numberOfSequences;
    file >> numberOfSequences;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    data.sequences.resize(numberOfSequences);
    for (int i = 0; i < numberOfSequences; ++i) {
        string line;
        getline(file, line); // Read the line of tokens
        stringstream sequence(line);
        string token;
        while (getline(sequence, token, ' ')) {
            if (!token.empty()) {
                data.sequences[i].tokens.push_back(token);
            }
        }
        file >> data.sequences[i].reward;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    file.close();
    return data;
}


int calculatePathReward(const vector<TokenInfo>& path, const vector<Sequence>& sequences) {

    
    int totalReward = 0;
    for (const auto& sequence : sequences) {
        if (sequence.tokens.size() > path.size()) continue;
        for (size_t i = 0; i <= path.size() - sequence.tokens.size(); ++i) {
            bool match = true;
            for (size_t j = 0; j < sequence.tokens.size(); ++j) {
                if (path[i + j].value != sequence.tokens[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                totalReward += sequence.reward;
                break; // Assuming a path can match a sequence only once
            }
        }
    }

    return totalReward;
}

void explorePaths(const vector<vector<string>>& matrix, Position pos, vector<TokenInfo>& path, vector<vector<bool>>& visited, int bufferSize, bool moveVertical, const vector<Sequence>& sequences, int& maxReward, vector<TokenInfo>& bestPath) {
    if (pos.x < 0 || pos.x >= matrix[0].size() || pos.y < 0 || pos.y >= matrix.size() || visited[pos.y][pos.x]) {
        return;
    }

    visited[pos.y][pos.x] = true;
    path.push_back(TokenInfo{matrix[pos.y][pos.x], pos});

    // Calculate reward for every path with size up to bufferSize
    int currentReward = calculatePathReward(path, sequences);
    if (currentReward > maxReward) {
        maxReward = currentReward;
        bestPath = path; // Update the best path
    }

    // Continue exploring if the path size is less than bufferSize
    if (path.size() < bufferSize) {
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
            explorePaths(matrix, nextPos, path, visited, bufferSize, !moveVertical, sequences, maxReward, bestPath);
        }
    }

    path.pop_back();
    visited[pos.y][pos.x] = false;
}

int main() {

    while(true){
        cout << "   Hello Welcome To CyberPunk MiniGame 2077   "<<endl;
        cout << "  ==========================================  "<<endl;

        cout << "Choose which option do you want to enter your data : " << endl;
        cout << "1. Input in terminal. " << endl;
        cout << "2. Input from txt file.  "<< endl;
        char selection;
        cin >> selection;
        InputData inputData;

        if(selection == '1'){

        }else if(selection == '2'){
            cout<<endl;
            cout<<"Masukan nama file text yang menyimpan data : ";
            string fileName;
            cin >> fileName;
            inputData = readFromFile(fileName);
            
        }
        
        vector<TokenInfo> path;

        vector<vector<bool>> visited(inputData.matrixHeight, vector<bool>(inputData.matrixWidth, false));
        int maxReward = 0;
        vector<TokenInfo> bestPath;
        int startPos;
        cout << "Choose the start point in the first row (1 - "<<inputData.matrixWidth<< ") : ";
        cin >> startPos;
        explorePaths(inputData.matrix, { startPos -1,0}, path, visited, inputData.bufferSize, true, inputData.sequences, maxReward, bestPath);
        cout<< endl;
        cout << "Best path with highest reward: " << maxReward << endl;
        cout << endl;
        for (const auto& token : bestPath) {
            cout << token.value << " ";
        }
        cout << endl;
        cout << endl;
        for (const auto& token : bestPath) {
            cout << token.pos.x + 1<< ", "<< token.pos.y + 1<<endl;
        }
        char choice;
        cout << "apakah anda ingin menyimpan solusi ? (y/n)" <<endl;
        cin >> choice;

        if(choice == 'y'){

        }else if(choice == 'n'){
            break;
        }

        

        
    }
    
    
    return 0;
}
