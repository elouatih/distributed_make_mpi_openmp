#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include <mpi.h>
#include <omp.h>


char **file_words = NULL;
node **file_nodes = NULL;
node **feuilles = NULL;
/*
 * Compare each word of haystack (split using space) with needle.
 * Return 1 if the word needle is found in haystack, 0 otherwise.
 */
int word_in_sentence(char *sentence, char *word) {
    // Duplicate sentence, otherwise it would be modified
    char *sentence_dup = malloc(sizeof(char) * strlen(sentence));
    strcpy(sentence_dup, sentence);
    char *curr_word = strtok(sentence_dup, " ");
    while (curr_word != NULL) {
        if (!strcmp(curr_word, word)) {
            free(sentence_dup);
            return 1;
        }
        curr_word = strtok(NULL, " ");
    }
    free(sentence_dup);
    return 0;
}


/*
 * To print node
 * ex: Node 2: p.py <- 1
 * ----> means node 2' target is : p.py and parent is node 1
 */
void display_node(node *n){
    printf("Node %d : %s \n", n->id, n->rule);
    printf("\tN_Dependencies : %d\n", n->number_dependencies);
    printf("\tDependencies : %s\n", n->str_dependencies);
    printf("\tCmd : %s\n", n->command);
    /*for (int i = 0; i < n->number_dependencies; i++) {
        display_node(n->dependencies[i]);
    }*/
}


/*
 * parse the Makefile
 */
void parse_makefile(char *filename, int *size){
    file_words = malloc(SIZE_WORDS * sizeof(char *));
    for(int i=0; i<SIZE_WORDS; i++){
        file_words[i] = malloc(SIZE_LETTERS * sizeof(char));
    }
    int count_words = 0;
    int count_letters = 0;
    int id = 0;
    FILE *in_file = fopen(filename, "r");
    char value;
    while((fscanf(in_file, "%c", &value)) != EOF) {
        if(id == 0){
            if(value == 13 || value == 10 || value == ' ' || value == '\t'){
                continue;
            } else if(value == ':'){
                id = 1;
                count_words++;
                count_letters = 0;
            } else {
                file_words[count_words][count_letters] = value;
                count_letters++;
            }
        } else if(id == 1){
            if(value == '\t' || value == 13){
                continue;
            } else if(value == 10){
                id = 2;
                count_words++;
                count_letters = 0;
            } else {
                file_words[count_words][count_letters] = value;
                count_letters++;
            }
        } else {
            if(value == 10) {
                id = 0;
                count_words++;
                count_letters = 0;
            } else if(value == 13) continue;
            else {
                file_words[count_words][count_letters] = value;
                count_letters++;
            }
        }
    }
    *size = count_words;
    fclose(in_file);
}

/*
 * Create a node for each rule
 */
void create_tree(int size, int *number_feuilles, char *c){
    file_nodes = malloc(((size+1)/3) * sizeof(node));
    feuilles = malloc(((size+1)/3) * sizeof(node));
    int count_feuilles = 0;
    for(int i = 0; i < (size+1) / 3; i++){
        feuilles[i] = malloc(sizeof(node));
        file_nodes[i] = malloc(sizeof(node));
        file_nodes[i]->id = i;
        file_nodes[i]->rule = file_words[3 * i];
        file_nodes[i]->parent_id = -1;
        file_nodes[i]->str_dependencies = file_words[3 * i + 1];
        file_nodes[i]->parents = malloc((size+1) / 3*sizeof(node*));
        file_nodes[i]->number_parents = 0;
        file_nodes[i]->isReady = 0;
        char *cmd = file_words[3 * i + 2];
        if(cmd[0] != '\t'){
            printf("Rule[*%s*] : Makefile missing separator error in spite of using the correct indentation", file_words[3 * i]);
            exit(EXIT_FAILURE);
        }
        if(system(c) != 0){
            printf("%sCannot open Makefile directory \n", KRED);
            exit(EXIT_FAILURE);
        }
        char *src = c;
        char *b  = "; ";
        size_t sizeFull = strlen(src) + strlen(b) + strlen(cmd) + 3;
        char *full = (char *) malloc(sizeFull);
        strcpy(full, src);
        strcat(full, b);
        strcat(full, cmd);
        file_nodes[i]->command = full;
    }

    for(int i = 0; i < (size + 1) / 3; i++){
        for(int j = i + 1; j < (size + 1) / 3; j++){
            //char *check_str_in_str = strstr(file_nodes[i]->str_dependencies, file_nodes[j]->rule);
            int check_str_in_str = word_in_sentence(file_nodes[i]->str_dependencies, file_nodes[j]->rule);
            if(check_str_in_str == 1){
                (file_nodes[i]->number_dependencies)++;
                file_nodes[j]->parents[file_nodes[j]->number_parents] = file_nodes[i];
                (file_nodes[j]->number_parents)++;
            }
        }
    }

    for(int i = 0; i < (size + 1) / 3; i++){
        int isEmpty = 0;
        for(size_t j=0; j<strlen(file_nodes[i]->str_dependencies); j++){
            if(file_nodes[i]->str_dependencies[j]!=' '){
                isEmpty++;
                break;
            }
        }
        if(isEmpty == 0){
            feuilles[count_feuilles] = file_nodes[i];
            count_feuilles++;
        }
    }
    *number_feuilles = count_feuilles;
}

