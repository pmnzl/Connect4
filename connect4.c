#include "connect4.h"

void sort(double values[], int length);
void PositionLocator(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move, int player, int *lastRow, int *lastCol);
void RandomMove(int size, char *side, int *move);


int SecondPlacePrize(int prize1, int prize2, int prize3)
{
	//Passing the function parameters into an array
	double prizeArray[3] = { prize1, prize2, prize3 };

	//Uses the sort helper function
	sort(prizeArray, 2);

	//Will always return the midddle value of the array therefore the second highest value
	return prizeArray[1];
}

int FourInARow(int values[], int length)
{
	int count = 1;
	int index = -1;

	//Loops through the values[] array and determines if 4 values in a row are the same or not and
	//returns the index of the value if there are four values in a row
	for (int i = 1; i < length; i++)
	{
		if (values[i] == values[i - 1])
		{
			//Iterates until a four in a row is found and then exits the for loop returining the initial index
			count++;
			if (count == 4)
			{
				index = i - 3;
				break;
			}
		}
		else
		{
			//If no sequences are detected count is reset and index is set to -1
			count = 1;
			index = -1;
		}
	}
	return index;
}

int BinaryToDecimal(int binary)
{
	int power = 1;
	int decimal = 0;

	//Takes the LSB each iteration and adds converts its value to denary and adds it to
	//the decimal value
	while (binary != 0)
	{
		//Adding the value of the LSB to a denary value
		decimal = decimal + (power*(binary % 10));
		//Iterates the power each iteration to get the correct place value
		power = power * 2;
		//Takes of the bit just calculated from the binary value
		binary = binary / 10;
	}
	return decimal;
}

double MedianAbility(double abilities[], int length)
{
	int swap;
	double median, temp;

	//Uses the sort helper function
	sort(abilities, length);

	//Checks if the length of the array is even or not and does the corresponding executions
	if (length % 2 == 0)
	{
		median = (abilities[length / 2] + abilities[(length / 2) - 1]) / 2;
	}
	else
	{
		median = abilities[length / 2];
	}
	return median;
}

void RemoveSpaces(char *name)
{
	char *tempname = name;
	//Looping until the string has reached its end point
	while (*name != '\0')
	{
		//Skipping chars that proceed after another space
		while (*name == ' ' && *(name - 1) == ' ')
			name++;
		//Putting all other values into a buffer string
		*tempname++ = *name++;
	}
	*tempname = '\0';
	//Assigning the original string to the string with removed spaces
	*name = *tempname;
}

void InitialiseBoard(int board[MAX_SIZE][MAX_SIZE], int size)
{
	int index = size / 2;

	//Initialises the board to all zeros of a given size
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = 0;
		}
	}

	//If the size of the board is even place 4 centre pieces
	if (size % 2 == 0)
	{
		board[index][index] = 3;
		board[index][index - 1] = 3;
		board[index - 1][index] = 3;
		board[index - 1][index - 1] = 3;
	}

	//If the board is odd place only one centre piece
	else
	{
		board[index][index] = 3;
	}
}

void AddMoveToBoard(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move, int player, int *lastRow, int *lastCol)
{
	int row, col;
	//Uses helper function
	PositionLocator(board, size, side, move, player, &row, &col);
	//Setting both row and col to -1 if a invalid possition is read
	if (row == -1 || col == -1 || row == size || col == size) 
	{
		row = -1;
		col = -1;
	}
	*lastRow = row;
	*lastCol = col;
	//If the slot trying to be occupied is already taken then the current move will not be put on the board
	if (board[*lastRow][*lastCol] == 0)
		board[*lastRow][*lastCol] = player;
	//printf("%d \t %d \t %d\n", player, *lastRow, *lastCol);
}

int CheckGameOver(int board[MAX_SIZE][MAX_SIZE], int size, int player, int row, int col)
{
	int i;
	int count = 0;
	int fullboard = 1;

	//Checking for invalid move
	if (row == -1 && col == -1)
	{
		return 0;
	}

	//Full Board
	for (i = 0; i < size; i++)
	{
		if (board[0][i] == 0) //Checking N side
		{
			fullboard = 0;
		}
		else if (board[size - 1][i] == 0) //Checking S side
		{
			fullboard = 0;
		}
		else if (board[i][0] == 0) //Checking W side
		{
			fullboard = 0;
		}
		else if (board[i][size - 1] == 0) //Checking E side
		{
			fullboard = 0;
		}
	}
	//Using fullboard as a boolean flag variable and then returning 1 if all outside spots on the board are occupied
	if (fullboard == 1)
		return player;

	//Horizontal
	//Scanning the whole column the token was placed in
	for (i = 0; i < size; i++)
	{
		if (board[row][i] == player)
		{
			count++;
			if (count == 4)
				return player;
		}
		else
		{
			count = 0; //Resets the count if not four in a row
		}
	}

	//Vertical
	//Scanning the whole row the token was placed in
	for (i = 0; i < size; i++)
	{
		if (board[i][col] == player)
		{
			count++;
			if (count == 4)
				return player;
		}
		else
		{
			count = 0; //Resets the count if not four in a row
		}
	}

	//Diagonal
	for (i = -3; i < 4; i++)
	{
		if (board[row + i][col + i] == player)
		{
			count++;
			if (count == 4)
				return player;
		}
		else
		{
			count = 0; //Resets the count if not four in a row
		}
	}
	//Need another diagonal check as there are two diagonal directions
	for (i = -3; i < 4; i++)
	{
		if (board[row + i][col - i] == player)
		{
			count++;
			if (count == 4)
				return player;
		}
		else
		{
			count = 0; //Resets the count if not four in a row
		}
	}
	return 0;
}

