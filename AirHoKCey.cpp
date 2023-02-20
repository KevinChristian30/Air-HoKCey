#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define BUFFERSIZE 1000

namespace utility{
	
	const HANDLE OUTPUTHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

  #define FOREGROUND_WHITE 0x07
  #define FOREGROUND_LIGHTBLUE 0x09
	
	void clear(){
		
		#ifdef _WIN32
			system("cls");
		#else
			system("clear");
		#endif
		
	}
	
	void moveConsoleCursor(short x, short y){

		COORD coordinate = {x, y};
		SetConsoleCursorPosition(OUTPUTHANDLE, coordinate);
	
	}

  void setColor(const char* color){

    if (strcmp(color, "FOREGROUND_RED") == 0){
      SetConsoleTextAttribute(OUTPUTHANDLE, FOREGROUND_RED);
    } else if (strcmp(color, "FOREGROUND_GREEN") == 0){
      SetConsoleTextAttribute(OUTPUTHANDLE, FOREGROUND_GREEN);
    } else if (strcmp(color, "FOREGROUND_BLUE") == 0){
      SetConsoleTextAttribute(OUTPUTHANDLE, FOREGROUND_LIGHTBLUE);
    } else if (strcmp(color, "FOREGROUND_WHITE") == 0){
      SetConsoleTextAttribute(OUTPUTHANDLE, FOREGROUND_WHITE);
    } 

  }
	
}

namespace game{
	
	#define H 21
	#define W 80
	
	char map[H + 1][W + 1] = {
	
		"################################################################################",
		"#                                                                              #",
		"#                                                                              #",
		"#                                                                              #",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"#                                                                              #",
		"#                                                                              #",
		"#                                                                              #",
		"################################################################################",
		
	};

  unsigned short int leftScore = 0, rightScore = 0;
	
	void displayMap(){
		
		for (int i = 0; i < H; i++){
			
			for (int j = 0; j < W; j++) printf("%c", map[i][j]);
			puts("");
			
		}
		
	}
	
	class Puck{
		
		public:
			int posX; // Starts from top left
			int posY;
			int height = 5;
      char color[BUFFERSIZE];
			
			Puck(int posX, int posY, const char* color){
				this->posX = posX;
				this->posY = posY;
        strcpy(this->color, color);
			}
			
			void clear(){
				
				for (int i = 0; i < this->height; i++){
					utility::moveConsoleCursor(posX, posY + i);
					printf(" ");
				}
				
			}
		
			void render(){
				
        utility::setColor(color);
				for (int i = 0; i < this->height; i++){
					utility::moveConsoleCursor(posX, posY + i);
					printf("%c", 219);
				}
				utility::moveConsoleCursor(0, 0);
        utility::setColor("FOREGROUND_WHITE");
			
			}
			
			void moveUp(){
				
       	if (this->posY - 1 <= 0) return;
				this->clear();
				this->posY--;
				this->render();
				
			}
			
			void moveDown(){
				
        if (this->posY + this->height + 1 >= H) return;
				this->clear();
				this->posY++;
				this->render();
				
			}
		
	};
	
  Puck left = Puck(4, 8, "FOREGROUND_BLUE");
  Puck right = Puck(W - 5, 8, "FOREGROUND_RED");

	class Ball{
		
		public:
			short int posX, posY, velX, velY;
			
			Ball(int posX, int posY, int velX, int velY){
				this->posX = posX;
				this->posY = posY;
        this->velX = velX;
        this->velY = velY;
			}
			
			void clear(){
				utility::moveConsoleCursor(posX, posY);
				printf(" ");
			}
			
			void render(){
				utility::moveConsoleCursor(posX, posY);
				printf("O");
			}
			
			void updatePosition(){

        // Collide with Wall
        if (posX + velX <= 0 || posX + velX >= W - 1) velX *= -1;
        if (posY + velY <= 0 || posY + velY >= H - 1) velY *= -1;

        // Collide with Puck
        if (posX + velX == left.posX && posY >= left.posY && posY <= left.posY + left.height) velX *= -1;
        if (posY + velY == left.posY && posX + velX == left.posX) velY *= -1;
        if (posY + velY == left.posY + left.height && posX + velX == left.posX) velY *= -1;

        if (posX + velX == right.posX && posY >= right.posY && posY <= right.posY + right.height) velX *= -1;
        if (posY + velY == right.posY && posX + velX == right.posX) velY *= -1;
        if (posY + velY == right.posY + right.height && posX + velX == right.posX) velY *= -1;

				posX += velX;
				posY += velY;

      }

