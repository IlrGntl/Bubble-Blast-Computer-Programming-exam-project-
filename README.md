This repository contains the final project for my **Programming (C)** university examination. The objective was to recreate the mechanics of the popular game "Bubble Blast" while implementing a custom solver to find the optimal solution for any given board state.

## Project Overview
The core of the project is a solver designed to calculate the **minimum number of moves** required to clear the board. 

### Key Technical Features:
* **Recursive Game Engine:** Developed in C to manage bubble states, chain reactions, and board updates.
* **Optimal Solution Algorithm:** To find the shortest path to victory, I implemented a search process that explores all possible move combinations.
* **State Tree Exploration:** The solver generates a decision tree where each node represents a click. By traversing the tree, the algorithm identifies the branch with the **minimum depth**, ensuring the most efficient solution is found.

## How it Works
The programme silently simulates every possible interaction on the grid. By using an iterative/recursive approach (Brute-force/Backtracking), it evaluates the consequences of each click until the board is empty, then backtracks to compare move counts and returns the most efficient sequence.<br>

## Instructions
To run this project, you need a C compiler.<br> 

1. Save both files in the same directory. <br>

2. Run `gcc bubbleblast.c -o random_name.exe` to compile.<br> 

3. Run `random_name.exe; Read-Host "Press ENTER to exit..."`.<br>

Worst guide ever. You're welcome!
