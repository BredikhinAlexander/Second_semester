#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define gap while(*s == ' ' || *s == '\n' ) s++



typedef struct Node
{
    double value;
    int type;
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
 };

enum Side
{
    left_side  = 0,
    right_side = 1,
};



const char* name_file_in = "cod.txt";
unsigned int Maxsize = 1000;



Node* GetN(); // числа + буквы (переменные)
Node* GetP(); //считывает скобочки, sin(E), cos(E), ln(E), (E)
Node* GetT(); // P */ P
Node* GetE(); // T +- T


Node* GetTo(); // L | A | If

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

int which_A()
{
    gap;
    char* tmp = s;
    while (!((*s) == ',' || (*s) == '.' || (*s) == ' '))
    {
        //printf("%c", *s);
        s++;
    }


    char tmp1 = *s;
    *s = '\0';
    gap;
    s++;

    if (strcmp(tmp, "::=") == 0)
    {
        *s = tmp1;
        return prisvaivanie;
    } else if (strcmp(tmp, "input") == 0)
    {
        *s = tmp1;
        return input;
    } else if (strcmp(tmp, "print") == 0)
    {
        *s = tmp1;
        return print;
    } else if (strcmp(tmp, "return") == 0)
    {
        *s = tmp1;
        return ret;
    }

}

Node* GetA() {
    assert(s);

    Node* val1 = NULL;

    if ((*s) == '"') {
        gap;

        val1 = GetE();

        assert((*s) == '"');
        s++;
        gap;


        Node* val2 = (Node*) calloc(1, sizeof(*val2));

        if(which_A() == print)   val2->type = print;

        else if(which_A() == ret) val2->type = ret;

        else                      printf("Auful\n");

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
                s++;
                gap;
                printf("PAaaaaaadsd %c\n", *s);
                  //////////////////////////////////////////////////

                Node *val5 = GetN();

                Node *val6 = (Node *) calloc(1, sizeof(*val6));

                val6->type = MODE_VARIABLES;

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

        gap;

        val2->type = MODE_CALL;

        val2->left = val1;
        val1->parent = val2;

        val2->right = val4;
        if (val4 != NULL) val4->parent = val2;

        return val2;
    }

    Node* val3 = NULL;
    switch (which_A()) {
        case input:
            val2->type = input;
            break;
/*
        case TUDY:
            (val2->Info).mode = MODE_BORN;
            break;
*/
        case prisvaivanie:
            val2->type = prisvaivanie;

            while ((*s) == ' ') s++;

            val3 = GetE();
            gap;
            printf("tyyyyyt\n");

            val2->left = val1;
            val1->parent = val2;

            val2->right = val3;
            val1->parent = val3;

            assert(s);
            return val2;

        case print:
            val2->type = print;
            break;

        case ret:
            val2->type = ret;
            break;

        default:
            printf("AAaa zadolbalo\n");
    }

    val2->left = val1;
    val1->parent = val2;

    val2->right = NULL;
    printf("eeee %c\n", *s);
    assert(((*s) == '.') || ((*s) == ',') );

    assert(s);
    return val2;
}

Node* GetIf()
{
    Node* val1 = GetC();

    assert(*s == '"');

    s++;

    if (*s == ':')
    {
        s++;
        gap;


        Node* val2 = GetTo();

        while (*s != '.')
        {
            s++;
            gap;


            Node* val3 = GetTo();

            Node* new_node = create_node('o', Op);

            add_node(val2, new_node, left_side);
            add_node(val3, new_node, right_side);

        }

        assert(*s == '.');
        s++;
        gap;

        Node* new_val = create_node('I', If);
        add_node(val1, new_val, left_side);
        add_node(val2, new_val, right_side);

        return new_val;
    }


}


int which_To()
{
    char* tmp = s;
    s = strchr(s, ' ');
    if (!s)
    {
        s = tmp;
        return op_top;
    }

    *s = '\0';

    if (strcmp(tmp, "if") == 0)
    {
        *s = ' ';
        s += 2;
        return If;
    } else
    {
        *s = ' ';
        s = tmp;
        while (((*s) == ' ') || ((*s) == '\n')) s++;
        return op_top;
    }
}


Node* GetTo()
{
    Node* val = NULL;

    int smth = which_To();
    switch (smth)
    {
        case If:
            val = GetIf();
            break;
        case op_top:
            val = GetA();
            break;
        default:
            printf("Smth wron\n");
    }
    printf("AAAAAAAAAAAAAAAAAAAAAA %d\n", *s);
    gap;
    assert((*s) == '.' || (*s) == ',');
    return val;

}

Node* GetG()
{
    Node* val1 = GetTo();

    while (*s == ',')
    {
        s++;
        gap;

        Node* val2 = GetTo();

        Node* new_node = create_node('o', Op);

        add_node(val1, new_node, left_side);
        add_node(val2, new_node, right_side);

        Node* tmp = val1;
        val1 = new_node;
        new_node = tmp;
    }
    printf("%c\n", *s);
    assert(*s == '.');
    s++;
    return val1;
}


Node* GetF(char* buff) {
    assert(buff);

    s = buff;

    char* tmp = s;
    s = strchr(s, ' ');

    (*s) = '\0';
    if (strcmp(tmp, "main") != 0) {
        printf("Need main!!!\n");

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

    val1->type = MODE_NAME;

    Node* val2 = GetG();

    gap;
    assert((*s) == '-');
    s += 5;

    gap;

    val1->left = val2;
    val2->parent = val1;

    val1->right = NULL;

    while ((*s) != '\0') {

        s = strchr(s, ' ');

        (*s) = '\0';

        Node* val3 = (Node *) calloc(1, sizeof(*val3));

        val3->type = MODE_NAME;

        (*s) = ' ';
        s++;
        assert((*s) == '"');

        s++;
        Node* val4 = NULL;
        if ((*s) != '"') {
            val4 = GetN();

            while ((*s) == ';') {
                s++;
                gap; /////////////////////////////////////////////////////////////

                Node *val5 = GetN();

                Node *val6 = (Node *) calloc(1, sizeof(*val6));

                val6->type = MODE_VARIABLES;

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
        s++;

        gap;
        printf("ddd %c\n", *s);
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

        val8->type = MODE_FUNC;

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