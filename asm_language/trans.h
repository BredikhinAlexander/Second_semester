#ifndef LANGUAGE_NEW_TRANS_H
#define LANGUAGE_NEW_TRANS_H




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

const size_t yad_place_var = 1607;
const size_t yad_var_num = 5678;


typedef struct info
{
    int mode;
    char* name;
    int number;
} info;

typedef struct Node
{
    struct info Info;
    struct Node* Parent;
    struct Node* Left;
    struct Node* Right;

} Node ;


typedef struct Variables {
    char* name_var;
    size_t place_var;
} Variables;


typedef struct tree {
    Node* main_node;
    size_t count;
} tree;

typedef struct Function {
    char* name;
    tree* tree_func;
    size_t var_num;
    size_t given_num;
    size_t sdvig;
    Variables var[10];
} Function;


enum {
    M_B = 0x42,
    M_s = 0x73,
    M_O = 0x4f,
    M_I = 0x49,
    M_i = 0x69,
    M_N = 0x4e,
    M_R = 0x52,
    M_V = 0x56,
    M_L = 0x4c,
    M_S = 0x53,
    M_C = 0x43,
    M_P = 0x50,
    M_e = 0x45,
};


void FunctionDtor(Function* funcs);

void make_std(char* buffer, Function* funcs);

Node* get_tree();

void std_tree_dot(tree* nameTree, char* dot_out);

void create_std_tree(Node* pos, FILE* dot_out);

void print_std_mode(Node* pos, FILE* dot_out);



int find_variable(char* name, Variables* buf_of_vars);

void create_elf_with_nope(Function* funcs);

void transform_var_table(Function* curr_func);

void aligh_stack(Variables* arr_of_vars);

void push_func_addr(char* name, Function* func_table, char* start_pos);

void take_from_memory(int offset);

void push_to_memory(char* name, Variables* arr_of_vars);

void cut_stack(Variables* arr_of_vars);

char* find_add_func(char* name_func, Function* arr_of_func);

size_t Compile(Function* curr_func, Function* func_table, char** start_pos);

void push_param(int counter);

void save_registers(Variables* arr_of_func);

void return_registers(Variables* arr_of_func);

// Operations
void sub();
void add();
void division();
void cmp();
void mul();
void num(int number);
void scan();
void print();

#endif //LANGUAGE_NEW_TRANS_H
