// 1. Given a file name, Write a C program to take it as an argument to count the number of tokens and detect the type of tokens.

// Tasks to be done.
// -Removing preprocessor directives, whitespaces, tab spaces, empty lines, and comments lines.
// -Identify the tokens and detect the tokens with their 6 types (
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_TOKEN_LENGTH 100
    char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while"
};
int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

typedef enum {
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    OPERATOR,
    SPECIAL_CHARACTER,
    STRING
} TokenType;

int isKeyword(char* token) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isOperator(char ch) {
    char operators[] = "+-*/%=<>&|^!~?:";
    for (int i = 0; i < strlen(operators); i++) {
        if (ch == operators[i]) {
            return 1;
        }
    }
    return 0;
}


int isSpecialCharacter(char ch) {
    char special_characters[] = "{}[]();,";
    for (int i = 0; i < strlen(special_characters); i++) {
        if (ch == special_characters[i]) {
            return 1;
        }
    }
    return 0;
}

int isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '#';
}

int isInteger(char *str){
    char qualifiers[] = "uUlL";
    int token_size = strlen(str);
    for(int i=0; i<token_size; i++){
        if(i==0){
            if(!isdigit(str[i])){
                return(0);
            }
        }
        else{
            if(!isdigit(str[i]) && !strchr(qualifiers, str[i])){
                return(0);
            }
        }
    }
    return(1);
}


int isHexOrOcta(char *str){
    int token_size = strlen(str);
    int state, check;
    char dhex[] = "0123456789ABCDEF";
    char doct[] = "01234567";
    char qualifiers[] = "uUlL";
    
    for(int i=0; i<token_size; i++){
        if(i==0){
            if(str[i] != '0'){
                return(0);
            }
        }
        else{
            if(i==1){
                if(str[i] == 'x' || str[i] == 'X'){
                    state = 1;
                }
                else if(strchr(doct, str[i])){
                    state = 2;
                }
                else{
                    return(0);
                }
            }
            else{
                if(state == 1){
                    if(i==2){
                        if(!strchr(dhex, str[i])){
                            return(0);
                        }
                    }
                    else{
                        if(!strchr(dhex, str[i]) && !strchr(qualifiers, str[i])){
                            return(0);
                        }
                    }
                }
                else{
                    if(!strchr(doct, str[i]) && !strchr(qualifiers, str[i])){
                            return(0);
                    }
                }
            }
        }
    }
    return(1);
}


int isReal(char *str){
    int token_size = strlen(str);
    int state = 0;
    char qualifiers[] = "fFlL";

    for(int i=0; i<token_size; i++){
        switch (state)
        {
        case 0: 
            if(isdigit(str[i])){
                state = 2;
            }
            else if(str[i]=='.'){
                state  = 1;
            }
            else{
                state = 4;
            }
            break;

        case 1:
            if(isdigit(str[i])){
                state = 3;
            }
            else{
                state = 4;
            }
            break;

        case 2:
            if(isdigit(str[i])){
                state = 2;
            }
            else if(str[i] == '.'){
                state = 3;
            }
            else if(str[i] == 'E' || str[i] == 'e'){
                state = 5;
            }
            else{
                state = 4;
            }
            break;
        case 3:
            if(isdigit(str[i])){
                state = 3;
            }
            else if(str[i] == 'E' || str[i] == 'e'){
                state = 5;
            }
            else{
                state = 4;
            }
            break;
        case 4:
            return(0);
        case 5:
            if(str[i] == '+' || str[i] == '-'){
                state = 6;
            }
            else if(isdigit(str[i])){
                state = 7;
            }
            else{
                state = 8;
            }
            break;
        case 6:
            if(isdigit(str[i])){
                state = 7;
            }
            else{
                state = 8;
            }
            break;
        case 7:
            if(isdigit(str[i])){
                state = 7;
            }
            else if(strchr(qualifiers, str[i])){
                state = 9;
            }
            else{
                state = 8;
            }
        case 8:
            return(0);
        case 9:
            if(str[i]){
                return(0);
            }
            break;
        default:
            break;
        }
        if (state == 4 || state == 8){
            return(0);
        }
    }
    return(1);
}


