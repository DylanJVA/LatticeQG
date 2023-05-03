#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
using namespace std;

// This method takes the name of the input file as an argument and constructs a set of key - value list pairs
// The key of each lattice point is the simplex id and the value is the list of its neighbors ids
unordered_map<int, vector<int>> construct_lattice(string input_file) {
    // The final data structure
    unordered_map<int, vector<int>> lattice;
    
    // File reading
    string line;
    ifstream myfile(input_file);
    if (myfile.is_open()) {

        while (getline(myfile, line)) {
            stringstream ss(line);
            vector<int> tokens;

            int token;
            while (ss >> token) {
                tokens.push_back(token);
            }

            int simplex_id = tokens[0];
            vector<int> neighbors(tokens.begin() + 1, tokens.end());
            lattice[simplex_id] = neighbors;
        }

        myfile.close();
    } else {
        cout << "Unable to open file" << endl;
    }

    return lattice;
}
// The goal is to search through the lattice via 'shells' and ultimately plot the size of the shell
int main(int argc, char **argv) {
    // Construct lattice from the data set
    unordered_map<int, vector<int>> lattice = construct_lattice(argv[1]);
    // The set of simplices that have been 'looked at' while searching, but not necesarily visited, or looked at its neighbors
    unordered_set<int> seen_simplices;
    // The set of simplices who have been visited and whose neighbors have been looked at
    // Note that c++ sets do not add duplicate members
    unordered_set<int> visited_simplices;
    // List of the shell sizes for plotting
    vector<int> shell_sizes;
    // The search queue of the current shell in each iteration
    queue<int> this_shell;

    // Only parameter: which simplex to start in
    int starting_simplex = stoi(argv[2]);
    // Begin search with specified simplex
    this_shell.push(starting_simplex);
    while(visited_simplices.size() < lattice.size()) {
        // The search queue of the next shell
        queue<int> next_shell;
        // This will count the number of simplices being visited this iteration, that is what we are looking for
        int shell_size = 0;

        // Must look at each simplex in the shell
        while (!this_shell.empty()) {
            int current_simplex = this_shell.front();
            this_shell.pop();
            shell_size++;
            visited_simplices.insert(current_simplex);

            // Must add the neighbors of each simplex in the shell to the search queue for the next shell
            for (int neighbor: lattice[current_simplex]) {
                // Check if the simplex has not already been added to the queue previously 
                if (!seen_simplices.count(neighbor)) {
                    seen_simplices.insert(neighbor);
                    next_shell.push(neighbor);
                }
            }
        }
        shell_sizes.push_back(shell_size);
        // This sets the next shell queue as the current shell queue in the next iteration
        swap(this_shell,next_shell);
    }

    //for now just print for output >> .csv 
    for (int s : shell_sizes) {
        cout << s << endl;
    }
}