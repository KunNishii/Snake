#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <time.h>
#include <windows.h>

using namespace std;

typedef unsigned int uint;
typedef pair<int, int> CurPos;

#define CONS_WD 80
#define CONS_HT 25
#define POINT_FOOD 100
#define EXT_FOOD 5
#define POINT_TIME 10
#define TERM 10
#define N_FOOD 10

//色定義
#define COL_BLACK 0x00
#define COL_DARK_BLUE 0x01
#define COL_DARK_GREEN 0x02
#define COL_DARK_CYAN 0x03
#define COL_DARK_RED 0x04
#define COL_DARK_VIOLET 0x05
#define COL_DARK_YELLOW 0x06
#define COL_GRAY 0x07
#define COL_LIGHT_GRAY 0x08
#define COL_BLUE 0x09
#define COL_GREEN 0x0a
#define COL_CYAN 0x0b
#define COL_RED 0x0c
#define COL_VIOLET 0x0d
#define COL_YELLOW 0x0e
#define COL_WHITE 0x0f
#define COL_INTENSITY 0x08
#define COL_RED_MASK 0x04
#define COL_GREEN_MASK 0x02
#define COL_BLUE_MASK 0x01

//文字色指定　for Windows Console
void setColor(int col) {
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (col & COL_INTENSITY)
		attr |= FOREGROUND_INTENSITY;
	if (col & COL_RED_MASK)
		attr |= FOREGROUND_RED;
	if (col & COL_GREEN_MASK)
		attr |= FOREGROUND_GREEN;
	if (col & COL_BLUE_MASK)
		attr |= FOREGROUND_BLUE;
	SetConsoleTextAttribute(hCons, attr);
}
void setColor(int fg, int bg) {
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (fg & COL_INTENSITY)
		attr |= FOREGROUND_INTENSITY;
	if (fg & COL_RED_MASK)
		attr |= FOREGROUND_RED;
	if (fg & COL_GREEN_MASK)
		attr |= FOREGROUND_GREEN;
	if (fg & COL_BLUE_MASK)
		attr |= FOREGROUND_BLUE;

	if (bg & COL_INTENSITY)
		attr |= BACKGROUND_INTENSITY;
	if (bg & COL_RED_MASK)
		attr |= BACKGROUND_RED;
	if (bg & COL_GREEN_MASK)
		attr |= BACKGROUND_GREEN;
	if (bg & COL_BLUE_MASK)
		attr |= BACKGROUND_BLUE;
	SetConsoleTextAttribute(hCons, attr);
}
//カーソル位置指定　for Windows Console
void setCursorPos(int x, int y) {
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hCons, pos);
}
bool isKeyPressed(int vKey) {
	return(GetAsyncKeyState(vKey) & 0x8000) != 0;
}

void add_foods(vector<CurPos>& foods, int cnt) {
	for (int i = 0; i < cnt; ++i) {
		int x = rand() % (CONS_WD - 2) + 1;
		int y = rand() % (CONS_HT - 4) + 2;
		foods.push_back(CurPos(x, y));
	}
}

void print_foods(const vector<CurPos>& foods) {
	setColor(COL_GREEN);
	for (uint i = 0; i < foods.size(); ++i) {
		setCursorPos(foods[i].first, foods[i].second);
		cout << "$";
	}
}
bool check_foods(vector<CurPos>& foods, int x, int y) {
		//for(int i= (int)foods.size(); --i >=0;){
		for (uint i = 0; i < foods.size(); ++i) {
			if (foods[i].first == x && foods[i].second == y) {
				foods.erase(foods.begin() + i); 
				return true;
			}
		}
		return false;
}
void print_field()
{
	setColor(COL_YELLOW, COL_YELLOW);
	setCursorPos(0, 1);
	for (int i = 0; i < CONS_WD; ++i) {
		cout << " ";
	}
	for (int y = 2; y < CONS_HT - 2; ++y) {
		setCursorPos(0, y);
		cout << " ";
		setCursorPos(CONS_WD - 1, y);
		cout << " ";
	}
	setCursorPos(0, CONS_HT - 2);
	for (int i = 0; i < CONS_WD; ++i) {
		cout << " ";
	}
	setColor(COL_YELLOW, COL_BLACK);
	for (int y = 0; y < CONS_HT - 4; ++y) {
		setCursorPos(1, y + 2);
		for (int x = 0; x < CONS_WD - 2; ++x) {
			cout << " ";
		}
	}
	setCursorPos(0, CONS_HT - 1);
	for (int i = 0; i < CONS_WD - 1; ++i) {
		cout << " ";
	}
}
void print_score(int score, int bodyLength) {
	setColor(COL_WHITE);
	setCursorPos(0, 0);
	cout << "SCORE:";
	cout.width(6);
	cout << bodyLength;
}
void print_snake(const deque<CurPos>& snake) {
	setColor(COL_BLUE);
	for (uint i = 1; i < snake.size(); ++i) {
		setCursorPos(snake[i].first, snake[i].second);
		cout << "*";
	}
	setColor(COL_VIOLET);
	setCursorPos(snake[0].first, snake[0].second);
	cout << "@";

}

