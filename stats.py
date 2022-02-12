#!/usr/bin/env python3

import sys
from matplotlib.pyplot import plot, show, scatter, subplot, xlabel, ylabel, xlim, ylim, savefig

machines = []
times = []
accelerations = []
efficacites = []

def transform_to_milliseconds(str):
    milliseconds = 0
    seconds = 0
    minutes = 0
    index_minutes = 0
    index_seconds = 0
    for i in range(len(str)):
        if str[i] == 'm':
            minutes = int(str[0:i])
            index_minutes = i
        if str[i] == '.':
            seconds = int(str[index_minutes+1:i])
            index_seconds = i
        if str[i] == 's':
            milliseconds = int(str[index_seconds+1:i])

    return minutes * 60 * 1000 + seconds * 1000 + milliseconds

def read_on_file():
    file = open("./distributed/resultfile", "r")
    #file = open("./resultfile", "r")
    lines = file.readlines()

    count = 2
    for line in lines:
        if "real" in line:
            machines.append(count)
            times.append(transform_to_milliseconds(line[5:14]))
            #print("{}:{}".format(count, transform_to_milliseconds(line[5:14])))
            count += 1

    for i in range(len(times)):
        #accelerations.append((times[0]/times[i]))
        accelerations.append(min(times)* (times[0]/times[i]))
        #efficacites.append(times[0]/(times[i] * (i+2)))
        efficacites.append(min(times) * times[0]/(times[i] * (i+2)))

read_on_file()
subplot()
plot(machines, times, color='orange')
plot(machines, accelerations, color='green')
plot(machines, efficacites, color='red')
xlabel('Nombre de threads')
ylabel("Temps d'exécution")
savefig("./distributed/images/image"+sys.argv[1])
#savefig(".//images/image"+sys.argv[1])
#show()