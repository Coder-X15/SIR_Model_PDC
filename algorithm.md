# The Algorithm in `sir_bam_serial.cpp`
## Part 0: Spawning Nodes and Taking Input
Nothing much in here. The input is read from `params.txt` in the following order:
1. Population size ($N$ or `N`)
2. Transmission probability ($\beta$ or `transmission_prob`)
3. Recovery probability ($\gamma$ or `recovery_prob`)
4. The number of initially infected people ($p$ or `initial_infected`)
5. The number of edges that a node adds upon being added to the Barabasi-Albert graph ($m$ or `m`)

## Part 1: Creating the Barabasi-Albert Network
1. Initially, make a complete network using $m_0$ out of the $N$ available nodes ($\mathcal{O}(m_0^2)$). The decision regarding the value of $m_0$ is done within a constant time.
2. Take the connected $m_0$ nodes, and append as many copies of each vertex as the degree of the vertex into a node pool($m_0^2$ operations)
3. For each of the other $N - m_0$ vertices, choose $m$ vertices randomly (assuming that the choosing mechanism doesn't choose the same vertex as the one in the current loop iteration, we can say that the complexity comes to $m$ operations) and connect the vertex in the current loop iteration to each of them and then add the pair to the node pool($4m$ operations in total). Now, the total number of operations should be $(N - m_0)(m + 4m) = 5Nm - 5mm_0$
Total number of operations : $5(N-m_0)m + m_0^2 = 5Nm + m_0(m_0 - 5m)$. Now, if $m_0 >> 5m$, then we have that the number of operations approximate to $5Nm + m_0^2$, thus the time complexity of the algorithm is $\mathcal{O}(Nm + m_0^2)$.

## Part 2: The Monte-Carlo Simulation of Infection Spread
1. Increase the time step $t$ if $t < t_{\text{stop}}$
2. For each node in the list of nodes ($N + 1$ loop iterations since we depend on numerical indices), if the node is infected, infect all of its susceptible neighbours with probability $\beta$(over all iterations of the inner loop, we'll have at least $E$ operations done) and then try to heal itself with probability $\gamma$ ($1$ operation). We can observe that over all the iterations of the inner loop, we will have done at most $Nm$ operations (since the operation involved is just infecting a neighbour and then curing itself, once can see that the number of operations will be approximately equal to the number of edges, which, using the average count of $m$ edges per node, we have that the time complexity is $\mathcal{O}(E) \leq \mathcal{O}(Nm)$ per loop).
Total time complexity: $\mathcal{O}(t_\text{stop}Nm)$

## Opportunities for Parallelisation : TODO
## Parallelised Time Complexity :TODO
## Checking Amdahl's Law : TODO