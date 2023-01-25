#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define BUFFERSIZE 1000

namespace utility{
	
	const HANDLE OUTPUTHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	
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
			
			Puck(int posX, int posY){
				this->posX = posX;
				this->posY = posY;
			}
			
			void clear(){
				
				for (int i = 0; i < this->height; i++){
					utility::moveConsoleCursor(posX, posY + i);
					printf(" ");
				}
				
			}
		
			void render(){
				
				for (int i = 0; i < this->height; i++){
					utility::moveConsoleCursor(posX, posY + i);
					printf("O");
				}
				utility::moveConsoleCursor(0, 0);
			
			}
			
			void moveUp(){
				
				this->clear();
				this->posY--;
				this->render();
				
			}
			
			void moveDown(){
				
				this->clear();
				this->posY++;
				this->render();
				
			}
		
	};
	
	Puck left = Puck(4, 8);
	
	void render(){
		
		left.render();	
		
	}
	
	void update(){
			
		char input = getch();
		if (input == 'w' || input == 'W') left.moveUp();
		else if (input == 's' || input == 'S') left.moveDown();
		
	}
	
	void loop(){
		
		utility::clear();
		displayMap();
		
		render();
		while (true){
			
			if (kbhit()){
				
				update();
			
			}
			
		}
		
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
	
	mainMenu::loop();
	
	return 0;
}
