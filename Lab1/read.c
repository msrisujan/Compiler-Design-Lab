//Name: M.Sri Sujan
//Roll No: CS21B1081
#include<stdio.h> 
void main(){  
    FILE *fp;  
    char text[300];  
    
    fp=fopen("input.txt","r"); 
    while(fgets(text,200,fp)){
        printf("%s",text);  
    }
    fclose(fp);  
}