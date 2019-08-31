#include "trans.h"



void TreeCtor(tree* nameTree) {
    assert(nameTree);

    nameTree->count = 0;

}



void FunctionDtor(Function* func) {
    free(func->name);
    //TreeDtor(func->tree_func);

    func->tree_func = NULL;

    func->var_num = yad_var_num;

    for (int i = 0; i < 10; i++) {
        if (func->var[i].name_var) free(func->var[i].name_var);
        func->var[i].place_var = yad_place_var;
    }
}

size_t CounT = 0;
int pt1 = 0;
int pt2 = 0;
int kpert = 0;

char* pc = NULL;
void make_std(char* buffer, Function* funcs) {
    assert(buffer);
    assert(funcs);

    pc = buffer;

    while (*pc) {
        assert(*pc == '#');

        pc++;

        char *tmp = pc;
        pc = strchr(pc, '(');
        *(pc - 2) = '\0';


        funcs[pt1].name = strdup(tmp);
        *(pc - 2) = ' ';
        *pc++ = '(';
        pc++;

        if (*pc != ')') {
            tmp = pc;
            pc = strchr(pc, ' ');
            *pc++ = '\0';


            funcs[pt1].var[pt2].name_var = strdup(tmp);
            funcs[pt1].var[pt2].place_var = pt2 + 1;
            funcs[pt1].given_num++;
            pt2++;

            while (*pc == ',') {
                pc += 2;

                tmp = pc;
                pc = strchr(pc, ' ');
                *pc++ = '\0';

                funcs[pt1].var[pt2].name_var = strdup(tmp);
                funcs[pt1].var[pt2].place_var = pt2 + 1;
                funcs[pt1].given_num++;
                pt2++;
            }
        }

        assert(*pc == ')');

        pc++;
        //pc++; //////////////

        //printf("ee %d\n", *pc);
        //printf("fff %d\n", CounT);

        assert(*pc == '{');

        //funcs[i].tree_func = new tree;
        funcs[pt1].tree_func = (tree*)calloc(1, sizeof(tree*));

        TreeCtor(funcs[pt1].tree_func);

        funcs[pt1].tree_func->main_node = get_tree(funcs);
        funcs[pt1].tree_func->count = CounT;

        pc++;
        pt1++;
        pt2 = 0;
        kpert = 0;
        CounT = 0;
    }
}

Node* get_tree(Function* funcs) {
    assert(*pc == '{');
    pc++;

    char* tmp = NULL;
    //auto val = new Node;

    Node* val = (Node*)calloc(1, sizeof(*val));

    CounT++;

    switch (*pc) {
        case M_B:
            val->Info.mode = *pc++;

            val->Left  = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_S:
            val->Info.mode   = *pc++;
            val->Info.number = (*pc++) - 0x30;

            val->Left  = get_tree(funcs);
            val->Right = NULL;

            assert(*pc == '}');
            pc++;

            return val;

        case M_e:
            val->Info.mode = *pc++;

            val->Left  = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_I:
            val->Info.mode = *pc++;

            val->Left  = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_C:
            val->Info.mode = *pc++;

            tmp = pc;
            pc = strchr(pc, '{');

            *pc = '\0';
            val->Info.name = strdup(tmp);
            *pc = '{';


            val->Left  = get_tree(funcs);
            val->Right = NULL;

            assert(*pc == '}');
            pc++;

            return val;

        case M_s:
            val->Info.mode = *pc++;
            val->Info.number = (int) *pc++;

            val->Left  = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_L:
            val->Info.mode = *pc++;

            val->Left  = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_O:
            val->Info.mode = *pc++;
            val->Info.number = (int) *pc++;

            val->Left  = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_R:
            val->Info.mode = *pc++;

            val->Left  = get_tree(funcs);
            val->Right = NULL;

            assert(*pc == '}');
            pc++;

            return val;

        case M_N:
            val->Info.mode = *pc++;
            val->Info.number = *((int*) pc);

            pc += 4;

            val->Left = NULL;
            val->Right = NULL;

            //printf("aaa %d\n", *pc);
            //printf("kol %d\n", CounT);
            assert(*pc == '}');
            pc++;

            return val;

        case M_P:
            val->Info.mode = *pc++;

            val->Left = get_tree(funcs);
            val->Right = get_tree(funcs);

            assert(*pc == '}');
            pc++;

            return val;

        case M_i:
            val->Info.mode = *pc++;

            tmp = pc;
            pc = strchr(pc, '}');

            *pc = '\0';
            val->Info.name = strdup(tmp);


            funcs[pt1].var[pt2].name_var = val->Info.name;
            funcs[pt1].var[pt2].place_var = ++kpert;
            funcs[pt1].var_num++;
            pt2++;


            *pc = '}';

            val->Right = NULL;
            val->Left  = NULL;

            pc++;

            return val;

        case M_V:
            val->Info.mode = *pc++;

            tmp = pc;
            pc = strchr(pc, '}');

            *pc = '\0';
            val->Info.name = strdup(tmp);

            *pc = '}';

            pc++;

            return val;

        case '}':
            free(val);
            CounT--;

            pc++;
            return NULL;
    }
}

