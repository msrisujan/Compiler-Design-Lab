// Given a file name, Write a C program to take it as an argument and count the number of lines, words, and characters in it.
// Also count the number of characters that are 1. Vowels[a,e,i,o,u,A,E,I,O,U], 2. Consonants, 3. Digits[0-9] and 4. Other than these three.
// Print all of them on the terminal.
// Sum of 1st + 2nd + 3rd + 4th = No.of character

// For these you need to explore strtok function and also learn about ascii values accordingly for successful completion of today's lab program.


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Invalid number of arguments\n");
        return 0;
    }
    FILE *fp;
    char line[200];
    char ch;
    int linecount, wordcount, charcount, vowelcount, consonantcount, digitcount, othercount;
    linecount = wordcount = charcount = vowelcount = consonantcount = digitcount = othercount = 0;
    fp = fopen(argv[1], "r");
    while (fgets(line,200,fp) != NULL){
        linecount++;
        charcount += strlen(line);
        for(int i = 0; i < strlen(line); i++){
            if(isdigit(line[i])){
                digitcount++;
            }
            else if(isalpha(line[i])){
                if(line[i] == 'a' || line[i] == 'e' || line[i] == 'i' || line[i] == 'o' || line[i] == 'u' || line[i] == 'A' || line[i] == 'E' || line[i] == 'I' || line[i] == 'O' || line[i] == 'U'){
                    vowelcount++;
                }
                else{
                    consonantcount++;
                }
            }
            else{
                othercount++;
            }
        }
        char *token = strtok(line, " \n\t");
        while(token != NULL){
            wordcount++;
            token = strtok(NULL, " \n\t");
        }
    }
    fclose(fp);
    printf("Number of lines: %d\n", linecount);
    printf("Number of words: %d\n", wordcount);
    printf("Number of characters: %d\n", charcount);
    printf("Number of vowels: %d\n", vowelcount);
    printf("Number of consonants: %d\n", consonantcount);
    printf("Number of digits: %d\n", digitcount);
    printf("Number of other characters: %d\n", othercount);
    printf("Sum of 1,2,3,4 : %d\n", vowelcount + consonantcount + digitcount + othercount);
    return 0;

}