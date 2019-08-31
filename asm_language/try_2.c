#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define gap s++;\
       while(*s == ' ' || *s == '\n' ) s++


typedef struct info
{
    int mode;
    char* name;
} info;

typedef struct Node
{
    double value;
    int type;
    struct info Info;
    struct Node* parent;
    struct Node* left;
    struct Node* right;

} Node ;

enum Type_of_data
{
    number    = 1,
    variable  = 2,
    operation = 3, // +, -, *, /, >, <, :, #
    Op        = 4,
    If        = 5,
    While     = 6,
    prisvaivanie = 7,  // ::=
    input = 8, // scanf
    print = 9, // printf
    ret = 10, // функции
    op_top = 11, // фигня какая-то
    Functions = 12,
    MODE_FUNC = 13,
    MODE_NAME = 14, // имена функций
    MODE_VARIABLES = 15,  // для имён переменных
    MODE_CALL = 16, // вызов функции
    Hi = 17,
};

enum Side
{
    left_side  = 0,
    right_side = 1,
};



const char* name_file_in = "cod.txt";
unsigned int Maxsize = 1000;



Node* create_node(double val, int type)
{
    Node *s = (Node *) calloc(1, sizeof(*s));
    s->left   = NULL;
    s->right  = NULL;
    s->parent = NULL;
    s->value  = val;
    s->type   = type;
    return s;
}

Node* add_node(Node* add_el, Node* cur_el, int side)
{
    //Node* el = create_node(val, type);
    if (side == 0)
        cur_el->left = add_el;
    else
        cur_el->right = add_el;

    if (add_el != NULL)
        add_el->parent = cur_el;

    return cur_el;
}



// F ::= баш "" '\n'-----'\n' G '\n'-----'\n' {Id "{Id;}+" '\n'-----'\n' G '\n'-----'\n'}*
Node* GetF(char* buff);

// G ::= {To','}* {To'.'}\0'
Node* GetG();

// To ::= L | A | If
Node* GetTo();

// L ::= әле "C": {To','}+ {To'.'} | әле "C" To
Node* GetL();

// If ::= әгәр "C": {To','}* {To'.'} | әгәр "C" To
Node* GetIf();

// A ::= Id бу E | Id туды | Id әйт | "E" күрсәт | Id "{Id; }*" | {"E" бир | Id бир}
Node* GetA();

// C ::= {(E) | N} {(E) | N} [зуррак | кечкенәрәк | тигез | түгeл]
Node* GetC();

// E ::= T{[+-]T}*
Node* GetE();

// T ::= P{[*/]P}*
Node* GetT();

// P ::= (E) | sin(E) | cos(E) | ln(E) | lg(E) | N | Id
Node* GetP();

// N ::= [0-9]{[0-9]}*
Node* GetN();



int which_A();
int which_To();



char* s  = NULL;

Node* GetN() // считывает переменные, числа и пропускает пробелы
{
    double val = 0;

    if (*s == ' ' || *s == '\n')
        s++;

    if (0 <= *s - 'a' && *s - 'a' <= 'z')
    {
        val = *s;
        s++;
        return create_node(val, variable);
    }

    while (0 <= *s - '0' && *s - '0' <= 9)
    {
        val = val*10 + *s - '0';
        s++;
    }

    //printf("%c\n", val);

    return create_node(val, number);

}


Node* GetP() //считывает скобочки
{
    if (*s == '(')
    {
        s++;
        Node* val = GetE();
        gap;
        assert(*s == ')');
        s++;
        return val;
    } else if (*s == 's' && *(s+1) == 'i' && *(s+2) == 'n')
    {
        s += 4;
        Node* val = GetE();
        gap;
        assert(*s == ')');
        s++;
        return val;
    } else if (*s == 'c' && *(s+1) == 'o' && *(s+2) == 's')
    {
        s += 4;
        Node* val = GetE();
        gap;
        assert(*s == ')');
        s++;
        return val;
    } else if (*s == 'l' && *(s+1) == 'n')
    {
        s += 3;
        Node* val = GetE();
        gap;
        assert(*s == ')');
        s++;
        return val;
    }
    else
        return GetN();
}

