#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int precedence(char ch) {
    if (ch == '+' || ch == '-')
        return 1;
    else if (ch == '*' || ch == '/')
        return 2;
    return 0;
}

char* infixToPostfix(char infix[]){
    char stack[100];
    int top = -1;
    char * postfix = (char*)malloc(sizeof(char)*100);
    int j = 0;
    for(int i = 0; i < strlen(infix); i++){
        if(isalnum(infix[i])){
            postfix[j++] = infix[i];
        }
        else if(infix[i] == '('){
            stack[++top] = infix[i];
        }
        else if(infix[i] == ')'){
            while(stack[top] != '('){
                postfix[j++] = stack[top--];
            }
            top--;
        }
        else{
            while(top != -1 && precedence(stack[top]) >= precedence(infix[i])){
                postfix[j++] = stack[top--];
            }
            stack[++top] = infix[i];
        }
    }
    while(top != -1){
        postfix[j++] = stack[top--];
    }
    postfix[j] = '\0';
    return postfix;
}

void threeAddressCode(char postfix[]){
    char stack[100];
    int top = -1;
    int v = 0;
    for(int i = 0; i < strlen(postfix); i++){
        if(!isOperator(postfix[i])){
            stack[++top] = postfix[i];
        }
        else{
            v++;
            char c1 = stack[top--];
            char c2 = stack[top--];
            if((c1 >= 97 && c1 <= 122) && (c2 >= 97 && c2 <= 122)) {
                printf("t%d = %c %c %c \n", v, c2, postfix[i], c1);
            }
            else {
                if(c1 >= 97 && c1 <= 122) {
                    printf("t%d = t%c %c %c \n", v, c2, postfix[i], c1);
                }
                else if(c2 >= 97 && c2 <= 122) {
                    printf("t%d = %c %c t%c \n", v, c2, postfix[i], c1);
                }
                else {
                    printf("t%d = t%c %c t%c \n", v, c2, postfix[i], c1);
                }
            }
            v = v + '0';
            stack[++top] = v;
            v = v - '0';
        }
    }
}

int main(){
    char infix[100];
    printf("Enter infix expression: ");
    scanf("%s", infix);
    printf("Postfix expression: %s\n", infixToPostfix(infix));
    printf("Three address code:\n");
    threeAddressCode(infixToPostfix(infix));
    return 0;
}