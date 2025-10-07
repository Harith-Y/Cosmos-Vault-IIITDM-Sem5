#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

const char *input;

int pos=0;
char lookahead;

void E();
void E_prime();
void T();
void T_prime();
void F();

void error(){
    printf("Parsing failed\n");
    exit(1);
}

void E(){
    T();
    E_prime();
}

void E_prime(){
    if(lookahead=='+'){
        pos++;
        lookahead=input[pos];
        T();
        T_prime();
    }
}

void T(){
    F();
    T_prime();
}

void T_prime(){
    if(lookahead=='*'){
        pos++;
        lookahead=input[pos];
        F();
        T_prime();
    }
}

void F(){
    if(lookahead=='('){
        pos++;
        lookahead=input[pos];
        E();
        if(lookahead==')'){
            pos++;
            lookahead=input[pos];
        }else{
            error();
        }
    }else if(lookahead=='i'){
        pos++;
        lookahead=input[pos];
        if(lookahead=='d'){
            pos++;
            lookahead=input[pos];
        }else{
            error();
        }
    }else{
        error();
    }
}


int main(){
    char buffer[256];
    printf("Enter expression :");
    if(fgets(buffer,sizeof(buffer),stdin)==NULL){
        return 1;
    }
    buffer[strcspn(buffer,"\n")]='\0';
    int len=strlen(buffer);
    buffer[len]='$';
    buffer[len+1]='\0';
    input= buffer;
    pos=0;
    lookahead=input[pos];
    E();
    if(lookahead=='$'){
        printf("Parsing succeeded");
    }else{
        printf("parsing failed");
    }
    return 0;
}