Node* GetT()
{
    gap;

    Node* val = GetP();
    gap;
    Node* new_node = NULL;
    while (*s == '*' || *s == '/')
    {
        double op = *s;
        s++;
        Node* val2 = GetP();
        gap;
        if (op == '*')
        {
            new_node = create_node(op, operation);
            add_node(val, new_node, left_side);
            add_node(val2, new_node, right_side);
            val = new_node;
        } else if (op == '/')
        {
            new_node = create_node(op, operation);
            add_node(val, new_node, left_side);
            add_node(val2, new_node, right_side);
            val = new_node;
        }

    }
    return val;
}

Node* GetE()
{
    gap;

    Node* val = GetT();
    gap;
    Node* new_node = NULL;
    while (*s == '+' || *s == '-')
    {
        double op = *s;
        s++;
        Node* val2 = GetT();
        gap;
        if (op == '+')
        {
            new_node = create_node(op, operation);
            add_node(val, new_node, left_side);
            add_node(val2, new_node, right_side);
            val = new_node;

        } else if (op == '-')
        {
            new_node = create_node(op, operation);
            add_node(val, new_node, left_side);
            add_node(val2, new_node, right_side);
            val = new_node;
        }
    }
    return val;
}

Node* GetC()
{
    gap;

    Node* val = GetE();
    gap;
    Node* new_node = NULL;
    while (*s == '<' || *s == '>' || *s == '#' || *s == ':')
    {
        double op = *s;
        s++;
        Node* val2 = GetE();
        gap;

        new_node = create_node(op, operation);
        add_node(val, new_node, left_side);
        add_node(val2, new_node, right_side);

        val = new_node;
    }
    return val;
}





Node* GetF(char* buff) {
    assert(buff);

    s = buff;

    char* tmp = s;
    s = strchr(s, ' ');

    (*s) = '\0';
    if (strcmp(tmp, "main") != 0) {
        printf("Need main!!!!!\n");

        abort();
    }

    (*s) = ' ';
    s++;
    assert((*s) == '"');

    s += 3;
    gap;

    assert((*s) == '-');
    s += 5;
    gap;

    Node *val1 = (Node *) calloc(1, sizeof(*val1));


    (val1->Info).mode = MODE_NAME;
    (val1->Info).name = strdup("main");

    Node* val2 = GetG();

    gap;
    assert((*s) == '-');
    s += 5;

    gap;

    val1->left = val2;
    val2->parent = val1;

    val1->right = NULL;

    while ((*s) != '\0') {
        tmp = s;
        s = strchr(s, ' ');

        (*s) = '\0';

        Node* val3 = (Node *) calloc(1, sizeof(*val3));


        (val3->Info).mode = MODE_NAME;
        (val3->Info).name = strdup(tmp);

        (*s) = ' ';
        s++;
        assert((*s) == '"');

        s++;
        Node* val4 = NULL;
        if ((*s) != '"') {
            val4 = GetN();

            while ((*s) == ';') {
                gap;

                Node *val5 = GetN();

                Node *val6 = (Node *) calloc(1, sizeof(*val6));

                (val6->Info).mode = MODE_VARIABLES;

                Node* tmp1 = val4;
                val4 = val6;
                val6 = tmp1;

                val4->left = val6;
                val6->parent = val4;

                val4->right = val5;
                val5->parent = val4;
            }
        }

        assert((*s) == '"');
        s++;

        assert((*s) == ':');

        gap;

        assert((*s) == '-');
        s += 5;

        gap;

        val3->right = val4;
        if (val4 != NULL) val4->parent = val3;

        Node* val7 = GetG();

        gap;
        assert((*s) == '-');
        s += 5;

        gap;

        val3->left = val7;
        val7->parent = val3;

        Node* val8 = (Node*) calloc(1, sizeof(*val8));

        (val8->Info).mode = MODE_FUNC;

        Node* tmp2 = val1;
        val1 = val8;
        val8 = tmp2;

        val1->left = val8;
        val8->parent = val1;

        val1->right = val3;
        val3->parent = val1;
    }

    assert(*(s) == '\0');
    return val1;
}