void update_snake(deque<CurPos>& snake, int x, int y, bool extend) {
	snake.push_front(CurPos(x, y));
	if (!extend) {
		setCursorPos(snake.back().first, snake.back().second);
		cout << " ";
		snake.pop_back();
	}

}

bool collapsed(const deque<CurPos>& snake) {
	int x = snake[0].first;
	int y = snake[0].second;
	if (x <= 0 || x > CONS_WD - 1 || y <= 1 || y >= CONS_HT - 2)
		return true;
	for (uint i = 1; i < snake.size(); ++i) {
		if (snake[i].first == x && snake[i].second == y)
			return true;
	}
	return false;
}

bool check_body(const deque<CurPos>& snake, int dx, int dy) {
	if (snake.size() < 2) return true;
	return snake[0].first + dx != snake[1].first
		|| snake[0].second + dy != snake[1].second;
}
int main() {
	srand((int)time(0));
	//string buffer;
	for (;;) {
		int score = 0;
		int x = CONS_WD / 2;
		int y = CONS_HT / 2;
		int dx = 0, dy = 1;
		deque<CurPos> snake;
		snake.push_front(CurPos(x, y - 2));
		snake.push_front(CurPos(x, y - 1));
		snake.push_front(CurPos(x, y));
		vector<CurPos> foods;
		add_foods(foods, N_FOOD);
		print_field();
		print_foods(foods);
		print_snake(snake);
		print_score(score, snake.size());
		int eating = 0;
		for (int cnt = 0;; ++cnt) {
			if ((isKeyPressed('H') || isKeyPressed(VK_LEFT)) && check_body(snake, -1, 0)) {
				dx = -1;
				dy = 0;
			}
			else if ((isKeyPressed('J') || isKeyPressed(VK_DOWN)) && check_body(snake, 0, 1)) {
				dx = 0;
				dy = 1;
			}
			else if ((isKeyPressed('K') || isKeyPressed(VK_UP)) && check_body(snake, 0, -1)) {
				dx = 0;
				dy = -1;
			}
			else if ((isKeyPressed('L') || isKeyPressed(VK_RIGHT)) && check_body(snake, 1, 0)) {
				dx = 1;
				dy = 0;
			}
			x += dx;
			y += dy;
			bool extend = cnt % TERM == TERM - 1;
			if (extend) {
				score += POINT_TIME;
				add_foods(foods, 1);
			}
			if (eating) {
				--eating;
				extend = true;
			}
			update_snake(snake, x, y, extend);
			if (check_foods(foods, x, y)) {
				cout << (char)0x07;
				eating = EXT_FOOD;
				score += POINT_FOOD;
			}
			print_foods(foods);
			print_snake(snake);
			print_score(score, snake.size());
			if (collapsed(snake)) {
				cout << (char)0x07 << (char)0x07 << (char)0x07;
				break;
			}
			Sleep(200);
		}
		setCursorPos(0, CONS_HT - 1);
		cout << "try again ?\n Yes → Press'Y'\n No  → Press'N'";
		for (;;) {
			if (isKeyPressed('Y'))
				break;
			if (isKeyPressed('N'))
				return 0;
		}
	}
	return 0;
}