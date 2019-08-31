#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>



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


enum  Type_of_mode{
    MODE_NUMBER    = 65,
    MODE_VAR       = 66,
    MODE_SIGN      = 67,
    MODE_OP        = 68,
    MODE_NAME      = 69,
    MODE_OPERATORR = 70,
    MODE_LOOP      = 71,
    MODE_IF        = 72,
    MODE_OUT       = 73,
    MODE_IN        = 74,
    MODE_ASSIGN    = 75,
    MODE_BORN      = 76,
    MODE_BIGGER    = 77,
    MODE_EQUAL     = 78,
    MODE_LESS      = 79,
    MODE_NOT_EQUAL = 80,
    MODE_VARIABLES = 81,
    MODE_FUNC      = 82,
    MODE_CALL      = 83,
    MODE_RETURN    = 84,
};

enum {
    While  = 97,
    If     = 98,
    OP_TOP = 99,
    print  = 100,
    input  = 101,
    hi_hi  = 102,
    prisv  = 103,
    ret    = 104,
};


// F ::= main "" '\n'-----'\n' G '\n'-----'\n' {Id "{Id;}+" '\n'-----'\n' G '\n'-----'\n'}*
Node* GetF(char* buff);

// G ::= {To','}* {To'.'}\0'
Node* GetG();

// To ::= L | A | If
Node* GetTo();

// L ::= while "C": {To','}+ {To'.'} | while "C" To
Node* GetL();

// If ::= if "C": {To','}* {To'.'} | if "C" To
Node* GetIf();

// A ::= Id ::= E | Id hi_hi | Id input | "E" print | Id "{Id; }*" | {"E" return | Id return}
Node* GetA();

// C ::= {(E) | N} {(E) | N} [< | > | : | #]
Node* GetC();

// E ::= T{[+-]T}*
Node* GetE();

// T ::= P{[*/]P}*
Node* GetT();

// P ::= (E) | sin(E) | cos(E) | ln(E) | lg(E) | N | Id
Node* GetP();

// N ::= [0-9]{[0-9]}*
Node* GetN();

// Id ::= [a-z & A-Z]{[a-z & A-Z]}*
Node* GetId();

const int max_word = 1000;

int which_A();
int which_To();

//void create_tree(Node* pos, FILE* dot_out);

char* pc = NULL;

const char* name_file_in = "cod.txt";
const char* name_file_tree_str = "str_tree.txt";
unsigned int Maxsize = 1000;

#define gap pc++;\
                   while (((*pc) == ' ') || ((*pc) == '\n')) pc++;


Node* GetF(char* buff) {
    assert(buff);

    pc = buff;

    char* tmp = pc;
    pc = strchr(pc, ' ');

    (*pc) = '\0';
    if (strcmp(tmp, "main") != 0) {
        printf("Need main!!!\n");

        abort();
    }

    (*pc) = ' ';
    pc++;
    assert((*pc) == '"');

    pc += 3;
    gap;

    assert((*pc) == '-');
    pc += 5;
    gap;

    Node *val1 = (Node *) calloc(1, sizeof(*val1));


    (val1->Info).mode = MODE_NAME;
    (val1->Info).name = strdup("main");

    Node* val2 = GetG();

    gap;
    assert((*pc) == '-');
    pc += 5;

    gap;

    val1->Left = val2;
    val2->Parent = val1;

    val1->Right = NULL;

    while ((*pc) != '\0') {
        tmp = pc;
        pc = strchr(pc, ' ');

        (*pc) = '\0';

        Node* val3 = (Node *) calloc(1, sizeof(*val3));


        (val3->Info).mode = MODE_NAME;
        (val3->Info).name = strdup(tmp);

        (*pc) = ' ';
        pc++;
        assert((*pc) == '"');

        pc++;
        Node* val4 = NULL;
        if ((*pc) != '"') {
            val4 = GetId();

            while ((*pc) == ';') {
                gap;

                Node *val5 = GetId();

                Node *val6 = (Node *) calloc(1, sizeof(*val6));

                (val6->Info).mode = MODE_VARIABLES;

                Node* tmp1 = val4;
                val4 = val6;
                val6 = tmp1;

                val4->Left = val6;
                val6->Parent = val4;

                val4->Right = val5;
                val5->Parent = val4;
            }
        }

        assert((*pc) == '"');
        pc++;

        assert((*pc) == ':');

        gap;

        assert((*pc) == '-');
        pc += 5;

        gap;

        val3->Right = val4;
        if (val4 != NULL) val4->Parent = val3;

        Node* val7 = GetG();

        gap;
        assert((*pc) == '-');
        pc += 5;

        gap;

        val3->Left = val7;
        val7->Parent = val3;
        Node* val8 = (Node*) calloc(1, sizeof(*val8));


        (val8->Info).mode = MODE_FUNC;

        Node* tmp2 = val1;
        val1 = val8;
        val8 = tmp2;

        val1->Left = val8;
        val8->Parent = val1;

        val1->Right = val3;
        val3->Parent = val1;
    }

    assert(*(pc) == '\0');
    return val1;
}