void GetDisplayBoardString(int board[MAX_SIZE][MAX_SIZE], int size, char *boardString)
{
	int linidx = 0;
	int i, j;

	//Creating the N borders
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	for (i = 0; i < size; i++)
	{
		boardString[linidx] = 'N';
		linidx++;
	}
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '\n';
	linidx++;

	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	//Getting the numbers for the borders of N
	for (j = 0; j < size; j++)
	{
		boardString[linidx] = j + 48; //48 is zero in ASCII
		linidx++;
	}
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '\n';
	linidx++;

	//Creating the side borders of W and E
	for (i = 0; i < size; i++)
	{
		boardString[linidx] = 'W';
		linidx++;
		boardString[linidx] = i + 48; //Numbers for W side
		linidx++;

		//Creating the actual board by converting the numbers into ASCII characters
		for (j = 0; j < size; j++)
		{
			if (board[i][j] == 0)
			{
				boardString[linidx] = '.';
			}
			else if (board[i][j] == 1)
			{
				boardString[linidx] = 'X';
			}
			else if (board[i][j] == 2)
			{
				boardString[linidx] = 'O';
			}
			else if (board[i][j] == 3)
			{
				boardString[linidx] = '#';
			}
			linidx++;
		}

		//Creating E side
		boardString[linidx] = i + 48; //E side numbers
		linidx++;
		boardString[linidx] = 'E';
		linidx++;
		boardString[linidx] = '\n';
		linidx++;
	}

	//Creating S side
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	for (j = 0; j < size; j++)
	{
		boardString[linidx] = j + 48; //S side numbers
		linidx++;
	}
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '\n';
	linidx++;

	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	for (i = 0; i < size; i++)
	{
		boardString[linidx] = 'S';
		linidx++;
	}
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '-';
	linidx++;
	boardString[linidx] = '\n';
	linidx++;

	boardString[linidx] = '\0';
}

void GetMoveBot1(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move)
{
	//This bot algorithm detects if a move is valid or not and wheter a move will have an impact on the board and only places tokens if they are vaild
	int row, col;
	char tempside;
	int tempmove;
	do
	{
		RandomMove(size, &tempside, &tempmove);//Calling on a helper function to generate a random move

		//Dereferencing the pointers
		*side = tempside;
		*move = tempmove;

		//Goes through the move and determines if the move will have any impact on the board if not another move is selected
		switch (*side)
		{
		case 'N':
			row = 0;
			col = *move;
			break;
		case 'S':
			row = size - 1;
			col = *move;
			break;
		case 'E':
			row = *move;
			col = size - 1;
			break;
		case 'W':
			row = *move;
			col = 0;
			break;
		}
	} while (board[row][col] != 0); // Iterates until a valid move is preoduced
}

void GetMoveBot2(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move)
{
	int row, col;
	char tempside;
	int tempmove;
	do
	{
		RandomMove(size, &tempside, &tempmove);//Calling on a helper function to generate a random move

		//Dereferencing the pointers
		*side = tempside;
		*move = tempmove;

		//Goes through the move and determines if the move will have any impact on the board if not another move is selected
		switch (*side)
		{
		case 'N':
			row = 0;
			col = *move;
			break;
		case 'S':
			row = size - 1;
			col = *move;
			break;
		case 'E':
			row = *move;
			col = size - 1;
			break;
		case 'W':
			row = *move;
			col = 0;
			break;
		}
	} while (board[row][col] != 0); // Iterates until a valid move is preoduced
}




//HELPER FUNCTIONS
void sort(double values[], int length)
//Helper function for SecondPlacePrize and MedianAbility functions
//Basic sorting of an array of a certain length using bubble sort
{
	int swap;
	double temp;

	for (int i = 0; i < length; i++)
	{
		swap = 0;
		for (int j = 0; j < length; j++)
		{
			if (values[j] <= values[j + 1])
			{
				//Swaps two values around
				temp = values[j];
				values[j] = values[j + 1];
				values[j + 1] = temp;
				swap = 1;
			}
		}
		if (swap == 0)
			break;
	}
}

void PositionLocator(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move, int player, int *lastRow, int *lastCol)
{
	//Function used in the assignment of a move or the addmovetoboard function and is useful in determining the where a token will land given a move
	int i;

	//Using a switch statement as it is a bit cleaner than an if statement here
	switch (side)
	{
	case 'N':
		i = 0;
		//Detecting the last free spot
		while (board[i][move] == 0 && i < size)
			i++;
		i--;
		//Assigning the rows and columns to their dereferenced pointers
		*lastRow = i;
		*lastCol = move;
		break;
	case 'S':
		i = size - 1;
		while (board[i][move] == 0 && i >= 0)
			i--;
		i++;
		*lastRow = i;
		*lastCol = move;
		break;
	case'W':
		i = 0;
		while (board[move][i] == 0 && i < size)
			i++;
		i--;
		*lastRow = move;
		*lastCol = i;
		break;
	case 'E':
		i = size - 1;
		while (board[move][i] == 0 && i >= 0)
			i--;
		i++;
		*lastRow = move;
		*lastCol = i;
		break;
	}
}

void RandomMove(int size, char *side, int *move)
{
	//Called from the bot functions and its purpose is to select a radom move for the bot
	int temp;
	*move = (rand() % (size)); //Generates random numbers between 0 and 9 inclusive if size = MAX_SIZE
	temp = (rand() % (4)); //Genrates random numbers between 0 and 3 inclusive

	//Selecting a random side value due to the random nature of the temp variable
	switch (temp)
	{
	case 0:
		*side = 'N';
		break;
	case 1:
		*side = 'E';
		break;
	case 2:
		*side = 'W';
		break;
	case 3:
		*side = 'S';
		break;
	default:
		break;
	}
}