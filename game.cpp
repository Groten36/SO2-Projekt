#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <functional> 
std::mutex m;
std::mutex sh;
std::mutex shv2;
std::condition_variable shot;
bool isShot=false;
 char **board;
 char **rectangle(int x,int y,  char **board){
	char *pointer;	
	board=new  char*[x];
	for(int i=0;i<y;i++){
		board[i]=new  char[y];
	}
	for(int i=0;i<x;i++)
	{
		board[i][0]='#';
		
		board[i][y-1]='#';
		
	}
	for(int i=0;i<y;i++){

		board[0][i]='#';
		
		board[x-1][i]='#';
		
	}
	for(int i=1;i<x-1;i++){
		for(int j=1; j<y-1;j++){
			board[i][j]=' ';
		}
	}
	
	
	
	return board;	
}
void timer(){
	int i=0;

	while(!isShot){
	
	sleep(1);
	i++;

	}


	mvprintw(52,20,"Your time:");
	mvprintw(52,35,"%d",i);
}
void print(int x,int y,char **board){
	char* pointer;
	for(int i=0; i<x;i++){
		for(int j=0;j<y;j++){
			pointer=&board[i][j];
			mvprintw(i,j,pointer);
			refresh();
		}
	}
}

void move_down(int x, int y,char type,   char **board){
{	std::lock_guard<std::mutex> guard(sh);
	if(board[x+1][y]=='X')
		
			isShot=true;

	shot.notify_one();
	}

	board[x][y]=' ';
	board[x+1][y]=type;
	print(50,150,board);
	
};


void move_up(int x, int y,char type,  char **board){
{	std::lock_guard<std::mutex> mlock(sh);
	if(board[x+1][y]=='X')
			isShot=true;
		
	shot.notify_one();
}

	board[x][y]=' ';
	board[x-1][y]=type;
	print(50,150,board);
	
};

void move_right(int x, int y,char type,  char **board){
{	std::lock_guard<std::mutex> mlock(sh);
	if(board[x+1][y]=='X')
			isShot=true;
	shot.notify_one();
}

	board[x][y]=' ';
	board[x][y+1]=type;
	print(50,150,board);
	
};

void move_left(int x, int y,char type, char **board){
	{std::lock_guard<std::mutex> guard(sh);
	if(board[x+1][y]=='X') 
			isShot=true;
		
	shot.notify_one();
}

	board[x][y]=' ';
	board[x][y-1]=type;
	print(50,150,board);
	
};

void enemy(int x, int y,  char **board){

	char symbol='X';
	board[x][y]=symbol;
	print(50,150,board);
	int i=0;
	int dir;
	while(!isShot){

	dir=rand()%4+1;
	m.lock();
	
	switch(dir){
	
	
	case 1:if(x!=1){ move_up(x,y,symbol,board);x--;}break;
	case 2:if(y!=148){ move_right(x,y,symbol,board);y++;}break;
	case 3:if(x!=48){ move_down(x,y,symbol,board);x++;}break;
	case 4:if(y!=1){move_left(x,y,symbol,board);y--;}break;
	std::unique_lock<std::mutex> mlock(sh);	
	shot.wait(mlock);
	}

	m.unlock();
	i++;

	sleep(1);

	};	
	
};
void gun(int x, int y, char **board){

	char symbol='O';
	board[x][y]=symbol;
	print(50,150,board);
	
	int i = 0;
	int dir;
	while(true){

	dir=getch();
	if(isdigit(dir))
		dir=dir-'0';
	m.lock();
	switch(dir){
	case 8: if(x!=1){move_up(x,y,symbol,board);x--;}break;
	case 6: if(y!=148){move_right(x,y,symbol,board);y++;}break;
	case 2: if(x!=48){move_down(x,y,symbol,board);x++;}break;
	case 4:if(y!=1){move_left(x,y,symbol,board),y--;}break;
	shot.notify_one();		
	}
	m.unlock();
	i++;
	}

}

bool check(int x, int y,int newX, int newY, char** board){

			
	if(board[x][y]=='O'&&board[newX][newY])
		return true;
	else
		return false;
}
int main(int argc, char * argv[]){
	std::vector<std::thread> v;
	int xstart=rand()%146+1;
	int ystart=rand()%46+1;
//	bool sh1=false;
//	bool sh2=false;
//	bool sh3=false;
//	bool sh4=false;
//	bool sh5=false;
	initscr();
	keypad( stdscr, TRUE );
	noecho();	
	char **board=rectangle(50,150,board);
	std::thread t1(enemy,ystart,xstart,board);
//	std::thread t2(enemy,31,25,board,issecond);
//	std::thread t3(enemy,10,50,board);
//	std::thread t4(enemy,40,120,board,isecond);
//	std::thread t5(enemy,48,100,board);
	std::thread t6(gun,15,80,board);
	std::thread t7(timer);
	print(50,150,board);
//	crush=false;	
	t1.join();
//	t2.join();
//	t3.join();
//	t4.join();
//	t5.join();
	t6.join();
	t7.join();
	getch();
	endwin();
	return 0;
}
