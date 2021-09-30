#include <stdio.h>           
#include <ctype.h>
#include <stdlib.h>

char player1Name[20], player2Name[20];
int playerTurn=0;
const int boardRows=6, boardColumns=7;
int **gameBoard;

/* Game Board Functions */
void setElement(int row, int column, int playerTurn) {                     // setElement() - takes the passed in location in memory and changes value at that location to a char based on playerTurn
	switch(playerTurn) {
		case 0:                                                                 // case 0: used in initialization method to differentiate between player and computer moves
			(gameBoard[row])[column] = '_';                                     // '_' represents empty space
			break;
		case 1:                                                                 // case 1: represents moved made by Player 1
			(gameBoard[row])[column] = 'a';
			break;
		case 2:                                                                 // case 2: represents moved made by Player 2
			(gameBoard[row])[column] = 'b';
			break;
		default:
			printf("Fatal Error in setElement()\n");
			break;
	}
}

int getElement(int row, int column) {                                      // getElement() - goes to the passed in location in memory and returns the value at that place
	return (gameBoard[row])[column];
}

int isColumnFull(int column) {                                             // isColumnFull() - checks the user selected column to see if it's full
	int numberOfPieces=0;
	for(int i=boardRows;i>0;i--) {
		if(getElement(i-1,column-1) != (int)'_')
			numberOfPieces++;
	}
	if(numberOfPieces==6)
		return 1;
		
	return 0;
}

int rowPlacement(int column) {                                             // rowPlacement() - checks the row entered by the user and returns the row to place their piece
	int row=6;
	for(int i=row;i>0;i--) {                                                    // loops through each row position in the column from bottom to top
		if(getElement(i-1,column-1) == (int)'_')
			return row;
		else
			row--;                                                              // decreases row counter by 1 if row is not empty                            
	}
	return row;
}

int horizontalCheck(int row, int column) {                                 // horizontalCheck() - checks left and right of the most recent move for four in a row
	int rightCount=0;
	int leftCount=0;
	int checker=getElement(row,column);                                         // checker is the most recent move by player
	
	for(int i=0;i<4;i++) {                                                      // check right side of most recent move
		if(getElement(row,column) == checker)
			rightCount++;
			
		column++;
		
		if(rightCount==4)                                                       // four in a row already guaranteed, no need to continue checking
			return 1;
		if(column>6)
			break;
	}
	
	column-=4;                                                                  // resets column variable to original value passed in
	
	for(int i=0;i<4;i++) {                                                      // check left side of most recent move
		if(getElement(row,column) == checker)
			leftCount++;

		column--;
		
		if(leftCount==4)                                                        // if four in a row, return 1 to signal winning move
			return 1;
		if(column<0)
			break;
	}
	
	if((rightCount+leftCount)>4)                                                // checks for in between moves not placed in a straight line
		return 1;
	else
		return 0;
}

int verticalCheck(int row, int column) {                                   // verticalCheck() - checks up and down of the most recent move for four in a row
	int count=0;
	int checker=getElement(row,column);                                         // checker is the most recent move by player
	
	for(int i=0;i<4;i++) {                                                      // check right side of most recent move
		if(getElement(row,column) == checker)
			count++;
			
		row++;
		if(row>5)                                                               // if row is greater than 5, out of bounds of game board
			break;
	}
	if(count==4)                                                                // four in a row achieved, return 1 for true, 0 for false
		return 1;
	else
		return 0;
}

int forwardDiagonalCheck(int row, int column) {                            // forwardDiagonalCheck() - checks "back slash" pattern of most recent move for four in a row
	int downRightCount=0;                                                              
	int upLeftCount=0;
	int checker=getElement(row,column);
	
	for(int i=0;i<4;i++) {
		if(getElement(row,column) == checker)
			downRightCount++;
		else                                                                    // if piece being looked at is not same as checker, not in a row, end loop
			break;
			
		column++;
		row++;
		
		if(downRightCount==4)                                                   // four in a row already guaranteed, no need to continue checking
			return 1;
		if(row>5 || column>6)                                                   // if row is greater than 5 or column greater than 6, out of bounds of gameboard
			break;
	}
	
	column-=downRightCount;
	row-=downRightCount;
	
	for(int i=0;i<4;i++) {                                                      
		if(getElement(row,column) == checker)                                          
			upLeftCount++;
		else     
			break;

		column--;
		row--;
		
		if(upLeftCount==4)                                                      // if four in a row, return 1 to signal winning move
			return 1;
		if(row<0 || column<0)                                                   // if row is less than 0 or column is less than 0, out of bounds of game board
			break;
	}
	if((downRightCount+upLeftCount)>4)                                          // checks for in between moves not placed in a straight line
		return 1;
	else
		return 0;
}

int backwardDiagonalCheck(int row, int column) {                           // backwardDiagonalCheck() - checks "forward slash" pattern of most revent move for four in a row
	int upRightCount=0;                                                             
	int downLeftCount=0;                                                              
	int checker=getElement(row,column);                                            
	
	for(int i=0;i<4;i++) {                                                      // check down and right side of most recent move
		if(getElement(row,column) == checker)                                
			upRightCount++;
		else                                                               
			break;
		
		column++;                                                        
		row--;                                                    
		
		if(upRightCount==4) {                                                   // four in a row already guaranteed, no need to continue checking
			return 1;
		}
		if(row<0 || column>6)                                                   // if row is less than 0 or column greater than 6, out of bounds of gameboard
			break;
	}
	
	column-=upRightCount;                                                     
	row+=upRightCount;                                                        
	
	for(int i=0;i<4;i++) {                                                      // check left side of most recent move
		if(getElement(row,column) == checker)                                
			downLeftCount++;
		else                                                                
			break;

		column--;                                                              
		row++;
		
		if(downLeftCount==4)                                                   
			return 1;
		if(row>5 || column<0)                                                   // if row is greater than 5 or column less than 0, out of bounds of game board
			break;
	}

	if((upRightCount+downLeftCount)>4)
		return 1;
	else
		return 0;
}

