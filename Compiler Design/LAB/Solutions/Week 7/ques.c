#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX 20
#define MAX_PROD 50
#define MAX_LEN 20
#define MAX_TER 20
#define MAX_NTER 20

char terminals[MAX_TER],non_terminals[MAX_NTER];
int tcount=0, ntcount=0;

typedef struct{
    char lhs;
    char rhs[MAX_LEN];
} Production;

Production productions[MAX_PROD];
int pcount=0;

char first[MAX_NTER][MAX_LEN];
char follow[MAX_NTER][MAX_LEN];

char parsing_table[MAX_NTER][MAX_TER][MAX_LEN];

int is_terminal(char c){
    for(int i=0;i<tcount;i++){
        if(terminals[i]==c) return 1;
    }
    return 0;
}

int is_non_terminal(char c){
    for(int i=0;i<ntcount;i++){
        if(non_terminals[i]==c) return 1;
    }
    return 0;
}

int get_ntindex(char c){
    for(int i=0;i<ntcount;i++){
        if(non_terminals[i]==c) return i;
    }
    return -1;
}

int get_tindex(char c){
    for(int i=0;i<tcount;i++){
        if(terminals[i]==c) return i;
    }
    return -1;
}

int contains(char *str,char c){
    for(int i=0;str[i];i++){
        if(str[i]==c) return 1;
    }
    return 0;
}

void add_unique(char *str,char c){
    if(!contains(str,c)){
        int len=strlen(str);
        str[len]=c;
        str[len+1]='\0';
    }
    
}

void add_string_unique(char *dest,char *src){
    for(int i=0;src[i];i++){
        add_unique(dest,src[i]);
    }
}

void find_first(){
    for(int i=0;i<ntcount;i++){
        first[i][0]='\0';
    }
    int changed=1;
    while(changed){
        changed=0;
        for(int i=0;i<pcount;i++){
            char lhs = productions[i].lhs;
            char *rhs =productions[i].rhs;
            int lhs_index = get_ntindex(lhs);
            if(rhs[0]=='#'){
                if(!contains(first[lhs_index],'#')){
                    add_unique(first[lhs_index],'#');
                    changed=1;
                }
                continue;
            }
            int nullable_prefix=1;
            for(int j=0;rhs[j] && nullable_prefix;j++){
                char sym = rhs[j];
                if(is_terminal(sym)){
                    if(!contains(first[lhs_index],sym)){
                        add_unique(first[lhs_index],sym);
                        changed=1;
                    }
                    nullable_prefix=0;
                }else{
                    int sym_index=get_ntindex(sym);
                    for(int k=0;first[sym_index][k];k++){
                        if(first[sym_index][k]!='#'){
                            if(!contains(first[lhs_index],first[sym_index][k])){
                                add_unique(first[lhs_index],first[sym_index][k]);
                                changed=1;
                            }
                        }
                    }
                    if(contains(first[sym_index],'#')){
                        nullable_prefix=1;
                    }else{
                        nullable_prefix=0;
                    }
                }
            }
            if(nullable_prefix){
                if(!contains(first[lhs_index],'#')){
                    add_unique(first[lhs_index],'#');
                    changed=1;
                }
            }
        }
    }
}


void find_follow(){
    for(int i=0;i<ntcount;i++){
        follow[i][0]='\0';
    }
    add_unique(follow[0],'$');
    int changed=1;
    while(changed){
        changed=0;
        for(int i=0;i<pcount;i++){
             char lhs = productions[i].lhs;
            char *rhs =productions[i].rhs;
            int lhs_index = get_ntindex(lhs);
            for(int pos=0;rhs[pos];pos++){
                if(is_non_terminal(rhs[pos])){
                    int ntindex=get_ntindex(rhs[pos]);
                    int next_pos=pos+1;
                    int nullable_suffix=1;
                    while(rhs[next_pos] && nullable_suffix){
                        char sym = rhs[next_pos];
                        if(is_terminal(sym)){
                            if(!contains(follow[ntindex],sym)){
                                add_unique(follow[ntindex],sym);
                                changed=1;
                            }
                            nullable_suffix=0;
                        }else{
                            int sym_index=get_ntindex(sym);
                            for(int k=0;first[sym_index][k];k++){
                                 if(first[sym_index][k]!='#'){
                                    if(!contains(follow[ntindex],first[sym_index][k])){
                                        add_unique(follow[ntindex],first[sym_index][k]);
                                        changed=1;
                                    }
                                }
                            }
                            if(contains(first[sym_index],'#')){
                                nullable_suffix=1;
                                next_pos++;
                            }else{
                                nullable_suffix=0;
                            }
                        }
                    }
                    if(!rhs[next_pos] || nullable_suffix ){
                        for(int k=0;follow[lhs_index][k];k++){
                            if(!contains(follow[ntindex],follow[lhs_index][k])){
                                add_unique(follow[ntindex],follow[lhs_index][k]);
                                changed=1;
                            }
                        }
                    }
                }
            }
        }
    }
}


