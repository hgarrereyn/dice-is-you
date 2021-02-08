
# Reference solution script for "Dice is you"
# DiceCTF 2021 - hgarrereyn

from z3 import *

# Puzzle size
W = 5

s = Solver()

# List of possible grid values.
gvals = [1, 5, 18, 25, 48, 49, 55, 61, 96, 119, 120, 135, 138, 148, 150, 160, 163, 171, 179, 183, 189, 192, 194, 212, 247]

grid = [BitVec('g%d' % i, 8) for i in range(W*W)]

# Code generation function.
def do(a,b,c,d,e):
    return ((a * 42) + (b * 1337) + c + (c ^ d) + (e*2))

for i in range(W*W):
    for j in range(i+1,W*W):
        # Uniqueness constraint.
        s.add(grid[i] != grid[j])
        
    opt = []
    for j in range(W*W):
        # Must be an element of "gvals"
        opt.append(grid[i] == gvals[j])
    s.add(Or(opt))

# Constrain 5 known grid values
s.add(grid[0] == 212)
s.add(grid[1] == 194)
s.add(grid[2] == 189)
s.add(grid[5] == 48)
s.add(grid[10] == 192)

f = []    

for r in range(W):
    f.append(do(*[grid[(r*W)+x] for x in range(W)]))

for c in range(W):
    f.append(do(*[grid[(y*W)+c] for y in range(W)]))
    
for i in range(len(f)):
    s.add(f[i] == 0)

print(s.check())
m = s.model()

out = [m[grid[i]].as_long() for i in range(W*W)]

for r in range(5):
    print(out[r*5:(r+1)*5])

# Ensure there are no other solutions.
c = []
for i in range(W*W):
    c.append(grid[i] == out[i])
s.add(Not(And(c)))

assert(s.check() == unsat)


# y x target plus pi
# e z j L K
# tri square halfsq T false
# G splitcirc D filltri I
# N F R / circdot