      void reset(){

        clear();
        posX = W / 2;
        posY = H / 2;

      }
		
	};

  Ball ball = Ball(W / 2, H / 2, 1, 1);
	
	void render(){
		
		left.render();	
    right.render();
		
	}
	
	void updatePlayerPosition(){
			
    fflush(stdin);
		char input = getch();
		if (input == 'w' || input == 'W') left.moveUp();
		else if (input == 's' || input == 'S') left.moveDown();
		
	}

  void updateEnemyPosition(){

    srand(time(NULL));
    bool moveUp = rand() % 2 == 0;
    if (moveUp) right.moveUp();
    else right.moveDown(); 

  }
  
  void updateBallPosition(){
  	
    ball.clear();
		ball.updatePosition();
    ball.render();
    utility::moveConsoleCursor(0, 0);
		  	
	}

  void updateScore(){

    if (ball.posX <= 1 && ball.posY >= 4 && ball.posY <= 16) {
      rightScore++;
      ball.velX = 1;
      ball.reset();
    }

    if (ball.posX >= W - 2 && ball.posY >= 4 && ball.posY <= 16){
      leftScore++;
      ball.velX = -1;
      ball.reset();
    }

    utility::moveConsoleCursor(W / 2 - 10, H + 3);
    printf("Left: %d | Right: %d", leftScore, rightScore);
    utility::moveConsoleCursor(0, 0);

  }

  void reset(){

    leftScore = 0;
    rightScore = 0;

    left = Puck(4, 8, "FOREGROUND_BLUE");
    right = Puck(W - 5, 8, "FOREGROUND_RED");
    ball = Ball(W / 2, H / 2, 1, 1);

  }
	
	void loop(){
		
		utility::clear();
		displayMap();
		render();

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    unsigned long long int rightThen = ts.tv_nsec / 1000000;
    unsigned long long int rightNow = rightThen, leftThen = rightThen, leftNow = rightThen;
    unsigned long long int ballNow = rightThen, ballThen = rightThen;

		while (true){
			
      clock_gettime(CLOCK_REALTIME, &ts);
      rightNow = ts.tv_nsec / 1000000;
      leftNow = ballNow = rightNow;

      // if (leftNow - leftThen >= 500){
        if (kbhit()){
          leftThen = leftNow;
          updatePlayerPosition();
        }
      // }

      if (rightNow - rightThen >= 500){
        rightThen = rightNow;
        updateEnemyPosition();
      }

      if (ballNow - ballThen >= 50){
        ballThen = ballNow;
        updateBallPosition();
        updateScore();
        if (leftScore >= 7 || rightScore >= 7) break;
      }

		}
		
    utility::moveConsoleCursor(W / 2 - 5, H + 5);
    if (leftScore > rightScore) printf("Left Won!");
    else printf("Right Won");

    getchar();
	
  }
	
}

namespace exitView{
	
	void exitApp(){
		
		utility::clear();
		puts("Wonderful Things Can be Achieved When There's Teamwork, Hardwork, and Perseverance");
		puts("Press Enter to Exit");
		getchar();
		
		exit(0);
		
	}
	
}

namespace mainMenu{
	
	void display(){
		
		utility::clear();
		printf("Air HoKCey!\n");
		printf("===========\n");
		printf("1. Play\n");
		printf("2. Exit\n");
		printf(">> ");
		
	}
	
	void route(char input[]){
		
		if (strcmp(input, "1") == 0){
			
      game::reset();
			game::loop();
			
		} else if (strcmp(input, "2") == 0){
			
			exitView::exitApp();
			
		}
		
	}
	
	void loop(){
		
		while (true){
			
			display();
			char input[BUFFERSIZE];
			scanf("%s", input);
			getchar();
			
			route(input);
			
		}
		
	}
	
}

int main(){

  HANDLE hInput;
  DWORD prev_mode;
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &prev_mode); 
  SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);
	
	mainMenu::loop();
	
	return 0;
}
