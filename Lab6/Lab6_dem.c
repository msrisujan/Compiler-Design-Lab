// Given a CFG as an input file in which each line is having exactly one production rule and epsilon as #,
// Write a c program to compute the first and follow sets of both T & NT (Terminals & Non-terminals) in the given CFG. 
// NT-Capital letters, T-small letters).

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX 100


typedef struct {
    char non_terminal;
    char first[MAX];
    char follow[MAX];
} NonTerminal;

typedef struct {
    char terminal;
    char first[MAX];
    char follow[MAX];
} Terminal;

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

void strunion(char *str1, char *str2){
    for(int i = 0;i<strlen(str2);i++){
        if(strchr(str1,str2[i]) == NULL){
            strncat(str1,&str2[i],1);
        }
    }
}

void compute_firstset(NonTerminal non_terminals[], CFG cfg, char non_terminal){
    int index = 0;
    for(int i = 0;i<strlen(cfg.non_terminals);i++){
        if(cfg.non_terminals[i] == non_terminal){
            index = i;
            break;
        }
    }
    for(int i = 0;i<cfg.rules[index].num_productions;i++){
        if(cfg.rules[index].production[i][0] == '#'){
            if(strchr(non_terminals[index].first,'#') == NULL){
                strcat(non_terminals[index].first,"#");
            }
            continue;
        }
        if(!isupper(cfg.rules[index].production[i][0])){
            if(strchr(non_terminals[index].first,cfg.rules[index].production[i][0]) == NULL){
                strncat(non_terminals[index].first,&cfg.rules[index].production[i][0],1);
            }
        }
        else{
            int epsilon = 0;
            for(int j = 0;j<strlen(cfg.rules[index].production[i]);j++){
                if(!isupper(cfg.rules[index].production[i][j])){
                    if(strchr(non_terminals[index].first,cfg.rules[index].production[i][j]) == NULL){
                        strncat(non_terminals[index].first,&cfg.rules[index].production[i][j],1);
                    }
                    break;
                }
                else{
                    compute_firstset(non_terminals,cfg,cfg.rules[index].production[i][j]);
                    int index2 = 0;
                    for(int k = 0;k<strlen(cfg.non_terminals);k++){
                        if(cfg.non_terminals[k] == cfg.rules[index].production[i][j]){
                            index2 = k;
                            break;
                        }
                    }
                    for(int k = 0;k<strlen(non_terminals[index2].first);k++){
                        if(strchr(non_terminals[index].first,non_terminals[index2].first[k]) == NULL){
                            strncat(non_terminals[index].first,&non_terminals[index2].first[k],1);
                        }
                    }
                    if(strchr(non_terminals[index2].first,'#') == NULL){
                        epsilon = 0;
                        break;
                    }
                    else if(j == strlen(cfg.rules[index].production[i])-1){
                        epsilon = 1;
                        char temp[MAX];
                        memset(temp,0,sizeof(temp));
                        for(int k = 0;k<strlen(non_terminals[index].first);k++){
                            if(non_terminals[index].first[k] != '#'){
                                strncat(temp,&non_terminals[index].first[k],1);
                            }
                        }
                        strcpy(non_terminals[index].first,temp);
                        continue;
                    }
                    else{
                        char temp[MAX];
                        memset(temp,0,sizeof(temp));
                        for(int k = 0;k<strlen(non_terminals[index].first);k++){
                            if(non_terminals[index].first[k] != '#'){
                                strncat(temp,&non_terminals[index].first[k],1);
                            }
                        }
                        strcpy(non_terminals[index].first,temp);
                        continue;
                    }
                }
            }
            if(epsilon == 1){
                if(strchr(non_terminals[index].first,'#') == NULL){
                    strcat(non_terminals[index].first,"#");
                }
            }
        }
    }
}

