#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import sys

my_dpi = 96
plt.figure(figsize=(400/my_dpi, 400/my_dpi), dpi=my_dpi)

r0x = list()
r0y = list()
w0x = list()
w0y = list()
r1x = list()
r1y = list()
w1x = list()
w1y = list()

test = False
if (len(sys.argv) > 3):
    test = True

f = open(sys.argv[1], 'r')
firstline = f.readline().split(' ')

outFile = sys.argv[2]

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

# if len(w0x + w0y) > len(r0x + r0y) \
#         and len(w1x + w1y) > len(r1x + r1y):
#     (r0x, w0x) = (w0x, r0x)
#     (r0y, w0y) = (w0y, r0y)
#     (r1x, w1x) = (w1x, r1x)
#     (r1y, w1y) = (w1y, r1y)

outStatName = outFile.split('_')[0] + "_correctness.csv"
# clear out file if this is _0.png (first) file
firstFile = outFile.split('_')[1][1] == '0' and outFile.split('_')[1][0] == '0'
fileMode = 'w' if firstFile else 'a'
outStatFile = open(outStatName, fileMode)
if firstFile:
    outStatFile.write("rights;total;percent\n")

# number of right guesses, total number, percent
rights = len(r0x) + len(r1x) - 1
total = rights + len(w0x) + len(w1x)
percent = float(rights) * 100 / float(total)
outStatFile.write("{0};{1};{2}\n".format(rights, total, percent))

plt.plot(r0x, r0y, 'bo', w0x, w0y, 'ro', r1x, r1y, 'gs', w1x, w1y, 'rs')


def isIntersected(p0, r0, p1, r1):
    d = np.sqrt((p0[0] - p1[0]) ** 2 + (p0[1] - p1[1]) ** 2)
    return d < r0 + r1 and d > abs(r0 - r1)


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

    a = (r1*r1-r2*r2+d*d) / (2*d)
    h = np.sqrt(r1*r1-a*a)
    xm = x1 + a*dx/d
    ym = y1 + a*dy/d
    xs1 = xm + h*dy/d
    xs2 = xm - h*dy/d
    ys1 = ym - h*dx/d
    ys2 = ym + h*dx/d

    return (xs1, ys1), (xs2, ys2)


if firstline[0] == "NC":
    # Nearest centroid
    # First center
    plt.plot(float(firstline[1]), float(firstline[2]),
             '*', ms=15, mec='k', mew=1,c ="#2222ff")
    # Second center
    plt.plot(float(firstline[3]), float(firstline[4]),
             '*', ms=15, mec='k', mew=1, c="#22ff22")
elif firstline[0] == "LPF":
    head, *coefsRaw, tmp = firstline
    coefs = []
    powsX = []
    powsY = []
    for i in range(len(coefsRaw)):
        splitted = coefsRaw[i].split(',')
        curC = float(splitted[0])
        curX = int(splitted[1])
        curY = int(splitted[2])
        coefs.append(curC)
        powsX.append(curX)
        powsY.append(curY)

    def legendre(n, x):
        if n == 0:
            return 1.0
        elif n == 1:
            return float(x)
        else:
            n -= 1
            res1 = float(2 * n + 1) / (n + 1) * x * legendre(n, x)
            res2 = float(n) / (n + 1) * legendre(n - 1, x)
            return res1 - res2

    @np.vectorize
    def polyVal(xV, yV):
        newcoefs = []
        for i in range(len(coefs)):
            val = legendre(powsX[i], xV) * legendre(powsY[i], yV)
            newcoefs.append(val)
        return np.dot(coefs, newcoefs)

    minX = min(r0x + w0x + r1x + w1x)
    maxX = max(r0x + w0x + r1x + w1x)
    minY = min(r0y + w0y + r1y + w1y)
    maxY = max(r0y + w0y + r1y + w1y)
    xLst = np.linspace(minX, maxX, 100)
    yLst = np.linspace(minY, maxY, 100)
    X, Y = np.meshgrid(xLst, yLst)
    Z = polyVal(X, Y)
    plt.contour(X, Y, Z, [0])
elif firstline[0] == "KM":
    sys.exit(-1)
elif firstline[0] == "TB":
    sys.exit(-1)

if test:
    plt.show(block=True)
else:
    plt.savefig(outFile, dpi=my_dpi)