/* Game Loop Functions */
void initialization() {                                                    // initialization() - initializes the game, asks for player names
	printf("Player 1, please enter your name: ");
	scanf("%s",player1Name);
	printf("Player 2, please enter your name: ");
	scanf("%s",player2Name);
	printf("\nWelcome to Connect Four, %s and %s!\n",player1Name,player2Name);
	
	printf("\nSetting up the game. Please wait...\n");
	
	playerTurn++;
	gameBoard = (int **)malloc(boardRows*sizeof(int *));                       // allocates memory for the rows
	for(int i=0;i<boardRows;i++)
		*(gameBoard+i) = (int*)malloc(boardColumns*sizeof(int));               // allocates memory for the array of columns in each row
	
	for(int a=0;a<boardRows;a++) {                                             // loops through entire board and sets every element to '_' to represent empty space
		for(int b=0;b<boardColumns;b++)
			setElement(a,b,0);
	}
}

int acceptInput() {                                                       // acceptInput() - asks user for input to be translated into their game move
	int acceptedCol;
	char input[5];                                                             // creates new char array
	char *ch=input;                                                            // creates pointer pointing to the array
	
	do {
		printf("Player %d. Enter a letter A-G corresponding to your move or 'X' to quit: ",playerTurn);
		scanf("%s",ch);
		acceptedCol = (int)toupper(*ch)-64;
		if((acceptedCol < 0 || acceptedCol > 7) && acceptedCol !=24)                             // invalid move if ASCII conversion is out of range or if player inputted 'X' to quit
			printf("Invalid Move! Please enter a letter A-G or 'X' to quit:\n");		
	} while((acceptedCol < 0 || acceptedCol > 7) && acceptedCol !=24);                           // repeat until player inputs a valid move or quits
	
	return acceptedCol;
}
int updateGame(int column) {                                              // updateGame(int column) - updates the game board with most recent player move 
	int row=rowPlacement(column);
	
	if(column==24)                                                             // 24 is converted input representing a players choice to Quit the game
		return -2;

	if(isColumnFull(column)==1)                                                // if the column is full, return to main() and prompt user again
		return -1;
	
	switch(column) {
		case 1:                                                                // user inputted 'A'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
		case 2:                                                                // user inputted 'B'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
		case 3:                                                                // user inputted 'C'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
		case 4:                                                                // user inputted 'D'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
		case 5:                                                                // user inputted 'E'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
		case 6:                                                                // user inputted 'F'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
		case 7:                                                                // user inputted 'G'
			setElement(row-1,column-1,playerTurn);
			if((horizontalCheck(row-1,column-1)==1) || (verticalCheck(row-1,column-1)==1) || (forwardDiagonalCheck(row-1,column-1)==1) || (backwardDiagonalCheck(row-1,column-1)==1))
				return 1;
			break;
	}
	return 0;
}
void displayGame() {                                                      // displayGame(int column) - displays the game board after most recent update
	for(int a=0;a<boardRows;a++) {
		for(int b=0;b<boardColumns;b++) {
			printf("[");
			printf("%c",getElement(a,b));
			printf("]");
		}
		printf("\n");
	}
	printf("\n");
}
void teardown() {                                                         // teardown() - destroys the game after someone has one or game is at stalemate
	printf("\nGame Over! Thanks for playing, %s and %s\n",player1Name,player2Name);
	printf("Destroying the game...\n");
	
	for(int i=0;i<boardRows;i++) {                                             // frees each individual array of columns
		free(gameBoard[i]);
	}
	free(gameBoard);                                                           // frees the array of rows
}
void main() {                                                             // main() - controls the game loop and calls all functions as necessary 
	int updateGameStatus;
	
	initialization();                                                          // calls initialization function
	displayGame();                                                             // displays empty board for players at start of game
	
	for(int gameLoop=0;gameLoop<(boardRows*boardColumns);gameLoop++) {
		do {
			updateGameStatus=updateGame(acceptInput());                        // calls updateGame which returns a number based on status (-2 = quit, -1 = column full, 1 = win)
			if(updateGameStatus==1 || updateGameStatus==-2) {
				gameLoop=(boardRows*boardColumns);                             // invalidates the game loop because a player won/quit
				if(updateGameStatus==1)
					printf("\nPlayer %d has won the game! Congratulations!\n\n",playerTurn);
				else if(updateGameStatus==-2)
					printf("\nPlayer %d has Quit!\n\n",playerTurn);
				break;
			}				
			if(updateGameStatus==-1)
				printf("Sorry! The column you selected is full!\n");
		} while(updateGameStatus==-1);
		
		displayGame();
		
		(playerTurn==1) ? playerTurn++ : playerTurn--;
	}
	teardown();                                                                 // destroys the game by freeing all memory allocated by game board
}