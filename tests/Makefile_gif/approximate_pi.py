#!/usr/bin/env python3
from simulator import simulator
from math import sqrt
from random import uniform
from point import Point
from subprocess import call
from PIL import Image, ImageFont, ImageDraw
import sys


"""def tracer_ligne_horizontal(x, y, longueur, liste):
    for i in range(6):
        for j in range(longueur):
            liste[x+i][y+j] = 2
    #return liste


def tracer_ligne_vertical(x, y, longueur, liste):
    for i in range(6):
        for j in range(longueur):
            liste[x+j][y+i] = 2
    #return liste

def tracer_texte(c, x, y, liste):
    if c == '0':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y, 100, liste)
        #tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x, y+30-6, 100, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '1':
        tracer_ligne_vertical(x, y+30-6, 100, liste)
    if c == '2':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y+30-6, 50, liste)
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x+50-6, y, 50, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '3':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y+30, 100, liste)
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '4':
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x, y, 50, liste)
        tracer_ligne_vertical(x, y+30-6, 100, liste)
    if c == '5':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y, 50, liste)
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x+50-6, y+30-6, 50, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '6':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y, 100, liste)
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x+50-6, y+30-6, 50, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '7':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y+30-6, 100, liste)
    if c == '8':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y, 100, liste)
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x, y+30-6, 100, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '9':
        tracer_ligne_horizontal(x, y, 30, liste)
        tracer_ligne_vertical(x, y, 50, liste)
        tracer_ligne_horizontal(x+50-6, y, 30, liste)
        tracer_ligne_vertical(x, y+30-6, 100, liste)
        tracer_ligne_horizontal(x+100-6, y, 30, liste)
    if c == '.':
        tracer_ligne_horizontal(x+100-6, y+30-6, 6, liste)
    #return liste"""

def fill_matrice(n, dim):
    out = [[-1 for _ in range(dim)] for _ in range(dim)]
    for i in range(1, n+1):
        x = uniform(-dim/2,dim/2)
        y = uniform(-dim/2,dim/2)
        point = Point(x, y)
        if(point.distance(Point(0,0)) <= dim/2):
            out[int((dim/2)+y)][int((dim/2)+x)] = 1
        else:
            out[int((dim/2)+y)][int((dim/2)+x)] = 0
    return out

def generate_ppm_file(taille, n, apres_virgule, numero):
    dim = sqrt(taille)
    pi = simulator(n)
    new_pi = str(round(pi, apres_virgule))
    out = fill_matrice(n, int(dim))
    """for i in range(len(new_pi)):
        tracer_texte(new_pi[i], int(dim/2)-50, int(dim/2)-100+i*40, out)"""
    name = new_pi[0] + "-"
    for i in range(2,len(new_pi)):
        name += new_pi[i]
    name_file = "img" + str(numero) + "_" + name + ".ppm"
    my_file = open(name_file, "w")
    my_file.write("P3 \n")
    my_file.write("{}, {}\n".format(int(dim), int(dim)))
    my_file.write(str(255)+"\n")
    for i in range(len(out)):
        for j in range(len(out)):
            if out[i][j] == 1:
                my_file.write("255   0   0\n")
            elif out[i][j] == 0:
                my_file.write("0   0   255\n")
            elif out[i][j] == 2:
                my_file.write("0   0   0\n")
            else:
                my_file.write("255   255   255\n")
    my_file.close()
    return name_file, pi


def image_simulation(taille, n, apres_virgule, numero):
    dim = sqrt(taille)
    names_file = []
    gif_name = 'simu' + str(numero) + '.gif'
    for i in range(10):
        name_file, pi = generate_ppm_file(taille, int((i+1)*n/10), apres_virgule, i)
        names_file.append(name_file)
    call(['convert', '-delay', '50']+ names_file +[gif_name])


if __name__ == "__main__":
    args = sys.argv[1:]
    taille = int(args[0])
    n = int(args[1])
    apres_virgule = int(args[2])
    numero = int(args[3])
    image_simulation(taille, n, apres_virgule, numero)
