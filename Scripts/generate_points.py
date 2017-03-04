#!/usr/bin/python3
import numpy as np
import sys
import random


def showHelp():
    helpMsg = '''
    Usage: ./generate_points.py type npoints noise output
    ---
    Available types:
    * simple -> generates two circular clusters
    * brackets -> generates two 'brackets'
    * circle -> generates 2 rings of points, one inside of another
    ---
    npoints: number of points in each cluster
    ---
    noise: number from 0 to 1, percent of fake points
    ---
    output: name of output file
    '''
    print(helpMsg)


def simple(npoints, noise, output):
    # print("Enter kernel1:")
    params = open("params.txt")
    l = params.readline().split(' ')
    # print("Enter kernel1:")
    k1 = (float(l[0]), float(l[1]))
    # print("Enter rad1_1 and rad2_2:")
    l = params.readline().split(' ')
    r1_1, r1_2 = (float(l[0]), float(l[1]))
    # print("Enter kernel2:")
    l = params.readline().split(' ')
    k2 = (float(l[0]), float(l[1]))
    # print("Enter rad2_1 and rad2_2:")
    l = params.readline().split(' ')
    r2_1, r2_2 = (float(l[0]), float(l[1]))
    # maxX = max(k1[0] + r1, k2[0] + r2)
    # minX = min(k1[0] - r1, k2[0] - r2)
    # maxY = max(k1[1] + r1, k2[1] + r2)
    # minY = min(k1[1] - r1, k2[1] - r2)
    # print("Range: {0}, {1} - {2}, {3}".format(minX, minY, maxX, maxY))
    points1 = list()
    points2 = list()
    for i in range(npoints):
        pr1_1 = random.random() * r1_1  # number from 0 to radius1
        pr1_2 = random.random() * r1_2  # number from 0 to radius1
        ang1 = random.random() * 2 * np.pi  # angle
        # Generate new point
        point1 = (k1[0] + np.cos(ang1) * pr1_1, k1[1] + np.sin(ang1) * pr1_2)
        if random.random() > noise:  # Check if it's noise
            points1.append(point1)
        else:
            points2.append(point1)

        pr2_1 = random.random() * r2_1  # number from 0 to radius1
        pr2_2 = random.random() * r2_2  # number from 0 to radius1
        ang2 = random.random() * 2 * np.pi  # angle
        # Generate new point
        point2 = (k2[0] + np.cos(ang2) * pr2_1, k2[1] + np.sin(ang2) * pr2_2)
        if random.random() > noise:  # Check if it's noise
            points2.append(point2)
        else:
            points1.append(point2)

    f = open(output, 'w')
    for point in points1:
        f.write("{0} {1} 0\n".format(point[0], point[1]))
    for point in points2:
        f.write("{0} {1} 1\n".format(point[0], point[1]))
    return


def brackets(npoints, noise, output):
    points1 = list()
    points2 = list()
    '''
    Generate smth like this:
    0111
    0101
    0101
    0001
    Top-left: 0.0 10.0
    Bottom-left: 0.0 0.0
    Top-right: 10.0 10.0
    Bottom-right: 10.0 0.0

    Summary: 16 blocks 2.5 * 2.5
    '''
    BLOCK_SIZE = 2.5
    perBlock = int(npoints * 2 / 16)

    def processBlock(x, y, kind):
        for i in range(perBlock):
            x1 = x + random.random() * BLOCK_SIZE
            y1 = y + random.random() * BLOCK_SIZE

            if random.random() < noise:  # Check if it's noise
                kind = int(abs(1 - kind))  # Make 1 if 0 or 0 if 1

            if kind == 0:
                points1.append((x1, y1))
            else:
                points2.append((x1, y1))

    def getType(i, j):
        if (i == 0) or (j == 3 and i != 3) or (i == 2 and j != 0):
            return 0
        else:
            return 1

    for i in range(4):
        for j in range(4):
            processBlock(2.5 * j, 2.5 * i, getType(i, j))

    if perBlock == 0:
        points1.append((0.1, 0.1))
        points2.append((9.9, 9.9))

    f = open(output, 'w')
    for point in points1:
        f.write("{0} {1} 0\n".format(point[0], point[1]))
    for point in points2:
        f.write("{0} {1} 1\n".format(point[0], point[1]))
    return


def spawn(noise, output):
    params = open("spawn.txt")
    nclusters = int(params.readline())
    clusters = []
    noisePool = []
    #outStr = "KM " + str(nclusters) + " "
    for _ in range(nclusters):
        # load data from noise pool
        points = []
        for p in noisePool:
            points.append(p)
        noisePool = []

        clusterParams = list(map(float, params.readline().split(' ')))
        npoints = int(clusterParams[0])
        kx, ky = clusterParams[1], clusterParams[2]
        r0, r1 = clusterParams[3], clusterParams[4]
        #outStr += "{0} {1} {2} ".format(kx, ky, max(r0, r1))
        for _ in range(npoints):
            pr1 = random.random() * r0  # number from 0 to radius1
            pr2 = random.random() * r1  # number from 0 to radius1
            ang1 = random.random() * 2 * np.pi  # angle
            # Generate new point
            point = (kx + np.cos(ang1) * pr1, ky + np.sin(ang1) * pr2)
            if random.random() > noise:  # Check if it's noise
                points.append(point)
            else:
                noisePool.append(point)

        clusters.append(points)

    # data from last noise pool goes to the first cluster
    for p in noisePool:
        clusters[0].append(p)

    f = open(output, 'w')
    #outStr += "\n"
    #f.write(outStr)
    for i in range(nclusters):
        for p in clusters[i]:
            f.write("{0} {1} {2}\n".format(p[0], p[1], i))

    return


if len(sys.argv) < 5:
    showHelp()
    sys.exit()

ptype = sys.argv[1].lower()
npoints = int(sys.argv[2])
noise = float(sys.argv[3])
random.seed()

for i in range(4, len(sys.argv)):
    output = sys.argv[i]
    if ptype == 'simple':
        simple(npoints, noise, output)
    elif ptype == 'brackets':
        brackets(npoints, noise, output)
    elif ptype == 'spawn':
        spawn(noise, output)
    else:
        showHelp()
        sys.exit()