TokenType getTokenType(char *token){
    if(isKeyword(token)){
        return KEYWORD;
    }
    else if(isOperator(token[0])){
        return OPERATOR;
    }
    else if(isSpecialCharacter(token[0])){
        return SPECIAL_CHARACTER;
    }
    else if(isInteger(token) || isHexOrOcta(token) || isReal(token)){
        return CONSTANT;
    }
    else if(token[0]=='"' && token[strlen(token)-1]=='"'){
        return STRING;
    }
    else{
        return IDENTIFIER;
    }
}

int main(int argc,char *argv[]){
    if(argc!=2){
        printf("Wrong number of arguments\n");
        return 0;
    }
    FILE *fp;
    fp=fopen(argv[1],"r");
    if(fp==NULL){
        printf("Error opening file\n");
        return 0;
    }
    char line[MAX_LINE_LENGTH];
    char token[MAX_TOKEN_LENGTH];
    int state=0;
    int line_number=0;
    int keyword_count=0;
    int identifier_count=0;
    int constant_count=0;
    int operator_count=0;
    int special_character_count=0;
    int string_count=0;

    FILE *fp2;
    fp2=fopen("output.txt","w");

    while(fgets(line,MAX_LINE_LENGTH,fp)!=NULL){
        line_number++;
        int i=0;
        while(line[i]!='\0'){
            if(line[i]=='#'){
                break;
            }
            else if(line[i]=='/'){
                if(line[i+1]=='/'){
                    break;
                }
                else if(line[i+1]=='*'){
                    state=1;
                    i+=2;
                    continue;
                }
            }
            else if(state==1){
                if(line[i]=='*'){
                    if(line[i+1]=='/'){
                        state=0;
                        i+=2;
                        continue;
                    }
                }
                i++;
                continue;
            }
            else{
            if(isWhitespace(line[i])){
                i++;
                continue;
            }
            else if(isOperator(line[i])){
                int j=0;
                while(isOperator(line[i])){
                    token[j++]=line[i++];
                }
                token[j]='\0';
                fprintf(fp2,"Line %d: %s is an operator\n",line_number,token);
                operator_count++;
            }

            else if(isSpecialCharacter(line[i])){
                fprintf(fp2,"Line %d: %c is a special character\n",line_number,line[i]);
                i++;
                special_character_count++;
            }
            else if(line[i]=='"'){
                int j=1;
                i++;
                while(line[i]!='"'){
                    token[j++]=line[i++];
                }
                token[0]='"';
                token[j]='"';
                token[j+1]='\0';
                fprintf(fp2,"Line %d: %s is a string\n",line_number,token);
                i++;
                string_count++;
            }
            else{
                int j=0;
                while(!isWhitespace(line[i]) && !isOperator(line[i]) && !isSpecialCharacter(line[i]) && line[i]!='\0'){
                    token[j++]=line[i++];
                }
                token[j]='\0';
                TokenType token_type=getTokenType(token);
                switch(token_type){
                    case KEYWORD:
                        fprintf(fp2,"Line %d: %s is a keyword\n",line_number,token);
                        keyword_count++;
                        break;
                    case IDENTIFIER:
                        fprintf(fp2,"Line %d: %s is an identifier\n",line_number,token);
                        identifier_count++;
                        break;
                    case CONSTANT:
                        fprintf(fp2,"Line %d: %s is a constant\n",line_number,token);
                        constant_count++;
                        break;
                    case OPERATOR:
                        fprintf(fp2,"Line %d: %s is an operator\n",line_number,token);
                        operator_count++;
                        break;
                    case SPECIAL_CHARACTER:
                        fprintf(fp2,"Line %d: %s is a special character\n",line_number,token);
                        special_character_count++;
                        break;
                    case STRING:
                        fprintf(fp2,"Line %d: %s is a string\n",line_number,token);
                        string_count++;
                        break;
                }
            }
        }
        }

    }
    printf("Total keywords: %d\n",keyword_count);
    printf("Total identifiers: %d\n",identifier_count);
    printf("Total constants: %d\n",constant_count);
    printf("Total operators: %d\n",operator_count);
    printf("Total special characters: %d\n",special_character_count);
    printf("Total strings: %d\n",string_count);
    int token_count=keyword_count+identifier_count+constant_count+operator_count+special_character_count+string_count;
    printf("Total tokens: %d\n",token_count);
}
