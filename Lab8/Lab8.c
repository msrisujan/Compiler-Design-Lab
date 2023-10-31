#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Stack {
    int top;
    unsigned capacity;
    char* array;
};

struct Stack* createStack(unsigned capacity);
int isEmpty(struct Stack* stack);
void push(struct Stack* stack, char item);
char pop(struct Stack* stack);
char peek(struct Stack* stack);
int isOperator(char ch);
int precedence(char ch);
char * infixToPostfix(char* infix);
void threeAddressCodeGenrator(char * str);

int main() {
    char infix[100];
    
    printf("Enter an infix expression: ");
    scanf("%s", infix);
    
    
    char * str = infixToPostfix(infix);
    printf("%s\n", str);

    threeAddressCodeGenrator(str);

    return 0;
}


struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (char*)malloc(stack->capacity * sizeof(char));
    return stack;
}

int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

void push(struct Stack* stack, char item) {
    stack->array[++stack->top] = item;
}

char pop(struct Stack* stack) {
    if (!isEmpty(stack)) {
        return stack->array[stack->top--];
    }
    return '$'; 
}

char peek(struct Stack* stack) {
    if (!isEmpty(stack)) {
        return stack->array[stack->top];
    }
    return '$'; 
}

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

char* infixToPostfix(char* infix) {
    int i, j;
    struct Stack* stack = createStack(strlen(infix));
    if (!stack) {
        printf("Memory error\n");
        return NULL;
    }
    
    char* postfix = (char*)malloc((strlen(infix) + 1) * sizeof(char));
    if (!postfix) {
        printf("Memory error\n");
        free(stack);
        return NULL;
    }

    for (i = 0, j = 0; infix[i]; i++) {
        if (isalnum(infix[i]))
            postfix[j++] = infix[i];
        else if (infix[i] == '(')
            push(stack, infix[i]);
        else if (infix[i] == ')') {
            while (peek(stack) != '$' && peek(stack) != '(')
                postfix[j++] = pop(stack);
            if (peek(stack) == '(')
                pop(stack);
        } else {
            while (!isEmpty(stack) && precedence(peek(stack)) >= precedence(infix[i]))
                postfix[j++] = pop(stack);
            push(stack, infix[i]);
        }
    }

    while (!isEmpty(stack))
        postfix[j++] = pop(stack);

    postfix[j] = '\0';
    
    free(stack);
    
    return postfix;
}

void threeAddressCodeGenrator(char * str) {
    struct Stack * stk = createStack(strlen(str));
    int v = 0;

    for(int i=0;i<strlen(str);i++) {
        if(!isOperator(str[i])) {
            push(stk, str[i]);
        }
        else {
            v++;
            char c1 = pop(stk);
            char c2 = pop(stk);
            if((c1 >= 97 && c1 <= 122) && (c2 >= 97 && c2 <= 122)) {
                printf("t%d = %c %c %c \n", v, c2, str[i], c1);
            }
            else {
                if(c1 >= 97 && c1 <= 122) {
                    printf("t%d = t%c %c %c \n", v, c2, str[i], c1);
                }
                else if(c2 >= 97 && c2 <= 122) {
                    printf("t%d = %c %c t%c \n", v, c2, str[i], c1);
                }
                else {
                    printf("t%d = t%c %c t%c \n", v, c2, str[i], c1);
                }
            }
            v = v + '0';
            push(stk, v);
            v = v - '0';
        }
    }
}