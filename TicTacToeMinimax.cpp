//Name: Tic-Tac-Toe 4 in a row with A.I.
//Author: Hunter Huynh

#include <iostream>  //for basic input/output
#include <string>    //for Strings
#include <windows.h> //for text color
using namespace std;

const int BOARDWIDTH = 6, BOARDHEIGHT = 6; //these two variables cannot be lower than 4
char gameBoard[BOARDWIDTH][BOARDHEIGHT];

const bool CONSOLEOUTPUT = false;
const int INITDEPTH = 3, TIMEBONUS = 2000, ROW4BONUS = 100000, ROW3BONOUS = 15, CENTERBONUS = 1;
int minMoveX = 0, minMoveY = 0, maxMoveX = 0, maxMoveY = 0, moveNum = 0;
int nodesSearched = 0, Xadv = 0;

//Used for input validation
int input(int userInput)
{
	while (!(cin >> userInput))
	{
		if (cin.eof()){ ; }
		else
		{
			char ch;
			cin.clear();
			cout << "Invalid input, please try again: ";
			while (cin.get(ch) && ch != '\n');
		}
	}
	return userInput;
}

//Used for input validation
char input(char userInput)
{
	while (!(cin >> userInput))
	{
		if (cin.eof()){ ; }
		else
		{
			char ch;
			cin.clear();
			cout << "Invalid input, please try again: ";
			while (cin.get(ch) && ch != '\n');
		}
	}
	return userInput;
}

