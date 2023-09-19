// Given a CFG which is LL(1) as an input file where each line has exactly one production rule,
// Write a C program to compute and print the LL(1) parsing table.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char non_terminal;
    char production[MAX];
} Rule;

typedef struct {
    char non_terminal;
    char first[MAX];
    char follow[MAX];
} NonTerminal;

typedef struct {
    char non_terminal;
    char terminal;
    char production[MAX];
} Table;

int numNonTerminals = 0;
char terminals[MAX] = "";

void compute_first(char non_terminal, Rule rules[], NonTerminal nonTerminals[]){
    // find production rule for non_terminal
    // search for non_terminal in the production rule

    for (int i = 0; i < numNonTerminals; i++) {
        if (rules[i].non_terminal == non_terminal) {
            for (int j = 0; j < strlen(rules[i].production); j++) {
                if (!isupper(rules[i].production[j])) {
                    if (strchr(nonTerminals[i].first, rules[i].production[j]) == NULL) {
                        strncat(nonTerminals[i].first, &rules[i].production[j], 1);
                    }
                    break;
                }
                else {
                    compute_first(rules[i].production[j], rules, nonTerminals);
                    for (int k = 0; k < numNonTerminals; k++) {
                        if (nonTerminals[k].non_terminal == rules[i].production[j]) {
                            for (int l = 0; l < strlen(nonTerminals[k].first); l++) {
                                if (strchr(nonTerminals[i].first, nonTerminals[k].first[l]) == NULL) {
                                    strncat(nonTerminals[i].first, &nonTerminals[k].first[l], 1);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
            
}
void compute_follow(char non_terminal, Rule rules[], NonTerminal nonTerminals[]){

    

}
void compute_table(Rule rules[], NonTerminal nonTerminals[], Table table[]);
void print_table(Table table[]);

int main() {
    Rule rules[MAX];
    NonTerminal nonTerminals[MAX];
    Table table[MAX];
    char non_terminal;
    int i, j;

    // Read the CFG from the input file
    FILE *fp = fopen("CFG", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(0);
    }

    // Read the CFG from the input file
    while (fscanf(fp, "%c = %s\n", &non_terminal, rules[numNonTerminals].production) != EOF) {
        rules[numNonTerminals].non_terminal = non_terminal;
        numNonTerminals++;
    }

    fclose(fp);

    // print the CFG
    printf("CFG:\n");
    for (i = 0; i < numNonTerminals; i++) {
        printf("%c = %s\n", rules[i].non_terminal, rules[i].production);
    }

    // // Compute the terminals
    // for (i = 0; i < numNonTerminals; i++) {
    //     for (j = 0; j < strlen(rules[i].production); j++) {
    //         if (!isupper(rules[i].production[j])) {
    //             if (strchr(terminals, rules[i].production[j]) == NULL) {
    //                 strncat(terminals, &rules[i].production[j], 1);
    //             }
    //         }
    //     }
    // }

    // // Print the terminals

    // printf("Terminals: %s\n", terminals);

    // // Compute the non-terminals
    // for (i = 0; i < numNonTerminals; i++) {
    //     if (strchr(terminals, rules[i].non_terminal) == NULL) {
    //         nonTerminals[i].non_terminal = rules[i].non_terminal;
    //     }
    // }

    // // Print the non-terminals
    // printf("Non-Terminals: ");
    // for (i = 0; i < numNonTerminals; i++) {
    //     if (strchr(terminals, rules[i].non_terminal) == NULL) {
    //         printf("%c ", nonTerminals[i].non_terminal);
    //     }
    // }

    // printf("%d",numNonTerminals);

    // printf("\n");
    

    // Compute the first set for each non-terminal
    for (i = 0; i < numNonTerminals; i++) {
        compute_first(rules[i].non_terminal, rules, nonTerminals);
    }

    //combine same non-terminal first sets and remove duplicates
    for (i = 0; i < numNonTerminals; i++) {
        for (j = 0; j < numNonTerminals; j++) {
            if (nonTerminals[i].non_terminal == nonTerminals[j].non_terminal) {
                for (int k = 0; k < strlen(nonTerminals[j].first); k++) {
                    if (strchr(nonTerminals[i].first, nonTerminals[j].first[k]) == NULL) {
                        strncat(nonTerminals[i].first, &nonTerminals[j].first[k], 1);
                    }
                }
            }
        }
    }

    // after combining first sets, remove duplicate non-terminals and their first sets
    for (i = 0; i < numNonTerminals; i++) {
        for (j = 0; j < numNonTerminals; j++) {
            if (nonTerminals[i].non_terminal == nonTerminals[j].non_terminal && i != j) {
                nonTerminals[j].non_terminal = '\0';
                strcpy(nonTerminals[j].first, "");
            }
        }
    }

    // print the first sets
    printf("First Sets:\n");
    for (i = 0; i < numNonTerminals; i++) {
        if (nonTerminals[i].non_terminal != '\0') {
            printf("%c = %s\n", nonTerminals[i].non_terminal, nonTerminals[i].first);
        }
    }
    

    // // Compute the follow set for each non-terminal
    // for (i = 0; i < numNonTerminals; i++) {
    //     compute_follow(rules[i].non_terminal, rules, nonTerminals);
    // }

    // // Compute the parsing table
    // compute_table(rules, nonTerminals, table);

    // // Print the parsing table
    // print_table(table);

    return 0;
}



