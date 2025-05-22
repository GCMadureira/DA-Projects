# knapsack_solver.py


from pulp import LpProblem, LpMaximize, LpVariable, lpSum, PULP_CBC_CMD
import sys


# Read input from file 

with open(sys.argv[1], 'r') as f:
    lines = f.readlines()
    n = int(lines[0].strip())
    capacity = int(lines[1].strip())
    weights = list(map(int, lines[2].strip().split()))
    profits = list(map(int, lines[3].strip().split()))



# Setup LP problem
model = LpProblem("Knapsack", LpMaximize)
x = [LpVariable(f"x{i}", cat="Binary") for i in range(n)]

model += lpSum(x[i] * profits[i] for i in range(n))
model += lpSum(x[i] * weights[i] for i in range(n)) <= capacity

# Solve
solver = PULP_CBC_CMD(msg=False)
model.solve(solver)

selected = [str(i) for i in range(n) if x[i].varValue > 0.5]
total_profit = sum(profits[int(i)] for i in selected)
total_weight = sum(weights[int(i)] for i in selected)

# Write result to output file -- with a system call ---- open(sys.argv[2], 'r') as f 
with open(sys.argv[2], 'w') as f:
    f.write(f"{total_profit}\n")
    f.write(f"{total_weight}\n")
    f.write(" ".join(selected) + "\n")
