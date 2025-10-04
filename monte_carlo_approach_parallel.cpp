# include <bits/stdc++.h>
# include <fstream>
# include <omp.h>

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

// 6. recovery function
void recover(people, int);

int main(){
    // try to clear the file if it exists
    try{
        fstream fout(sim_path, ios::out | ios::trunc);
        fout << "";
        fout.close();
    } catch(const ifstream::failure& e){
        // do nothing
    }
    read_graph();
    int initial_infected;
    cout << "Enter the number of initially infected nodes out of the current population of " << N << ": ";
    cin >> initial_infected;
    randomly_infect(initial_infected);
    print_states(P); // print the initial states
    int time_steps;
    cout << "Enter the number of time steps: ";
    cin >> time_steps;
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
    fin >> N >> transmission_prob >> recovery_prob;
    G.resize(N); // resize the graph
    P.resize(N, SUSCEPTIBLE); // populating the people vector with SUSCEPTIBLE state (all are susceptible initially)
    int u, v;
    while(fin >> u >> v){
        G[u].push_back(v);
        G[v].push_back(u);
    }
    fin.close();
}

// Function 2
void randomly_infect(int initial_infected){
    for(int i = 0; i < initial_infected; i++){
        int rand_node = rand() % N;
        P[rand_node] = INFECTED;
    }
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

// Function 6
void recover(people new_P, int node){
    # pragma omp master
    if(random_decision(recovery_prob)){
        cout << "Recovered node " << node << endl;
        new_P[node] = RECOVERED;
    }
}

// Function 4
void evolve(int time_steps){
    double start = clock();

    
    for(int i = 0; i < time_steps; i++){
        people new_P = P; // copy the current states
        # pragma omp parallel for
        for(int node = 0; node < N; node++){
            if(P[node] == INFECTED){
                // try to infect neighbors
                for(int neighbor : G[node]){
                    if(P[neighbor] == SUSCEPTIBLE && random_decision(transmission_prob)){
                        new_P[neighbor] = INFECTED;
                    }
                }
                // try to recover (only from the master thread since this is a write operation
                // and also we're only doing this once)
                recover(new_P, node);
            }
        }
        P = new_P; // update the states
        print_states(P); // print the states after this time step
    }
    double end = clock();
    cout << "Simulation completed in " << (end - start) / CLOCKS_PER_SEC << " seconds.\n";
}