#!/usr/bin/env python3
from os import system
import sys

def add_machine(working_machine_name):
    """ Add a machine to the list of working machines
    :param working_machine_name: The name of working machine
    :return: 0 if machine already exists in the file and 1 otherwise
    """
    tmp = working_machine_name.split(".")
    #assert(len(tmp) == 4)
    print(len(tmp))
    if len(tmp) == 3:
        tmp[2] = "grid5000"
        tmp.append("fr")
    if len(tmp) == 4:
        tmp[3] = "fr"
    #tmp[3] = "fr"
    working_machine_name = ".".join(tmp)
    file = open("./distributed/machine_working", "r")
    lignes = file.readlines()
    for ligne in lignes:
        if (ligne in working_machine_name) or (working_machine_name in ligne):
            print("here")
            return 0
    command = "echo " + working_machine_name + " >> " + "./distributed/machine_working"
    system(command)
    file.close()
    return 1

def get_processes():
    """ Get list of all processes available
    :return: list of all processes
    """
    processes = []
    file = open("./distributed/machinefile", "r")
    lignes = file.readlines()
    for ligne in lignes:
        words = ligne.split()
        processes.append(words[0])
    return processes

def get_working_machines():
    """
    :return:
    """
    file = open("./distributed/machine_working", "r")
    lignes = file.readlines()
    return [machine for machine in lignes]


def send_working_machine_file(current_machine):
    """ Send the file to all machines by SCP
    :param current_machine:
    """
    processes = get_processes()
    command_initial = "sshpass -p \"admin\" scp -o StrictHostKeyChecking=no ./distributed/machine_working root@"
    command_final = ":distributed"
    for i in range(len(processes)):
        if not((current_machine in processes[i]) or (processes[i] in current_machine)):
            command = ""
            tmp = processes[i].split(".")
            assert(len(tmp) == 4)
            tmp[3] = "fr"
            name = ".".join(tmp)
            command += command_initial
            command += name
            command += command_final
            #print(command)
            system(command)

def send_output_files_to_machines(current_machine, path):
    """
    :param current_machine:
    :param path:
    :return:
    """
    machines = get_working_machines()
    command_initial = "sshpass -p \"admin\" scp -o StrictHostKeyChecking=no "
    command_inter = "* root@"
    command_final = ":"
    for i in range(len(machines)):
        if not((current_machine in machines[i]) or (machines[i] in current_machine)):
            command = ""
            tmp = machines[i].split(".")
            assert(len(tmp) == 4)
            tmp[3] = "fr"
            name = ".".join(tmp)
            command += command_initial
            command += path
            command += command_inter
            command += name
            command += command_final
            command += path
            #print(command)
            system(command)

if sys.argv[1] == "0":
    add_machine(sys.argv[2])
elif sys.argv[1] == "1":
    get_processes()
elif sys.argv[1] == "2":
    get_working_machines()
elif sys.argv[1] == "3":
    send_working_machine_file(sys.argv[2])
elif sys.argv[1] == "4":
    send_output_files_to_machines(sys.argv[2], sys.argv[3])
else:
    print("Code de fonction invalide")


