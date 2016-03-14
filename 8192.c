#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

#define CHOICE 10
#define MAXback 20

typedef enum lit{OFF,ON}LIT;
typedef enum res{LOSE,PLAY,WIN}RES;
typedef unsigned short GRID[4][4];
typedef struct gridscore{
    GRID array;
    unsigned int score;
}GS;
typedef struct back{
    GS store[MAXback+1];
    unsigned short front;
    unsigned short rear;
}BACK;

char MainInterface(void);
void StartGame(LIT light);
void HowToPlay(void);
LIT Settings(LIT light);
void Leave(void);
int IsGameAgain(RES result);
void InitialBack(BACK*back);
void AddBack(BACK *back,GRID array,unsigned int score);
int Back(BACK *back,GRID array,unsigned int*score);
void CreateARandomNumber(GRID array);
RES IsGameOver(GRID array);
RES PrintArray(GRID array,GRID oldarray,unsigned int score,LIT light);
int GetKey(GRID array,unsigned int *score,BACK *back);
void UpKey(GRID array,unsigned int *score);
void LeftKey(GRID array,unsigned int *score);
void RightKey(GRID array,unsigned int *score);
void DownKey(GRID array,unsigned int *score);
int LMovable(GRID array);
int RMovable(GRID array);
int UMovable(GRID array);
int DMovable(GRID array);
void color(unsigned short fore,unsigned short back);
unsigned short Bit(unsigned short elem);
void ShowNum(unsigned short num);
void Goto(unsigned short x,unsigned short y);
void HideCursor(void);
void PrintCursor(void);

int main(void)
{
    LIT light=OFF;
    system("color 0f");    system("title 8192");
    while(1){
        char choice=MainInterface();
        switch(choice){
            case '1': StartGame(light);     break;
            case '2': HowToPlay();          break;
            case '3': light=Settings(light);break;
            case '0': Leave();              break;
        }
    }
    return 0;
}

