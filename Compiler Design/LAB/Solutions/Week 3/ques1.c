#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_PROD 10
#define MAX_LEN 100

typedef struct{
    char lhs;
    char rhs[MAX_PROD][MAX_LEN];
    int count;
}Production;

Production grammar[26];
int prod_count=0;

int find_non_terminal_index(char nt){
    for(int i=0;i<prod_count;i++){
        if(grammar[i].lhs == nt) return i;
    }
    return -1;
}

int derive(char non_terminal,char *str,int *index, int len){
    int nt_index=find_non_terminal_index(non_terminal);
    if(nt_index == -1) return 0;
    for(int i=0;i<grammar[nt_index].count;i++){
        char *prod = grammar[nt_index].rhs[i];
        if(strcmp(prod,"e")==0) return 1;
        
        int saved_index = *index;
        int j=0;
        int success=1;
        
        while(prod[j] && success){
            char sym = prod[j];
            if(sym>='a' && sym<='z'){
                if(*index<len && str[*index]==sym){
                    (*index)++;
                }
                else{
                    success=0;
                }
            }
            else if(sym>='A' && sym<='Z'){
                if(!derive(sym,str,index,len)){
                success=0;
                }
            }
            else{
                success=0;
            }
            j++;
        }
        if(success) return 1;
        
        *index = saved_index;
    }
    return 0;
}


int main(){
    int n;
    printf("Enter number of productions\n");
    scanf("%d",&n);
    getchar();
    
    printf("Enter production lines\n");
    
    for(int i=0;i<n;i++){
        char line[200];
        fgets(line,sizeof(line),stdin);
        char lhs;
        char *rhs_str;
        char *arrow = strstr(line,"->");
        if(!arrow){
            printf("Invalid Production Format\n");
            return 0;
        }
        
        lhs=line[0];
        rhs_str=arrow+2;
        int len = strlen(rhs_str);
        while(len>0 && (rhs_str[len-1]=='\n' || rhs_str[len-1]==' ')){
            rhs_str[len-1]='\0';
            len--;
        }
        int idx = find_non_terminal_index(lhs);
        if(idx == -1){
            idx = prod_count++;
            grammar[idx].lhs=lhs;
            grammar[idx].count=0;
        }
        char *token = strtok(rhs_str,"|");
        while(token !=NULL){
            while(*token==' ') token++;
            strncpy(grammar[idx].rhs[grammar[idx].count],token,MAX_LEN);
            grammar[idx].rhs[grammar[idx].count][MAX_LEN-1]='\0';
            grammar[idx].count++;
            token = strtok(NULL,"|");
        }
    }
    // int num_of_strings;
    // printf("Enter the number of strings to verify\n");
    // scanf("%d",&num_of_strings);
    // for(int k=0;k<num_of_strings;k++){
        char input[100];
        printf("Enter the string to verify\n");
        scanf("%s",input);
        int index=0;
        int start_symbol=grammar[0].lhs;
        
        int result = derive(start_symbol,input,&index,strlen(input));
        
        if(result && index==(int)strlen(input)){
            printf("String Accepted by the grammar");
        }else{
            printf("String Rejected by the grammar");
        }
    // }
    return 0;
}