/*
 * Parse tree and execute commande
 */
void execute(node *n){
    if(system(n->command) != 0){
        printf("%sRule[%s] : Cannot build target \n", KRED, n->rule);
        exit(EXIT_FAILURE);
    }
    if(n->number_parents != 0){
        for(int i=0; i<(n->number_parents); i++){
            if(n->parents[i] != NULL){
                (n->parents[i]->isReady)++;
                if(n->parents[i]->isReady == n->parents[i]->number_dependencies){
                    execute(n->parents[i]);
                }
            }
        }
    }
}

void execute_all(int size_feuilles){
    for(int i=0; i<size_feuilles; i++){
        execute(feuilles[i]);
    }
}

void make_simple(char *filename, char *cmd){
    int size = 0;
    int size_feuilles = 0;
    parse_makefile(filename, &size);
    create_tree(size, &size_feuilles, cmd);
    execute_all(size_feuilles);
}


void make_distribue(int argc, char **argv, char *filename, char *path, char *cmd){
    int len_words = 0;
    int len_nodes = 0;
    int size_feuilles = 0;

    parse_makefile(filename, &len_words);
    len_nodes = (len_words + 1)/3;
    create_tree(len_words, &size_feuilles, cmd);

    int size, rank, hostlen;
    char hostname[MPI_MAX_PROCESSOR_NAME] = {};

    int *tasks;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    // CREATE STRING TYPE FOR MPI
    MPI_Datatype string_type;
    MPI_Type_contiguous(SIZE_WORDS, MPI_CHAR, &string_type);
    MPI_Type_commit(&string_type);

    data_worker_to_master *localdata;
    localdata = malloc(sizeof(localdata));
    localdata->indexes = malloc(CHUNKSIZE * sizeof(int));
    localdata->worker = malloc(MPI_MAX_PROCESSOR_NAME * sizeof(char));
    MPI_Datatype data_mpi;
    data_worker_to_master dataWorkerToMaster;
    int blocklens[2];
    MPI_Datatype old_types[2];
    MPI_Aint indices[2];
    MPI_Aint addr1, addr2, baseaddr;
    /* Set block lenghts */
    blocklens[0] = CHUNKSIZE;
    blocklens[1] = MPI_MAX_PROCESSOR_NAME;
    /* Set Data types */
    old_types[0] = MPI_INT;
    old_types[1] = MPI_CHAR;

    /* Set byte displacement for each piece of data in structure*/
    MPI_Get_address (localdata, &baseaddr);
    MPI_Get_address(localdata->indexes, &addr1);
    MPI_Get_address(localdata->worker, &addr2);

    indices[0] = addr1 - baseaddr;
    indices[1] = addr2 - baseaddr;
    MPI_Type_create_struct(2, blocklens, indices, old_types, &data_mpi);
    MPI_Type_commit(&data_mpi);


    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &hostlen);

    char commands[CHUNKSIZE][SIZE_WORDS];
    int indexes[CHUNKSIZE];
    char hostname_local[SIZE_LETTERS];
    int count_sending = 0;
    int indexes_to_be_sent[CHUNKSIZE];
    int *index_ready = malloc(len_nodes * sizeof(int));
    int len_node_ready = 0;
    char *c_python;
    char *add_machine = "python3 ./distributed/working_machine.py 0 ";
    char *send_working_list = "python3 ./distributed/working_machine.py 3 ";
    char *send_files = "python3 ./distributed/working_machine.py 4 ";
    char *space = " ";
    size_t size_python;

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0){
        for (int i = 0; i < size_feuilles; ++i) {
            file_nodes[feuilles[i]->id]->is_executed = 1;
            index_ready[len_node_ready + i] = feuilles[i]->id;
        }
        len_node_ready += size_feuilles;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0) {
        tasks = (int *) malloc( (size - 1) * sizeof(int));

    }

    if(rank == 0){
        int sender;
        int offset = 0;
        while(1){
            //MPI_Recv(indexes, CHUNKSIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(localdata, 1, data_mpi, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            sender = status.MPI_SOURCE;
            for (int i = 0; i < CHUNKSIZE; ++i) {
                indexes[i] = localdata->indexes[i];
            }
            strncpy(hostname_local, localdata->worker, strlen(localdata->worker) - 5);
            printf("%s \n", hostname_local);
            printf("starting to add \n");
            size_python = strlen(add_machine) + strlen(hostname_local) + 1;
            c_python = (char *) malloc(size_python);
            strcpy(c_python, add_machine);
            strcat(c_python, hostname_local);
            printf("%s %s %s\n", KYELLOW, c_python, KWHITE);
            if(system(c_python) != 0){
                free(c_python);
                printf("%s Unable to add machine %s to list of working machines %s \n", KRED, hostname_local, KWHITE);
                exit(EXIT_FAILURE);
            }
            free(c_python);
            //add_machine_working(hostname_local);
            printf("finishing to add \n");
            printf("starting to send \n");

            //send_machine_working_file(hostname);
            size_python = strlen(send_working_list) + strlen(hostname) + 1;
            c_python = (char *) malloc(size_python);
            strcpy(c_python, send_working_list);
            strcat(c_python, hostname);
            printf("%s %s %s\n", KYELLOW, c_python, KWHITE);
            if(system(c_python) != 0){
                free(c_python);
                printf("%s Unable to send list to %s %s \n", KRED, hostname, KWHITE);
                exit(EXIT_FAILURE);
            }
            free(c_python);
            printf("finishing to send \n");

            if (status.MPI_TAG == TAG_BEGIN || status.MPI_TAG == TAG_FINISHED ) {
                //printf("I am receiving \n");
                //MPI_Recv(&hostname_local, 1, string_type, sender, TAG_WORKING, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (status.MPI_TAG == TAG_FINISHED) {
                    for (int i = 0; i < CHUNKSIZE; ++i) {
                        if (indexes[i] != -1) {
                            printf("%s is executed %s \n", KGREEN, KWHITE);
                            file_nodes[indexes[i]]->is_executed = 1;
                        }
                    }
                }
                int count = 0;
                //printf("%s start preparing  %s \n", KRED, KWHITE);
                for (int j = 0; j < len_node_ready; ++j) {
                    if (file_nodes[index_ready[j]]->is_executed == 1) {
                        if (file_nodes[index_ready[j]]->number_parents != 0) {
                            for (int i = 0; i < (file_nodes[index_ready[j]]->number_parents); i++) {
                                if (file_nodes[index_ready[j]]->parents[i] != NULL) {
                                    (file_nodes[index_ready[j]]->parents[i]->isReady)++;
                                    if (file_nodes[index_ready[j]]->parents[i]->isReady ==
                                        file_nodes[index_ready[j]]->parents[i]->number_dependencies) {
                                        index_ready[len_node_ready +
                                                    count] = file_nodes[index_ready[j]]->parents[i]->id;
                                        count++;
                                    }
                                }
                            }
                        }
                    }
                }
                len_node_ready += count;
                //printf("%s end preparing  %s \n", KRED, KWHITE);
            }
            if(offset <= len_nodes - 1){
                size_python = strlen(send_files) + strlen(hostname) + strlen(path) + strlen(space) + 1;
                c_python = (char *) malloc(size_python);
                strcpy(c_python, send_files);
                strcat(c_python, hostname);
                strcat(c_python, space);
                strcat(c_python, path);
                printf("%s %s %s\n", KYELLOW, c_python, KWHITE);
                if(system(c_python) != 0){
                    free(c_python);
                    printf("%s Unable to send files to %s %s \n", KRED, hostname, KWHITE);
                    exit(EXIT_FAILURE);
                }
                free(c_python);
                //send_machine_working(hostname, path);
                for (int i = 0; i < CHUNKSIZE; ++i) {

                    if (offset + i <= len_nodes - 1) {
                        if (index_ready[offset + i] < len_words && 0 <= index_ready[offset + i]) {
                            if (file_nodes[index_ready[offset + i]]->is_sent != 1) {
                                strncpy(commands[i], file_nodes[index_ready[offset + i]]->command,
                                        strlen(file_nodes[index_ready[offset + i]]->command) + 1);
                                indexes_to_be_sent[i] = index_ready[offset + i];
                            } else {

                                strncpy(commands[i], "none", 5);
                                indexes_to_be_sent[i] = -1;

                            }
                        }
                    }
                    else {
                        strncpy(commands[i], "none", 5);
                        indexes_to_be_sent[i] = -1;
                    }

                }
                //printf("%s [offset = %d, len_nodes = %d] start %s \n",KRED, offset, len_nodes, KWHITE);
                MPI_Send(commands, CHUNKSIZE, string_type, sender, TAG_DATA, MPI_COMM_WORLD);
                for (int i = 0; i < CHUNKSIZE; ++i) {
                    if(indexes_to_be_sent[i] != -1){
                        //printf("index to be sent %d \n", indexes_to_be_sent[i]);
                        file_nodes[indexes_to_be_sent[i]]->is_sent = 1;
                        count_sending++;
                    }
                }
                printf("%s Root has been told that %s is working ... from %d %s \n", KRED, localdata->worker, sender ,KWHITE);
                localdata->indexes = indexes_to_be_sent;
                strncpy(localdata->worker, hostname_local, strlen(hostname_local));
                MPI_Send(localdata, 1, data_mpi, sender, TAG_EXECUTED, MPI_COMM_WORLD);
                offset += count_sending;
                count_sending = 0;
                tasks[sender - 1] = offset;
                //printf("[offset = %d, len_nodes = %d] done \n", offset, len_nodes);
            } else {
                MPI_Send(commands, CHUNKSIZE, string_type, sender, TAG_END, MPI_COMM_WORLD);
                for (int i = 2; i < size; ++i) {
                    MPI_Recv(localdata, 1, data_mpi, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    sender = status.MPI_SOURCE;
                    MPI_Send(commands, CHUNKSIZE, string_type, sender, TAG_END, MPI_COMM_WORLD);
                }

                goto finish;
            }
        }
    } else {
        localdata->indexes = indexes;
        printf("hostlen = %d and strlen = %ld \n", hostlen, strlen(hostname));
        strncpy(localdata->worker, hostname, hostlen);
        MPI_Send(localdata, 1, data_mpi, 0, TAG_BEGIN, MPI_COMM_WORLD);
        //MPI_Send(indexes, CHUNKSIZE, MPI_INT, 0, TAG_BEGIN, MPI_COMM_WORLD);
        //machines[0]->working = 1;
        while(1) {
            //MPI_Send(&hostname, 1, string_type, 0, TAG_WORKING, MPI_COMM_WORLD);
            MPI_Recv(commands, CHUNKSIZE, string_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("[%d] received something \n", rank);
            //printf("%s [%d] received something  %s \n", KRED, rank, KWHITE);
            if(TAG_DATA == status.MPI_TAG) {
                MPI_Recv(localdata, 1, data_mpi, 0, TAG_EXECUTED, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                /*for (int i = 0; i < CHUNKSIZE; ++i) {
                    printf("%s [rank=%d] Index received is %d %s \n", KGREEN, rank, indexes[i], KWHITE);
                }*/
                #pragma omp parallel
                {
                    #pragma omp for
                    for (int i = 0; i < CHUNKSIZE; ++i) {
                        printf("Command %d received by %d is : %s \n", i, rank, commands[i]);
                        if(strcmp(commands[i], "none") != 0){
                            if(system(commands[i]) != 0){
                                printf("%s [%d/%d] Cannot run command \n%s", KRED, rank, size, KWHITE);
                                exit(EXIT_FAILURE);
                            }
                        }
                    }
                };
                //MPI_Barrier(MPI_COMM_WORLD);
                //send_machine_working(hostname, path);
                size_python = strlen(send_files) + strlen(hostname) + strlen(path) + strlen(space) + 1;
                c_python = (char *) malloc(size_python);
                strcpy(c_python, send_files);
                strcat(c_python, hostname);
                strcat(c_python, space);
                strcat(c_python, path);
                printf("%s %s %s\n", KYELLOW, c_python, KWHITE);
                if(system(c_python) != 0){
                    free(c_python);
                    printf("%s Unable to send files to %s %s \n", KRED, hostname, KWHITE);
                    exit(EXIT_FAILURE);
                }
                free(c_python);
                //MPI_Barrier(MPI_COMM_WORLD);
                /*char *scp = "sshpass -p \"admin\" scp -o StrictHostKeyChecking=no ";
                //char *scp = "scp ./distributed/tests/Makefile6/premier/list* root@";
                char *scp1 = "* root@";
                char *scp2 = ":";
                size_t sizeScp;
                #pragma omp parallel
                {
                    #pragma omp for
                    for (int i = 0; i < countProcesses; ++i) {
                        if (strcmp(processes[i], hostname) != 0) {
                            char *processorWorker = processes[i];
                            sizeScp = strlen(scp) + strlen(scp1) + 2*strlen(path)  + strlen(scp2)  + strlen(processorWorker) + 1;
                            char *commandScp = (char *) malloc(sizeScp);
                            strcpy(commandScp, scp);
                            strcat(commandScp, path);
                            strcat(commandScp, scp1);
                            strcat(commandScp, processorWorker);
                            strcat(commandScp, scp2);
                            strcat(commandScp, path);
                            //printf("%s \n", commandScp);
                            //system(commandScp);
                        }
                    }
                }*/
                MPI_Send(localdata, 1, data_mpi, 0, TAG_FINISHED, MPI_COMM_WORLD);
            }
            if(TAG_END == status.MPI_TAG){
                goto finish;
            }
        }
    }

    finish:
    printf("[%d] finished \n", rank);
    if(rank == 0){
        free(tasks);
    }
    MPI_Finalize();
}
