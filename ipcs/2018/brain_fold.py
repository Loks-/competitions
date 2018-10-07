import pprint
from collections import Counter

def build():
    return [
        [Counter(c=1), Counter(s=1), Counter(c=1)],
        [Counter(s=1), Counter(i=1), Counter(s=1)],
        [Counter(c=1), Counter(s=1), Counter(c=1)],
    ]

def flip_v(x):
    return list(reversed(x))

def flip_h(x):
    return [list(reversed(y)) for y in x]

def transpose(x):
    return [[x[z][y] for z in range(3)] for y in range(3)]

def rotate_cw(x):
    return transpose(flip_v(x))

def rotate_ccw(x):
    return transpose(flip_h(x))

########################################
def B(x):
    return [
        [x[0][0] + x[2][0], x[0][1] + x[2][1], x[0][2] + x[2][2]],
        [x[1][0] + x[1][0], x[1][1] + x[1][1], x[1][2] + x[1][2]],
        [x[1][0], x[1][1], x[1][2]],
    ]

def T(x):
    return flip_v(B(flip_v(x)))

def L(x):
    return rotate_cw(B(rotate_ccw(x)))

def R(x):
    return rotate_ccw(B(rotate_cw(x)))

########################################

def solve(line):
    x = build()
    for c in line:
        x = globals()[c](x)
    return x

with open('b2.in') as f:
    lines = f.read().splitlines()
    cases, lines = lines[0], lines[3:]
    for i in range(int(cases)):
        s = solve(lines[0])
        cut = sorted(lines[1])
        if cut == sorted('BR'):
            s = rotate_cw(rotate_cw(s))
        if cut == sorted('LB') or cut == sorted('TB'):
            s = rotate_cw(s)
        if cut == sorted('TR'):
            s = rotate_ccw(s)
        if cut == sorted('LR') or cut == sorted('TB'):
            res = (s[1][0]['s'] + s[1][2]['s']) // 2 + 1
        else:
            res = sum(s[0][0].values()) + 1
        print res % 1000000007
        lines = lines[4:]
