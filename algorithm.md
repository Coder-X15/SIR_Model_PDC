# The Algorithm in `sir_bam_serial.cpp`
### Part 0: Spawning Nodes and Taking Input
Nothing much in here. The input is read from `params.txt` in the following order:
1. Population size ($N$ or `N`)
2. Transmission probability ($\beta$ or `transmission_prob`)
3. Recovery probability ($\gamma$ or `recovery_prob`)
4. The number of initially infected people ($p$ or `initial_infected`)
5. The number of edges that a node adds upon being added to the Barabasi-Albert graph ($m$ or `m`)

### Part 1: Creating the Barabasi-Albert Network
1. Initially, make a complete network using $m_0$ out of the $N$ available nodes ( $\mathcal{O}(m_0^2)$ ). The decision regarding the value of $m_0$ is done within a constant time.
2. Take the connected $m_0$ nodes, and append as many copies of each vertex as the degree of the vertex into a node pool($m_0^2$ operations)
3. For each of the other $N - m_0$ vertices, choose $m$ vertices randomly (assuming that the choosing mechanism doesn't choose the same vertex as the one in the current loop iteration, we can say that the complexity comes to $m$ operations) and connect the vertex in the current loop iteration to each of them and then add the pair to the node pool($4m$ operations in total). Now, the total number of operations should be $(N - m_0)(m + 4m) = 5Nm - 5mm_0$
Total number of operations : $5(N-m_0)m + m_0^2 = 5Nm + m_0(m_0 - 5m)$. Now, if $m_0 >> 5m$, then we have that the number of operations approximate to $5Nm + m_0^2$, thus the time complexity of the algorithm is $\mathcal{O}(Nm + m_0^2)$.

### Part 2: The Monte-Carlo Simulation of Infection Spread
1. Increase the time step $t$ if $t < t_{\text{stop}}$
2. For each node in the list of nodes ($N + 1$ loop iterations since we depend on numerical indices), if the node is infected, infect all of its susceptible neighbours with probability $\beta$(over all iterations of the inner loop, we'll have at least $E$ operations done) and then try to heal itself with probability $\gamma$ ($1$ operation). We can observe that over all the iterations of the inner loop, we will have done at most $Nm$ operations (since the operation involved is just infecting a neighbour and then curing itself, once can see that the number of operations will be approximately equal to the number of edges, which, using the average degree of $m$ edges per node, we have that the time complexity is $\mathcal{O}(E) \leq \mathcal{O}(Nm)$ per loop).
Total time complexity: $\mathcal{O}(t_\text{stop}Nm) = \mathcal{O}(NmT)$ (taking $t_{\text{stop}}$ as $T$)
### Net Time Complexity:
$\mathcal{O}(NmT) + \mathcal{O}(Nm + m_0^2) \approx \mathcal{O}(NmT + m_0^2)$
## Opportunities for Parallelisation
The most easily parallelisable region in the entrie program is the Monte Carlo simulation. This is because there is a common data structure that each task spawned will access, and also there is no simultaneous read and write operations on the same data structure. Besides, each vertex in the inner loop that iterates over all vertices in the old list of flags and they in turn, will iterate over all the pssible edges that they have; in here, the read occurs from the old value of the vector `P` and the write ocurs on the new vector `new_P`. Making these write operations atomic will result in a really parallely algorithm with no write-write conflicts. Besides, only those vertices those that have been marked infected in `P` in the current iteration alone are taken for this branching, thus making this parallel algorithm the mosty easily achievable bit of parallelism in the program. The network formation part has non-rectangular loops and so the task of creating threads will bear an uncertain cost which will affect the program's performance.

The fork-join part of the parallelisable region is in that part where the infection is spread from the infected node to its neighbours. The serila implementation of this infection spread is as follows:
```cpp
void evolve(int time_steps){
    double start = clock();
    for(int i = 0; i < time_steps; i++){
        double S, I, R;
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
        write_log({S, I, R});
        P = new_P; // update the states
        print_states(P); // print the states after this time step
    }
    double end = clock();
    cout << "Simulation completed in " << (end - start) / CLOCKS_PER_SEC << " seconds.\n";
}
```

Upon parallelising it, we have:
```cpp
void evolve(int time_steps){
    double start = omp_get_wtime();
    for(int i = 0; i < time_steps; i++){
        double S, I, R;
        people new_P = P; // copy the current states
        # pragma omp parallel
        {
            # pragma omp for schedule(dynamic, 8)
            for(int node = 0; node < N; node++){
                if(P[node] == INFECTED){
                    // try to infect neighbors
                    for(int neighbor : G[node]){
                        if(P[neighbor] == SUSCEPTIBLE && random_decision(transmission_prob)){
                            # pragma omp atomic write
                            new_P[neighbor] = INFECTED;
                        }
                    }
                    // try to recover
                    if(random_decision(recovery_prob)){
                        # pragma omp atomic write
                        new_P[node] = RECOVERED;
                    }
                }
            }
        }
        S = count(new_P.begin(), new_P.end(), SUSCEPTIBLE);
        I = count(new_P.begin(), new_P.end(), INFECTED);
        R = count(new_P.begin(), new_P.end(), RECOVERED);
        write_log({S, I, R});
        P = new_P; // update the states
        print_states(P); // print the states after this time step
    }
    double end = omp_get_wtime();
    cout << "Simulation completed in " << (end - start) << " seconds.\n";
}
```

## Parallelised Time Complexity
Notice that according to the previous section, the region we have parallelised may create nearly $2E$ threads. Now, according to the Handshaking Theorem, the sum of all the degrees of an undirected graph will be greater than or equal to twice the number of edges; this gives us that $Nm \geq 2E$ threads will be created at most ansd so upon parallelising these using $p$ processors, the total work of $Nm$ operations is divided down to $\frac{Nm}{p}$ operations run concurrently, so the total time complexity per one outer loop iteration will be $\mathcal{O}(\frac{Nm}{p})$. This gives us the total parallelised time complexity of the `evolve` function as $\mathcal{O}(\frac{Nm}{p}T)$. So, on average, our parallelised time complexity will be $\mathcal{O}(\frac{Nm}{p}T + m_0^2)$

## Checking Amdahl's Law : TODO
