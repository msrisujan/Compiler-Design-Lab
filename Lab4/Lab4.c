// Given an input string in the format (a+b+c)*$ as an argument, write a C program for recursive descent parsing and 
// identifying whether the given string is accepted / rejected by the following CFG given in attachments.
/* 
Context Free Grammar :
    S -> ABC
    A -> aA | a
    B -> bB | b
    C -> cC | c
*/

//Name: M.Sri Sujan
//Roll No: CS21B1081

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char token, string[100];
int i = 0;

void accept()
{
    printf("String accepted\n");
    exit(0);
}

void reject()
{
    printf("String rejected\n");
    exit(0);
}

void get_token()
{
    token = string[i++];
}

void S(), A(), B(), C();

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Provide string as an argument in command line!!\n");
        exit(1);
    }

    printf("Language : \n");
    printf("\tS -> ABC\n\tA -> aA | a\n\tB -> bB | b\n\tC -> cC | c\n\n");
    printf("Language : {a^n b^m c^k | n, m, k >= 1}\n");
    printf("String   : %s\n", argv[1]);

    strcpy(string, argv[1]);
    int l = strlen(string);

    string[l++] = '$';
    string[l] = '\0';

    S();
    if (token == '$')
        accept();
    else
        reject();

    return 0;
}

void S()
{
    get_token();
    A();
    B();
    C();
}

void A()
{
    switch (token)
    {
        case 'a':
            get_token();
            A();
            break;
        case 'b':
            return;
        default:
            reject();
    }
}

void B()
{
    switch (token)
    {
        case 'b':
            get_token();
            B();
            break;
        case 'c':
            return;
        default:
            reject();
    }
}

void C()
{
    switch (token)
    {
        case 'c':
            get_token();
            C();
            break;
    }
}