Node* GetG() {
    assert(pc);

    Node* val1 = GetTo();

    while ((*pc) == ',') {
        gap;

        Node* val2 = GetTo();

        Node *val3 = (Node *) calloc(1, sizeof(*val3));

        (val3->Info).mode = MODE_OPERATORR;

        Node* tmp = val1;
        val1 = val3;
        val3 = tmp;

        val1->Right = val2;
        val2->Parent = val1;

        val1->Left = val3;
        val3->Parent = val1;
    }

    assert(pc);
    assert((*pc) == '.');
    return val1;
}

Node* GetTo() {
    assert(pc);

    Node* val = NULL;

    int smth = which_To();
    switch (smth) {
        case If:
            val = GetIf();
            break;

        case While:
            val = GetL();
            break;

        case OP_TOP:
            val = GetA();
            break;

        default:
            printf("Smth wrong!\n");
            abort();
    }

    assert(pc);
    assert((*pc) == '.' || (*pc) == ',');
    return val;
}

Node* GetL() {
    assert(pc);

    Node* val1 = GetC();

    assert((*pc) == '"');

    pc++;

    if ((*pc) == ':') {
        gap;

        Node* val2 = GetTo();

        while ((*pc) != '.') {
            gap;

            Node *val4 = GetTo();

            Node* val5 = (Node *) calloc(1, sizeof(*val1));


            (val5->Info).mode = MODE_OPERATORR;

            Node* tmp = val2;
            val2 = val5;
            val5 = tmp;

            val2->Right = val4;
            val4->Parent = val2;

            val2->Left = val5;
            val5->Parent = val2;
        }

        assert((*pc) == '.');
        pc++;

        Node* val3 = (Node *) calloc(1, sizeof(*val1));


        (val3->Info).mode = MODE_LOOP;

        val3->Left = val1;
        val1->Parent = val3;

        val3->Right = val2;
        val2->Parent = val3;

        assert(pc);
        return val3;
    }

    gap;

    Node* val2 = GetTo();

    Node* val3 = (Node *) calloc(1, sizeof(*val1));


    (val3->Info).mode = MODE_LOOP;

    val3->Left = val1;
    val1->Parent = val3;

    val3->Right = val2;
    val2->Parent = val3;

    assert((*pc) == ',' || (*pc) == '.');
    assert(pc);
    return val3;
}

Node* GetIf() {
    assert(pc);

    Node* val1 = GetC();

    assert((*pc) == '"');

    pc++;

    if ((*pc) == ':') {
        gap;

        Node* val2 = GetTo();

        while ((*pc) != '.') {
            gap;

            Node *val4 = GetTo();

            Node* val5 = (Node *) calloc(1, sizeof(*val1));


            (val5->Info).mode = MODE_OPERATORR;

            Node* tmp = val2;
            val2 = val5;
            val5 = tmp;

            val2->Right = val4;
            val4->Parent = val2;

            val2->Left = val5;
            val5->Parent = val2;
        }

        assert((*pc) == '.');
        pc++;

        Node* val3 = (Node *) calloc(1, sizeof(*val1));

        (val3->Info).mode = MODE_IF;

        val3->Left = val1;
        val1->Parent = val3;

        val3->Right = val2;
        val2->Parent = val3;

        assert(pc);
        return val3;
    }

    gap;

    Node* val2 = GetTo();

    Node* val3 = (Node *) calloc(1, sizeof(*val1));

    (val3->Info).mode = MODE_IF;

    val3->Left = val1;
    val1->Parent = val3;

    val3->Right = val2;
    val2->Parent = val3;

    assert((*pc) == ',' || (*pc) == '.');
    assert(pc);
    return val3;
}

