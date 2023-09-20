// Given a CFG which is LL(1) as an input file where each line has exactly one production rule,
// Write a C program to compute and print the LL(1) parsing table.

// CFG in input file is in the following format:
// V = S$
// S = aAS
// S = c
// A = ba
// A = SB
// B = bA
// B = S

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char non_terminal;
    char production[MAX][MAX];
    int num_productions;
} Rule;

typedef struct {
    char start_symbol;
    Rule rules[MAX];
    int rule_count;
    char non_terminals[MAX];
    char terminals[MAX];
} CFG;

void print_table(Rule table[][MAX], CFG cfg)
{
    printf("LL(1) Parsing Table:\n");
    printf("\t\t");
    for (int i = 0; i < strlen(cfg.terminals); i++)
    {
        printf("%c\t\t", cfg.terminals[i]);
    }
    printf("\n");
    for (int i = 0; i < strlen(cfg.non_terminals); i++)
    {
        printf("%c\t\t", cfg.non_terminals[i]);
        for (int j = 0; j < strlen(cfg.terminals); j++)
        {
            if (strlen(table[i][j].production[0]) == 0)
            {
                printf("\t\t");
            }
            else
            {
                printf("%c->%s\t\t",cfg.non_terminals[i], table[i][j].production[0]);
            }
        }
        printf("\n");
    }
}

char *next_terms(CFG *cfg, char *prod)
{
    char next[MAX];
    memset(next, 0, sizeof(next));
    
    if (isupper(prod[0]))
    {
        int n = strchr(cfg->non_terminals, prod[0]) - cfg->non_terminals;
        for (int i = 0; i < cfg->rules[n].num_productions; i++)
        {
            strcat(next, next_terms(cfg, cfg->rules[n].production[i]));
        }
    }
    else
    {
        next[0] = prod[0];
    }

    char *ret = malloc(sizeof(next));
    strcpy(ret, next);
    return ret;
}

void ll1_table(CFG cfg)
{
    // Table
    int n_nt = strlen(cfg.non_terminals);
    int n_t = strlen(cfg.terminals);
    int n_r = cfg.rule_count;
    Rule table[n_nt][MAX];
    
    int total = 0;
    // For all rules with a non-terminal
    for (int i = 0; i < n_r; i++)
    {
        // For all productions of the rule
        for (int j = 0; j < cfg.rules[i].num_productions; j++)
        {
            char *next = next_terms(&cfg, cfg.rules[i].production[j]);
            for (int k = 0; k < strlen(next); k++)
            {
                int n = strchr(cfg.non_terminals, cfg.rules[i].non_terminal) - cfg.non_terminals;
                int m = strchr(cfg.terminals, next[k]) - cfg.terminals;
                strcpy(table[n][m].production[0], cfg.rules[i].production[j]);
            }
        }
    }

    print_table(table, cfg);

}



int main(int argc,char *argv[]){
    FILE *fp;
    char *filename = argv[1];
    fp = fopen(filename,"r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(0);
    }
    CFG cfg;
    char line[MAX];
    int num_nonterminals = 0;
    int num_terminals = 0;
    while(fgets(line,MAX,fp) != NULL){

        char *token = strtok(line," =\n");
        if(strchr(cfg.non_terminals,token[0]) == NULL){
            cfg.non_terminals[num_nonterminals] = token[0];
            cfg.rules[num_nonterminals].non_terminal = token[0];
            cfg.rules[num_nonterminals].num_productions = 0;
            num_nonterminals++;
        }
        int index = 0;
        for(int i = 0;i<num_nonterminals;i++){
            if(cfg.rules[i].non_terminal == token[0]){
                index = i;
                break;
            }
        }
    
        token = strtok(NULL," =\n");
        for(int i = 0;i<strlen(token);i++){
            if(!isupper(token[i]) && strchr(cfg.terminals,token[i]) == NULL){
                cfg.terminals[num_terminals] = token[i];
                num_terminals++;
            }
        }
        strcpy(cfg.rules[index].production[cfg.rules[index].num_productions],token);
        cfg.rules[index].num_productions++;
    
    }
    cfg.start_symbol = cfg.non_terminals[0];
    cfg.rule_count = num_nonterminals;

    printf("CFG:\n");
    printf("Start: %c\n", cfg.start_symbol);
    printf("Non-terminals: %s\n", cfg.non_terminals);
    printf("Terminals: %s\n", cfg.terminals);
    printf("Rules:\n");
    for (int i = 0; i < cfg.rule_count; i++)
    {
        printf("%c ->", cfg.rules[i].non_terminal);
        for (int j = 0; j < cfg.rules[i].num_productions; j++)
        {
            if (j != 0)
            {
                printf(" |");
            }
            printf(" %s ", cfg.rules[i].production[j]);
        }
        printf("\n");
    }
    printf("\n");

    ll1_table(cfg);

    return 0;


}