Node* GetG() {
    assert(s);

    Node* val1 = GetTo();

    while ((*s) == ',') {
        gap;

        Node* val2 = GetTo();

        Node *val3 = (Node *) calloc(1, sizeof(*val3));

        (val3->Info).mode = Op;

        Node* tmp = val1;
        val1 = val3;
        val3 = tmp;

        val1->right = val2;
        val2->parent = val1;

        val1->left = val3;
        val3->parent = val1;
    }

    assert(s);
    assert((*s) == '.');
    return val1;
}

Node* GetTo() {
    assert(s);

    Node* val = NULL;

    int smth = which_To();
    switch (smth) {
        case If:
            val = GetIf();
            break;

        case While:
            val = GetL();
            break;

        case op_top:
            val = GetA();
            break;

        default:
            printf("Smth wrong!\n");
            abort();
    }

    assert(s);
    printf("asd %c\n", *s);
    assert((*s) == '.' || (*s) == ',');
    return val;
}

Node* GetL() {
    assert(s);

    Node* val1 = GetC();

    assert((*s) == '"');

    s++;

    if ((*s) == ':') {
        gap;

        Node* val2 = GetTo();

        while ((*s) != '.') {
            gap;

            Node *val4 = GetTo();

            Node* val5 = (Node *) calloc(1, sizeof(*val1));

            (val5->Info).mode = Op;

            Node* tmp = val2;
            val2 = val5;
            val5 = tmp;

            val2->right = val4;
            val4->parent = val2;

            val2->left = val5;
            val5->parent = val2;
        }

        assert((*s) == '.');
        s++;

        Node* val3 = (Node *) calloc(1, sizeof(*val1));

        (val3->Info).mode = While;

        val3->left = val1;
        val1->parent = val3;

        val3->right = val2;
        val2->parent = val3;

        assert(s);
        return val3;
    }

    gap;

    Node* val2 = GetTo();

    Node* val3 = (Node *) calloc(1, sizeof(*val1));

    (val3->Info).mode = While;

    val3->left = val1;
    val1->parent = val3;

    val3->right = val2;
    val2->parent = val3;

    assert((*s) == ',' || (*s) == '.');
    assert(s);
    return val3;
}

Node* GetIf() {
    assert(s);

    Node* val1 = GetC();

    assert((*s) == '"');

    s++;

    if ((*s) == ':') {
        gap;

        Node* val2 = GetTo();

        while ((*s) != '.') {
            gap;

            Node *val4 = GetTo();

            Node* val5 = (Node *) calloc(1, sizeof(*val1));

            (val5->Info).mode = Op;

            Node* tmp = val2;
            val2 = val5;
            val5 = tmp;

            val2->right = val4;
            val4->parent = val2;

            val2->left = val5;
            val5->parent = val2;
        }

        assert((*s) == '.');
        s++;

        Node* val3 = (Node *) calloc(1, sizeof(*val1));

        (val3->Info).mode = If;

        val3->left = val1;
        val1->parent = val3;

        val3->right = val2;
        val2->parent = val3;

        assert(s);
        return val3;
    }

    gap;

    Node* val2 = GetTo();

    Node* val3 = (Node *) calloc(1, sizeof(*val1));

    (val3->Info).mode = If;

    val3->left = val1;
    val1->parent = val3;

    val3->right = val2;
    val2->parent = val3;

    assert((*s) == ',' || (*s) == '.');
    assert(s);
    return val3;
}