Node* GetA() {
    assert(pc);

    Node* val1 = NULL;

    if ((*pc) == '"') {
        gap;

        val1 = GetE();

        assert((*pc) == '"');

        gap;

        Node* val2 = (Node*) calloc(1, sizeof(*val2));

        if(which_A() == print)   (val2->Info).mode = MODE_OUT;

        else if(which_A() == ret) (val2->Info).mode = MODE_RETURN;

        else                      abort();

        val2->Left = val1;
        val1->Parent = val2;

        val2->Right = NULL;

        assert(pc);
        return val2;
    }


    else val1 = GetId();

    Node* val2 = (Node*) calloc(1, sizeof(*val2));

    if ((*pc) == '"') {
        pc++;

        Node* val4 = NULL;
        if ((*pc) != '"') {
            val4 = GetId();

            while ((*pc) == ';') {
                gap;

                Node *val5 = GetId();

                Node *val6 = (Node *) calloc(1, sizeof(*val6));

                (val6->Info).mode = MODE_VARIABLES;

                Node* tmp1 = val4;
                val4 = val6;
                val6 = tmp1;

                val4->Left = val6;
                val6->Parent = val4;

                val4->Right = val5;
                val5->Parent = val4;
            }
        }

        assert((*pc) == '"');

        gap;

        (val2->Info).mode = MODE_CALL;

        val2->Left = val1;
        val1->Parent = val2;

        val2->Right = val4;
        if (val4 != NULL) val4->Parent = val2;

        return val2;
    }

    Node* val3 = NULL;
    switch (which_A()) {
        case input:
            (val2->Info).mode = MODE_IN;
            break;

        case hi_hi:
            (val2->Info).mode = MODE_BORN;
            break;

        case prisv:
            (val2->Info).mode = MODE_ASSIGN;

            while ((*pc) == ' ') pc++;

            val3 = GetE();

            val2->Left = val1;
            val1->Parent = val2;

            val2->Right = val3;
            val1->Parent = val3;

            assert(pc);
            return val2;

        case print:
            (val2->Info).mode = MODE_OUT;
            break;

        case ret:
            (val2->Info).mode = MODE_RETURN;
            break;

        default:
            abort();
    }

    val2->Left = val1;
    val1->Parent = val2;

    val2->Right = NULL;

    assert(((*pc) == '.') || ((*pc) == ',') );

    assert(pc);
    return val2;
}

Node* GetC() {
    Node* val1 = GetE();
    Node* val2 = GetE();

    char* tmp = pc;
    pc = strchr(pc, '"');
    (*pc) = '\0';

    Node* val3 = (Node*) calloc(1, sizeof(*val3));

    if      (strcmp(tmp, ">") == 0)     (val3->Info).mode = MODE_BIGGER;

    else if (strcmp(tmp, "<") == 0) (val3->Info).mode = MODE_LESS;

    else if (strcmp(tmp, ":") == 0)      (val3->Info).mode = MODE_EQUAL;

    else if (strcmp(tmp, "#") == 0)      (val3->Info).mode = MODE_NOT_EQUAL;

    val3->Left = val1;
    val1->Parent = val3;

    val3->Right = val2;
    val2->Parent = val3;

    (*pc) = '"';

    assert(pc);
    return val3;
}