char MainInterface(void)
{
    char choice[CHOICE];    system("cls");
    printf("\n");
    printf("                   * * * * * * * * * * * * *\n");
    printf("                   *                       *\n");
    printf("                   *     1. Start Game     *\n");
    printf("                   *                       *\n");
    printf("                   *     2. How To Play    *\n");
    printf("                   *                       *\n");
    printf("                   *     3. Settings       *\n");
    printf("                   *                       *\n");
    printf("                   *     0. Leave          *\n");
    printf("                   *                       *\n");
    printf("                   * * * * * * * * * * * * *\n");
    printf("choice:");scanf("%8s",choice);
    while(strcmp("1",choice)&&strcmp("2",choice)&&strcmp("3",choice)&&strcmp("0",choice)){
        printf("Useless.\n");printf("choice:");scanf("%8s",choice);
    }
    return choice[0];
}
void StartGame(LIT light)
{
    RES result=PLAY;    HideCursor();
    do{
        GRID array,oldarray;        BACK back;   system("cls");
        unsigned int score=0;   InitialBack(&back);
        //game body    //Inital
        memset(array,0,sizeof(GRID)); memset(oldarray,-1,sizeof(GRID));
        CreateARandomNumber(array);  CreateARandomNumber(array);
        PrintArray(array,oldarray,score,light);
        while(result=IsGameOver(array)){
            memcpy(oldarray,array,sizeof(GRID));
            if(GetKey(array,&score,&back)){
                PrintArray(array,oldarray,score,light);Sleep(50);
                memcpy(oldarray,array,sizeof(GRID));
                CreateARandomNumber(array);}
            result=PrintArray(array,oldarray,score,light);
            if(result==WIN) break;
        }
    }while(IsGameAgain(result));

    PrintCursor();
}
void HowToPlay(void)
{
    system("cls");
    printf("\n\n");
    printf("         Press UP,DOWN,LEFT,RIGHT key to move.\n\n");
    printf("         When the same numbers meet, they join together.\n\n");
    printf("         You can press Backspace and return to the last time.\n\n");
    printf("         Try to create 8192 !\n\n");
    printf("         Press any key and back to the game.");
    getch();
}
LIT Settings(LIT light)
{
    char choice[CHOICE];    system("cls");
    printf("\n\n                    1. Light ON\n");
    printf("                    0. Light OFF\n");
    printf("choice:");scanf("%8s",choice);
    while(strcmp("1",choice)&&strcmp("0",choice)){
        printf("Useless.\n");printf("choice:");scanf("%8s",choice);
    }
    if(choice[0]=='1'){ system("color f0");return ON; }
    else{ system("color 0f");return OFF; }
}
void Leave(void)
{
    exit(0);
}
int IsGameAgain(RES result)
{
    Goto(0,19);
    char choice[CHOICE];printf("\a");
    if(result==LOSE){   //Try again
        printf("  Oh, you lose! Try again ? ( y/n ):");scanf("%s",choice);
    }
    else{          //WIN:Once again
        printf("  Win! Play again ? ( y/n ):");scanf("%s",choice);
    }
    while(choice[0]!='y'&&choice[0]!='n'){
        printf("  Invalid answer !  y/n:");scanf("%s",choice);
    }
    if(choice[0]=='y')  return 1;
    else return 0;
}
void InitialBack(BACK *back)
{
    back->front=back->rear=0;
}
void AddBack(BACK *back,GRID array,unsigned int score)
{
    if((back->rear+1)%MAXback==back->front)
    back->front=(back->front+1)%MAXback;
    memcpy(back->store[back->rear].array,array,sizeof(GRID));
    back->store[back->rear].score=score;
    back->rear=(back->rear+1)%MAXback;
}
int Back(BACK *back,GRID array,unsigned int*score)
{
    if(back->front==back->rear)
    return 0;
    back->rear=(back->rear-1+MAXback)%MAXback;
    memcpy(array,back->store[back->rear].array,sizeof(GRID));
    *score=back->store[back->rear].score;
    return 1;
}
void CreateARandomNumber(GRID array)
{
    int n,k;
    srand((unsigned)time(NULL));
    n=rand()%2+0;
    switch(n){
         case 0:k=2;break;
         case 1:k=4;break;
    }
    int i,j,add;
    int zeronum=0;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            if(array[i][j]==0)
              zeronum++;
    add=rand()%zeronum+1;
    zeronum=0;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++){
            if(array[i][j]==0){
                zeronum++;
            }
            if(zeronum==add){
                array[i][j]=k;
                return;
            }
        }
}
RES IsGameOver(GRID array)
{
    int i,j;
    for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        if(array[i][j]==0)
            return PLAY;
    for(i=0;i<3;i++)
      for(j=0;j<4;j++)
        if(array[i][j]==array[i+1][j])
            return PLAY;
    for(i=0;i<4;i++)
      for(j=0;j<3;j++)
        if(array[i][j]==array[i][j+1])
            return PLAY;
    return LOSE;
}
RES PrintArray(GRID array,GRID oldarray,unsigned int score,LIT light)
{
    int backcolor[14] = {15,10,2,9,5,13,12,4,1,6,3,7,8,11};
    static int best = 0;
    int i ,j;
    int x=0,y=0;
    if(light == ON)
    {
        backcolor[0]=0;
        color(0,15);
    }
    else
    {
        color(14,0);
    }
    if(best<score)
        best = score;
    Goto(3,1);
    printf("score:");
    printf("%-6d",score);
    Goto(21,1);
    printf("best:");
    printf("%-6d",best);
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(array[i][j]!=oldarray[i][j]){
                x = 2+8*j;
                y = 3+4*i;
                Goto(x,y);
                color(15,backcolor[Bit(array[i][j])]);
                printf("      ");
                Goto(x,y+1);
                ShowNum(array[i][j]);
                Goto(x,y+2);
                printf("      ");
            }
        }
    }
    //Goto(32,17);
    if(light==ON)
        color(0,15);
    else
        color(15,0);
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            if(array[i][j]==8192)
                return WIN;
    return PLAY;
}