Node* GetA() {
    assert(s);

    Node* val1 = NULL;

    if ((*s) == '"') {
        gap;

        val1 = GetE();

        assert((*s) == '"');

        gap;

        Node* val2 = (Node*) calloc(1, sizeof(*val2));

        if(which_A() == print)   (val2->Info).mode = print;

        else if(which_A() == ret) (val2->Info).mode = ret;

        else                      abort();

        val2->left = val1;
        val1->parent = val2;

        val2->right = NULL;

        assert(s);
        return val2;
    }


    else val1 = GetN();

    Node* val2 = (Node*) calloc(1, sizeof(*val2));

    if ((*s) == '"') {
        s++;

        Node* val4 = NULL;
        if ((*s) != '"') {
            val4 = GetN();

            while ((*s) == ';') {
                gap;

                Node *val5 = GetN();

                Node *val6 = (Node *) calloc(1, sizeof(*val6));

                (val6->Info).mode = MODE_VARIABLES;

                Node* tmp1 = val4;
                val4 = val6;
                val6 = tmp1;

                val4->left = val6;
                val6->parent = val4;

                val4->right = val5;
                val5->parent = val4;
            }
        }

        assert((*s) == '"');

        gap;

        (val2->Info).mode = MODE_CALL;

        val2->left = val1;
        val1->parent = val2;

        val2->right = val4;
        if (val4 != NULL) val4->parent = val2;

        return val2;
    }

    Node* val3 = NULL;
    switch (which_A()) {
        case input:
            (val2->Info).mode = input;
            break;

        case Hi:
            (val2->Info).mode = Hi;
            break;

        case prisvaivanie:
            (val2->Info).mode = prisvaivanie;

            while ((*s) == ' ') s++;

            val3 = GetE();

            val2->left = val1;
            val1->parent = val2;

            val2->right = val3;
            val1->parent = val3;

            assert(s);
            return val2;

        case print:
            (val2->Info).mode = print;
            break;

        case ret:
            (val2->Info).mode = ret;
            break;

        default:
            printf("Zaadfadf\n");
            abort();
    }

    val2->left = val1;
    val1->parent = val2;

    val2->right = NULL;

    assert(((*s) == '.') || ((*s) == ',') );

    assert(s);
    return val2;
}











int which_To() {
    assert(s);

    char* tmp = s;
    s = strchr(s, ' ');
    if (!s) {
        s = tmp;

        assert(s);
        return op_top;
    }

    (*s) = '\0';

    if (strcmp(tmp, "While") == 0) {
        (*s) = ' ';

        s += 2;

        assert(s);
        return While;
    }

    else if (strcmp(tmp, "If") == 0) {
        (*s) = ' ';

        s += 2;

        assert(s);
        return If;
    }

    else {
        (*s) = ' ';

        s = tmp;
        while (((*s) == ' ') || ((*s) == '\n')) s++;

        assert(s);
        return op_top;
    }
}

int which_A() {
    assert(s);
    gap; /////////////////////////////////////////////////////////////////
    char* tmp = s;
    while ( !((*s) == ',' || (*s) == '.' || (*s) == ' ') ) s++;

    char tmp1 = (*s);
    (*s) = '\0';

    if (strcmp(tmp, "::=") == 0) {
        (*s) = tmp1;

        assert(s);
        return prisvaivanie;
    }

    else if (strcmp(tmp, "Hi") == 0) {
        (*s) = tmp1;

        assert(s);
        return Hi;
    }

    if (strcmp(tmp, "input") == 0) {
        (*s) = tmp1;

        assert(s);
        return input;
    }

    if (strcmp(tmp, "print") == 0) {
        (*s) = tmp1;

        assert(s);
        return print;
    }

    else if (strcmp(tmp, "return") == 0) {
        (*s) = tmp1;

        assert(s);
        return ret;
    }
}



void print_mode (Node* cur_el, FILE* dot_out);
int tmp = 0;