Node* GetE() {
    assert(pc);

    Node* val1 = GetT();

    while(*pc == '+' || *pc == '-') {
        char op = (*pc);

        gap;

        Node* val2 = GetT();

        Node* val3 = (Node*) calloc(1, sizeof(*val3));

        (val3->Info).mode = MODE_SIGN;
        (val3->Info).number = op;

        Node* tmp = val1;
        val1 = val3;
        val3 = tmp;

        val1->Right = val2;
        val2->Parent = val1;

        val1->Left = val3;
        val3->Parent = val1;
    }

    assert(pc);
    return val1;
}

Node* GetT() {
    assert(pc);

    Node* val1 = GetP();

    while(*pc == '*' || *pc == '/') {
        char op = (*pc);

        gap;

        Node* val2 = GetP();

        Node* val3 = (Node*) calloc(1, sizeof(*val3));

        (val3->Info).mode = MODE_SIGN;
        (val3->Info).number = op;


        Node* tmp = val1;
        val1 = val3;
        val3 = tmp;

        val1->Right = val2;
        val2->Parent = val1;

        val1->Left = val3;
        val3->Parent = val1;
    }

    assert(pc);
    return val1;
}

Node* GetP() {
    assert(pc);

    if (*pc == '(')
    {
        pc++;
        Node* val = GetE();
        gap;
        assert(*pc == ')');
        pc++;
        return val;
    } else if (*pc == 's' && *(pc+1) == 'i' && *(pc+2) == 'n')
    {
        pc += 4;
        Node* val = GetE();
        gap;
        assert(*pc == ')');
        pc++;
        return val;
    } else if (*pc == 'c' && *(pc+1) == 'o' && *(pc+2) == 's')
    {
        pc += 4;
        Node* val = GetE();
        gap;
        assert(*pc == ')');
        pc++;
        return val;
    } else if (*pc == 'l' && *(pc+1) == 'n')
    {
        pc += 3;
        Node* val = GetE();
        gap;
        assert(*pc == ')');
        pc++;
        return val;
    }
    else
        return GetN();
}

Node* GetN() {
    assert(pc);

    Node* val = (Node*) calloc(1, sizeof(*val));;

    if ('0' <= (*pc) && (*pc) <= '9') {
        (val->Info).mode = MODE_NUMBER;

        while ('0' <= (*pc) && (*pc) <= '9') {
            (val->Info).number = ((val->Info).number * 10) + (*pc - '0');

            pc++;
        }
    }

    else if (('a' <= (*pc) && (*pc) <= 'z') || ('A' <= (*pc) && (*pc) <= 'Z')) {
        //char var[max_word];
        char* var = (char*)calloc(2, sizeof(char));
        (val->Info).mode = MODE_VAR;

        int i = 0;
        while (('a' <= (*pc) && (*pc) <= 'z') || ('A' <= (*pc) && (*pc) <= 'Z')) {
            var[i] = (*pc);

            pc++;
            i++;
        }

        (val->Info).name = strdup(var);
        //(val->Info).name =
    }

    else {
        printf("Invalid entry!!!\n--> %c\n", *pc);

        abort();
    }

    while ((*pc) == ' ') pc++;

    assert(pc);
    return val;
}

Node* GetId() {
    assert(pc);

    Node* val = (Node*) calloc(1, sizeof(*val));


    if (('a' <= (*pc) && (*pc) <= 'z') || ('A' <= (*pc) && (*pc) <= 'Z')) {
        //char var[max_word] ;
        char* var = (char*)calloc(max_word, sizeof(*var));
        (val->Info).mode = MODE_VAR;

        int i = 0;
        while (('a' <= (*pc) && (*pc) <= 'z') || ('A' <= (*pc) && (*pc) <= 'Z')) {
            var[i] = (*pc);

            pc++;
            i++;
        }

        (val->Info).name = strdup(var);
    }

    else {
        printf("Invalid entry!!!\n--> %c\n", *pc);

        abort();
    }

    while ((*pc) == ' ') pc++;

    assert(pc);
    return val;
}



