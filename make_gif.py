import networkx as nx
import matplotlib.pyplot as plt
import imageio
import csv
import os

def cleanup():
    # cleaning up output directory before every simulation
    for file in os.listdir("./sim"):
        if file.endswith(".png") or file.endswith(".gif"):
            os.remove("./sim/" + file)

def read_graph_from_file(filename):
    """Reads edges from file and creates a graph."""
    G = nx.Graph()
    with open(filename, 'r') as f:
        lines = f.readlines()
        N = int(lines[0].strip())  # first line is number of nodes
        for line in lines[1:]:
            u, v = map(int, line.strip(',').split())
            if not G.has_edge(u, v):
                G.add_edge(u, v)
        for i in range(N):
            if i not in G:
                G.add_node(i)
    return G

def read_states_from_csv(filename):
    """Reads epidemic states from CSV (no header)."""
    states = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            states.append([int(x) for x in row if x != ''])
    return states

def draw_and_save_frames(G, states, pos, output_gif):
    """Draws network frames and saves them into a GIF."""
    images = []
    color_map = {0: 'blue', 1: 'orange', 2: 'green'}  # S, I, R

    for frame, state in enumerate(states):
        node_colors = [color_map[s] for s in state]

        plt.figure(figsize=(6,6))
        nx.draw(G, pos, with_labels=True, node_color=node_colors, node_size=500, font_size=10)
        plt.title(f"Frame {frame}")
        plt.tight_layout()

        # Save temporary image
        temp_file = f"./sim/frame_{frame}.png"
        plt.savefig(temp_file)
        plt.close()

        images.append(imageio.imread(temp_file))

    # Save as GIF (loop=0 -> infinite loop)
    imageio.mimsave(output_gif, images, duration=0.5, loop=0)  # 0.5 sec per frame

def main():
    edges_file = 'edges.txt'
    states_file = './sim/states.csv'  # your CSV file with rows = frames
    output_gif = './sim/epidemic.gif'

    # Step 0: Clean up
    cleanup()

    # Step 1: Read graph
    G = read_graph_from_file(edges_file)

    # Step 2: Get positions for consistent layout
    pos = nx.spring_layout(G, seed=42)

    # Step 3: Read states
    states = read_states_from_csv(states_file)

    # Step 4: Draw frames + save GIF
    draw_and_save_frames(G, states, pos, output_gif)

    print(f"GIF saved as {output_gif}")

if __name__ == "__main__":
    main()