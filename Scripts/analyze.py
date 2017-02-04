#!/usr/bin/python3

import matplotlib.pyplot as plt
import sys

r0x = list()
r0y = list()
w0x = list()
w0y = list()
r1x = list()
r1y = list()
w1x = list()
w1y = list()

test = False
if (len(sys.argv) > 2):
    test = True

f = open(sys.argv[1], 'r')
for line in f:
    vals = line.split(' ')
    x = float(vals[0])
    y = float(vals[1])
    right = True
    kind = int(vals[2])
    if (not test):
        right = int(vals[3]) == 1
    if kind == 0:
        if right:
            r0x.append(x)
            r0y.append(y)
        else:
            w0x.append(x)
            w0y.append(y)
    else:
        if right:
            r1x.append(x)
            r1y.append(y)
        else:
            w1x.append(x)
            w1y.append(y)

if len(w0x) + len(w0y) > len(r0x) + len(r0y) \
        and len(w1x) + len(w1y) > len(r1x) + len(r1y):
    (r0x, w0x) = (w0x, r0x)
    (r0y, w0y) = (w0y, r0y)
    (r1x, w1x) = (w1x, r1x)
    (r1y, w1y) = (w1y, r1y)

maxX = max(r0x + w0x + r1x + w1x)
maxY = max(r0y + w0y + r1y + w1x)

# if (sys.argc == 3):
#     f = open(sys.argv[1], 'r')
#     vals = line.split(' ')
#     ncoefs = len(vals)
    # for x in range()


plt.plot(r0x, r0y, 'bo', w0x, w0y, 'ro', r1x, r1y, 'gs', w1x, w1y, 'rs')
plt.show(block=True)

