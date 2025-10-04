# SIR Model Simulation Using Both Monte Carlo Simulation and Barabasi-Albert Graphs
The SIR Model is a prominent model, albeit basic, in simulating the spread of epidemics in a population. This repository contains the work of Sam Ruben Abraham and Anushka Kishor Nikam in implementing these approaches according to their understanding and also attempts at parallelizing the smae using OpenMP.

##  How to run it
The authors of this repository have not worked with Makefiles before and hence have not included them in the project. Hence, to run the programs properly:
1. **Choose the file you would like to execute**. The file names of the C++ source programs imply what type of simulation they are going to work on. For the serial programs, do:
    ```shell
    $ g++ <filename>.cpp -o <executable_name>
    ```
and for the parallel ones:
    ```shell
    $ g++ -fopenmp <filename>.cpp -o <executable_name>
    ```
    
2. Run the executable in the shell:
    ```shell
    $ ./<executable_name>
    ```

3. To view a GIF animation fo the same, run:
    ```shell
    $ python3 make_gif.py 
    ```

4. To check for the output, look in the `sim` folder. It contains many PNG files, indicating the individual frames in the epidemic simulation, an `epidemic.gif` showing a GIF animation of the same and a `states.csv`, which shows the evolution of the $i$-th node in the graph from states `SUSCEPTIBLE` (= 0), `INFECTED` (= 1) and `RECOVERED` (=2), row-wise.