//Used to set console text color
void setTextColor(std::string color)
{
	if (color == "green")
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//Returns true if we can make a move on the given square on the given game board
bool isMoveValid(char board[BOARDHEIGHT][BOARDWIDTH], int x, int y)
{
	if ((0 <= x) && (x <= (BOARDWIDTH - 1)) && (0 <= y) && (y <= (BOARDHEIGHT - 1)))
	{
		if (board[y][x] == '-')
		{
			return true;
		}
	}

	return false;
}

//Makes a move that adds an 'X' or 'O' to the given square by modifying the game board
void makeMove(char board[BOARDHEIGHT][BOARDWIDTH], int x, int y, bool turnX)
{
	if (turnX)
	{
		board[y][x] = 'X';
	}
	else
	{
		board[y][x] = 'O';
	}

}

//Clear the given square by modifying the game board
void makeNullMove(char board[BOARDHEIGHT][BOARDWIDTH], int x, int y)
{
	board[y][x] = '-';
}

//Displays the game board differently depending on the size of the board
void displayGameBoard(char board[BOARDHEIGHT][BOARDWIDTH])
{
	setTextColor("green");
	cout << endl << "   0";
	
	for (int i = 1; i < BOARDWIDTH; i++)
	{
		cout << "   " << i;
	}

	cout << endl << endl << "  ";
	
	for (int i = 0; i < (BOARDWIDTH - 1); i++)
	{
		cout << "   |";
	}

	cout << endl;

	for (int i = 0; i < (BOARDHEIGHT - 1); i++)
	{
		cout << i << "  ";

		for (int ii = 0; ii < (BOARDWIDTH - 1); ii++)
		{
			setTextColor("white");
			cout << board[i][ii];
			setTextColor("green");
			cout << " | ";
		}
		setTextColor("white");

		cout << board[i][(BOARDWIDTH - 1)];

		setTextColor("green");


		cout << endl << "  ";

		for (int i = 0; i < (BOARDWIDTH - 1); i++)
		{
			cout << "   |";
		}
		cout << endl;

		cout << "  ";

		for (int i = 0; i < (BOARDWIDTH - 1); i++)
		{
			cout << "----";
		}

		cout << "---";

		cout << endl << "  ";

		for (int i = 0; i < (BOARDWIDTH - 1); i++)
		{
			cout << "   |";
		}

		cout << endl;
	}

	cout << (BOARDWIDTH - 1) << "  ";

	for (int ii = 0; ii < (BOARDWIDTH - 1); ii++)
	{
		setTextColor("white");
		cout << board[(BOARDHEIGHT - 1)][ii];
		setTextColor("green");
		cout << " | ";
	}

	setTextColor("white");

	cout << board[(BOARDHEIGHT - 1)][(BOARDWIDTH - 1)];

	setTextColor("green");

	cout << endl << "  ";

	for (int i = 0; i < (BOARDWIDTH - 1); i++)
	{
		cout << "   |";
	}
	cout << endl;

	setTextColor("white");
}

//Returns a large number > 0 if the given board contains a winning position (4 in a row) for X
//Return a large number < 0 if the given board contains a losing position (4 in a row) for O
//Returns 0 if the given board does not contain any such positions 
int checkIfWin(char board[BOARDHEIGHT][BOARDWIDTH], int n)
{
	//Horz.
	for (int i = 0; i < BOARDHEIGHT; i++)
	{
		for (int ii = 0; ii < (BOARDWIDTH - 3); ii++)
		{
			if (board[i][0 + ii] == 'X' && board[i][1 + ii] == 'X' && board[i][2 + ii] == 'X' && board[i][3 + ii] == 'X')
			{
				return ROW4BONUS - n * TIMEBONUS;
			}
			if (board[i][0 + ii] == 'O' && board[i][1 + ii] == 'O' && board[i][2 + ii] == 'O' && board[i][3 + ii] == 'O')
			{
				return -ROW4BONUS + n * TIMEBONUS;
			}
		}
	}

	//Vert.
	for (int i = 0; i < BOARDWIDTH; i++)
	{
		for (int ii = 0; ii < (BOARDHEIGHT - 3); ii++)
		{
			if (board[0 + ii][i] == 'X' && board[1 + ii][i] == 'X' && board[2 + ii][i] == 'X' && board[3 + ii][i] == 'X')
			{
				return ROW4BONUS - n * TIMEBONUS;
			}
			if (board[0 + ii][i] == 'O' && board[1 + ii][i] == 'O' && board[2 + ii][i] == 'O' && board[3 + ii][i] == 'O')
			{
				return -ROW4BONUS + n * TIMEBONUS;
			}
		}
	}

	//Crosses
	for (int i = 0; i < (BOARDHEIGHT - 3); i++)
	{
		for (int ii = 0; ii < (BOARDWIDTH - 3); ii++)
		{
			if (board[0 + i][0 + ii] == 'X' && board[1 + i][1 + ii] == 'X' && board[2 + i][2 + ii] == 'X' && board[3 + i][3 + ii] == 'X')
			{
				return ROW4BONUS - n * TIMEBONUS;
			}
			if (board[0 + i][0 + ii] == 'O' && board[1 + i][1 + ii] == 'O' && board[2 + i][2 + ii] == 'O' && board[3 + i][3 + ii] == 'O')
			{
				return -ROW4BONUS + n * TIMEBONUS;
			}
			if (board[3 + i][0 + ii] == 'X' && board[2 + i][1 + ii] == 'X' && board[1 + i][2 + ii] == 'X' && board[0 + i][3 + ii] == 'X')
			{
				return ROW4BONUS - n * TIMEBONUS;
			}
			if (board[3 + i][0 + ii] == 'O' && board[2 + i][1 + ii] == 'O' && board[1 + i][2 + ii] == 'O' && board[0 + i][3 + ii] == 'O')
			{
				return -ROW4BONUS + n * TIMEBONUS;
			}
		}
	}
	return 0;
}

//Returns true if the given board contains a drawn position (it is full), returns false otherwise
bool checkIfDraw(char board[BOARDHEIGHT][BOARDWIDTH])
{
	bool isDraw = true;

	//If there are still empty sqaures, it is not a draw
	for (int i = 0; i < BOARDHEIGHT; i++)
	{
		for (int ii = 0; ii < BOARDWIDTH; ii++)
		{
			if (board[i][ii] == '-') { isDraw = false; }
		}
	}

	return isDraw;
}

//Returns a value representing X's advantage regarding rows of three for the given board
int checkIf3InRow(char board[BOARDHEIGHT][BOARDWIDTH], int n)
{
	int totalBonus = 0;

	//Horz.
	for (int i = 0; i < BOARDHEIGHT; i++)
	{
		for (int ii = 0; ii < (BOARDWIDTH - 2); ii++)
		{
			if (board[i][0 + ii] == 'X' && board[i][1 + ii] == 'X' && board[i][2 + ii] == 'X')
			{
				totalBonus += ROW3BONOUS; totalBonus -= n;
			}
			if (board[i][0 + ii] == 'O' && board[i][1 + ii] == 'O' && board[i][2 + ii] == 'O')
			{
				totalBonus -= ROW3BONOUS; totalBonus += n;
			}
		}
	}

	//Vert.
	for (int i = 0; i < BOARDWIDTH; i++)
	{
		for (int ii = 0; ii < (BOARDHEIGHT - 2); ii++)
		{
			if (board[0 + ii][i] == 'X' && board[1 + ii][i] == 'X' && board[2 + ii][i] == 'X')
			{
				totalBonus += ROW3BONOUS; totalBonus -= n;
			}
			if (board[0 + ii][i] == 'O' && board[1 + ii][i] == 'O' && board[2 + ii][i] == 'O')
			{
				totalBonus -= ROW3BONOUS; totalBonus += n;
			}
		}
	}

	//Crosses
	for (int i = 0; i < (BOARDHEIGHT - 2); i++)
	{
		for (int ii = 0; ii < (BOARDWIDTH - 2); ii++)
		{
			if (board[0 + i][0 + ii] == 'X' && board[1 + i][1 + ii] == 'X' && board[2 + i][2 + ii] == 'X')
			{
				totalBonus += ROW3BONOUS; totalBonus -= n;
			}
			if (board[0 + i][0 + ii] == 'O' && board[1 + i][1 + ii] == 'O' && board[2 + i][2 + ii] == 'O')
			{
				totalBonus -= ROW3BONOUS; totalBonus += n;
			}
			if (board[2 + i][0 + ii] == 'X' && board[1 + i][1 + ii] == 'X' && board[0 + i][2 + ii] == 'X')
			{
				totalBonus += ROW3BONOUS; totalBonus -= n;
			}
			if (board[2 + i][0 + ii] == 'O' && board[1 + i][1 + ii] == 'O' && board[0 + i][2 + ii] == 'O')
			{
				totalBonus -= ROW3BONOUS; totalBonus += n;
			}
		}
	}

	return totalBonus;
}

//Used by minimax(); returns bonuses for the given board
int getBonuses(char board[BOARDHEIGHT][BOARDWIDTH], int x, int y, bool turnX, int n)
{
	int centerBonus = 0, row3Bonus = 0;

	//Bonus for central control
	if ((moveNum < 3) && (n < 1) && (2 <= x) && (x <= (BOARDWIDTH - 3)) && (2 <= y) && (y <= (BOARDHEIGHT - 3)))
	{
		centerBonus += CENTERBONUS;
	}

	if ((moveNum < 5) && (n < 3))
	{
		row3Bonus += checkIf3InRow(board, n);
	}

	return turnX ? (centerBonus + row3Bonus) : (-centerBonus + row3Bonus);
}

//Used by the computer to search for a good move; the move is stored in minMoveX and minMoveY
//This is a standard minimax function with alpha-beta pruning
//n is how far we are in the search tree, where 0 is the top
int minimax(int player, char board[BOARDHEIGHT][BOARDWIDTH], int n, int depth, int alpha, int beta)
{
	int eval = 0; nodesSearched++;

	//Console output
	if (nodesSearched % 2000000 == 0)
	{
		cout << endl << "Computer is thinking..." << endl; 
	}

	//If 4 in a row (win or lose), return ROW4BONUS - n * 10 (Bonus is higher if we are higher in the search tree)
	if (checkIfWin(board, n) != 0)
	{
		return checkIfWin(board, n);
	}

	//If position is a draw, return 0
	else if (checkIfDraw(board))
	{
		return 0;
	}

	//If this is a leaf node (n == depth), and position is not won, lost, or drawn, simply return zero. 
	else if (n == depth)
	{
		return 0;
	}

	//O; wants to minimize X's advantage
	if (player == 2)
	{
		//Go through every possible move with given position and find the minimzing move
		for (int i = 0; i <= BOARDWIDTH; i++)
		{
			for (int ii = 0; ii <= BOARDHEIGHT; ii++)
			{
				if (isMoveValid(board, i, ii))
				{
					//Console Output
					if (n == 0 && CONSOLEOUTPUT)
					{
						cout << "Thinking about move " << i << "," << ii << " with depth=" << depth << "...";
					}

					//Make move i
					makeMove(board, i, ii, false);

					//Apply bonuses
					eval = getBonuses(board, i, ii, false, n);

					//Evaulate the move using this method 
					eval += minimax(1, board, n + 1, depth, alpha, beta);

					//Undo the move
					makeNullMove(board, i, ii);

					//Console Output
					if (n == 0 && CONSOLEOUTPUT)
					{
						cout << " X advantage = " << eval << endl;
					}

					//If evaulation of move i is better than ever before for the min player,
					//and we are at the top of the tree (n == 0), set minMove to i.
					if (eval < beta)
					{
						beta = eval;
						if (n == 0)
						{
							if (CONSOLEOUTPUT)
							{
								cout << "Computer has decided to make move " << i << ", " << ii << "" << endl;
							}

							minMoveX = i; minMoveY = ii; Xadv = eval;
						}
					}

					//Alpha-beta pruning 
					if (alpha >= beta)
					{
						return beta;
					}

				}
			}
		}

		//Return evaulation given that the minimzing move is played
		return beta;
	}

	//X; wants to maximize X's advantage
	else if (player == 1)
	{
		//Go through every possible move with given position and find the maximizing move
		for (int i = 0; i <= 6; i++)
		{
			for (int ii = 0; ii <= 6; ii++)
			{
				if (isMoveValid(board, i, ii))
				{
					//Console Output
					if (n == 0 && CONSOLEOUTPUT)
					{
						cout << "Thinking about move " << i << "," << ii << " with depth=" << depth << "...";
					}

					//Make move i
					makeMove(board, i, ii, true);

					//Apply bonuses
					eval = getBonuses(board, i, ii, true, n);

					//Evaulate the move using this method 
					eval += minimax(2, board, n + 1, depth, alpha, beta);

					//Undo move
					makeNullMove(board, i, ii);

					//Console Output
					if (n == 0 && CONSOLEOUTPUT)
					{
						cout << " X advantage = " << eval << endl;
					}

					//If evaulation of move i is better than ever before for the max player,
					//and we are at the top of the tree (n == 0), set maxMove to i.
					if (eval > alpha)
					{
						alpha = eval;
						if (n == 0)
						{
							if (CONSOLEOUTPUT)
							{
								cout << "Computer has decided to make move " << i << ", " << ii << "" << endl;
							}
							maxMoveX = i; maxMoveY = ii;
						}
					}

					//Alpha-beta pruning
					if (alpha >= beta)
					{
						return alpha;
					}
				}

			}
		}

		//Return evaulation given that maximizing move is played
		return alpha;
	}

}

//Allow human to make a move
void humanMove()
{
	int humanMoveX = -1, humanMoveY = -1;

	//Keep asking for human move until move is valid
	while (!isMoveValid(gameBoard, humanMoveX, humanMoveY))
	{
		cout << "Enter x coordinate of your move: ";
		humanMoveX = input(humanMoveX);
		cout << "Enter y coordinate of your move: ";
		humanMoveY = input(humanMoveY);
	}

	//Actually make the move
	makeMove(gameBoard, humanMoveX, humanMoveY, true);
}

//Tells the computer to make a move
//Returns if the computer resigns or not
bool computerMove(int player)
{
	nodesSearched = 0;  int foo = 0; int counter = 0; Xadv = 0; int searchDepth = INITDEPTH;

	if ((BOARDWIDTH + BOARDHEIGHT) <= 14)
	{
		searchDepth += moveNum / 4;
	}
	else
	{
		searchDepth += moveNum / 8;
	}

	//If computer is playing as player 2 (O)
	if (player == 2)
	{
		//Calculate the best move using minimax.
		//Keep calculating with increasing depth until a good move (Xadv < 0) is found or we've gone too far (counter > 3)
		while (Xadv >= 0)
		{
			cout << endl << "Computer is thinking..." << endl;

			//The minimax() function is the core of the A.I., recursively calculating the 'optimal' move and 
			//storing it into minMoveX/minMoveY/maxMoveX/maxMoveY; the function itself just returns X's advantage.
			int foo = minimax(player, gameBoard, 0, searchDepth + counter, -99999, 99999);

			//Makes sure we don't go too far into the search tree (so that calculation times don't become larger than they already are)
			counter++;
			if (counter >= 3) break;
		}
		cout << endl << "Looked at " << nodesSearched << " nodes in total." << endl;

		//Actually make the move was previously calculated
		makeMove(gameBoard, minMoveX, minMoveY, false);

		//Resignation; happens when lost position is almost unstoppable 
		if (Xadv >= (ROW4BONUS - TIMEBONUS * 2 - 5))
		{
			displayGameBoard(gameBoard);
			cout << "THE COMPUTER RESIGNS." << endl << "THE GAME IS OVER. YOU WIN." << endl;
			return true;
		}
	}

	//If computer is playing as player 1 (X)
	else
	{
		//Calculate the best move using minimax.
		//Keep calculating with increasing depth until a good move (Xadv < 0) is found or we've gone too far (counter > 3)
		while (Xadv <= 0)
		{
			cout << endl << "Computer is thinking..." << endl;

			//The minimax() function is the core of the A.I., recursively calculating the 'optimal' move and 
			//storing it into minMoveX/minMoveY/maxMoveX/maxMoveY; the function itself returns X's advantage.
			int foo = minimax(player, gameBoard, 0, searchDepth, -99999, 99999);

			//Makes sure we don't go too far into the search tree (so that calculation times don't become larger than they already are)
			counter++;
			if (counter > 3) break;
		}
		cout << "Looked at " << nodesSearched << " nodes." << endl;

		//Actually make the move was previously calculated
		makeMove(gameBoard, maxMoveX, maxMoveY, true);

		//Resignation
		if (Xadv <= (-ROW4BONUS + TIMEBONUS * 2 + 5))
		{
			displayGameBoard(gameBoard);
			cout << "THE COMPUTER RESIGNS." << endl << "THE GAME IS OVER. YOU WIN." << endl;
			return true;
		}
	}

	//Return 'false' - computer does not resign.
	return false;
}

//Returns 1 if the given position is won for the player, -1 if lost, 0 if drawn, and 2 if neither.
int checkIfWonLostOrDrawn()
{
	if (checkIfDraw(gameBoard) || (checkIfWin(gameBoard, 0) != 0))
	{
		if (checkIfWin(gameBoard, 0) > 0)
		{
			return 1;
		}
		else if (checkIfWin(gameBoard, 0) < 0)
		{
			return -1;
		}
		else if (checkIfWin(gameBoard, 0) == 0)
		{
			return 0;
		}

	}

	return 2;
}

//Initalize the game board by setting all squares to '-'
void initalizeGameBoard()
{
	for (int i = 0; i <= (BOARDWIDTH - 1); i++)
	{
		for (int ii = 0; ii <= (BOARDHEIGHT - 1); ii++)
		{
			makeNullMove(gameBoard, i, ii);
		}
	}
}

//Display text that showns when a game is complete (won, lost, or drawn)
void displayClosingText(int gameStatus)
{
	switch (gameStatus)
	{
	case 1:
		cout << endl << "THE GAME IS OVER. YOU WIN." << endl;
		break;
	case 0:
		cout << endl << "THE GAME IS OVER. IT'S A TIE." << endl;
		break;
	case -1:
		cout << endl << "THE GAME IS OVER. YOU LOSE. \n\nFoolish humans..." << endl;
		break;
	}
}

//Program execution starts here
int main()
{
	//Initalize everything --------------------------------------------------------------------------
	bool gameQuit = false; 
	int gameStatus = 2; //1 = won by human, 0 = tie, -1 = lost, 2 = neither

	//Display starting text
	cout << endl << BOARDWIDTH << "x" << BOARDHEIGHT << " Tic-Tac-Toe: Get 4 in a row to win." << endl << "Try to beat the NEW AND IMPROVED A.I." << endl << "Created by Hunter Huynh." << endl;

	//Init. display of game board
	initalizeGameBoard();
	displayGameBoard(gameBoard);

	//Ask human to start
	cout << endl << "Would you like to start? (Y/N)";
	char consoleInput = 'N'; 
	consoleInput = input(consoleInput); 

	while (consoleInput == 'Y' || consoleInput == 'y')
	{
		//Init. game board and other variables
		initalizeGameBoard();
		moveNum = 1; gameQuit = false; 

		//Main game loop----------------------------------------------------------
		while (!gameQuit)
		{
			//Keep track of the move number
			moveNum++;

			//Computer's turn to move
			gameQuit = computerMove(2); 
			if (gameQuit) 
			{ 
				break; 
			}

			displayGameBoard(gameBoard);

			//Check for win/draw/lost position, and display closing text and exit the game loop if the game is done
			gameStatus = checkIfWonLostOrDrawn();
			displayClosingText(gameStatus);
			if (gameStatus != 2)
			{
				break;
			}

			//Human's turn to move
			humanMove();

			displayGameBoard(gameBoard);

			//Check for win/draw/lost position, and display closing text if the game is done
			gameStatus = checkIfWonLostOrDrawn();
			displayClosingText(gameStatus);
			if (gameStatus != 2)
			{
				break;
			}

		}
		//End of game loop---------------------------------------------------------------


		//Ask human to play again
		cout << endl << "Play again? (Y/N)" << endl;
		consoleInput = input(consoleInput);
	}

	//End of program
	return 0;
}