int GetKey(GRID array,unsigned int *score,BACK *back)
{
    int c;
    while(1){
        c=getch();
        if(c==0||c==0xe0) c=getch();
      switch(c){
        case 72: if(UMovable(array)){
                    AddBack(back,array,*score);UpKey(array,score);   return 1;}break;
        case 75: if(LMovable(array)){
                    AddBack(back,array,*score);LeftKey(array,score); return 1;}break;
        case 77: if(RMovable(array)){
                    AddBack(back,array,*score);RightKey(array,score);return 1;}break;
        case 80: if(DMovable(array)){
                    AddBack(back,array,*score);DownKey(array,score); return 1;}break;
        case 8 : if(Back(back,array,score)) return 0; break;//Backspace
      }
    }
}
void UpKey(GRID array,unsigned int *score)
{
    int  i,j,compare;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 1, compare = 0; j < 4; j++)
        {
            if (array[j][i] > 0)
            {
                if (array[compare][i] == array[j][i])
                {
                    array[compare++][i] = array[j][i]*2;
                    *score =  *score + array[j][i] * 2;
                    array[j][i] = 0 ;
                }
                else if (array[compare][i] == 0)
                {
                    array[compare][i] = array[j][i];
                    array[j][i] = 0;
                }
                else
                {
                    array[++compare][i] = array[j][i];
                    if (j != compare)
                    {
                        array[j][i] = 0;
                    }
                }
            }
        }
    }
}
void LeftKey(GRID array,unsigned int *score)
{
    int i,j,compare;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 1, compare = 0; j <4; j++)
        {
            if (array[i][j] > 0)
            {
                if (array[i][compare] == array[i][j])
                {
                    array[i][compare++] = array[i][j]*2;
                    *score = *score + array[j][i] * 2;
                    array[i][j] = 0 ;
                }
                else if (array[i][compare] == 0)
                {
                    array[i][compare] = array[i][j];
                    array[i][j] = 0;
                }
                else
                {
                    array[i][++compare] = array[i][j];
                    if (j != compare)
                    {
                        array[i][j]= 0;
                    }
                }
            }
        }
    }
}
void RightKey(GRID array,unsigned int *score)
{
    int  i,j,compare;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 2, compare = 3; j > -1; j--)
        {
            if (array[i][j] > 0)
            {
                if (array[i][compare] == array[i][j])
                {
                    array[i][compare--] = array[i][j]*2;
                    *score = *score + array[i][j]*2;
                    array[i][j] = 0 ;
                }
                else if (array[i][compare] == 0)
                {
                    array[i][compare] = array[i][j];
                    array[i][j] = 0;
                }
                else
                {
                    array[i][--compare] = array[i][j];
                    if (j != compare)
                    {
                        array[i][j]= 0;
                    }
                }
            }
        }
    }
}
void DownKey(GRID array,unsigned int *score)
{
    int  i,j,compare;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 2, compare = 3; j >-1; j--)
        {
            if (array[j][i] > 0)
            {
                if (array[compare][i] == array[j][i])
                {
                    array[compare--][i] = array[j][i]*2;
                    *score =  *score + array[j][i] * 2;
                    array[j][i] = 0 ;
                }
                else if (array[compare][i] == 0)
                {
                    array[compare][i] = array[j][i];
                    array[j][i] = 0;
                }
                else
                {
                    array[--compare][i] = array[j][i];
                    if (j != compare)
                    {
                        array[j][i] = 0;
                    }
                }
            }
        }
    }
}
int LMovable(GRID array)
{
    int i,j,compare;
    for(i=0; i<4; i++)
    {
        for(j=1,compare=0; j<4; j++)
        {
            if(array[i][j]>0)
            {
                if(array[i][compare] == 0)   return 1;
                else if(array[i][compare] ==array[i][j])   return 1;
                else compare++;
            }
            else compare++;
        }
    }
    return 0;
}
int RMovable(GRID array)
{
    int i,j,compare;
    for(i=0; i<4; i++)
    {
        for(j=2,compare=3; j >-1; j--)
        {
            if(array[i][j]>0)
            {
                if(array[i][compare] == 0)   return 1;
                else if(array[i][compare] ==array[i][j])   return 1;
                else compare--;
            }
            else compare--;
        }
    }
    return 0;
}
int UMovable(GRID array)
{
    int i,j,compare;
    for(j=0; j<4; j++)
    {
        for(i=1,compare=0; i<4; i++)
        {
            if(array[i][j]>0)
            {
                if(array[compare][j] == 0)   return 1;
                else if(array[compare][j] ==array[i][j])   return 1;
                else compare++;
            }
            else compare++;
        }
    }
    return 0;
}
int DMovable(GRID array)
{
    int i,j,compare;
    for(j=0; j<4; j++)
    {
        for(i=2,compare=3; i>-1; i--)
        {
            if(array[i][j]>0)
            {
                if(array[compare][j] == 0)   return 1;
                else if(array[compare][j] ==array[i][j])   return 1;
                else compare--;
            }
            else compare--;
        }
    }
    return 0;
}
void color(unsigned short fore,unsigned short back)
{
    HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut,fore+back*16);
}
unsigned short Bit(unsigned short elem)
{
    int count=0;
    if(elem==0)
        return 0;
    else
        while(elem!=1)
        {
            elem/=2;
            count++;
        }
    return count;
}

void ShowNum(unsigned short num)
{
    switch(num){
        case 0:printf("      ");break;
        case 2:case 4:case 8:printf("  %d   ",num);break;
        case 16:case 32:case 64:printf("  %d  ",num);break;
        case 128:case 256:case 512:printf(" %d  ",num);break;
        case 1024:case 2048:case 4096:case 8192:printf(" %d ",num);break;
    }
}
void Goto(unsigned short x,unsigned short y)
{
    COORD pos;
    pos.X=x;    pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
void HideCursor(void)
{
    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void PrintCursor(void)
{
    CONSOLE_CURSOR_INFO cursor_info={20,1};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
