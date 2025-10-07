#include<stdio.h>
#include<string.h>

#define MAX_ALT 50
#define MAX_LEN 200

typedef char Production[MAX_LEN];

void longest_common_prefix(Production *arr, int n, char out[]){
    if(n==0){
        out[0]='\0';
        return;
    }
    int len=0;
    while(arr[0][len]){
        for(int i=1;i<n;i++){
            if(!arr[i][len] || arr[i][len]!=arr[0][len]){
                out[len]='\0';
                return;
                
            }
            
        }
        out[len]=arr[0][len];
        len++;
    }
    out[len]='\0';
}

void make_non_terminal(char base, char prime_count, char out[]){
    out[0]=base;
    for(int i=1;i<=prime_count;i++){
        out[i]='\'';
        
    }
    out[prime_count+1] = '\0';
    
}



void factorize(char base,int prime_count,Production *productions,int count){
    int used[256];
    memset(used,-1,sizeof(used));
    int groups[MAX_ALT][MAX_ALT], group_size[MAX_ALT]={0};
    int group_count=0;
    for(int i=0;i<count;i++){
        unsigned char first_char=productions[i][0];
        if(used[first_char]==-1){
            used[first_char]=group_count++;
        }
        groups[used[first_char]][group_size[used[first_char]]]=i;
        group_size[used[first_char]]++;
    }
    
    Production lhs_expressions[MAX_ALT];
    int lhs_count=0;
    
    Production sub_productions[MAX_ALT][MAX_ALT];
    int sub_count[MAX_ALT],sub_primes[MAX_ALT];
    int subs=0;
    
    for(int g=0;g<group_count;g++){
        if(group_size[g]<2){
            strcpy(lhs_expressions[lhs_count++],productions[groups[g][0]]);
            continue;
        }
        
        Production group_array[MAX_ALT];
        for(int t=0;t<group_size[g];t++){
            strcpy(group_array[t],productions[groups[g][t]]);
        }
        char lcp[MAX_LEN];
        longest_common_prefix(group_array,group_size[g],lcp);
        char new_non_terminal[16];
        make_non_terminal(base,prime_count+subs+1,new_non_terminal);
        sprintf(lhs_expressions[lhs_count++],"%s%s",lcp,new_non_terminal);
        int new_count=0;
        int lcp_len=strlen(lcp);
        for(int t=0;t<group_size[g];t++){
            const char *suffix= group_array[t]+lcp_len;
            strcpy(sub_productions[subs][new_count++],*suffix?suffix:"e");
            
        }
        sub_count[subs]=new_count;
        sub_primes[subs]=prime_count+subs+1;
        subs++;
        
    }
    char lhs[16];
    make_non_terminal(base,prime_count,lhs);
    printf("%s->",lhs);
    for(int i=0;i<lhs_count;i++){
        printf("%s",lhs_expressions[i]);
        if(i!=lhs_count-1){
            printf("|");
        }
        
    }
    printf("\n");
    for(int i=0;i<subs;i++){
        factorize(base,sub_primes[i],sub_productions[i],sub_count[i]);
    }
}

void parse_and_factor(const char *production ){
    char base=production[0];
    Production alternatives[MAX_ALT];
    int count=0;
    for(int i=3;i<strlen(production);){
        int k=0;
        while(production[i] && production[i]!='|'){
            alternatives[count][k++]=production[i++];
            
        }
        alternatives[count][k]='\0';
        count++;
        if(production[i]=='|'){
            i++;
        }
        
    }
    factorize(base,0,alternatives,count);
}

int main(){
    int n;
    char line[512];
    printf("Enter number of productions: ");
    if(scanf("%d",&n)!=1){
        return 1;
    }
    while(getchar()!='\n')
        ;
    printf("enter each production ");
    for(int i=0;i<n;i++){
        if(scanf("%s",line)==1){
            parse_and_factor(line);   
        }
    }
    return 0;
}


























