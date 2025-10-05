## SIR Model Simulation Using Monte Carlo and Barabási-Albert Graphs
> **Authored by**: Sam Ruben Abraham and Anushka Kishor Nikam

This repository contains C++ and Python code for simulating the spread of epidemics using the SIR (Susceptible-Infected-Recovered) model on a Barabási-Albert (BA) network. The project also includes tools for visualizing the simulation results.

## Directory and File Overview

- `sir_bam_serial.cpp`  
    Serial C++ implementation of the SIR model on a Barabási-Albert graph.

- `params.txt`  
    Input parameters for the simulation (e.g., population size, infection rate, recovery rate, initial infected count).

- `make_gif.py`  
    Python script to generate a GIF animation of the epidemic spread using PNG frames in the `sim/` directory.

- `sim/`  
    Output directory containing:
    - `epidemic.gif`: Animation of the epidemic spread
    - `frame_*.png`: Individual frames of the simulation
    - `states.csv`: State evolution of each node (0 = Susceptible, 1 = Infected, 2 = Recovered)

## How to Run the Simulations

1. **Compile the C++ simulation code**
     - For serial code:
         ```shell
         g++ sir_bam_serial.cpp -o sir_bam_serial.exe
         ```
     - (If you have parallel code, use `-fopenmp` as needed.)

2. **Prepare the input**
     - Edit `params.txt` to set simulation parameters as required by the C++ code.

3. **Run the simulation**
     - Execute the compiled program:
         ```shell
         .\sir_bam_serial.exe
         ```
     - Output files will be generated in the `sim/` directory.

4. **Visualize the results**
     - To create a GIF animation from the simulation frames:
         ```shell
         python make_gif.py
         ```
     - The resulting `epidemic.gif` will appear in the `sim/` folder.

## Output Details

- **PNG frames**: Each `frame_*.png` shows the state of the network at a simulation step.
- **epidemic.gif**: Animation of the epidemic progression.
- **states.csv**: Each row shows the state (0, 1, 2) of every node at each time step.

---
**Note:**
- PDF files and executables are not described here.
- For additional simulation methods, add or modify C++ source files as needed.

## Simulation Results (Eg.)
![](https://github.com/Coder-X15/SIR_Model_PDC/blob/44e716071028594d32dec545ab13f350f33df203/sim/epidemic.gif)
