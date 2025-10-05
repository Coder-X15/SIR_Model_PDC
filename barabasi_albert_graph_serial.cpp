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
// we read the following from `edges_bam.txt` and generate the graph:
//  1. Population size
//  2. Recovery probability
//  3. Edges of the graph (hence the number of infected nodes in the network)
// after getting the graph, we randomly infect a few nodes as given by the user input

// population metadata
int N; // population size
double transmission_prob, recovery_prob; // recovery probability
graph G; // the graph
people P; // the people's states
ifstream fin("edges_bam.txt"); // input file stream
string sim_path = "./sim_bam/states.csv"; // where the simulation is being written to

// helper functions

// 1. read the graph from the file
void read_graph();

// 2. print the states of the nodes into a csv file
void print_states(people);

// 3. a random yes/np decision based on a probability
bool random_decision(double);

// 4. evolution function
void evolve(int);

// 5. get the set of infected nodes
set<int> get_infected();

// 6. choose a set of nodes based on the law of preferential attachment
set<int> choose(set<int>);

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
    cout << "\n    Initial Infected = " << (get_infected()).size();
    cout  << "\n    Transmission Probability = " << transmission_prob;
    cout  << "\n    Recovery Probability = " << recovery_prob << endl;
    int time_steps;
    cout << "Enter the number of time steps: ";
    cin >> time_steps;
    print_states(P); // print the initial states
    cout << "Starting simulation... " << endl;
    cout << "Time step 0: S = " << count(P.begin(), P.end(), SUSCEPTIBLE); 
    cout << ", I = " << count(P.begin(), P.end(), INFECTED); 
    cout << ", R = " << count(P.begin(), P.end(), RECOVERED) << endl;
    evolve(time_steps);
    return 0;
}

// Function 5
set<int> get_infected(){
    set<int> infected = {};
    for(int i = 0; i < N; i++){
        if(P[i] == INFECTED){infected.insert(i);}
    }

    return infected;
}

// Function 3
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
        if(find(G[u].begin(), G[u].end(), v) == G[u].end()){G[u].push_back(v);} 
        if(find(G[v].begin(), G[v].end(), u) == G[v].end()){G[v].push_back(u);}
        P[u] = INFECTED; P[v] = INFECTED;
    }
    fin.close();
}

// Function 2
void print_states(people P){
    ofstream fout(sim_path, ios::app);
    for(int state : P){
        fout << state << ",";
    }
    fout << "\n";
    fout.close();
}

// Function 4
void evolve(int time_steps){
    double start = clock();
    for(int i = 0; i < time_steps; i++){
        int S, I, R;
        people new_P = P; // copy the current states
        set<int> infected = get_infected();
        for(int node = 0; node < N; node++){
            if(P[node] == SUSCEPTIBLE){
                // connect to the graph of infected persons
                set<int> to_connect = choose(infected);
                for(auto target : to_connect){
                    // connect to target, be counted as infected
                    G[target].push_back(node);
                    G[node].push_back(target);
                    new_P[node] = INFECTED;
                }
                
            } else {continue;}
        }
        for(auto candidate : infected){
            if(random_decision(recovery_prob)){
                new_P[candidate] = RECOVERED;
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


// Function 6
set<int> choose(set<int> available){
    double total_deg = 0;
    set<int> ret = {};
    for(auto choice : available){
        total_deg += G[choice].size();
    }
    for(auto choice : available){
        double prob = G[choice].size() / total_deg; 
        if(random_decision(prob * transmission_prob)){
            ret.insert(choice);
        }
    }
    return ret;
}