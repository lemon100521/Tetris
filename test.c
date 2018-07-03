#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include "./keyboard/keyboard.h"

#define BC 7
//#define FC 5
int FC=0;
#define W 10
#define H 20

int cur = 0;
int score = 0;

struct data{
  int x;//lie
  int y;//hang
};
struct data t={5,0};//tuxingdeweizhi
int backgroud[H][W]={};//baocunbeijingdeshuzu
struct shape {
  int s[5][5];
};




struct shape shape_arr[7]={
   {
     0, 0, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
    },
    { 
     0, 0, 0, 0, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
    },
   { 
     0, 0, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
    },
   { 
     0, 0, 0, 0, 0,
     0, 0, 1, 1, 0,
     0, 1, 1, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
    },
   { 
     0, 0, 0, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 1, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
    },
    { 
     0, 0, 0, 0, 0,
     0, 1, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
    },
   { 
     0, 0, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 0, 0,
    },
};

void drow_element(int x,int y,int c){
   x *=2;//xzhanyonglainggezifu
   x++;
   y++;
   printf("\033[?25l");//yincanggaungbiao
   printf("\033[%d;%dH",y,x);//kognzhigaungbaioweizhi
   printf("\033[3%dm\033[4%dm",c,c);//shezhiyanse
   printf("[]");//shuchufnagkuai
   fflush(stdout);//shuaxinhuanchong
   printf("\033[0m");//gaunbishezhideshuxing
}

//zhuangfangkuaituan
void drow_shape(int x,int y,struct shape p,int c ){

   int i = 0;
   int j = 0;
   for(i=0;i<5;i++){
      for(j=0;j<5;j++){
        if(p.s[i][j]!=0){
              drow_element(x+j,y+i,c);
         }
      }
   }
}

void drow_back(void){
    int i = 0;
    int j = 0;
    for(i=0;i<H;i++){
      for(j=0;j<W;j++){
         if(backgroud[i][j]==0){
             drow_element(j,i,BC);
           }else{
            // drow_element(j,i,FC);
            drow_element(j,i,backgroud[i][j]);
            }
       // drow_element(j,i,BC);
      }
   }
   printf("\n\tscore:%d\n",score);
 }

void set_back(struct data * t,struct shape p){
       int i = 0;
       int j = 0;
      for(i = 0;i < 5; i++){
       for(j = 0;j < 5;j++){
           if(p.s[i][j]!=0){
          //  backgroud[t->y+i][t->x+j]=1;
          backgroud[t->y+i][t->x+j]=FC;
            }
         }
      }
   }

int can_move(int x ,int y ,struct shape p){

   int i = 0; 
   for(i = 0;i < 5;i++){
    int j = 0;
    for(j = 0; j<5;j++){
       if(p.s[i][j]==0){
           continue; 
       } 
       if(x+j>=W){
        return 0;
        }  
      if(y+i>=H){
        return 0;
        }
       if(x+j<0){
        return 0;
        }
       if(backgroud[y+i][x+j]!=0){
        return 0;
         }
     }
   }
      return 1;
}

void mclean_line(void){
   int i = 0;
   int j = 0;
   int k;
   for(i = 0; i < H;i++){
      int total = 0;
      for(j = 0; j <W;j++){
         if(backgroud[i][j]!=0){
           total++;
         }
       }
       if(total==W){
            score+=10;
            for(k = i;k>0;k--){
              memcpy(backgroud[k],backgroud[k-1],sizeof(backgroud[k]));
            } 
              memset(backgroud[0],0x00,sizeof(backgroud[0]));
        }
   }
}

void tetris_timer(struct data* t){

     drow_shape(t->x,t->y,shape_arr[cur],BC);
     if(can_move(t->x,t->y+1,shape_arr[cur])){
        t->y++;
     }else{
       set_back(t,shape_arr[cur]);
       mclean_line();
       drow_back();
       do{
         FC =rand()%8;
       }while(FC==BC);
       t->y = 0;
       t->x = 0;
       cur = rand()%7;
    }
    // t->y++;
     drow_shape(t->x,t->y,shape_arr[cur],FC);
  }


struct shape turn_90(struct  shape p){
       struct shape t;
       int i = 0;
       int j = 0;
       for(i = 0;i <5;i++){
         for(j = 0;j<5;j++){
           t.s[i][j] = p.s[4-j][i];
          }
       }
       return t;
 }
int tetris(struct data *t){
    int ret = 0;
    int c = get_key();
    if(is_up(c)){
       drow_shape(t->x,t->y,shape_arr[cur],BC);
       shape_arr[cur] = turn_90(shape_arr[cur]);
       if(can_move(t->x,t->y,shape_arr[cur])==0){
         shape_arr[cur] = turn_90(shape_arr[cur]);
         shape_arr[cur] = turn_90(shape_arr[cur]);
         shape_arr[cur] = turn_90(shape_arr[cur]);
      }
       //t->y--;
       drow_shape(t->x,t->y,shape_arr[cur],FC);
     }else if(is_left(c)){
       drow_shape(t->x,t->y,shape_arr[cur],BC);
       if(can_move(t->x-1,t->y,shape_arr[cur]))
       t->x--;
       drow_shape(t->x,t->y,shape_arr[cur],FC);
    }else if(is_right(c)){
       drow_shape(t->x,t->y,shape_arr[cur],BC);
       if(can_move(t->x+1,t->y,shape_arr[cur]))
       t->x++;
       drow_shape(t->x,t->y,shape_arr[cur],FC);
   }else if(is_down(c)){
       drow_shape(t->x,t->y,shape_arr[cur],BC);
       if(can_move(t->x,t->y+1,shape_arr[cur]))
       t->y++;
       drow_shape(t->x,t->y,shape_arr[cur],FC);
   }else if(is_esc(c)){
    ret =1;
   }
}

void handler(int s){
  if(s==SIGALRM){
  tetris_timer(&t);
  }else if(s==SIGINT){
    printf("\033[0m");
    printf("\033[?25h");
    recover_keyboard();
    exit(0);
   }
}
int main(void){
   init_keyboard();
   drow_back();

    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    sigaction(SIGINT,&act,NULL);  
    struct itimerval it;
    
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1;
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL,&it,NULL);
    while(1){
      if(tetris(&t)==1)
       break;
  }
  // int i = 0;
  // for(i;i<6;i++){
  //   drow_shape(5,i,shape_arr[0],FC);
  //   sleep(1);
  //   drow_shape(5,i,shape_arr[0],BC);
  // }
   //for(i;i<10;i++){
      // drow_element(10,i,FC);
      // sleep(1);
     //  drow_element(10,i,BC);
   //}
  printf("\033[?25h");
  recover_keyboard();
}
