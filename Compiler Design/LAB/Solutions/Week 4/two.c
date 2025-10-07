#include<stdio.h>
#include<string.h>
#include<ctype.h>

int main(){
    char non_terminal;
    char production[100],alpha[50][50],beta[50][50];
    int i,j=0,k=0,num_alpha=0,num_beta=0;
    
    printf("Enter prodution");
    scanf("%s",production);
    non_terminal=production[0];
    i=3;
    while(production[i]!='\0'){
        if(production[i]=='|'){
            j++;
            i++;
        }else{
            if(production[i]==non_terminal){
                i++;
                k=0;
                while(production[i]!='\0' && production[i]!='|'){
                    alpha[num_alpha][k++]=production[i++];
                    
                }
                alpha[num_alpha][k]='\0';
                // printf("alpha %s \n",alpha[num_alpha]); //debugging
                num_alpha++;
                //  printf("%d",num_alpha); //debugging purpose
                j=0;
            }else{
                k=0;
                while(production[i]!='\0' && production[i]!='|'){
                    beta[num_beta][k++]=production[i++];
                }
                beta[num_beta][k]='\0';
                num_beta++;
                j=0;
            }
        }
    }
  
    
    if(num_alpha==0){
        printf("No left recursion found");
        return 0;
    }
    
    printf("left recursion:");
    printf("%c->",non_terminal);
    for(i=0;i<num_beta;i++){
        printf("%s%c'",beta[i],non_terminal);
        if(i!=num_beta-1) printf("|");
        
    }
    
    printf("\n");
    printf("%c'->",non_terminal);
    //debugging purpose
    // printf("%s",alpha[0]);
    // printf("%d",num_alpha);
    for(i=0;i<num_alpha;i++){
        printf("%s%c'|",alpha[i],non_terminal);
        
    }
    printf("e\n");
    return 0;
    
    
}