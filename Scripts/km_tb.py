#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import sys

my_dpi = 96
#plt.figure(figsize=(400/my_dpi, 200/my_dpi), dpi=my_dpi)

test = False
if (len(sys.argv) > 3):
    test = True

f = open(sys.argv[1], 'r')
firstline = f.readline().split(' ')

nclasses = int(firstline[1])
#nsteps = int(firstline[2])
#steps_file = open("")

outFile = sys.argv[2]

rx = {}
ry = {}
wx = {}
wy = {}

for i in range(nclasses):
    rx[i] = []
    ry[i] = []
    wx[i] = []
    wy[i] = []

for line in f:
    vals = line.split(' ')
    x = float(vals[0])
    y = float(vals[1])
    right = True
    kind = int(vals[2])
    if (not test):
        right = bool(vals[3])
    if right:
        rx[kind].append(x)
        ry[kind].append(y)
    else:
        wx[kind].append(x)
        wy[kind].append(y)


# outStatName = outFile.split('_')[0] + "_correctness.csv"
# # clear out file if this is _0.png (first) file
# firstFile = outFile.split('_')[1][1] == '0' and outFile.split('_')[1][0] == '0'
# fileMode = 'w' if firstFile else 'a'
# outStatFile = open(outStatName, fileMode)
# if firstFile:
#     outStatFile.write("rights;total;percent\n")

# number of right guesses, total number, percent
# rights = len(r0x) + len(r1x) - 1
# total = rights + len(w0x) + len(w1x)
# percent = float(rights) * 100 / float(total)
# outStatFile.write("{0};{1};{2}\n".format(rights, total, percent))

# plt.plot(r0x, r0y, 'bo', w0x, w0y, 'ro', r1x, r1y, 'gs', w1x, w1y, 'rs')

markers = ['^', 's', 'o', 'p', 'h', 'p', 'h', 'v']
rightCols = ['b', 'g', 'c', 'm', 'y', 'k', 'w', 'r']
etalonCols = ['#3333ff', '#33ff33', '#33ffff', '#ff33ff', '#ffff33', 'k', 'w', '#ff3333']
wrongCols = ['r'] * len(rightCols)
etalons = ['*'] * len(rightCols)
add = lambda x, y: y + x

rightMarkers = list(map(add, rightCols, markers))
wrongMarkers = list(map(add, wrongCols, markers))
etalonMarkers = list(map(add, etalonCols, etalons))

fig = plt.figure()
ax = fig.add_subplot(111, aspect='equal')

for i in range(nclasses):
    ax.plot(rx[i], ry[i], rightMarkers[i])
    ax.plot(wx[i], wy[i], wrongMarkers[i])


def getIntersectionPoints(x1, y1, r1, x2, y2, r2):
    dx, dy = x2 - x1, y2 - y1
    d = np.sqrt(dx*dx + dy*dy)
    if d > r1+r2:
        # no solutions, the circles are separate
        return None
    if d < abs(r1-r2):
        # no solutions because one circle is contained within the other
        return None
    if d == 0 and r1 == r2:
        # circles are coincident and there are an infinite number of solutions
        return None

    a = (r1 * r1 - r2 * r2 + d * d) / (2 * d)
    h = np.sqrt(r1 * r1 - a * a)
    xm = x1 + a * dx / d
    ym = y1 + a * dy / d
    xs1 = xm + h * dy / d
    xs2 = xm - h * dy / d
    ys1 = ym - h * dx / d
    ys2 = ym + h * dx / d

    return (xs1, ys1), (xs2, ys2)

pxs = []
pys = []
ds = []

_, _, _, *rawPoints = firstline


for i in range(nclasses):
    pxs.append(float(rawPoints[i * 3]))
    pys.append(float(rawPoints[i * 3 + 1]))
    ds.append(float(rawPoints[i * 3 + 2]))
    ax.plot(pxs[i], pys[i], etalons[i], color=etalonCols[i],
            ms=20, mec='k', mew=1)
    if firstline[0] == "TBR":
        circle = patches.Circle(
            (pxs[i], pys[i]),
            ds[i],
            facecolor=rightCols[i],
            alpha=0.1
        )
        ax.add_artist(circle)

if firstline[0] == "TBR":
    for i in range(nclasses):
        for j in range(nclasses):
            if i == j:
                continue
            intersect = getIntersectionPoints(pxs[i], pys[i], ds[i],
                                              pxs[j], pys[j], ds[j])
            if intersect is None:
                continue
            ax.plot([intersect[0][0], intersect[1][0]],
                    [intersect[0][1], intersect[1][1]])

test = False
if test:
    fig.show()
    input()
else:
    fig.savefig(outFile, dpi=my_dpi)
