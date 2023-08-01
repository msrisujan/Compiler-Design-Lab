//Name: M.Sri Sujan
//Roll No: CS21B1081
#include<stdio.h> 
void main(){  
    FILE *fp;  
    char text[300];  
    
    fp=fopen("output.txt","a"); 
    
    fputs("Appended text to output.txt",fp);
    fclose(fp);  
}