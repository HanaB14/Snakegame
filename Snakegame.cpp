#include <iostream>
#include <windows.h>
#include <conio.h>
#include <deque>
#include <ctime>
#include <string>
#include <stdexcept> 

using namespace std;

class Snake
{
	public:
		char image;
		short x,y;
};

class snakeGame
{
	public:
		deque <Snake> snake;
		short height = 40;
		short width = 120;
		HANDLE hOut; COORD pos;
		char direction;
		bool eatFood=false;
		bool beg;
		double speed = 200;
		short food_x;
		short food_y;
		int score=0;
		snakeGame();

void gotoxy(short x, short y)
{
	hOut = GetStdHandle (STD_OUTPUT_HANDLE);
	pos = {x,y};
	SetConsoleCursorPosition (hOut, pos);
}

void Map ()
{
	for (int i=0; i<width; i=i+2)		cout << "¡õ";
	gotoxy (0,1);
	for (int i=1; i<height;i++)			cout << "¡õ"<<endl;
	for (int i=1; i<height;i++)
	{
		gotoxy (width-2,i);
		cout << "¡õ";
	}
	gotoxy (0,height-1);
	for (int i=0; i<width;i=i+2)		cout << "¡õ";
	cout << endl;
}

void HideCursor ()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);
}


void initSnake()
{
snake.push_back({'+',width/2,height/2});
for (int i=0; i<2; ++i)
snake.push_back({'+',width/2,(height/2+i+1)});
}

void createFood()
{
	food_x = rand()%(width-4)+2;
	food_y = rand()%(height-2)+1;
	gotoxy(food_x,food_y);
	cout <<'#';
}

void printSnake()
{
	deque<Snake>::const_iterator iter=snake.begin();
	for (; iter<snake.end();++iter)
	{
		gotoxy(iter->x,iter->y);
		cout << iter->image;
	}
}

void clearSnake(Snake &tail)
{
	gotoxy(tail.x,tail.y);
	cout << ' ';
}

void userInput()
{
	switch (char ch=getch())
	{
		case 'w': if (direction != 's')		direction = ch; 	break;
		case 'a': if (direction != 'd')		direction = ch; 	break;
		case 's': if (direction != 'w')		direction = ch; 	break;
		case 'd': if (direction != 'a')		direction = ch; 	break;
		default: break;
	}
}

void foodEaten()
{
	createFood();
	eatFood = true;
	speed = speed * 0.6;
	score++;
}

void judgeCrash()
{
	if (snake.front().x==1 || snake.front().x==width-2 || snake.front().y==0 || snake.front().y==height-1)
	{
		gotoxy(1, height+1);
		cout << "Game over. Your score is : "<<score<< " (Press any key to exist)"<<endl;
		direction = getch();
		runtime_error quit ("Bye Bye");
		throw quit;
	}
}
};

snakeGame::snakeGame()
{
	HideCursor();
	srand (time(0));
	beg=true;
	Snake tmp1, tmp2;
	while (1)
	{
		if (beg)
		{
			Map ();
			direction = getch();
			initSnake();
			createFood();
			beg=eatFood=false;
		}
		tmp1= snake.front();
		tmp2= snake.back();
		snake.pop_back();
		if (eatFood)
		{
			tmp2.image='+' ;
			snake.push_back(tmp2);
			eatFood = false;
		}
		else		clearSnake(tmp2);
		if (direction == 's')		tmp1.y++;
		else if (direction == 'w')		tmp1.y--;
		else if (direction == 'a')		tmp1.x--;
		else 		tmp1.x++;
		
		try {judgeCrash();}
		
		catch (runtime_error &quitSignal)
		{
			throw quitSignal;
		}
		
		snake.front().image = '+';
		snake.push_front(tmp1);
		printSnake();
		Sleep(speed+20);
		if (tmp1.x==food_x && tmp1.y==food_y)
		foodEaten();
		
		if (kbhit())
		userInput();
	}
}

int main ()
{
	system("mode con cols=120 lines=45");

	try 
	{
		snakeGame game;
	}

	catch (runtime_error & gameEnd)
	{
		cout << gameEnd.what();
		getch();
	}
}
