import pandas as pd
import matplotlib.pyplot as plt

actuals = pd.read_csv("./sim/actual.csv")
simulated = pd.read_csv("./sim/simulated.csv")

# Create a figure and three subplots (2x2 grid, use 3 cells)
plt.figure(figsize=(10, 7))

# plot 1: Simulated S vs Actual S values
ax1 = plt.subplot(2, 2, 1)
ax1.plot(actuals['S'], label='actual', color='tab:blue', marker='o', linestyle='-')
ax1.plot(simulated['S'], label='simulated', color='tab:orange', marker='x', linestyle='--')
ax1.set_title('Susceptible count (S)')
ax1.set_xlabel('Time step')
ax1.set_ylabel('Count')
ax1.legend()
ax1.grid(True)

# plot 2: Simulated I vs Actual I values
ax2 = plt.subplot(2, 2, 2)
ax2.plot(actuals['I'], label='actual', color='tab:blue', marker='o', linestyle='-')
ax2.plot(simulated['I'], label='simulated', color='tab:orange', marker='x', linestyle='--')
ax2.set_title('Infected count (I)')
ax2.set_xlabel('Time step')
ax2.set_ylabel('Count')
ax2.legend()
ax2.grid(True)

# plot 3: Simulated R vs Actual R values
ax3 = plt.subplot(2, 2, 3)
ax3.plot(actuals['R'], label='actual', color='tab:blue', marker='o', linestyle='-')
ax3.plot(simulated['R'], label='simulated', color='tab:orange', marker='x', linestyle='--')
ax3.set_title('Recovered count (R)')
ax3.set_xlabel('Time step')
ax3.set_ylabel('Count')
ax3.legend()
ax3.grid(True)

# plot 4: Error plots
ax4 = plt.subplot(2, 2, 4)
ax4.plot(actuals['S'] - simulated['S'], label='Actual S - Simulated S', color='tab:blue')
ax4.plot(actuals['I'] - simulated['I'], label='Actual I - Simulated I', color='tab:orange')
ax4.plot(actuals['R'] - simulated['R'], label='Actual R - Simulated R', color='tab:green')
ax4.set_title('Errors')
ax4.set_xlabel('Time step')
ax4.set_ylabel('Count')
ax4.legend()
ax4.grid(True)

plt.tight_layout()
import os

# ensure output directory exists
os.makedirs('./sim', exist_ok=True)

output_path = './sim/comparison.png'
plt.savefig(output_path, dpi=300, bbox_inches='tight')
print(f"Saved comparison image to {output_path}")
plt.show()
