#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

unsigned int MAX_SIZE = 1000;
int smech = 85;

int file_size(FILE* file);
int hack_file(FILE* file_in, FILE* file_out);

int main ()
{
    FILE *file_in = fopen("C:/2_sem/Steck/PASSWORD.COM","rb");

    FILE *file_out = fopen("W:/PASSWORD_hack.COM","wb");

    assert(file_in);
    assert(file_out);

    if (file_in == nullptr)
    {
        printf("File is not exit\n");
        return 0;
    }

    if (file_size(file_in) > MAX_SIZE)
    {
        return 0;
    } else
        hack_file(file_in, file_out);

    fclose(file_in);
    fclose(file_out);

    return 0;

}

int file_size(FILE* file)
{
    assert(file);

    fseek(file, 0, 0);

    int count = ftell(file);

    fseek(file, 0, SEEK_SET);

    return count;
}

int hack_file(FILE* file_in, FILE* file_out)
{
    assert(file_in);
    assert(file_out);

    char* buf = (char*)calloc(MAX_SIZE, sizeof(char));

    fread(buf, MAX_SIZE, sizeof(char), file_in);

    if (buf[smech] == 67)
    {
        printf("alredy hack\n");
        return 0;
    } else
    {
        buf[smech] = 67;
        fwrite(buf, sizeof(char), MAX_SIZE, file_out);

        printf("programm is hack\n");

        free(buf);
    }
    return 0;
}