int which_To() {
    assert(pc);

    char* tmp = pc;
    pc = strchr(pc, ' ');
    if (!pc) {
        pc = tmp;

        assert(pc);
        return OP_TOP;
    }

    (*pc) = '\0';

    if (strcmp(tmp, "while") == 0) {
        (*pc) = ' ';

        pc += 2;

        assert(pc);
        return While;
    }

    else if (strcmp(tmp, "if") == 0) {
        (*pc) = ' ';

        pc += 2;

        assert(pc);
        return If;
    }

    else {
        (*pc) = ' ';

        pc = tmp;
        while (((*pc) == ' ') || ((*pc) == '\n')) pc++;

        assert(pc);
        return OP_TOP;
    }
}

int which_A() {
    assert(pc);

    char* tmp = pc;
    while ( !((*pc) == ',' || (*pc) == '.' || (*pc) == ' ') ) pc++;

    char tmp1 = (*pc);
    (*pc) = '\0';

    if (strcmp(tmp, "::=") == 0) {
        (*pc) = tmp1;

        assert(pc);
        return prisv;
    }

    else if (strcmp(tmp, "hi_hi") == 0) {
        (*pc) = tmp1;

        assert(pc);
        return hi_hi;
    }

    if (strcmp(tmp, "input") == 0) {
        (*pc) = tmp1;

        assert(pc);
        return input;
    }

    if (strcmp(tmp, "print") == 0) {
        (*pc) = tmp1;

        assert(pc);
        return print;
    }

    else if (strcmp(tmp, "return") == 0) {
        (*pc) = tmp1;

        assert(pc);
        return ret;
    }
}

void print_mode(Node* pos, FILE* dot_out);


void create_tree(Node* pos, FILE* dot_out);