void construct_parsing_table(){
    for(int i=0;i<ntcount;i++){
        for(int j=0;j<tcount;j++){
            parsing_table[i][j][0]='\0';
        }
    }
    for(int i=0;i<pcount;i++){
        char lhs = productions[i].lhs;
        char *rhs =productions[i].rhs;
        int lhs_index = get_ntindex(lhs);
        char first_rhs[MAX_LEN]="";
        if(rhs[0]=='#'){
            strcpy(first_rhs,"#");
        }else{
            int nullable_prefix=1;
            for(int pos=0; rhs[pos] && nullable_prefix; pos++){
                char sym=rhs[pos];
                if(is_terminal(sym)){
                    add_unique(first_rhs,sym);
                    nullable_prefix=0;
                }else{
                    int sym_index=get_ntindex(sym);
                    add_string_unique(first_rhs,first[sym_index]);
                    if(contains(first[sym_index],'#')){
                        nullable_prefix=1;
                    }else{
                        nullable_prefix=0;
                    }
                }
            }
            if(nullable_prefix){
                add_unique(first_rhs,'#');
            }
        }
        for(int k=0;first_rhs[k];k++){
            char terminal=first_rhs[k];
            if(terminal!='#'){
                int tindex= get_tindex(terminal);
                if(tindex==-1) continue;
                if(parsing_table[lhs_index][tindex][0]!='\0'){
                    printf("Multiple entries for parsing table");
                }
                strcpy(parsing_table[lhs_index][tindex],rhs);
            }
        }
        
        if(contains(first_rhs,'#')){
            for(int k=0;follow[lhs_index][k];k++){
                char terminal=follow[lhs_index][k];
                int tindex=get_tindex(terminal);
                if(tindex==-1) continue;
                if(parsing_table[lhs_index][tindex][0]!='\0'){
                    printf("Multiple entries for parsing table");
                }
                strcpy(parsing_table[lhs_index][tindex],"#");
            }
        }
    }
}

void display_parsing_table(){
    printf("\nPARSING TABLE: \n\t");
    for(int j=0;j<tcount;j++){
        printf("%c\t",terminals[j]);
    }
    printf("\n");
    for(int i=0;i<ntcount;i++){
        printf("%c\t",non_terminals[i]);
        for(int j=0;j<tcount;j++){
            if(parsing_table[i][j][0]!='\0'){
                printf("%s\t",parsing_table[i][j]);
            }else{
                printf("\t");
            }
        }
        printf("\n");
    }
}


void parse_input(char *input){
    printf("Parsing input string %s",input);
    int top=0;
    char stack[MAX]={0};
    stack[top++]='$';
    stack[top++]=non_terminals[0];
    int ip=0;
    char a=input[ip];
    printf("\n %-20s %-20s %-20s \n ", "Stack", "Input", "Action");
    while(top>0){
        for(int i=0;i<top;i++){
            printf("%c", stack[i]);
        }
        printf("\t\t");
        printf("%s\t\t", input+ip);
        char X = stack[top-1];
        if(X==a && X=='$'){
            printf("Accept \n");
            return;
        }
        if(X==a){
            top--;
            ip++;
            a=input[ip];
            printf("Match %c\n",X);
        }
        else if(is_terminal(X)){
            printf("error - terminal mismatch at %c", X);
            return;
        }
        else if(is_non_terminal(X)){
            int row= get_ntindex(X);
            int col = get_tindex(a);
            if(col==-1){
                printf("error - unexpexted symbol at %c",a);
                return;
            }
            if(parsing_table[row][col][0]=='\0'){
                printf("No rule");
                return;
            }
            char *prod = parsing_table[row][col];
            printf("%c -> %s\n",X,strcmp(prod,"#")==0?"e":prod);
            top--;
            if(strcmp(prod,"#")!=0){
                int len = strlen(prod);
                for(int i=len-1;i>=0;i--){
                    stack[top++]=prod[i];
                }
            }
        }
        else{
            printf("unknown symbol on stack\n");
            return;
        }
    }
    printf("stack empty but input not consumed\n");
}


int main(){
    printf("enter number of terminals : ");
    scanf("%d",&tcount);
    printf("Enter terminals without spaces : ");
    scanf("%s",terminals);
    printf("enter number of non terminals : ");
    scanf("%d",&ntcount);
    printf("Enter non terminals without spaces : ");
    scanf("%s",non_terminals);
    printf("enter number of Productions : ");
    scanf("%d",&pcount);
    printf("Enter productions (# for epsilon)\n");
    
    for(int i=0;i<pcount;i++){
        char prod_str[30];
        scanf("%s",prod_str);
        productions[i].lhs= prod_str[0];
        strcpy(productions[i].rhs,prod_str+3);
        
    }
   
    find_first();
    find_follow();
    
    
    //Debugging
    // printf("FIRST");
    // for(int i=0;i<ntcount;i++){
    //     for(int j=0;first[i][j];j++){
    //         printf("%c",first[i][j]);
    //     }
    // }
    
    // printf("FOLLOW");
    //   for(int i=0;i<ntcount;i++){
    //     for(int j=0;follow[i][j];j++){
    //         printf("%c",follow[i][j]);
    //     }
    // }
    
    construct_parsing_table();
    display_parsing_table();
    char input[MAX];
    printf("Enter string ending with $ : ");
    scanf("%s",input);
    parse_input(input);
    return 0;
}



