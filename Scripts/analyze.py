#!/usr/bin/python3

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

if len(w0x) + len(w0y) > len(r0x) + len(r0y) \
        and len(w1x) + len(w1y) > len(r1x) + len(r1y):
    (r0x, w0x) = (w0x, r0x)
    (r0y, w0y) = (w0y, r0y)
    (r1x, w1x) = (w1x, r1x)
    (r1y, w1y) = (w1y, r1y)

maxX = max(r0x + w0x + r1x + w1x)
maxY = max(r0y + w0y + r1y + w1x)

outStatName = outFile.split('_')[0] + "_correctness.csv"
# clear out file if this is _0.png (first) file
fileMode = 'a' if outFile.split('_')[1][0] != '0' else 'w'
outStatFile = open(outStatName, fileMode)
# number of right guesses, total number, percent
rights = len(r0x) + len(r1x) - 1
total = rights + len(w0x) + len(w1x)
percent = float(rights) * 100 / float(total)
outStatFile.write("{0};{1};{2}\n".format(rights, total, percent))

plt.plot(r0x, r0y, 'bo', w0x, w0y, 'ro', r1x, r1y, 'gs', w1x, w1y, 'rs')

if firstline[0] == "NC":
    # Nearest centroid
    # First center
    plt.plot(float(firstline[1]), float(firstline[2]),
             '*', ms=15, mec='k', mew=1, c="#2222ff")
    # Second center
    plt.plot(float(firstline[3]), float(firstline[4]),
             '*', ms=15, mec='k', mew=1, c="#22ff22")

if test:
    plt.show(block=True)
else:
    plt.savefig(outFile, dpi=my_dpi)
