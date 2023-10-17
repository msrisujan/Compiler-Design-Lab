#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct{
    char non_terminal;
    char production[MAX][MAX];
    char no_of_prod;
} Rule;

typedef struct{
    char start;
    char non_terminals[MAX];
    char terminals[MAX];
    Rule rules[MAX];
    int no_of_rules;
} CFG;

typedef struct{
    char symbol;
    char first[MAX];
    char follow[MAX];
} firstfollow;

CFG cfg;
firstfollow table[MAX];

int find_index(char non_terminal, int n_s){
    for(int i = 0; i < n_s; i++){
        if(table[i].symbol == non_terminal)
            return i;
    }
}

char* remove_epsilon(char first[]){
    char* string = (char*)malloc(sizeof(char)* MAX);
    int l = strlen(first), j = 0;
    for(int i = 0; i < l; i++)
    {
        if(first[i] != '#')
            string[j] = first[i];
        j++;
    }
    return string;
}

int sets_union(char* src, char* dest, int changed){
    int l = strlen(src);
    for(int i = 0; i < strlen(dest); i++)
    {
        if(strchr(src, dest[i]) == NULL)
        {
            src[l] = dest[i];
            changed = 1;
            l++;
        }
    }
    return changed;
}

int if_epsilon(char string[])
{
    for(int i = 0; i < strlen(string); i++)
        if(string[i] == '#')
            return 1;
    return 0;
}

void computefirst(){
    int n_nt = strlen(cfg.non_terminals);
    int n_t = strlen(cfg.terminals);
    int n_s = n_nt + n_t;

    for(int i = 0; i < n_t; i++){
        table[i].symbol = cfg.terminals[i];
        table[i].first[0] = cfg.terminals[i];
    }
    for(int i = n_t; i < n_s; i++){
        table[i].symbol = cfg.non_terminals[i - n_t];
        strcpy(table[i].first, "");
    }
    int changing = 1;
    while(changing){
        changing = 0;
        for(int i = 0; i < cfg.no_of_rules; i++){
            for(int j = 0; j < cfg.rules[i].no_of_prod; j++){
                char nonterminal = cfg.rules[i].non_terminal;
                char prod[MAX];
                strcpy(prod, cfg.rules[i].production[j]);
                
                int index = find_index(nonterminal, n_s);
                int k = 0;
                int x_index = find_index(prod[k], n_s);
                char *x_first_set = remove_epsilon(table[x_index].first);
                changing = sets_union(table[index].first, x_first_set, changing);
                int n = strlen(prod);
                while(if_epsilon(table[x_index].first) && k <= n - 2){
                    x_index = find_index(prod[k+1], n_s);
                    char *x_first_set = remove_epsilon(table[x_index].first);
                    changing = sets_union(table[index].first, x_first_set, changing);
                    k++;
                }
                if (k == n-1 && if_epsilon(table[x_index].first)){
                    char *epsilon = "#";
                    changing = sets_union(table[index].first, epsilon, changing);
                }
            }
        }
    }
    printf("First:\n");
    for(int i = 0; i < n_s; i++){
        printf("%c: { ", table[i].symbol);
        int n = strlen(table[i].first);
        for(int j = 0; j < n - 1; j++)
            printf("%c, ", table[i].first[j]);
        printf("%c }\n", table[i].first[n - 1]);
    }
}

void computefollow(){
    int n_nt = strlen(cfg.non_terminals);
    int n_t = strlen(cfg.terminals);
    int n_s = n_nt + n_t;

    for(int i = 0; i < n_t; i++){
        table[i].symbol = cfg.terminals[i];
        strcpy(table[i].follow, "");
    }
    for(int i = n_t; i < n_s; i++){
        table[i].symbol = cfg.non_terminals[i - n_t];
        strcpy(table[i].follow, "");
    }
    for(int i = 0; i < n_s; i++){
        if(table[i].symbol == cfg.start){
            table[i].follow[0] = '$';
        }
    }
    int changing = 1;
    while(changing){
        changing = 0;
        for(int i = 0; i < cfg.no_of_rules; i++){
            for(int j = 0; j < cfg.rules[i].no_of_prod; j++){
                char nonterminal = cfg.rules[i].non_terminal;
                char prod[MAX];
                strcpy(prod, cfg.rules[i].production[j]);
                int index = find_index(nonterminal, n_s);
                int x_index = find_index(prod[strlen(prod) - 1], n_s);
                changing = sets_union(table[x_index].follow, table[index].follow, changing);
                char *rest = table[x_index].follow;
                int n = strlen(prod);
                for(int k = n - 1; k >= 1; k--){
                    x_index = find_index(prod[k], n_s);
                    int x_i_index = find_index(prod[k - 1], n_s);
                    char *x_first_set = remove_epsilon(table[x_index].first);
                    if(if_epsilon(table[x_index].first)){
                        changing = sets_union(table[x_i_index].follow, x_first_set, changing);
                        changing = sets_union(table[x_i_index].follow, rest, changing);
                    }
                    else{
                        changing = sets_union(table[x_i_index].follow, x_first_set, changing);
                    }
                    rest = table[x_i_index].follow;
                }
            }
        }
    }
    printf("Follow:\n");
    for(int i = 0; i < n_s; i++){
        printf("%c: { ", table[i].symbol);
        int n = strlen(table[i].follow);
        for(int j = 0; j < n - 1; j++)
            printf("%c, ", table[i].follow[j]);
        printf("%c }\n", table[i].follow[n - 1]);
    }
}

int main(){
    FILE *fp;
    char str[MAX];
    int rules_c = 0;
    fp = fopen("CFG","r");

    while(fgets(str, MAX, fp)){
        
        char nonterminal;
        nonterminal = strtok(str, " =\n")[0];
        char prod[MAX];
        strcpy(prod, strtok(NULL, " =\n"));

        if(strchr(cfg.non_terminals,nonterminal) == NULL){
            cfg.non_terminals[strlen(cfg.non_terminals)] = nonterminal;
            cfg.rules[strlen(cfg.non_terminals) - 1].non_terminal = nonterminal;
            rules_c++;
        }
        if(rules_c == 1)
            cfg.start = nonterminal;
        for(int i = 0; i < strlen(cfg.non_terminals); i++){
            if(cfg.rules[i].non_terminal == nonterminal){
                int j = cfg.rules[i].no_of_prod;
                strcpy(cfg.rules[i].production[j], prod);
                cfg.rules[i].no_of_prod++;
                break;
            }
        }
        for(int i = 0; i < strlen(prod); i++){
            if(!isupper(prod[i])){
                if(strchr(cfg.terminals, prod[i]) == NULL){
                    cfg.terminals[strlen(cfg.terminals)] = prod[i];
                }
            }
        }
    }
    cfg.no_of_rules = rules_c;
    printf("CFG:\n");
    printf("Start: %c\n", cfg.start);
    printf("Non Terminals: %s\n", cfg.non_terminals);
    printf("Terminals: %s\n", cfg.terminals);
    printf("Rules:\n");
    for(int i = 0; i < cfg.no_of_rules; i++){
        printf("%c -> ", cfg.rules[i].non_terminal);
        for(int j = 0; j < cfg.rules[i].no_of_prod; j++){
            printf("%s | ", cfg.rules[i].production[j]);
        }
        printf("\n");
    }
    computefirst();
    computefollow();
    return 0;
}