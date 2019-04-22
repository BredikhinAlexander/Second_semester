#include "List.c"


int hash_f1(char* word);
int hash_f2(char* word);
int hash_f3(char* word);
int hash_f4(char* word);
int hash_f5(char* word);
int GNU_hash(char* word);

void many_requests(List* buf_list, char** array, unsigned int n_strings);

unsigned int Len_file(FILE *file_in);
unsigned int calc_strings(char *mem);

int comparison_hash_fun(FILE* file_out, int num, int num_strings, char** array, List* buf_list);

int size_word(char* word);


void dot(List* root);


const int NUMBER_HASH = 2002;


int main()
{
    FILE* file_in = fopen("dict.txt", "r");

    if (file_in == NULL)
    {
        printf("Dict not found\n");
        return 1;
    }

    unsigned int size = Len_file(file_in);

    char* buf = (char*)calloc(size, sizeof(*buf));
    fread(buf, size, sizeof(char), file_in);

    unsigned int num_strings = calc_strings(buf);
    printf("%d\n", num_strings);

    char** array = (char**)calloc(num_strings + 1, sizeof(char*));

    array[0] = &buf[0];
    int index = 1;



    for (int i = 0; i < size; i++)
    {
        if (buf[i] == '\n')
        {
            array[index] = &buf[i+1];
            index++;

            buf[i] = '\0';
        }
    }


    List* buf_list = (List*) calloc(NUMBER_HASH, sizeof(List));


    for (int i = 0; i < NUMBER_HASH; i++)
    {
        list_ctor(&buf_list[i]);
    }


    for (int i = 0; i < num_strings; i++)
    {
        char* word = array[i];

        int hash = GNU_hash(word);

        add_back(&buf_list[hash], word);

    }

    //FILE* file_out = fopen("graph.txt", "w");

    //comparison_hash_fun(file_out, 5, num_strings, array, buf_list);




    char word1[] = "transposition";

    int el = GNU_hash(word1);
    printf("%d\n", el);


    List_Elem* slovo = find_val(&buf_list[el], word1);
    printf("%s\n", slovo->value);
    dot(&buf_list[el]);

    many_requests(buf_list, array, num_strings);

    return 0;
}










void many_requests(List* buf_list, char** array, unsigned int n_strings)
{
    printf("%d\n", n_strings);
    List_Elem* slovo = NULL;
    for (int iter = 0; iter < 2; iter++)
    {
        for (int i = 0; i < n_strings; i++)
        {
            char* word = array[i];
            int hash = GNU_hash(word);
            slovo = find_val(&buf_list[hash], word);
            if (slovo->value != NULL)
                printf("%d : %s\n", i, slovo->value);

        }
    }
}














int comparison_hash_fun(FILE* file_out, int num, int num_strings, char** array, List* buf_list)
{
    for (int i = 0; i < NUMBER_HASH; i++)
    {
        list_ctor(&buf_list[i]);

    }


    for (int i = 0; i < num_strings; i++)
    {
        char* word = array[i];

        int hash = 0;

        #define hash_f(n)  \
            if (num == n) \
                hash = hash_f##n(word);\


        #include "doc.h"
        #undef hash_f

        add_front(&buf_list[hash], word);
    }

    for (int i = 1; i < NUMBER_HASH ; i++)
    {
        printf("%d ", buf_list[i].count);

        fprintf(file_out, "%d\n", buf_list[i].count);

    }

    return 0;
}













unsigned int Len_file(FILE *file_in)
{
    assert(file_in);

    fseek(file_in, 0, SEEK_END);
    unsigned int kol_simvols = ftell(file_in);
    fseek(file_in, 0, SEEK_SET);

    return kol_simvols;
}

int size_word(char* word)
{
    int size = 0;
    while (word[size] != '\0')
    {
        size++;
    }
    return size;
}


unsigned int calc_strings(char *mem)
{
    assert(mem);

    unsigned int number_string = 0;
    int i = 0;

    while (mem[i] != '\0')
    {
        if (mem[i] == '\n')
            number_string++;

        i++;
    }
    return number_string;
}




//================== ---- hash_function ---- ===========================


int hash_f1(char* word)  // функция, которая всегда выдаёт одно и то же число
{
    return 10;
}

int hash_f2(char* word) // функция, возвращающая аски код первого символа слова
{
    return word[0];
}

int hash_f3(char* word) // функция, вшзвращающая сумму аски кодов слова
{
    int summ = 0;
    int len = size_word(word);

    for (int i = 0; i < len; i++)
    {
        summ += word[i];
    }
    return summ;
}


int hash_f4(char* word) // функция, возвращающая длину слова
{
    return size_word(word);
}

int hash_f5(char* word) // целое число, полученное при делении суммы аски кодов на длину слова
{
    int len = hash_f4(word);
    int result = 0;
    if (len != 0)
    {
        result = (hash_f3(word)/len);
    }


    return result;

}


int GNU_hash(char* word) //GNU_hash
{
    long long int sum = 0;
    int len = size_word(word);
    for (int i = 0; i < len; i++)
    {
        sum = ((sum << 5) + sum) + word[i];
    }
    int result = abs(sum%2001);

    return result;
}




//================== ---- dot ---- ===========================

void print_mode (List_Elem* cur_el, FILE* dot_out);
int tmp = 0;



void create_tree(List_Elem* cur_el, FILE* dot_out)
{
    fprintf(dot_out, "\t\they%d", tmp);
    print_mode(cur_el, dot_out);
    fprintf(dot_out, ";\n");

    int iskan = tmp;

    if (cur_el->next != NULL)
    {
        fprintf(dot_out, "\t\they%d", iskan);
        fprintf(dot_out, "->");
        fprintf(dot_out, "hey%d;\n", ++tmp);
        create_tree(cur_el->next, dot_out);
    }


}

void dot(List* root)
{
    FILE *file_dot = fopen("list.dot", "wb");
    assert(file_dot);

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\n");

    create_tree(root->head, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

}



void print_mode (List_Elem* cur_el, FILE* dot_out)
{
    fprintf(dot_out, "[label = \"%s\", shape = \"Mcircle\", "
                     "color=\"#006400\", style=\"filled\", "
                     "fillcolor = \"#32CD32\"]", cur_el->value);

}