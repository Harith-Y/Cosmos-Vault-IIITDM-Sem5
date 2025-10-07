#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define MAX 20
#define MAX_LEN 100
#define MAX_SYMBOLS 26

char productions[MAX][MAX_LEN];
int n;
char non_terminals[MAX_SYMBOLS];
char non_terminal_count=0;
char terminals[100];
int terminal_count=0;
int first_set[MAX_SYMBOLS][100];
int first_count[MAX_SYMBOLS];
int follow_set[MAX_SYMBOLS][100];
int follow_count[MAX_SYMBOLS];

int is_epsilon(char c){
    return c=='e';
}

int is_non_terminal(char c){
    return (c>='A' && c<='Z');
}

int get_non_terminal_index(char c){
    for(int i=0;i<non_terminal_count;i++){
        if(non_terminals[i]==c){
            return i;
        }
    }
    return -1;
}

void add_to_set(int set[][100],int *count,int index,char c){
    for(int i=0;i<count[index];i++){
        if(set[index][i]==c){
            return;
        }
    }
    set[index][count[index]]=c;
    count[index]++;
}

void extract_non_terminals(){
    for(int i=0;i<n;i++){
        char lhs=productions[i][0];
        if(!is_non_terminal(lhs)){
            printf("LHS should be a non terminal");
            exit(1);
        }
        int found=0;
        for(int j=0;j<non_terminal_count;j++){
            if(non_terminals[j]==lhs){
                found=1;
                break;
            }
        }
        if(!found){
            non_terminals[non_terminal_count++]=lhs;
        }
    }
}


void extract_terminals(){
    for(int i=0;i<n;i++){
        char *rhs=strchr(productions[i],'>')+1;
        for(int j=0;rhs[j]!='\0';j++){
            char c=rhs[j];
            if(c==' '||c=='|'||c=='\n'||c=='\t')
                continue;
            if(!is_non_terminal(c) && !is_epsilon(c)){
                int found=0;
                for(int k=0;k<terminal_count;k++){
                    if(terminals[k]==c){
                        found=1;
                        break;
                    }
                }
                if(!found){
                    terminals[terminal_count++]=c;
                }
            }
        }
    }
}


int first_computed[MAX_SYMBOLS];
void compute_first(char symbol);

void compute_first_of_string(char *str,int index, int *set_index){
    int i=0;
    int all_nullable=1;
    while(str[i]!='\0'){
        char c=str[i];
        if(c==' ' || c=='\t'){
            i++;
            continue;
        }
        if(!is_non_terminal(c)){
            if(c=='e'){
                add_to_set(first_set,first_count,index,'e');
            }
            else{
                add_to_set(first_set,first_count,index,c);
            }
            all_nullable=0;
            break;
        }
        else{
            int nt_index=get_non_terminal_index(c);
            if(nt_index==-1){
                printf("Non terminal not found\n");
                exit(1);
            }
            for(int k=0;k<first_count[nt_index];k++){
                if(first_set[nt_index][k]!='e'){
                    add_to_set(first_set,first_count,index,first_set[nt_index][k]);
                }
            }
            int has_epsilon=0;
            for(int k=0;k<first_count[nt_index];k++){
                if(first_set[nt_index][k]=='e'){
                    has_epsilon=1;
                    break;
                }
            }
            if(!has_epsilon)
                break;
            i++;
            if(str[i]=='\0'){
                add_to_set(first_set,first_count,index,'e');
            }
        }
    }
    if(all_nullable){
        add_to_set(first_set,first_count,index,'e');
    }
}



void compute_first(char symbol){
    int index=get_non_terminal_index(symbol);
    if(index==-1) return;
    if(first_computed[index]) return;
    first_computed[index]=1;
    for(int i=0;i<n;i++){
        if(productions[i][0]==symbol){
            char *rhs=strchr(productions[i],'>')+1;
            char *alt=strtok(rhs,"|");
            while(alt!=NULL){
                while(*alt==' ' || *alt =='\t') alt++;
                if(*alt=='\0'){
                    add_to_set(first_set,first_count,index,'e');
                }
                else{
                    if(is_non_terminal(alt[0])){
                        compute_first(alt[0]);
                    }
                    compute_first_of_string(alt,index,first_count);
                }
                alt=strtok(NULL,"|");
            }
        }
    }
}

