#!/usr/bin/env python3
from point import Point
from random import uniform
import sys

def simulator(n):
    count = 0
    for i in range(1, n+1):
        x = uniform(-1, 1)
        y = uniform(-1, 1)
        point = Point(x, y)
        if(point.distance(Point(0,0)) <= 1):
            count += 1
    return 4*(count / n)

if __name__ == "__main__":
    n = int(sys.argv[1])
    print(simulator(n))
