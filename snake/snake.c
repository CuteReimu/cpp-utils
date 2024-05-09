#include<stdlib.h>
#include<bios.h>
#include<dos.h>
#include<graphics.h>        /*这是TC的一个头文件*/
#define MAXX 640            /*游戏最大分辨率*/
#define MAXY 480
#define NOR    60            /*表示游戏难度*/
#define DIF    30
#define HELL   10
#define LOSE   3
#define LEN    3            /*初始时蛇长度*/
#define FIR   10            /* 过第一关所需食物数 */
#define SEC   20            /* 过第二关所需食物数 */
#define THR   30            /* 过第三关所需食物数 */
#define MOVIE 10            /*表示蛇移动一步的坐标变化量*/
#define LEFT  0x4B00        /*蛇头转向*/
#define RIGHT 0x4D00
#define UP    0x4800
#define DOWN  0x5000
#define ESC   0x011B        /*退出游戏*/
#define ENTER 0x1C0D        /*暂停游戏*/
typedef struct snake{
    int x;
    int y;
    struct snake *previous;
    struct snake *next;
}SNAKE;
typedef struct wall{
    int x1;          /*用 对角线坐标 表示墙的范围*/
    int y1;
    int x2;
    int y2;
    struct wall *next;
}WALL;
typedef struct food{
    int x;
    int y;
    char exist;      /* 1表食物存在，0表不存在 */
}FOOD;
SNAKE *head=NULL,*tail=NULL,*ps=NULL ;
WALL *wallhead=NULL,*wallp=NULL;
FOOD f[3]={0,0,0,0,0,0,0,0,0};
int gameres=0;       /*记录游戏状态，0继续游戏、1游戏胜利，进入下一关、2游戏通全关*/
char direx=MOVIE,direy=0,speed=DIF,screeni=1,mode=0; /*mode为游戏模式，1为经典，0为闯关*/
/*(direx,direy)表蛇矢量运动方向*****speed 蛇移动速度*/
char checkin(SNAKE scope,int x,int y)                /*检测点是否在蛇头内，若是返回1，否则返回0*/
{                                        
    if(scope.x-5<=x&&scope.x+5>=x&&scope.y-5<=y&&scope.y+5>=y)return 1;
    else return 0;
}
char checkwall(WALL scope,int x,int y)               /*检测点是否在墙内，若是返回1，否则返回0*/
{
    if(scope.x1<=x&&scope.x2>=x&&scope.y1<=y&&scope.y2>=y)return 1;
    else return 0;
}
void graphbegin(void)
{                                       /* 初始化图形驱动 */
    int gdriver=DETECT,gmode;
    initgraph(&gdriver,&gmode,"bgi");
}
void newgame(void)                      /* 初始化背景、蛇信息 */
{
    int i;     
    gameres=0;                          /* 初始化游戏情况 */
    direx=MOVIE,direy=0;                /* 初始化运动方向 */
    head=(SNAKE *)malloc(sizeof(SNAKE));
    ps=head;
    head->previous=NULL;                /* 建立蛇头 */
    head->x=MAXX/2;
    head->y=MAXY/2;
    for(i=1;i<=LEN;i++)                 /* 建立蛇身 */
    {
        ps->next=(SNAKE *)malloc(sizeof(SNAKE));
        ps->next->previous=ps;
        ps=ps->next;
        ps->x=ps->previous->x-MOVIE;
        ps->y=ps->previous->y;
    }
    tail=ps;
    ps->next=NULL;
    cleardevice();                      /* 下一关的开始前清除本关图像 */
    setbkcolor(0);                      /* 初始化背景 */
    setcolor(GREEN);              /* 画蛇身 */
    for(ps=head->next;ps!=NULL;ps=ps->next)rectangle(ps->x-5,ps->y-5,ps->x+5,ps->y+5);
    setcolor(BLUE);               /* 蛇头是蓝的 */
    rectangle(head->x-5,head->y-5,head->x+5,head->y+5);
}
char length(void)                       /*返回蛇长度*/
{
    SNAKE *temp;
    char n=0;
    for(temp=head;temp!=NULL;temp=temp->next)n++;
    return n;
}
void direction(char speed)              /*游戏主体--键盘响应，蛇移动及游戏控制*/
{
    SNAKE *ps;
   {
        int i;
        for(i=1;i<=3*speed;i++)        /* 3*speed*delay(1000)=无控制下蛇移动一步所需时间 */
        {
            if(kbhit())                 /* 控制信息 */
            {
                while(bioskey(1)==0);
                switch(bioskey(0))
                {
                    case ESC:
                    {
                        gameres=-1;     /*结束游戏，但不出现game over*/
                        closegraph();
                        break;
                    }
                    case ENTER:         /*暂停游戏*/
                    {
                        getch();
                        break;
                    }
                    case LEFT:
                    if(MOVIE==direx)break; /* if使蛇头不能反向 */
                    else
                    {
                        direx=-MOVIE;
                        direy=0;
                        i=3*speed;
                        break;          /* left direx=-MOVIE direy=0 */
                    }
                    case RIGHT:
                    if(-MOVIE==direx)break;
                    else
                    {
                        direx=MOVIE;
                        direy=0;
                        i=3*speed;
                        break;          /* right direx=MOVIE direy=0 */
                    }
                    case UP:
                    if(MOVIE==direy)break;
                    else
                    {
                        direx=0;
                        direy=-MOVIE;
                        i=3*speed;
                        break;          /* up direx=0 direy=MOVIE */
                    }
                    case DOWN:
                    if(-MOVIE==direy)break;
                    else
                    {
                        direx=0;
                        direy=MOVIE;
                        i=3*speed;
                        break;          /* down direx=0 direy=-MOVIE */
                    }
                    default:break;
                }
            }
        delay(1000);
        }
    }
    setfillstyle(0,LIGHTGRAY);                        /* 擦掉蛇尾巴 */
    bar(tail->x-5,tail->y-5,tail->x+5,tail->y+5);
    for(ps=tail;ps->previous!=NULL;ps=ps->previous)   /* 蛇身坐标重置 */
    {
        ps->x=ps->previous->x;
        ps->y=ps->previous->y;
    }
    head->x+=direx;                                   /* 蛇头坐标重置 */
    head->y+=direy;
    setcolor(BLUE);                             /* 画蛇 */
    rectangle(head->x-5,head->y-5,head->x+5,head->y+5);
    for(ps=head->next;ps!=NULL;ps=ps->next)
    {
        setcolor(GREEN);
        rectangle(ps->x-5,ps->y-5,ps->x+5,ps->y+5);
    }
}
void profood(void)                     /*BUG发生概率很小*/
{
    char i;
    struct{
                unsigned int food:2;   /*检测食物生成情况，错误 1，正确 0 */
                unsigned int wall:2;
                unsigned int snake:2;
                unsigned int info:2;
            }flag={0,0,0,0};
    for(i=0;i<3;i++)                   /* 检查是否同时存在3个食物，若不是则生成 */
    {
        if(0==f[i].exist)              /* 如果食物不存在，生成 */
       {
            char j;
            WALL *t;
            SNAKE *temp;
            f[i].x=random(MAXX);       /* 生成食物信息 */
            f[i].y=random(MAXY);
            do
            {
                for(j=0;j<3;j++)           /*避免食物重叠*/
                    if(j!=i)
                    while(f[j].x-11<=f[i].x&&f[j].x+11>=f[i].x&&f[j].y-11<=f[i].y&&f[j].y+11>=f[i].y)
                    {
                        f[i].x=random(MAXX);
                        f[i].y=random(MAXY);          /*清除重叠的食物图像*/
                        setfillstyle(0,0);
                        bar(f[i].x-5,f[i].y-5,f[i].x+5,f[i].y+5);
                        bar(f[j].x-5,f[j].y-5,f[j].x+5,f[j].y+5);
                        setfillstyle(1,YELLOW);       /*重画应该存在的食物*/
                        bar(f[j].x-5,f[j].y-5,f[j].x+5,f[j].y+5);
                        flag.food=0;
                    }
                while(f[i].x-5<=2||f[i].x+5>MAXX-2||f[i].y-5<25||f[i].y+5>MAXY-11)
                {
                    f[i].x=random(MAXX);              /* 避免食物生成在基本墙或游戏信息区 */
                    f[i].y=random(MAXY);
                    setfillstyle(1,RED);              /*恢复食物与墙重叠时消失的部分*/
                    for(wallp=wallhead;wallp!=NULL;wallp=wallp->next)
                        bar(wallp->x1,wallp->y1,wallp->x2,wallp->y2);
                    flag.info=0;
                }
                for(temp=head;temp!=NULL;temp=temp->next) /* 避免食物生成在蛇里 */
                    while(f[i].x-11<=temp->x&&f[i].x+11>=temp->x&&f[i].y-11<=temp->y&&f[i].y+11>=temp->y)
                    {
                        f[i].x=random(MAXX);
                        f[i].y=random(MAXY);
                        flag.snake=0;
                    }
                for(t=wallhead;t!=NULL;t=t->next)         /* 避免食物生成在墙里 */
                    while(t->x2>=f[i].x-6&&t->x1<=f[i].x+6&&t->y2>=f[i].y-6&&t->y1<=f[i].y+6)
                    {
                        f[i].x=random(MAXX);
                        f[i].y=random(MAXY);
                        setfillstyle(1,RED);              /*恢复食物与墙重叠时消失的部分*/
                        for(wallp=wallhead;wallp!=NULL;wallp=wallp->next)
                            bar(wallp->x1,wallp->y1,wallp->x2,wallp->y2);
                        flag.wall=0;
                    }
                for(t=wallhead;t!=NULL;t=t->next)
                    if(t->x2>=f[i].x-6&&t->x1<=f[i].x+6&&t->y2>=f[i].y-6&&t->y1<=f[i].y+6)
                        flag.wall=1;
                for(temp=head;temp!=NULL;temp=temp->next)
                    if(f[i].x-11<=temp->x&&f[i].x+11>=temp->x&&f[i].y-11<=temp->y&&f[i].y+11>=temp->y)
                        flag.snake=1;
                if(f[i].x-5<=2||f[i].x+5>MAXX-2||f[i].y-5<25||f[i].y+5>MAXY-11)
                    flag.info=1;
                for(j=0;j<3;j++)
                    if((j!=i)&&(f[j].x-11<=f[i].x&&f[j].x+11>=f[i].x&&f[j].y-11<=f[i].y&&f[j].y+11>=f[i].y))
                        flag.food=1;
            }while(1==flag.food||1==flag.snake||1==flag.info||1==flag.wall);
            setfillstyle(1,YELLOW);
            bar(f[i].x-5,f[i].y-5,f[i].x+5,f[i].y+5);
            f[i].exist=1;
        }
    }
}
void printwall(char i)        /* 墙的信息 */
{
    wallhead=(WALL *)malloc(sizeof(WALL));
    wallhead->x1=0;           /* 基本墙 */
    wallhead->y1=20;
    wallhead->x2=MAXX;
    wallhead->y2=24;
    wallp=(WALL *)malloc(sizeof(WALL));
    wallhead->next=wallp;
    wallp->x1=0;
    wallp->y1=20;
    wallp->x2=1;
    wallp->y2=MAXY;
    wallp->next=(WALL *)malloc(sizeof(WALL));
    wallp=wallp->next;
    wallp->x1=MAXX-1;
    wallp->y1=20;
    wallp->x2=MAXX;
    wallp->y2=MAXY;
    wallp->next=(WALL *)malloc(sizeof(WALL));
    wallp=wallp->next;
    wallp->x1=0;
    wallp->y1=MAXY-10;
    wallp->x2=MAXX;
    wallp->y2=MAXY;
    switch(i)                 /* 关卡墙 */
    {
        case 1:
        {
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=100;
            wallp->y1=100;
            wallp->x2=110;
            wallp->y2=MAXY-100;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX-110;
            wallp->y1=100;
            wallp->x2=MAXX-100;
            wallp->y2=MAXY-100;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX/2-70;
            wallp->y1=120;
            wallp->x2=MAXX/2+70;
            wallp->y2=130;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX/2-70;
            wallp->y1=MAXY-130;
            wallp->x2=MAXX/2+70;
            wallp->y2=MAXY-120;
            wallp->next=NULL;           
            break;
        }
        case 2:
        {
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=60;
            wallp->y1=70;
            wallp->x2=70;
            wallp->y2=MAXY-70;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX-70;
            wallp->y1=70;
            wallp->x2=MAXX-60;
            wallp->y2=MAXY-70;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=135;
            wallp->y1=100;
            wallp->x2=MAXX-135;
            wallp->y2=110;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=135;
            wallp->y1=MAXY-110;
            wallp->x2=MAXX-135;
            wallp->y2=MAXY-100;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX-217;
            wallp->y1=60;
            wallp->x2=MAXX-210;
            wallp->y2=195;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=210;
            wallp->y1=MAXY-195;
            wallp->x2=217;
            wallp->y2=MAXY-60;
            wallp->next=NULL;
            break;
        }
        case 3:
        {
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=88;
            wallp->y1=150;
            wallp->x2=95;
            wallp->y2=MAXY-150;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX-95;
            wallp->y1=150;
            wallp->x2=MAXX-88;
            wallp->y2=MAXY-150;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=165;
            wallp->y1=90;
            wallp->x2=MAXX-165;
            wallp->y2=100;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=165;
            wallp->y1=MAXY-100;
            wallp->x2=MAXX-165;
            wallp->y2=MAXY-90;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX/2-60;
            wallp->y1=55;
            wallp->x2=MAXX/2-55;
            wallp->y2=195;
            wallp->next=(WALL *)malloc(sizeof(WALL));
            wallp=wallp->next;
            wallp->x1=MAXX/2+55;
            wallp->y1=MAXY-195;
            wallp->x2=MAXX/2+60;
            wallp->y2=MAXY-55;
            wallp->next=NULL;
            break;
        }
        default :break;
    }
    setfillstyle(1,RED);    /*画墙*/
    for(wallp=wallhead;wallp!=NULL;wallp=wallp->next)
        bar(wallp->x1,wallp->y1,wallp->x2,wallp->y2);
}
void eat(void)
{
    int i;
    WALL *t=wallhead;
    SNAKE *temp=(SNAKE *)malloc(sizeof(SNAKE));
    for(i=0;i<3;i++)        /* 检查每个食物，如果吃到食物，尾节点+1 */
    {
        if(checkin(*head,f[i].x,f[i].y)||checkin(*head,f[i].x-5,f[i].y-5)||checkin(*head,f[i].x-5,f[i].y+5)||checkin(*head,f[i].x+5,f[i].y-5)||checkin(*head,f[i].x+5,f[i].y+5))
        {
            setfillstyle(0,LIGHTGRAY);
            bar(f[i].x-5,f[i].y-5,f[i].x+5,f[i].y+5);
            f[i].exist=0;
            setcolor(BLUE);
            rectangle(head->x-5,head->y-5,head->x+5,head->y+5);
            temp->x=tail->x*2-tail->previous->x;
            temp->y=tail->y*2-tail->previous->y;
            tail->next=temp;
            temp->previous=tail;
            temp->next=NULL;
            tail=temp;
        }
    }
    for(t=wallhead;t!=NULL;t=t->next)     /* 如果撞墙，gameover */
        if(checkwall(*t,head->x,head->y)||checkwall(*t,head->x-5,head->y-5)||checkwall(*t,head->x+5,head->y-5)||checkwall(*t,head->x-5,head->y+5)||checkwall(*t,head->x+5,head->y+5))gameres=-1;
    for(temp=head->next->next->next;temp!=NULL;temp=temp->next) /* 如果咬自己，gameover */
        if(checkin(*head,temp->x,temp->y))gameres=-1;
}
char advance(void)                        /* 返回0 游戏继续，1 下一关or通全关 */
{                                         /* 换关 */
    if(1==screeni&&(length()-LEN-1)>=FIR)
        {
            gameres=1;                    /* 说明游戏胜利 */
            screeni++;                    /* 下一关 */
             /* 重置所有信息 */
            for(ps=head;head!=NULL;free(ps))   /* 清空蛇信息 */
            {
                ps=head;
                head=ps->next;
            }
            for(wallp=wallhead;wallhead!=NULL;free(wallp))
            {                                  /* 清空墙信息 */
                wallp=wallhead;
                wallhead=wallp->next;
            }
            f[0].exist=0;
            f[1].exist=0;
            f[2].exist=0;
            return 1;
        }
    else if(2==screeni&&(length()-LEN-1)>=SEC)
        {
            gameres=1;                      
            screeni++;                     
            for(ps=head;head!=NULL;free(ps))
            {
                ps=head;
                head=ps->next;
            }
            for(wallp=wallhead;wallhead!=NULL;free(wallp))
            {
                wallp=wallhead;
                wallhead=wallp->next;
            }
            f[0].exist=0;
            f[1].exist=0;
            f[2].exist=0;
            return 1; 
        }
    else if(3==screeni&&(length()-LEN-1)>=THR)
        {                                    /* 通全关，置空 */
            gameres=2;                       /* 通全关退出 */
            for(ps=head;head!=NULL;free(ps))
            {
                ps=head;
                head=ps->next;
            }
            for(wallp=wallhead;wallhead!=NULL;free(wallp))
            {
                wallp=wallhead;
                wallhead=wallp->next;
            }
            f[0].exist=0;
            f[1].exist=0;
            f[2].exist=0;
            return 1; 
        }
    else return 0;
}
void information(char mode)      /*游戏信息区*/
{
    char flag;
    char score[4];
    settextstyle(0,0,2);
    setcolor(8);
    outtextxy(5,0,"Score:");     /*得分情况*/
    if((length()-LEN-1)<10)
    {
        flag=1;
        score[0]=(length()-LEN-1)+'0';
        score[1]='\0';
    }
    else if((length()-LEN-1)<100)
    {
        flag=2;
        score[0]=(length()-LEN-1)/10+'0';
        score[1]=(length()-LEN-1)%10+'0';
        score[2]='\0';
    }
    else
    {
        flag=3;
        score[0]=(length()-LEN-1)/100+'0';
        score[1]=(length()-LEN-1)/10-(length()-LEN-1)/100*10+'0';
        score[2]=(length()-LEN-1)%10+'0';
        score[3]='\0';
    }
    setfillstyle(0,0);
    switch(flag)
    {
        case 1:bar(110,0,185,15);outtextxy(110,0,score);break;
        case 2:bar(110,0,185,15);outtextxy(110,0,score);break;
        case 3:bar(110,0,185,15);outtextxy(110,0,score);break;
    }
    if(0==mode)
    {   
        outtextxy(170,0,"Aim:");
        outtextxy(310,0,"Screen:");
        switch(screeni)
        {
            case 1:outtextxy(235,0,"10");outtextxy(405,0," 1 ");break;
            case 2:outtextxy(235,0,"20");outtextxy(405,0," 2 ");break;
            case 3:outtextxy(235,0,"30");outtextxy(405,0," 3 ");break;
        }
    }
    else
    {
        outtextxy(240,0,"Aim:");
        outtextxy(305,0,"100");
    }
    outtextxy(475,0,"Mode:");    /*游戏模式*/
    switch(speed)
    {
        case HELL:outtextxy(560,0,"DIF");break;
        case DIF:outtextxy(560,0,"NOR");break;
        case NOR:outtextxy(560,0,"EASY");break;
        case LOSE:outtextxy(560,0,"HELL");break;
    }
}
void beforegame(void)               /* 选择游戏模式，游戏难度 */
{
    settextstyle(0,0,2);
    setcolor(8);
    outtextxy(20,160,"Please choose the game mode:");
    outtextxy(200,200,"1.Classic");
    outtextxy(200,230,"2.Stage");
    switch(getch())
    {
        case 0x31:mode=1;break;     /*按数字键“1”时选择游戏模式1*/
        case 0x32:mode=0;break;
        default:break;
    }
    cleardevice();
    outtextxy(20,160,"Please choose the gamedifficulty:");
    outtextxy(200,200,"1.Easy");
    outtextxy(200,230,"2.Normal");
    outtextxy(200,260,"3.Expert");
    outtextxy(200,290,"4.Hell");
    switch(getch())
    {
        case 0x31:speed=NOR;break;  /*按数字键“1”时选择游戏难度 Easy*/
        case 0x32:speed=DIF;break;
        case 0x33:speed=HELL;break;
        case 0x34:speed=LOSE;break;
        default:break;
    }
}
void playgame(char mode)
{
    switch(mode)
    {
        case 0:
        {
            do
            {   
                newgame();
                printwall(screeni);
                information(mode);
                getch();
                getch();                /*按方向键开始游戏*/
                do
                {
                    information(mode);
                    direction(speed);   /* 蛇以speed的速度朝（direx，direy）矢量运动 */
                    eat();              /* 吃到食物？咬到自己？撞墙？ */
                    profood();          /* 食物不到3个，则生成 */
                }while(0==advance()&&0==gameres);/* advance()监视游戏情况,1表过关,0表正常*/
            }while(gameres!=-1&&gameres!=2);
            return;             /* gameres=0 游戏继续,-1 游戏失败or退出,1 下一关,2 通全关 */
        }
        case 1:
        {
            newgame();
            printwall(0);
            information(mode);
            getch(); 
            getch();
            do
            {
                information(mode);
                direction(speed);
                eat();
                profood();
                if(100==length()-LEN-1)gameres=2;
            }while(0==gameres);
            return;
        }
    }
}
void closegame(void)         /*游戏结束监视，失败结束or通全关结束，若按ESC键直接跳过*/
{
    char i;
    cleardevice();
    settextstyle(0,0,8);
    setcolor(RED);
    if(2==gameres)outtextxy(50,180,"YOU WIN !");
    else outtextxy(50,180,"GAME OVER");
    for(i=0;i<10;i++)delay(30000);
    closegraph();
}
int main()
{
    graphbegin();     /*初始驱动*/
    randomize();      /*随机数发生器，必须放在main中*/
    beforegame();     /* 选择游戏模式，游戏难度 */
    playgame(mode);   /*玩游戏*/
    closegame();      /*结束游戏*/
    return 0;
}
