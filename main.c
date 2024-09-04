#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 50
#define HEIGHT 25
#define HIGHSCORE_FILE "highscore.txt"


int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int highScore = 0;
char highScoreName[50] = "None";
int speed = 100;
enum eDirection {
STOP = 0, LEFT, RIGHT, UP, DOWN

};
enum eDirection dir;


void Setup() {
	
	gameOver = 0;
	dir = STOP;
	x = WIDTH / 2;
	y = HEIGHT / 2;
	fruitX = rand() % WIDTH;
	fruitY = rand() % HEIGHT;
	score = 0;
	nTail = 0;
	
}


void Draw(HANDLE hConsole) {
	
	COORD cursorPos = {0, 0};
	SetConsoleCursorPosition(hConsole, cursorPos);
	
	printf("  ");
	for (int i = 0; i < WIDTH; i++) printf("#");
		printf("\n");
	
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (j == 0)
			printf("#");
			if (i == y && j == x)
				printf("O");
			else if (i == fruitY && j == fruitX)
				printf("F");
			else {
				int print = 0;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						printf("o");
						print = 1;
					}
				}
				if (!print)
					printf(" ");
			}
			
			if (j == WIDTH - 1)
				printf("#");
		}
		printf("\n");
	}
	
	for (int i = 0; i < WIDTH + 2; i++)
		printf("#");
	printf("\n");
	printf("Score: %d     High Score: %d by %s\n", score, highScore, highScoreName);
}


void LoadHighScore() {
	
	FILE *file = fopen(HIGHSCORE_FILE, "r");
	if (file) {
		fscanf(file, "%d %s", &highScore, highScoreName);
		fclose(file);
	}
}


void SaveHighScore() {
	
	FILE *file = fopen(HIGHSCORE_FILE, "w");
	if (file) {
		fprintf(file, "%d %s", highScore, highScoreName);
		fclose(file);
	}
}


void Input() {
	
	if (_kbhit()) {
		switch (_getch()) {
			case 'a':
				if (dir != RIGHT) dir = LEFT;
				break;
			case 'd':
				if (dir != LEFT) dir = RIGHT;
				break;
			case 'w':
				if (dir != DOWN) dir = UP;
				break;
			case 's':
				if (dir != UP) dir = DOWN;
				break;
			case 'x':
				gameOver = 1;
				break;
			case '+':
				if (speed > 10) speed -= 10;
				break;
			case '-':
				if (speed < 200) speed += 10;
				break;
		}
	}
}


void Logic() {
	
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	
	switch (dir) {
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
	}
	
	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
		gameOver = 1;
		
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = 1;
			
	if (x == fruitX && y == fruitY) {
		score += 10;
		fruitX = rand() % WIDTH;
		fruitY = rand() % HEIGHT;
		nTail++;
	}
}




int main(int argc, char *argv[]) {
	
	LoadHighScore();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char playAgain;
	do {
		Setup();
		while (!gameOver) {
			Draw(hConsole);
			Input();
			Logic();
			Sleep(speed);
		}
		
		if (score > highScore) {
			highScore = score;
			printf("New High Score! Enter your name: ");
			fflush(stdin);
			fgets(highScoreName, sizeof(highScoreName), stdin);
			highScoreName[strcspn(highScoreName, "\n")] = 0;
			SaveHighScore();
		}
		
		printf("Game Over! Do you want to play again? (y/n): ");
		playAgain = _getch();
	}
	while (playAgain == 'y' ||playAgain == 'Y');
		
	return 0;
}
