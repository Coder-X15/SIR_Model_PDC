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
//  4. The number of initially infected people
// after getting the graph, we randomly infect a few nodes as given by the user input

// population metadata
int N; // population size
int initial_infected; // number of initially infected nodes
double transmission_prob, recovery_prob; // transmission and recovery probabilities
int m; // the number of connects that a node (person) may have on average per timestep
graph G; // the graph
people P; // the people's states
ifstream fin("params.txt"); // input file stream
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

// 6. Create the Barabasi-Albert network
void create_barabasi_albert_network();

int main(){
    // try to clear the file if it exists
    srand(time(NULL));
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
    create_barabasi_albert_network();
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
    fin >> N >> transmission_prob >> recovery_prob >> initial_infected >> m;
    if(m > N){
        throw("m should be smaller than N");
    }
    G.resize(N); // resize the graph
    P.resize(N, SUSCEPTIBLE); // populating the people vector with SUSCEPTIBLE state (all are susceptible initially)
    fin.close();
    cout << "Finished taking input parameters" << endl;
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

// Function 6
void create_barabasi_albert_network(){
    cout << "Creating the requisite Barabasi-Albert network..." << endl;
    vector<int> node_pool;

    // we'll initially start with a complete graph of m + 5 nodes
    int m0 = m + 5; // initialising a random growth rate
    if(m0 > N){
        m0 = m;
    }
    for(int i = 0; i < m0; i++){
        for(int j = i+1; j < m0; j++){
            G[i].push_back(j);
            G[j].push_back(i);
        }
    }

    // populate the node_pool to do preferential attachment
    for(int i = 0; i < m0; i++){
        for(int target : G[i]){
            node_pool.push_back(i);
        }
    }

    // growth based on preferential attachment
    for(int k = m0; k < N; k++){
        cout << "   Connecting node: " << k << "..." << endl;
        set<int> targets;
        while(targets.size() < m){
            int idx = rand()%node_pool.size();;
            int target = node_pool[idx];
            if(target != k){targets.insert(target);}
        }

        for(int target : targets){
            G[k].push_back(target);
            G[target].push_back(k);

            node_pool.push_back(k);
            node_pool.push_back(target);

        }   
    }

    // writing output graph into file for simulation purposes
    ofstream output("edges.txt");
    output << N << endl;
    for(int node = 0; node < N; node++){
        for(int target : G[node]){
            output << node << " " << target << endl;
        }
    }
    output.close();
}