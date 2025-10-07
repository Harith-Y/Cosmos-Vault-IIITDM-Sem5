#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAX_TOKEN_LEN 1000

const char *keywords[] = {
    "int","if","else","float","void","double","while","char","for","return","main"
};

const int keywords_count=sizeof(keywords)/sizeof(keywords[0]);

int is_keyword(const char *str){
    for(int i=0;i<keywords_count;i++){
        if(strcmp(str,keywords[i])==0){
            return 1;
        }
    }
     return 0;
}

int is_operator_char(char c){
    return ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='=')||(c=='<')||(c=='>')||(c=='|')||(c=='!')||(c=='&')||(c=='%'));
}

int is_punctuation_char(char c){
    return((c=='{')||(c=='}')||(c==';')||(c==',')||(c=='(')||(c==')')||(c=='[')||(c==']'));
}

int is_number(const char *str){
    int i=0;
    if(str[0]=='-' || str[0]=='+') i=1;
    for(;str[i];i++){
        if(!isdigit(str[i])) return 0;
    }
    return i > (str[0]=='-' || str[0]=='+')?1:0;
}


int main(){
    char c;
    char token[MAX_TOKEN_LEN];
    int idx=0;
    printf("Enter your code (ctrl+d to end)\n");
    
    while((c=getchar())!=EOF){
        if(isspace(c)){
            continue;
        }
        if((isalpha(c)) || c=='_'){
            idx=0;
            token[idx++]=c;
            while(((c=getchar())!=EOF) && (isalnum(c) || c=='_') ){
                if (idx<MAX_TOKEN_LEN-1) token[idx++]=c;
            }
            token[idx]='\0';
            if(c!=EOF) ungetc(c,stdin);
            
            if(is_keyword(token))
            {
                printf("\nKeyword : %s",token);
            }
            else
            {
                printf("\nIdentifier : %s",token);
            }
            continue;
        }
        
        if(isdigit(c)){
            idx=0;
            token[idx++]=c;
            while(((c=getchar())!=EOF) && isdigit(c)){
                if (idx<MAX_TOKEN_LEN-1) token[idx++]=c;
            }
            token[idx]='\0';
            if(c!=EOF) ungetc(c,stdin);
            printf("\nLiteral : %s",token);
            continue;
        }
        
        if(c=='\''){
            idx=0;
            token[idx++]=c;
            char next = getchar();
            if(next==EOF) break;
            token[idx++]=next;
            char closing = getchar();
            if(closing==EOF) break;
            token[idx++]=closing;
            token[idx]='\0';
            if(closing=='\''){
                printf("\nLiteral : %s",token);
            }
            else{
                printf("\nUnknown Literal : %s",token);
            } 
            continue;
        }
        
        if(is_operator_char(c)){
            idx=0;
            token[idx++]=c;
            char next = getchar();
            if(next!=EOF){
                if(((c=='=')&&(next =='='))||
                    ((c=='!')&&(next =='='))||
                    ((c=='>')&&(next =='='))||
                    ((c=='<')&&(next =='='))||
                    ((c=='&')&&(next =='&'))||
                    ((c=='+')&&(next =='='))||
                    ((c=='-')&&(next =='='))||
                    ((c=='*')&&(next =='='))||
                    ((c=='/')&&(next =='='))||
                    ((c=='+')&&(next =='+'))||
                    ((c=='-')&&(next =='-'))||
                    ((c=='|')&&(next =='|'))){
                        token[idx++]=next;
                    }
                else{
                    ungetc(next,stdin);
                }
            }
            
            token[idx]='\0';
            printf("\nOperator : %s",token);
            continue;
        }
        if(is_punctuation_char(c)){
            printf("\nPunctuation : %c",c);
            continue;
        }
        
        
        
    }
    
    return 0;
}