int in_follow_set(int index,char c){
    for(int i=0;i<follow_count[index];i++){
        if(follow_set[index][i]==c){
            return 1;
        }
    }
    return 0;
}

void compute_follow(){
    add_to_set(follow_set,follow_count,0,'$');
    int changed=1;
    while(changed){
        changed=0;
        for(int i=0;i<n;i++){
            char lhs=productions[i][0];
            int lhs_index=get_non_terminal_index(lhs);
            char *rhs=strchr(productions[i],'>')+1;   
            char rhs_copy[MAX_LEN];
            strcpy(rhs_copy,rhs);
            char *alt=strtok(rhs_copy,"|");
            while(alt!=NULL){
                while(*alt==' ' || *alt =='\t') alt++;
                int len=strlen(alt);
                for(int pos=0;pos<len;pos++){
                    char c = alt[pos];
                    if(is_non_terminal(c)){
                        int c_index=get_non_terminal_index(c);
                        int k=pos+1;
                        int add_follow_of_lhs=0;
                        while(k<len){
                            char next_symbol=alt[k];
                            if(next_symbol==' ' || next_symbol=='\t'){
                                k++;
                                continue;
                            }
                            if(!is_non_terminal(next_symbol)){
                                if(!in_follow_set(c_index,next_symbol)){
                                    add_to_set(follow_set,follow_count,c_index,next_symbol);
                                    changed=1;
                                }
                                break;
                            }else{
                                int next_index=get_non_terminal_index(next_symbol);
                                for(int m=0;m<first_count[next_index];m++){
                                    char f= first_set[next_index][m];
                                    if(f!='e' && !in_follow_set(c_index,f)){
                                        add_to_set(follow_set,follow_count,c_index,f);
                                        changed=1;
                                    }
                                }
                                int has_epsilon=0;
                                for(int m=0;m<first_count[next_index];m++){
                                    if(first_set[next_index][m]=='e'){
                                        has_epsilon=1;
                                        break;
                                    }
                                }
                                if(!has_epsilon){
                                    break;
                                }
                                
                            }
                            k++;
                        }
                        if(k==len){
                            for(int m=0;m<follow_count[lhs_index];m++){
                                char f=follow_set[lhs_index][m];
                                if(!in_follow_set(c_index,f)){
                                    add_to_set(follow_set,follow_count,c_index,f);
                                    changed=1;
                                }
                            }
                        }
                    }
                }
                alt=strtok(NULL,"|");
            }
        }
    }
}

void print_set(char *set_name,char symbols[],int count, int first_or_follow_set[][100],int set_count[]){
    for(int i=0;i<count;i++){
        printf("%s(%c) = { ",set_name,symbols[i]);
        for(int j=0;j<set_count[i];j++){
            printf("%c",first_or_follow_set[i][j]);
            if(j!=set_count[i]-1){
                printf(", ");
            }
        }
        printf(" }\n");
    }
}

int main(){
    printf("Enter number of productions: ");
    scanf("%d",&n);
    getchar();
    printf("Enter the productions: ");
    for(int i=0;i<n;i++){
        fgets(productions[i], MAX_LEN, stdin);
        size_t len= strlen(productions[i]);
        if(productions[i][len-1]=='\n')
        productions[i][len-1]='\0';
    }
    extract_non_terminals();
    extract_terminals();
    for(int i=0;i<non_terminal_count;i++){
        first_computed[i]=0;
        follow_count[i]=0;
        first_count[i]=0;
    }
    for(int i=0;i<non_terminal_count;i++){
        compute_first(non_terminals[i]);
    }
    compute_follow();
    
    printf("FIRST SET");
    print_set("FIRST",non_terminals,non_terminal_count,first_set,first_count);
    
    printf("FOLLOW SET");
    print_set("FOLLOW",non_terminals,non_terminal_count,follow_set,follow_count);
    return 0;
}