void create_tree(Node* cur_el, FILE* dot_out)
{
    fprintf(dot_out, "\t\they%d", tmp);
    print_mode(cur_el, dot_out);
    fprintf(dot_out, ";\n");

    int iskan = tmp;

    if (cur_el->left != NULL)
    {
        fprintf(dot_out, "\t\they%d", iskan);
        fprintf(dot_out, "->");
        fprintf(dot_out, "hey%d;\n", ++tmp);
        create_tree(cur_el->left, dot_out);
    }

    if (cur_el->right != NULL)
    {
        fprintf(dot_out, "\t\they%d", iskan);
        fprintf(dot_out, "->");
        fprintf(dot_out, "hey%d;\n", ++tmp);
        create_tree(cur_el->right, dot_out);
    }

}

void dot(Node* root)
{
    FILE *file_dot = fopen("language.dot", "wb");
    assert(file_dot);

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\n");

    create_tree(root, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

}



void print_mode (Node* cur_el, FILE* dot_out)
{
    switch (cur_el->type) {
        case operation:     fprintf(dot_out, "[label = \"%c\", shape = \"Mcircle\", "
                                             "color=\"#006400\", style=\"filled\", "
                                             "fillcolor = \"#32CD32\"]", (int)cur_el->value);
            break;

        case variable:    fprintf(dot_out, "[label = \"%c\", shape = \"component\", "
                                           "color=\"#8B008B\", style=\"filled\", "
                                           "fillcolor = \"#9932CC\"]", (int)cur_el->value);
            break;

        case number: fprintf(dot_out, "[label = \"%d\", shape = \"Msquare\", "
                                      "color=\"#8B0000\", style=\"filled\", "
                                      "fillcolor = \"#FFA07A\"]", (int)cur_el->value);
            break;

        case If: fprintf(dot_out, "[label = \"%s\", shape = \"house\", "
                                  "color=\"#8B0000\", style=\"filled\", "
                                  "fillcolor = \"#FFA07A\"]", "If");
            break;

        case Op: fprintf(dot_out, "[label = \"%c\", shape = \"invtriangle\", "
                                  "color=\"#8B0000\", style=\"filled\", "
                                  "fillcolor = \"#FFA07A\"]", (int)cur_el->value);
            break;

        case prisvaivanie: fprintf(dot_out, "[label = \"%s\", shape = \"invtriangle\", "
                                            "color=\"#8B0000\", style=\"filled\", "
                                            "fillcolor = \"#FFA07A\"]", "::=");
            break;

        case input: fprintf(dot_out, "[label = \"%s\", shape = \"box3d\", "
                                     "color=\"#8B0000\", style=\"filled\", "
                                     "fillcolor = \"#FFA07A\"]", "input");
            break;

        case print: fprintf(dot_out, "[label = \"%s\", shape = \"invtriangle\", "
                                     "color=\"#8B0000\", style=\"filled\", "
                                     "fillcolor = \"#FFA07A\"]", "print");
            break;

        case ret: fprintf(dot_out, "[label = \"%s\", shape = \"invtriangle\", "
                                   "color=\"#8B0000\", style=\"filled\", "
                                   "fillcolor = \"#FFA07A\"]", "return");
            break;

        case Functions: fprintf(dot_out, "[label = \"%c\", shape = \"invtriangle\", "
                                         "color=\"#8B0000\", style=\"filled\", "
                                         "fillcolor = \"#FFA07A\"]", (int)cur_el->value);
            break;

        case MODE_FUNC:      fprintf(dot_out, "[label = \"function\", shape = \"egg\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFFFFF\"]");
            break;

        case MODE_NAME:      fprintf(dot_out, "[label = \"name\", shape = \"egg\", "
                                              "color=\"#000000\", style=\"filled\", "
                                              "fillcolor = \"#FFFFFF\"]");
            break;

        case MODE_VARIABLES:      fprintf(dot_out, "[label = \"variable\", shape = \"egg\", "
                                                   "color=\"#000000\", style=\"filled\", "
                                                   "fillcolor = \"#FFFFFF\"]");
            break;


        default:          fprintf(dot_out, "\"error\"");
            break;
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

    Node *val = GetF(buf);
    printf("main_Node %c\n", (int) val->value);


    printf("\n");
    dot(val);
}