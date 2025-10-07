#include<string.h>
#include<stdio.h>
#include<ctype.h>

#define MAX 20
#define MAX_RULE_LEN 100

char non_terminals[MAX];
char productions[MAX][MAX_RULE_LEN];

int n=0;

int get_index(char nt){
    for(int i=0;i<n;i++){
        if(non_terminals[i]==nt){
            return i;
        }
    }
    return -1;
}

int has_direct_left_recursion(char nt, char *prod){
    char temp[MAX_RULE_LEN];
    strcpy(temp,prod);
    char * token = strtok(temp,"|");
    while (token){
        if(token[0]==nt){
            return 1;
        }
        token=strtok(NULL,"|");
        
    }
    return 0;
}

int has_indirect_left_recursion(char start,char current,int visited[]){
    if(visited[current-'A']) return 0;
    visited[current-'A']=1;
    int idx=get_index(current);
    if(idx== -1) return 0;
    char temp[MAX_RULE_LEN];
    strcpy(temp,productions[idx]);
    char * token = strtok(temp,"|");
    while (token){
        if(isupper(token[0])){
           if(token[0]==start){
               return 1;
           }
           if(has_indirect_left_recursion(start,token[0],visited)){
               return 1;
           }
        }
        token=strtok(NULL,"|");
    }
    return 0;
}

void trim_new_line(char *str){
    int len = strlen(str);
    if( len>0 && str[len-1]=='\n'){
        str[len-1]='\0';
    }
}

int main(){
    printf("Enter number of production lines\n");
    scanf("%d",&n);
    printf("enter productions\n");
    for(int i=0;i<=n;i++){
        char input[MAX_RULE_LEN];
        fgets(input,sizeof(input),stdin);
        char lhs=input[0];
        char* rhs=input+3;
        non_terminals[i]=lhs;
        strcpy(productions[i],rhs);
        printf("%s",productions[i]);
    }
    
    printf("---left  recursion ---\n");
    for(int i=0;i<=n;i++){
        char lhs=non_terminals[i];
        if(has_direct_left_recursion(lhs,productions[i])){
            printf("Direct left recursion detected %c",lhs);
        }
        else{
            int visited[26]={0};
            if(has_indirect_left_recursion(lhs,lhs,visited)){
                printf("Indirect left recursion detected %c",lhs);
            }
        }
    }
    return 0;
}


