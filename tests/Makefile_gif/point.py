#!/usr/bin/env python3
from math import sqrt


class Point():

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance(self, other):
        abscisse = self.x - other.x
        ordonnee = self.y - other.y
        return sqrt((abscisse**2)+(ordonnee**2))

if __name__ == "__main__":
    print("2")
