#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <ctime>

using namespace std;

// construct_lattice takes the name of the input file as an argument and constructs a set of key - value list pairs
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

/* The goal is to search through the lattice via 'shells'
   Inputs: 
        lattice - the lattice as a map from simplex id to its neighbors' ids
        starting_simplex - the first simplex to look at while shelling
   Output: a tuple whose first value is a 2d vector, aka a list of lists of members of each shell
        and whose second value is the shell with the largest number of members
*/ 
tuple<vector<deque<int>>,int> get_shells(unordered_map<int, vector<int>> lattice, int starting_simplex) {
    // The set of simplices that have been 'looked at' while searching, but not necesarily visited, or looked at its neighbors
    unordered_set<int> seen_simplices;
    // The set of simplices who have been visited and whose neighbors have been looked at
    // Note that c++ sets do not add duplicate members
    unordered_set<int> visited_simplices;
    // List of the shells
    vector<deque<int>> shells;
    // Keeps track of the shell with the highest number of simplices
    int max_slice = 0;
    // Keeps track of the current shell number and thus total number of shells
    int shell_num = 0;
    // The search queue of the current shell in each iteration
    deque<int> this_shell;
    // The search queue of the next shell
    deque<int> next_shell;

    
    // Begin search with specified simplex
    this_shell.push_back(starting_simplex);
    while(visited_simplices.size() < lattice.size()) {
        shells.push_back(this_shell);
        if (shells[max_slice].size() < this_shell.size()) 
            max_slice = shell_num;
        // Must look at each simplex in the shell
        while (!this_shell.empty()) {
            int current_simplex = this_shell.front();
            this_shell.pop_front();
            visited_simplices.insert(current_simplex);

            // Must add the neighbors of each simplex in the shell to the search queue for the next shell
            for (int neighbor: lattice[current_simplex]) {
                // Check if the simplex has not already been added to the queue previously 
                if (!seen_simplices.count(neighbor)) {
                    seen_simplices.insert(neighbor);
                    next_shell.push_back(neighbor);
                }
            }
        }
        // This sets the next shell queue as the current shell queue in the next iteration
        swap(this_shell,next_shell);
        shell_num++;
    }
    return {shells,max_slice};
}

float get_return_prob(unordered_map<int, vector<int>> lattice, int origin, int num_walks, int max_steps) {
    srand(time(0));
    cout << "Number of steps,Return probability" << endl;
    for (int num_steps = 100; num_steps < max_steps; num_steps += 100) {
        int current_simplex = origin;
        double return_prob = 0;
        for (int walk = 0; walk < num_walks; walk++) {
            int returned = 0;
            for (int step = 0; step < num_steps; step++) {
                current_simplex = lattice[current_simplex][rand() % num_sides];
            }
            if (current_simplex == origin)
                return_prob++;
        cout << num_steps << "," << return_prob/num_walks << endl;
    }
}


int main(int argc, char **argv) {
    // Construct lattice from the data set given as the first parameter in runtime
    unordered_map<int, vector<int>> lattice = construct_lattice(argv[1]);

    // Finds the number of sides of each simplex, assumes all simplices have the same number of sides as the first
    int num_sides = lattice[0].size();
    
    // Get list of shells starting from the simplex given as the second parameter in runtime
    // Also get the shell with the largest number of members
    auto [shells, max_slice] = get_shells(lattice, stoi(argv[2]));

    // Perform a series of random walks with a varying number of steps to determine
    // return probability as a funtion of steps
    // Choose a random simplex in the max slice to start
    int origin = shells[max_slice][rand() % shells[max_slice].size()];
    //
    /* printing for output >> .csv 
    cout << "Max slice: " << max_slice << endl;
    for (deque<int> shell : shells) {
        cout << shell.size() << endl;
    }*/
}