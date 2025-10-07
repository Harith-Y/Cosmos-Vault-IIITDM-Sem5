#include<stdio.h>
#include<string.h>
/*
Transitions
state    a   b   c   d
0        1   0   7   7
1        1   2   7   7
2        1   3   7   7
3        4   0   5   6
4        4   2   5   6
5        5   7   5   6
6        7   7   7   7
7        7   7   7   7
*/
int main()
{
    int states[8] = {0,1,2,3,4,5,6,7};
    //7th state is dead state
    char inp[100];
    printf("Enter the string\n");
    scanf("%s",&inp);
    int length=strlen(inp);
    int current_state=0;
    int next_state;
    for(int i=0;i<=length;i++){
        char val=inp[i];
        switch(current_state){
            case 0:
                if(inp[i]=='a'){
                    next_state=1;
                }
                else if(inp[i]=='b'){
                    next_state=0;
                }
                else if(inp[i]=='c'){
                    next_state=7;
                }
                else if(inp[i]=='d'){
                    next_state=7;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
            case 1:
                if(inp[i]=='a'){
                    next_state=1;
                }
                else if(inp[i]=='b'){
                    next_state=2;
                }
                else if(inp[i]=='c'){
                    next_state=7;
                }
                else if(inp[i]=='d'){
                    next_state=7;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
            case 2:
                if(inp[i]=='a'){
                    next_state=1;
                }
                else if(inp[i]=='b'){
                    next_state=3;
                }
                else if(inp[i]=='c'){
                    next_state=7;
                }
                else if(inp[i]=='d'){
                    next_state=7;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
            case 3:
                if(inp[i]=='a'){
                    next_state=4;
                }
                else if(inp[i]=='b'){
                    next_state=0;
                }
                else if(inp[i]=='c'){
                    next_state=5;
                }
                else if(inp[i]=='d'){
                    next_state=6;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
            case 4:
                if(inp[i]=='a'){
                    next_state=4;
                }
                else if(inp[i]=='b'){
                    next_state=2;
                }
                else if(inp[i]=='c'){
                    next_state=5;
                }
                else if(inp[i]=='d'){
                    next_state=6;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
            case 5:
                if(inp[i]=='a'){
                    next_state=5;
                }
                else if(inp[i]=='b'){
                    next_state=7;
                }
                else if(inp[i]=='c'){
                    next_state=5;
                }
                else if(inp[i]=='d'){
                    next_state=6;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
            case 6:
                if(inp[i]=='a'){
                    next_state=7;
                }
                else if(inp[i]=='b'){
                    next_state=7;
                }
                else if(inp[i]=='c'){
                    next_state=7;
                }
                else if(inp[i]=='d'){
                    next_state=7;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
                
            case 7:
                if(inp[i]=='a'){
                    next_state=7;
                }
                else if(inp[i]=='b'){
                    next_state=7;
                }
                else if(inp[i]=='c'){
                    next_state=7;
                }
                else if(inp[i]=='d'){
                    next_state=7;
                }
                else if(i!=length){
                    printf("Please enter valid string which contains only a,b,c,d");
                    return 0;
                }
                break;
        }
        current_state=next_state;
    }
    if(current_state==6){
        printf("ACCEPTED");
    }
    else{
        printf("REJECTED");
    }
    return 0;
}