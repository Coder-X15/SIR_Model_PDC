
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iomanip>

using namespace std;

// helper functions
void setup();
void write_log(const vector<double>& vals);
void simulate(int);

int N; // population size
double transmission_prob, recovery_prob; // transmission and recovery probabilities
int m; // one residual parameter which we will ignore for now
double S; // the number of susceptible people
double I; // number of initially infected nodes
double R; // the number of recovered people
double delta_S = 0; // the gradient of S
double delta_I = 0; // the gradient of I
double delta_R = 0; // the gradient of R

int main(){
    cout << "BY: Sam Ruben Abraham (2023BCD0002) \n    Anushka Kishor Nikam (2023BCS0047)" << endl;
    setup();
    cout << "Setting:\n    Total Population = " << N ;
    cout << "\n    Initial Infected = " << I;
    cout << "\n    Transmission Probability = " << transmission_prob;
    cout  << "\n    Recovery Probability = " << recovery_prob << endl;
    int time_steps;
    cout << "Enter the number of time steps: ";
    cin >> time_steps;
    simulate(time_steps);
    cout << "Output of simulation written to ./sim/actual.csv" << endl;
    return 0;
}

void setup(){
    // (re)create output file and write header
    ofstream fout("./sim/actual.csv", ios::out | ios::trunc);
    if(!fout.is_open()){
        throw runtime_error("Failed to open ./sim/actual.csv for writing");
    }
    fout << "S,I,R\n";
    fout.close();

    // read parameters
    ifstream fin("params.txt");
    if(!fin.is_open()){
        throw runtime_error("Failed to open params.txt");
    }
    fin >> N >> transmission_prob >> recovery_prob >> I >> m;
    fin.close();

    if(N <= 0) throw runtime_error("N must be positive");
    if(I < 0) throw runtime_error("Initial infected I must be non-negative");
    if(I > N) throw runtime_error("I should be less than or equal to N");
    if(transmission_prob < 0 || transmission_prob > 1) throw runtime_error("transmission_prob must be in [0,1]");
    if(recovery_prob < 0 || recovery_prob > 1) throw runtime_error("recovery_prob must be in [0,1]");

    S = static_cast<double>(N) - I;
    R = 0.0;
    write_log({S, I, R});
}

void simulate(int t){
    while(t--){
        // continuous SIR ODEs using Euler forward discrete step (dt = 1)
        delta_S = -transmission_prob * S * I / static_cast<double>(N);
        delta_R = recovery_prob * I;
        delta_I = -(delta_S + delta_R);

        S += delta_S;
        I += delta_I;
        R += delta_R;

        // numerical safeguard: keep values within [0, N]
        if(S < 0) S = 0;
        if(I < 0) I = 0;
        if(R < 0) R = 0;
        // minor floating rounding can push sum slightly off N; optional correction omitted

        write_log({S, I, R});
    }
}

void write_log(const vector<double>& vals){
    ofstream fout("./sim/actual.csv", ios::app);
    if(!fout.is_open()){
        cerr << "Warning: failed to append to ./sim/actual.csv\n";
        return;
    }
    // write with fixed precision to capture fractional values
    fout << fixed << setprecision(0) << vals[0] << "," << vals[1] << "," << vals[2] << "\n";
    fout.close();
}