#ifndef __PARSER__
#define __PARSER__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>


#define SIZE_WORDS 1500
#define SIZE_LETTERS 1500
#define TAG_BEGIN   1
#define TAG_DATA    2
#define TAG_EXECUTED 12
#define TAG_FINISHED 3
#define TAG_SENT_ID 16
#define TAG_RESULT  4
#define TAG_END     8
#define TAG_WORKING 5
#define CHUNKSIZE 4

#define KRED "\x1B[31m"
#define KBLACK "\x1B[30m"
#define KGREEN "\x1B[32m"
#define KWHITE "\x1B[37m"
#define KYELLOW "\x1B[33m"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct node{
    char *rule;
    int parent_id;
    int id;
    int number_dependencies;
    char *str_dependencies;
    struct node **dependencies;
    char *command;
    int number_parents;
    struct node **parents;
    int isReady;
    int is_sent;
    int is_executed;
} node;

typedef struct machine {
    char *name;
    int working;
} machine;

typedef struct data_worker_to_master {
    int *indexes;
    char *worker;
} data_worker_to_master;

int word_in_sentence(char *sentence, char *word);
void display_node(node *n);
void parse_makefile(char *filename, int *size);
void create_tree(int size, int *size_feuilles, char *cmd);
void execute(node *n);
void execute_all(int size_feuilles);
void make_simple(char *filename, char *cmd);
void make_distribue(int argc, char **argv, char *filename, char *path, char *cmd);



#endif