void std_tree_dot(tree* nameTree, char* dot_out) {
    assert(dot_out);

    FILE *file_dot = fopen("dott.dot", "wb");
    assert(file_dot);

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\n");

    create_std_tree(nameTree->main_node, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

    system("dot -Tjpg -o tree.jpg dott.dot");
    system("xviewer tree.jpg");

}

int tmp = 0;
void create_std_tree(Node* pos, FILE* dot_out) {
    assert(dot_out);
    assert(pos);

    fprintf(dot_out, "\t\they%d", tmp);
    print_std_mode(pos, dot_out);
    fprintf(dot_out, ";\n");

    int iskan = tmp;

    if (pos->Left != NULL) {
        fprintf(dot_out, "\t\they%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d;\n", ++tmp);

        create_std_tree(pos->Left, dot_out);
    }

    if (pos->Right != NULL) {
        fprintf(dot_out, "\t\they%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d;\n", ++tmp);

        create_std_tree(pos->Right, dot_out);
    }

    assert(pos);
    assert(dot_out);
}

void print_std_mode(Node* pos, FILE* dot_out) {
    assert(pos);
    assert(dot_out);

    switch ((pos->Info).mode) {
        case M_B:        fprintf(dot_out, "[label = \"B\", shape = \"Mcircle\", "
                                          "color=\"#000000\", style=\"filled\", "
                                          "fillcolor = \"#32CD32\"]");
            break;

        case M_s:      fprintf(dot_out, "[label = \"s %c\", shape = \"diamond\", "
                                        "color=\"#000000\", style=\"filled\", "
                                        "fillcolor = \"#F0E68C\"]", (pos->Info).number);
            break;

        case M_O:       fprintf(dot_out, "[label = \"O %c\", shape = \"component\", "
                                         "color=\"#000000\", style=\"filled\", "
                                         "fillcolor = \"#9932CC\"]", (pos->Info).number);
            break;

        case M_I:    fprintf(dot_out, "[label = \"I\", shape = \"Msquare\", "
                                      "color=\"#000000\", style=\"filled\", "
                                      "fillcolor = \"#FFA07A\"]");
            break;

        case M_i:      fprintf(dot_out, "[label = \"i %s\", shape = \"invtriangle\", "
                                        "color=\"#000000\", style=\"filled\", "
                                        "fillcolor = \"#00FFFF\"]", (pos->Info).name);
            break;

        case M_N: fprintf(dot_out, "[label = \"N %d\", shape = \"egg\", "
                                   "color=\"#000000\", style=\"filled\", "
                                   "fillcolor = \"#FFFFFF\"]", pos->Info.number);
            break;

        case M_R:      fprintf(dot_out, "[label = \"R\", shape = \"house\", "
                                        "color=\"#000000\", style=\"filled\", "
                                        "fillcolor = \"#D2691E\"]");
            break;

        case M_V:        fprintf(dot_out, "[label = \"V %s\", shape = \"house\", "
                                          "color=\"#000000\", style=\"filled\", "
                                          "fillcolor = \"#ff6161\"]", pos->Info.name);
            break;

        case M_L:       fprintf(dot_out, "[label = \"L\", shape = \"box3d\", "
                                         "color=\"#000000\", style=\"filled\", "
                                         "fillcolor = \"#32CD32\"]");
            break;

        case M_S:        fprintf(dot_out, "[label = \"S %d\", shape = \"box3d\", "
                                          "color=\"#000000\", style=\"filled\", "
                                          "fillcolor = \"#FFDEAD\"]", pos->Info.number);
            break;

        case M_C:    fprintf(dot_out, "[label = \"C %s\", shape = \"box3d\", "
                                      "color=\"#000000\", style=\"filled\", "
                                      "fillcolor = \"#708090\"]", pos->Info.name);
            break;

        case M_P:      fprintf(dot_out, "[label = \"P\", shape = \"box3d\", "
                                        "color=\"#000000\", style=\"filled\", "
                                        "fillcolor = \"#EE82EE\"]");
            break;

        case M_e:    fprintf(dot_out, "[label = \"E\", shape = \"parallelogram\", "
                                      "color=\"#000000\", style=\"filled\", "
                                      "fillcolor = \"#00b34a\"]");
            break;

        default:             fprintf(dot_out, "\"error\"");
            break;
    }

    assert(pos);
    assert(dot_out);
}


//======================================================================

unsigned int Len_file(FILE *file_in)
{
    assert(file_in);

    fseek(file_in, 0, SEEK_END);
    unsigned int kol_simvols = ftell(file_in);
    fseek(file_in, 0, SEEK_SET);

    return kol_simvols;
}




char* pos = NULL;


void Translation(Node* curr_node, Variables* buf_of_vars, Function* buf_of_func)
{
    if (curr_node == NULL)
        return;

    switch (curr_node->Info.mode)
    {
        case M_s: // sign +-*/
        {
            Translation(curr_node->Right, buf_of_vars, buf_of_func);
            Translation(curr_node->Left, buf_of_vars, buf_of_func);

            switch (curr_node->Info.number)
            {
                case '+':
                    add();
                    break;
                case '-':
                    sub();
                    break;
                case '*':
                    mul();
                    break;
                case '/':
                    division();
                    break;

                default:
                    printf("Not +-/*\n");
            }
            break;
        }

        case M_B: // связка body
        {
            Translation(curr_node->Left, buf_of_vars, buf_of_func);
            Translation(curr_node->Right, buf_of_vars, buf_of_func);
            break;
        }

        case M_O: // cmp plus sign > < ==
        {
            Translation(curr_node->Left, buf_of_vars, buf_of_func);
            Translation(curr_node->Left, buf_of_vars, buf_of_func);

            cmp();

            switch (curr_node->Info.number)
            {
                case '=':
                {
                    *pos++ = 0x0f;
                    *pos++ = 0x85;
                    break;
                }
                case  '>':
                {
                    *pos++ = 0x0f;
                    *pos++ = 0x86;
                    break;
                }
                case '<':
                {
                    *pos++ = 0x0f;
                    *pos++ = 0x83;
                    break;
                }
                default:
                    printf("Not > < ==\n");
            }
            pos++;
            break;
        }

        case M_I: // IIIIffff with otnositel jmp
        {
            // body of if in right node
            Translation(curr_node->Left, buf_of_vars, buf_of_func);
            char* jump_pos = pos;

            pos += 4;

            Translation(curr_node->Right, buf_of_vars, buf_of_func);
            int offset = pos - jump_pos - 4;
            *((int *) jump_pos) = offset;
            break;
        }

        case M_R: // ??????
        {
            Translation(curr_node->Left, buf_of_vars, buf_of_func);
            *pos = 0x58;
            aligh_stack(buf_of_vars);
            *pos++ = 0x5d;
            *pos++ = 0xc3;
            break;
        }

        case M_V: //????
        {
            int offset = find_variable(curr_node->Info.name, buf_of_vars);
            take_from_memory(offset);
            break;
        }

        case M_S:
        {
            if (curr_node->Info.number == 0)
            {
                scan();
                Translation(curr_node->Left, buf_of_vars, buf_of_func);
            } else if (curr_node->Info.number == 1)
            {
                Translation(curr_node->Left, buf_of_vars, buf_of_func);
                print();
            }
            break;
        }

        case M_N:
        {
            num(curr_node->Info.number);
            break;
        }

        case M_i: // this all realisation!)
        {
            break;
        }

        case M_e:

            Translation(curr_node->Right, buf_of_vars, buf_of_func);
            *pos++ = 0x58;

            push_to_memory(curr_node->Left->Info.name, buf_of_vars);
            break;

        case M_P:
            Translation(curr_node->Left, buf_of_vars, buf_of_func);
            Translation(curr_node->Right, buf_of_vars, buf_of_func);
            break;

        case M_C:
        {
            if (curr_node->Right != NULL)
                Translation(curr_node->Right, buf_of_vars, buf_of_func);

            save_registers(buf_of_vars);

            *pos++ = 0xe8;
            *((int*) pos) = find_add_func(curr_node->Info.name, buf_of_func) - (pos + 4);
            pos += 4;
            return_registers(buf_of_vars);
            *pos++ = 0x50;
            break;
        }

        case M_L:
        {
            break;
        }

        default:
            printf("I do not write this case\n");
    }

}



int find_variable(char* name, Variables* buf_of_vars)
{
    int index = 0;
    while (buf_of_vars[index].name_var != NULL)
    {
        if (!(strcmp(buf_of_vars[index].name_var, name)))
            return  buf_of_vars[index].place_var;
        index++;
    }
    printf("do not find var %s\n", name);
    abort();

}



void create_elf_with_nope(Function* funcs)
{
    FILE* nope_file = fopen("nope_file", "rb");

    unsigned int num_symbols = Len_file(nope_file);

    char* buffer = (char*)calloc(num_symbols, sizeof(char));

    fread(buffer, sizeof(char), num_symbols, nope_file);
    fclose(nope_file);

    pos = buffer;

    while (*((unsigned char*) pos) != 0x90) pos++; // for first nope

    char* save_pos = pos;

    //print();

    int new_index = 0;
    while (funcs[new_index].tree_func) {
        *pos++ = 0x55;  // push rbp

        *pos++ = 0x48;  // mov rbp, rsp
        *pos++ = 0x89;  //
        *pos++ = 0xe5;  //

        Compile(&(funcs[new_index]), funcs, &pos);
        new_index++;

        *pos++ = 0x5d;  // pop rbp
        *pos++ = 0x90;  // nop
        *pos++ = 0x90;
    }

    pos = save_pos;
    new_index = 0;

    while (funcs[new_index].tree_func) {
        *pos++ = 0x55;  // push rbp

        *pos++ = 0x48;  // mov rbp, rsp
        *pos++ = 0x89;  //
        *pos++ = 0xe5;  //

        Compile(&(funcs[new_index]), funcs, &pos);
        new_index++;

        *pos++ = 0x5d;  // pop rbp
        *pos++ = 0x90;  // nop
        *pos++ = 0x90;
    }

    FILE* my_elf_file = fopen("my_elf_file", "wb");
    fwrite(buffer, sizeof(char), num_symbols, my_elf_file);
    fclose(my_elf_file);

    free(buffer);


}



void push_param(int counter)
{
    if (counter > 6) {
        return;
    }

    else if (counter == 1)
        *(pos++) = 0x5f;
    else if (counter == 2)
        *(pos++) = 0x5e;
    else if (counter == 3)
        *(pos++) = 0x5a;
    else if (counter == 4)
        *(pos++) = 0x59;
    else if (counter == 5)
    {
        *(pos++) = 0x41;
        *(pos++) = 0x58;
    } else if (counter == 6)
    {
        *(pos++) = 0x41;
        *(pos++) = 0x59;
    }
}

void aligh_stack(Variables* arr_of_vars) {
    int i = 0;
    int counter = 0;
    while (arr_of_vars[i].name_var != NULL) {
        if (arr_of_vars[i].place_var < 0)
            counter++;
        i++;
    }

    *(pos++) = 0x48;
    *(pos++) = 0x83;
    *(pos++) = 0xc4;
    *(pos++) = counter * 8;
}


void cut_stack(Variables* arr_of_vars) {
    int i = 0;
    int counter = 0;
    while (arr_of_vars[i].name_var != NULL) {
        if (arr_of_vars[i].place_var < 0)
            counter++;
        i++;
    }

    *pos++ = 0x48;
    *(pos++) = 0x83;
    *(pos++) = 0xec;
    *(pos++) = counter * 8;
}


void transform_var_table(Function* curr_func) {
    int i = 0;
    while (curr_func->var[i].name_var != NULL) {
        if (i < curr_func->given_num)
            curr_func->var[i].place_var = i + 1;
        else
            curr_func->var[i].place_var = -((curr_func->var[i].place_var - curr_func->given_num) * 8);

        i++;
    }
}



size_t Compile(Function* curr_func, Function* func_table, char** start_pos)
{
    assert(curr_func != NULL);
    assert(func_table != NULL);

    pos = *start_pos;

    transform_var_table(curr_func);
    push_func_addr(curr_func->name, func_table, start_pos);
    Translation(curr_func->tree_func->main_node, curr_func->var, func_table);

    *start_pos = pos;

    return pos - *start_pos;
}

char* find_add_func(char* name_func, Function* arr_of_func)
{
    int i = 0;
    while (arr_of_func[i].name != NULL) {
        if (!strcmp(name_func, arr_of_func[i].name))
            return arr_of_func[i].sdvig;
        i++;
    }
    //printf("I can't find this func - %s", name_func);
    //abort();
}


void push_func_addr(char* name, Function* func_table, char* start_pos) {
    int i = 0;

    start_pos = (unsigned char*) start_pos;

    while (func_table[i].name != NULL) {
        if (!strcmp(name, func_table[i].name)) {
            func_table[i].sdvig = pos - start_pos;
            break;
        }
        i++;
    }
}


void take_from_memory(int offset) {
    if (offset <= 0) {
        *(pos++) = 0x48;
        *(pos++) = 0x8b;
        *(pos++) = 0x45;
        *(pos++) = 0x00 + (char) offset;
        *(pos++) = 0x50;
    } else
        {
        if (offset < 5) {
            *(pos++) = 0x48;
            *(pos++) = 0x89;
            switch (offset) {
                case 1:
                    *(pos++) = 0xf8;
                    break;
                case 2:
                    *(pos++) = 0xf0;
                    break;
                case 3:
                    *(pos++) = 0xc8;
                    break;
                case 4:
                    *(pos++) = 0xd0;
                    break;
            }
        } else if (offset == 5) {
            *(pos++) = 0x4c;
            *(pos++) = 0x89;
            *(pos++) = 0xc0;
        } else if (offset == 6) {
            *(pos++) = 0x4c;
            *(pos++) = 0x89;
            *(pos++) = 0xc8;
        }
        *(pos++) = 0x50;
    }
}


void push_to_memory(char* name, Variables* arr_of_vars)
{
    int offset = find_variable(name, arr_of_vars);

    if (offset <= 0) {
        *(pos++) = 0x48; // mov [rbp + offset], rax
        *(pos++) = 0x89; //
        *(pos++) = 0x45; //
        *(pos++) = 0x00 + (char) offset;
    } else
    {
        if (offset < 5)
        {
            *(pos++) = 0x48;
            *(pos++) = 0x89;

            switch (offset)
            {
                case 1:
                    *(pos++) = 0xc7;
                    break;
                case 2:
                    *(pos++) = 0xc6;
                    break;
                case 3:
                    *(pos++) = 0xc2;
                    break;
                case 4:
                    *(pos++) = 0xc1;
                    break;
            }
        } else if (offset == 5)
        {
            *(pos++) = 0x49;
            *(pos++) = 0x89;
            *(pos++) = 0xc0;
        } else if (offset == 6)
        {
            *(pos++) = 0x49;
            *(pos++) = 0x89;
            *(pos++) = 0xc1;
        }
        pos++;
    }

}


void save_registers(Variables* arr_of_func)
{
    int i = 0;
    while (arr_of_func[i].name_var != NULL)
    {
        if (arr_of_func[i].place_var > 0)
        {
            switch (arr_of_func[i].place_var)
            {
                case 1:
                    *pos = 0x57;
                    pos++;
                    break;
                case 2:
                    *pos = 0x56;
                    pos++;
                    break;
                case 3:
                    *pos = 0x52;
                    pos++;
                    break;
                case 4:
                    *pos = 0x51;
                    pos++;
                    break;
                case 5:
                    *pos = 0x41;
                    pos++;
                    *pos = 0x50;
                    pos++;
                    break;
                case 6:
                    *pos = 0x41;
                    pos++;
                    *pos = 0x51;
                    pos++;
                    break;
            }
        }
        i++;
    }
}


void return_registers(Variables* arr_of_func)
{
    int i = 0;
    while (arr_of_func[i].name_var != NULL)
    {
        if (arr_of_func[i].place_var > 0)
        {
            switch (arr_of_func[i].place_var)
            {
                case 1:
                    *pos = 0x5f;
                    pos++;
                    break;
                case 2:
                    *pos = 0x5e;
                    pos++;
                    break;
                case 3:
                    *pos = 0x5a;
                    pos++;
                    break;
                case 4:
                    *pos = 0x59;
                    pos++;
                    break;
                case 5:
                    *pos = 0x41;
                    pos++;
                    *pos = 0x58;
                    pos++;
                    break;
                case 6:
                    *pos = 0x41;
                    pos++;
                    *pos = 0x59;
                    pos++;
                    break;
            }
        }
        i++;
    }
}


// operators
void add() {
    *pos++ = 0x58;// pop rax, pop rbx
    *pos++ = 0x5b;

    *pos++ = 0x48;// add rax, rbx
    *pos++ = 0x01;
    *pos++ = 0xd8;

    *pos++ = 0x50;// push rax

    //pos++;
}

void division() {
    *pos++ = 0x58;// pop rax, pop rbx
    *pos++ = 0x5b;

    *pos++ = 0x52;// push rdx
    *pos++ = 0x48;// xor rdx, rdx
    *pos++ = 0x31;
    *pos++ = 0xd2;
    *pos++ = 0x48;// div rbx
    *pos++ = 0xf7;
    *pos++ = 0xf3;
    *pos++ = 0x5a;// pop rdx
    *pos++ = 0x50;// push rax
}

void cmp() {
    *pos++ = 0x58; // pop rax
    *pos++ = 0x5b; // pop rbx

    *pos++ = 0x48; // cmp rax, rbx
    *pos++ = 0x39;
    *pos++ = 0xd8;

}

void sub() {
    *pos++ = 0x58; // pop rax
    *pos++ = 0x5b; // pop rbx

    *pos++ = 0x48; // sub rax, rbx
    *pos++ = 0x29;
    *pos++ = 0xd8;

    *pos++ = 0x50; // push rax
}

void mul() {
    *pos++ = 0x58; // pop rax
    *pos++ = 0x5b; // pop rbx

    *pos++ = 0x48; // mul rbx
    *pos++ = 0xf7; //
    *pos++ = 0xe3; //

    *pos++ = 0x50; // push rax
}

void num(int number) {
    *pos++ = 0xb8;          // mov rax, number
    *((int*) pos) = number; //
    pos += 4;               //

    *pos++ = 0x50; // push rax
}

void scan() {   // scanf
    *pos++ = 0x41;
    *pos++ = 0x53;
    *pos++ = 0x56;
    *pos++ = 0x57;
    *pos++ = 0x52;
    *pos++ = 0x51;
    *pos++ = 0x48;
    *pos++ = 0x83;
    *pos++ = 0xec;
    *pos++ = 0x10;
    *pos++ = 0xb8;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x48;
    *pos++ = 0x89;
    *pos++ = 0xe6;
    *pos++ = 0xba;
    *pos++ = 0x0a;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0xbf;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x0f;
    *pos++ = 0x05;
    *pos++ = 0x48;
    *pos++ = 0x89;
    *pos++ = 0xc1;
    *pos++ = 0x48;
    *pos++ = 0xff;
    *pos++ = 0xc9;
    *pos++ = 0x48;
    *pos++ = 0x31;
    *pos++ = 0xc0;
    *pos++ = 0x41;
    *pos++ = 0xbb;
    *pos++ = 0x0a;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x48;
    *pos++ = 0x31;
    *pos++ = 0xf6;
    *pos++ = 0x8a;
    *pos++ = 0x1c;
    *pos++ = 0x34;
    *pos++ = 0x80;
    *pos++ = 0xeb;
    *pos++ = 0x30;
    *pos++ = 0x49;
    *pos++ = 0xf7;
    *pos++ = 0xe3;
    *pos++ = 0x48;
    *pos++ = 0x01;
    *pos++ = 0xd8;
    *pos++ = 0x48;
    *pos++ = 0xff;
    *pos++ = 0xc6;
    *pos++ = 0xe2;
    *pos++ = 0xef;
    *pos++ = 0x48;
    *pos++ = 0x83;
    *pos++ = 0xc4;
    *pos++ = 0x10;
    *pos++ = 0x59;
    *pos++ = 0x5a;
    *pos++ = 0x5f;
    *pos++ = 0x5e;
    *pos++ = 0x41;
    *pos++ = 0x5b;
    *pos++ = 0x50;
}

void print() {     // from my_print
/*    *pos++ = 0xb8;
    *pos++ = 0x34;
    *pos++ = 0xb1;
    *pos++ = 0x01;
    *pos++ = 0x00;
    *pos++ = 0x50;
*/
    *pos++ = 0x55;
    *pos++ = 0x48;
    *pos++ = 0x89;
    *pos++ = 0xe5;
    *pos++ = 0x57;
    *pos++ = 0x56;
    *pos++ = 0x51;
    *pos++ = 0x52;
    *pos++ = 0x41;
    *pos++ = 0x53;
    *pos++ = 0x48;
    *pos++ = 0x83;
    *pos++ = 0xec;
    *pos++ = 0x10;
    *pos++ = 0x48;
    *pos++ = 0x8b;
    *pos++ = 0x45;
    *pos++ = 0x08;
    *pos++ = 0x48;
    *pos++ = 0x31;
    *pos++ = 0xd2;
    *pos++ = 0x48;
    *pos++ = 0x39;
    *pos++ = 0xd0;
    *pos++ = 0x75;
    *pos++ = 0x0c;
    *pos++ = 0xb2;
    *pos++ = 0x30;
    *pos++ = 0x88;
    *pos++ = 0x14;
    *pos++ = 0x24;
    *pos++ = 0xb8;
    *pos++ = 0x01;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0xeb;
    *pos++ = 0x3d;
    *pos++ = 0x48;
    *pos++ = 0x31;
    *pos++ = 0xff;
    *pos++ = 0xbb;
    *pos++ = 0x0a;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x48;
    *pos++ = 0xff;
    *pos++ = 0xc7;
    *pos++ = 0x48;
    *pos++ = 0x31;
    *pos++ = 0xd2;
    *pos++ = 0x48;
    *pos++ = 0xf7;
    *pos++ = 0xf3;
    *pos++ = 0x48;
    *pos++ = 0x83;
    *pos++ = 0xf8;
    *pos++ = 0x00;
    *pos++ = 0x75;
    *pos++ = 0xf1;
    *pos++ = 0x48;
    *pos++ = 0x89;
    *pos++ = 0xf9;
    *pos++ = 0x48;
    *pos++ = 0x8b;
    *pos++ = 0x45;
    *pos++ = 0x08;
    *pos++ = 0x48;
    *pos++ = 0xff;
    *pos++ = 0xcf;
    *pos++ = 0x48;
    *pos++ = 0x31;
    *pos++ = 0xd2;
    *pos++ = 0x48;
    *pos++ = 0xf7;
    *pos++ = 0xf3;
    *pos++ = 0x48;
    *pos++ = 0x83;
    *pos++ = 0xc2;
    *pos++ = 0x30;
    *pos++ = 0x88;
    *pos++ = 0x14;
    *pos++ = 0x3c;
    *pos++ = 0x48;
    *pos++ = 0x83;
    *pos++ = 0xff;
    *pos++ = 0x00;
    *pos++ = 0x75;
    *pos++ = 0xea;
    *pos++ = 0xb8;
    *pos++ = 0x0a;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x48;
    *pos++ = 0x89;
    *pos++ = 0x04;
    *pos++ = 0x0c;
    *pos++ = 0xb8;
    *pos++ = 0x01;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x48;
    *pos++ = 0x89;
    *pos++ = 0xe6;
    *pos++ = 0xba;
    *pos++ = 0x0a;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0xbf;
    *pos++ = 0x01;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x00;
    *pos++ = 0x0f;
    *pos++ = 0x05;
    *pos++ = 0x41;
    *pos++ = 0x5b;
    *pos++ = 0x5a;
    *pos++ = 0x59;
    *pos++ = 0x5e;
    *pos++ = 0x5f;
}




int main()
{
    FILE* str_tree = fopen("str_tree.txt", "rb");

    unsigned int num_symbols = Len_file(str_tree);

    char* buffer = (char*)calloc(num_symbols + 1, sizeof(char));

    fread(buffer, sizeof(char), num_symbols, str_tree);
    fclose(str_tree);


    Function funcs[10] = {};

    make_std(buffer, funcs);
    for (int j = 0; j < 10; j++)
        if (funcs[j].tree_func)
            std_tree_dot(funcs[j].tree_func, "str_tree_dot");

    create_elf_with_nope(funcs);

    for (int j = 0; j < 10; j++)
        if (funcs[j].tree_func) FunctionDtor(&(funcs[j]));


    free(buffer);

    return 0;

}