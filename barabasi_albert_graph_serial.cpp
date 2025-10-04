/*
    Required changes:
    1. A function to randomly choose `n` out of `M` nodes in a graph 
       provided (provided `m` > `n`).
    2. Modify the `evolve` function to fit the logic of the infection spread properly.
    3. Update documentation regarding different input formats for the different 
       algorithms (if required).
*/

# include <bits/stdc++.h>
# include <fstream>

using namespace std;

# define graph vector<vector<int>>
# define people vector<int>

# define SUSCEPTIBLE 0
# define INFECTED 1
# define RECOVERED 2

// we use the integer indices to represent the nodes
// we read the following from `edges.txt` and generate the graph:
//  1. Population size
//  2. Transmission probability
//  3. Recovery probability
//  4. Edges of the graph
// after getting the graph, we randomly infect a few nodes as given by the user input

// population metadata
int N; // population size
int initial_infected; // number of initially infected nodes
double transmission_prob, recovery_prob; // transmission and recovery probabilities
graph G; // the graph
people P; // the people's states
ifstream fin("edges.txt"); // input file stream
string sim_path = "./sim/states.csv"; // where the simulation is being written to

// helper functions

// 1. read the graph from the file
void read_graph();

// 2. randomly infect a few nodes
void randomly_infect(int);

// 3. print the states of the nodes into a csv file
void print_states(people);

// 4. a random yes/np decision based on a probability
bool random_decision(double);

// 5. evolution function
void evolve(int time_steps);

int main(){
    // try to clear the file if it exists
    cout << "BY: Sam Ruben Abraham (2023BCD0002) \n    Anushka Kishor Nikam (2023BCS0047)" << endl;
    try{
        fstream fout("./sim/states.csv", ios::out | ios::trunc);
        fout << "";
        cout << "Cleared existing states.csv file.\n";
        fout.close();
    } catch(const ifstream::failure& e){
        // do nothing
    }
    read_graph();
    cout << "Setting:\n    Total Population = " << N ;
    cout << "\n    Initial Infected = " << initial_infected;
    cout << "\n    Transmission Probability = " << transmission_prob;
    cout  << "\n    Recovery Probability = " << recovery_prob << endl;
    int time_steps;
    cout << "Enter the number of time steps: ";
    cin >> time_steps;
    randomly_infect(initial_infected);
    print_states(P); // print the initial states
    cout << "Starting simulation... " << endl;
    cout << "Time step 0: S = " << count(P.begin(), P.end(), SUSCEPTIBLE); 
    cout << ", I = " << count(P.begin(), P.end(), INFECTED); 
    cout << ", R = " << count(P.begin(), P.end(), RECOVERED) << endl;
    evolve(time_steps);
    return 0;
}

// Function 4
bool random_decision(double prob){
    double rand_val = (double)rand() / RAND_MAX;
    return rand_val < prob;
}

// Function 1
void read_graph(){
    fin >> N >> transmission_prob >> recovery_prob >> initial_infected;
    G.resize(N); // resize the graph
    // since the assumption under the Barabasi-Albert graph as in the paper is that
    // each node in the whole sample is linked by at least one link, we shall connect
    // every node in the list of nodes in a loop
    for(int i = 0; i < N;i++){
        G[i%N].push_back((i+1)%N);
        G[(i+1)%N].push_back(i%N);
    }
    P.resize(N, SUSCEPTIBLE); // populating the people vector with SUSCEPTIBLE state (all are susceptible initially)
    int u, v;
    while(fin >> u >> v){
        if(find(G[u].begin(), G[u].end(), v) == G[u].end()){G[u].push_back(v);}
        if(find(G[v].begin(), G[v].end(), u) == G[v].end()){G[v].push_back(u);}
    }
    fin.close();
}

// Function 2
void randomly_infect(int initial_infected){
    int infected_count = 0;
    while(infected_count < initial_infected){
        int index = rand() % N;
        if(P[index] == INFECTED) continue; // already infected
        P[index] = INFECTED;
        cout << "Infected node " << index << endl;
        infected_count++;
    }
    cout << "Randomly infected " << initial_infected << " nodes.\n";
}

// Function 3
void print_states(people P){
    ofstream fout(sim_path, ios::app);
    for(int state : P){
        fout << state << ",";
    }
    fout << "\n";
    fout.close();
}

// Function 5
void evolve(int time_steps){
    double start = clock();
    for(int i = 0; i < time_steps; i++){
        int S, I, R;
        people new_P = P; // copy the current states
        for(int node = 0; node < N; node++){
            if(P[node] == INFECTED){
                // try to infect neighbors
                for(int neighbor : G[node]){
                    if(P[neighbor] == SUSCEPTIBLE && random_decision(transmission_prob)){
                        new_P[neighbor] = INFECTED;
                    }
                }
                // try to recover
                if(random_decision(recovery_prob)){
                    new_P[node] = RECOVERED;
                }
            }
        }
        S = count(new_P.begin(), new_P.end(), SUSCEPTIBLE);
        I = count(new_P.begin(), new_P.end(), INFECTED);
        R = count(new_P.begin(), new_P.end(), RECOVERED);
        cout << "After time step " << i + 1 << ": S = " << S << ", I = " << I << ", R = " << R << endl;
        P = new_P; // update the states
        print_states(P); // print the states after this time step
    }
    double end = clock();
    cout << "Simulation completed in " << (end - start) / CLOCKS_PER_SEC << " seconds.\n";
}