void compute_followset(NonTerminal non_terminals[], CFG cfg, char character){
    int x =cfg.rule_count;
    for(int i = 0;i<x;i++){
        for(int j = 0;j < cfg.rules[i].num_productions; j++){
            for(int k = 0;k < strlen(cfg.rules[i].production[j]) ; k++){
                if(cfg.rules[i].production[j][k] == character){
                    int index1 = 0;
                    for(int l = 0;l<strlen(cfg.non_terminals);l++){
                        if(cfg.non_terminals[l] == character){
                            index1 = l;
                            break;
                        }
                    }
                    if(!isupper(cfg.rules[i].production[j][k+1])){
                        if(strchr(non_terminals[index1].follow,cfg.rules[i].production[j][k+1]) == NULL){
                            strncat(non_terminals[index1].follow,&cfg.rules[i].production[j][k+1],1);
                        }
                    }
                    else{
                        int epsilon = 0;
                        for(int l = k+1;l<strlen(cfg.rules[i].production[j]);l++){
                            if(!isupper(cfg.rules[i].production[j][l])){
                                if(strchr(non_terminals[index1].follow,cfg.rules[i].production[j][l]) == NULL){
                                    strncat(non_terminals[index1].follow,&cfg.rules[i].production[j][l],1);
                                }
                                break;
                            }
                            else{
                                int index = 0;
                                for(int m = 0;m<strlen(cfg.non_terminals);m++){
                                    if(cfg.non_terminals[m] == cfg.rules[i].production[j][l]){
                                        index = m;
                                        break;
                                    }
                                }
                                for(int m = 0;m<strlen(non_terminals[index].first);m++){
                                    if(strchr(non_terminals[index1].follow,non_terminals[index].first[m]) == NULL){
                                        strncat(non_terminals[index1].follow,&non_terminals[index].first[m],1);
                                    }
                                }
                                if(strchr(non_terminals[index].first,'#') == NULL){
                                    break;
                                }
                                else if(l == strlen(cfg.rules[i].production[j])-1){
                                    epsilon = 1;
                                    char temp[MAX];
                                    memset(temp,0,sizeof(temp));
                                    for(int m = 0;m<strlen(non_terminals[index1].follow);m++){
                                        if(non_terminals[index1].follow[m] != '#'){
                                            strncat(temp,&non_terminals[index1].follow[m],1);
                                        }
                                    }
                                    strcpy(non_terminals[index1].follow,temp);
                                    continue;
                                }
                                else{
                                    char temp[MAX];
                                    memset(temp,0,sizeof(temp));
                                    for(int m = 0;m<strlen(non_terminals[index1].follow);m++){
                                        if(non_terminals[index1].follow[m] != '#'){
                                            strncat(temp,&non_terminals[index1].follow[m],1);
                                        }
                                    }
                                    strcpy(non_terminals[index1].follow,temp);
                                    continue;
                                }

                            }
                        }
                        if(epsilon == 1){
                            compute_followset(non_terminals,cfg,cfg.rules[i].non_terminal);
                            int index = 0;
                            for(int m = 0;m<strlen(cfg.non_terminals);m++){
                                if(cfg.non_terminals[m] == cfg.rules[i].non_terminal){
                                    index = m;
                                    break;
                                }
                            }
                            for(int m = 0;m<strlen(non_terminals[index].follow);m++){
                                if(strchr(non_terminals[index1].follow,non_terminals[index].follow[m]) == NULL){
                                    strncat(non_terminals[index1].follow,&non_terminals[index].follow[m],1);
                                }
                            }
                        }
                    }
                    
                    
                }
            }
        }
    }
}



int main(){
    FILE *fp;
    fp = fopen("CFG", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(0);
    }
    CFG cfg;
    char line[MAX];
    NonTerminal non_terminals[MAX];
    Terminal terminals[MAX];
    int num_nonterminals = 0;
    int num_terminals = 0;
    while(fgets(line,MAX,fp) != NULL){

        char *token = strtok(line," =\n");
        if(strchr(cfg.non_terminals,token[0]) == NULL){
            cfg.non_terminals[num_nonterminals] = token[0];
            cfg.rules[num_nonterminals].non_terminal = token[0];
            cfg.rules[num_nonterminals].num_productions = 0;
            non_terminals[num_nonterminals].non_terminal = token[0];
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
                if(token[i] == '$'){
                    continue;
                }
                cfg.terminals[num_terminals] = token[i];
                terminals[num_terminals].terminal = token[i];
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

    for(int i = 0;i<num_nonterminals;i++){
        compute_firstset(non_terminals,cfg,cfg.non_terminals[i]);
    }
    printf("First Sets:\n");
    for(int i = 0;i<num_nonterminals;i++){
        printf("%c: %s\n",non_terminals[i].non_terminal,non_terminals[i].first);
    }

    // first set for terminals
    for(int i = 0;i<num_terminals;i++){
        terminals[i].first[0] = terminals[i].terminal;
    }
    for(int i = 0;i<num_terminals;i++){
        printf("%c: %s\n",terminals[i].terminal,terminals[i].first);
    }

    printf("\n");

    for(int i = 0;i<num_nonterminals;i++){
        compute_followset(non_terminals,cfg,non_terminals[i].non_terminal);
    }
    printf("Follow Sets:\n");
    for(int i = 0;i<num_nonterminals;i++){
        printf("%c: %s\n",non_terminals[i].non_terminal,non_terminals[i].follow);
    }
    

    return 0;
}