//Name: M.Sri Sujan
//Roll No: CS21B1081
#include<stdio.h> 
void main(){  
    FILE *fp1; 
    FILE *fp2; 
    char text[300];  
    
    fp1=fopen("input.txt","r"); 
    fp2=fopen("output.txt","w");
    while(fgets(text,200,fp1)){
        fputs(text,fp2);  
    }
    fclose(fp1);  
    fclose(fp2);
}