void dot(Node* main_node) {


    FILE *file_dot = fopen("language.dot", "wb");
    assert(file_dot);

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\n");

    create_tree(main_node, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

    system("dot -Tjpg -o tree.jpg language.dot");
    system("xviewer tree.jpg");


}

int tmp = 0;
void create_tree(Node* pos, FILE* dot_out) {
    assert(dot_out);
    assert(pos);

    fprintf(dot_out, "\t\they%d", tmp);
    print_mode(pos, dot_out);
    fprintf(dot_out, ";\n");

    int iskan = tmp;

    if (pos->Left != NULL) {
        fprintf(dot_out, "\t\they%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d;\n", ++tmp);

        create_tree(pos->Left, dot_out);
    }

    if (pos->Right != NULL) {
        fprintf(dot_out, "\t\they%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d;\n", ++tmp);

        create_tree(pos->Right, dot_out);
    }

    assert(pos);
    assert(dot_out);
}

void print_mode(Node* pos, FILE* dot_out) {
    assert(pos);
    assert(dot_out);
    //printf("tyt\n");

    switch ((pos->Info).mode) {
        case MODE_OP:        fprintf(dot_out, "[label = \"%s\", shape = \"Mcircle\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#32CD32\"]", (pos->Info).name);
            break;

        case MODE_SIGN:      fprintf(dot_out, "[label = \"%c\", shape = \"diamond\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#F0E68C\"]", (pos->Info).number);
            break;

        case MODE_VAR:       fprintf(dot_out, "[label = \"%s\", shape = \"component\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#9932CC\"]", (pos->Info).name);
            break;

        case MODE_NUMBER:    fprintf(dot_out, "[label = \"%d\", shape = \"Msquare\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFA07A\"]", (pos->Info).number);
            break;

        case MODE_NAME:      fprintf(dot_out, "[label = \"%s\", shape = \"invtriangle\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#00FFFF\"]", (pos->Info).name);
            break;

        case MODE_OPERATORR: fprintf(dot_out, "[label = \"operator\", shape = \"egg\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFFFFF\"]");
            break;

        case MODE_LOOP:      fprintf(dot_out, "[label = \"loop\", shape = \"house\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#D2691E\"]");
            break;

        case MODE_IF:        fprintf(dot_out, "[label = \"if\", shape = \"house\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#800000\"]");
            break;

        case MODE_OUT:       fprintf(dot_out, "[label = \"out\", shape = \"box3d\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#32CD32\"]");
            break;

        case MODE_IN:        fprintf(dot_out, "[label = \"in\", shape = \"box3d\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFDEAD\"]");
            break;

        case MODE_ASSIGN:    fprintf(dot_out, "[label = \"=\", shape = \"box3d\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#708090\"]");
            break;

        case MODE_BORN:      fprintf(dot_out, "[label = \"born\", shape = \"box3d\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#EE82EE\"]");
            break;

        case MODE_BIGGER:    fprintf(dot_out, "[label = \">\", shape = \"parallelogram\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#006400\"]");
            break;

        case MODE_EQUAL:     fprintf(dot_out, "[label = \"==\", shape = \"parallelogram\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FF7F50\"]");
            break;

        case MODE_LESS:      fprintf(dot_out, "[label = \"<\", shape = \"parallelogram\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFE4B5\"]");
            break;

        case MODE_NOT_EQUAL: fprintf(dot_out, "[label = \"!=\", shape = \"parallelogram\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FF4500\"]");
            break;

        case MODE_VARIABLES: fprintf(dot_out, "[label = \"variable\", shape = \"egg\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFFFFF\"]");
            break;

        case MODE_FUNC:      fprintf(dot_out, "[label = \"function\", shape = \"egg\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFFFFF\"]");
            break;

        case MODE_CALL:      fprintf(dot_out, "[label = \"call\", shape = \"egg\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFFFFF\"]");
            break;

        case MODE_RETURN:    fprintf(dot_out, "[label = \"return\", shape = \"box3d\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#F5C32F\"]");
            break;

        default:             fprintf(dot_out, "\"error\"");
                            printf("Error in dot\n");
            break;
    }

    assert(pos);
    assert(dot_out);
}




void take_params(Node* pos);
void std_tree_filling(Node* pos);
void take_body(Node* pos);
void write_as_std_tree(Node* main_node, char* buffer);

char* ip = NULL;
void write_as_std_tree(Node* main_node, char* buffer) {

    ip = buffer;
    std_tree_filling(main_node);

    *ip++ = 0x00;

}


int skobka = 0;
void std_tree_filling(Node* pos) {
    if (pos->Info.mode == MODE_FUNC) {
        std_tree_filling(pos->Left);

        std_tree_filling(pos->Right);
        return;
    }

    assert(pos->Info.mode == MODE_NAME);

    *ip++ = '#';

    for (int i = 0; pos->Info.name[i]; i++)
        *ip++ = pos->Info.name[i];

    *ip++ = ' ';
    *ip++ = '(';
    if (pos->Right) take_params(pos->Right);
    else            *ip++ = ' ';
    *ip++ = ')';

    //*ip++ = ' ';
    take_body(pos->Left);

    *ip++ = '{';
    *ip++ = '}';

    for (int i = 0; i < skobka; i++)
        *ip++ = '}';

    *ip++ = '\n';

    skobka = 0;
}

void take_params(Node* pos) {
    if (pos->Info.mode != MODE_VARIABLES) {
        assert(pos->Info.mode == MODE_VAR);

        for (int i = 0; pos->Left->Info.name[i]; i++)
            *ip++ = pos->Info.name[i];
    }

    assert(pos->Left->Info.mode  == MODE_VAR);
    assert(pos->Right->Info.mode == MODE_VAR);

    *ip++ = ' ';
    for (int i = 0; pos->Left->Info.name[i]; i++)
        *ip++ = pos->Left->Info.name[i];

    *ip++ = ' ';
    *ip++ = ',';
    *ip++ = ' ';

    for (int i = 0; pos->Right->Info.name[i]; i++)
        *ip++ = pos->Right->Info.name[i];

    *ip++ = ' ';
}

#define new_body *ip++ = '{';\
                 *ip++ = 0x42;\
                 *ip++ = '{';

#define end_body *ip++ = '}';\
                 skobka++;

void take_body(Node* pos) {
    if (pos->Info.mode == MODE_OPERATORR) {
        take_body(pos->Left);

        take_body(pos->Right);

        return;
    }

    switch (pos->Info.mode) {
        case MODE_BORN: //+
        new_body;

            *ip++ = 0x69;

            assert(pos->Left->Info.mode == MODE_VAR);

            for (int i = 0; pos->Left->Info.name[i]; i++)
                *ip++ = pos->Left->Info.name[i];

            end_body;
            break;

        case MODE_IN: //+
        new_body;

            *ip++ = 0x53;
            *ip++ = 0x30;

            assert(pos->Left->Info.mode == MODE_VAR);

            *ip++ = '{';
            *ip++ = 0x50;

            take_body(pos->Left);

            *ip++ = '{';
            *ip++ = '}';
            *ip++ = '}';

            end_body;
            break;

        case MODE_OUT:
        new_body;

            *ip++ = 0x53;
            *ip++ = 0x31;

            *ip++ = '{';
            *ip++ = 0x50;

            take_body(pos->Left);

            *ip++ = '{';
            *ip++ = '}';
            *ip++ = '}';

            end_body;
            break;

        case MODE_ASSIGN:
            new_body;

            *ip++ = 0x45;

            assert(pos->Left->Info.mode == MODE_VAR);

            take_body(pos->Left);
            take_body(pos->Right);

            end_body;
            break;

        case MODE_IF:
            new_body;

            *ip++ = 0x49;

            take_body(pos->Left);
            take_body(pos->Right);

            end_body;
            break;

        case MODE_CALL:
        new_body;

            *ip++ = 0x43;

            assert(pos->Left->Info.mode == MODE_VAR);

            for (int i = 0; pos->Left->Info.name[i]; i++)
                *ip++ = pos->Left->Info.name[i];

            if (pos->Right) take_body(pos->Right);
            else {
                *ip++ = '{';
                *ip++ = '}';
            }

            end_body;

            break;

        case MODE_SIGN:
            *ip++ = '{';
            *ip++ = 0x73;
            *ip++ = (char) pos->Info.number;

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        case MODE_LOOP:
            *ip++ = '{';

            *ip++ = 0x4c;

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        case MODE_BIGGER:
            *ip++ = '{';

            *ip++ = 0x4f;
            *ip++ = '>';

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        case MODE_LESS:
            *ip++ = '{';

            *ip++ = 0x4f;
            *ip++ = '<';

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        case MODE_NOT_EQUAL:
            *ip++ = '{';

            *ip++ = 0x4f;
            *ip++ = '!';

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        case MODE_EQUAL:
            *ip++ = '{';

            *ip++ = 0x4f;
            *ip++ = '=';

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        case MODE_RETURN:
            *ip++ = '{';

            *ip++ = 0x52;

            take_body(pos->Left);

            *ip++ = '}';

            break;

        case MODE_VAR: //+
            *ip++ = '{';

            *ip++ = 0x56;
            for (int i = 0; pos->Info.name[i]; i++)
                *ip++ = pos->Info.name[i];

            *ip++ = '}';
            break;

        case MODE_NUMBER: //+
            *ip++ = '{';

            *ip++ = 0x4e;
            *((int*) ip) = pos->Info.number;
            ip += sizeof(int);

            *ip++ = '}';
            break;

        case MODE_OP:

            break;

        case MODE_VARIABLES:
            *ip++ = '{';
            *ip++ = 0x50;

            take_body(pos->Left);
            take_body(pos->Right);

            *ip++ = '}';
            break;

        default:
            printf("Ne rabotart((((\n");
    }
}





int main()
{
    FILE *file_in = fopen(name_file_in, "r");
    if (file_in == NULL) {
        printf("Do not open file equation.txt");
        return -1;
    }
    char *buf = (char *) calloc(Maxsize, sizeof(*buf));
    fread(buf, Maxsize, sizeof(*buf), file_in);

    fclose(file_in);

    Node *val = GetF(buf);
    //printf("main_Node %c\n", (int) val->value);

    char* BUFERICHE = (char*) calloc(Maxsize, sizeof(*BUFERICHE));
    write_as_std_tree(val, BUFERICHE);

    FILE* str_tree = fopen(name_file_tree_str, "wb");

    fwrite(BUFERICHE, sizeof(char), Maxsize, str_tree);

    fclose(str_tree);
    printf("\n");
    dot(val);

    free(BUFERICHE);
    free(buf);
}