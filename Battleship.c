#include "Battleship.h"
unsigned int DIM = 10;






/**
 * Prints the board given by parameter.
 * @param board
 */
void showBoard(char* board)
{
    unsigned int DIM_num_digits = 0;
    for (unsigned int j = DIM; j > 0; j /= 10)
    {
        DIM_num_digits++;
    }

	printf("\n ");
	for (unsigned int i = 0; i < DIM_num_digits; i++)
    {
	    printf(" ");
    }
	for (unsigned int i = 0; i < DIM; i++)
	{
		printf("%c", 'A' + i);
	}
	printf("\n");
	for (unsigned int j = 0; j < DIM; j++)
	{	
		printf("\n%i", j + 1);
		unsigned int current_row_num_digits = 0;
        for (unsigned int i = j + 1; i > 0; i /= 10)
        {
            current_row_num_digits++;
        }
        for (unsigned int i = 0; i < DIM_num_digits - current_row_num_digits + 1; i++)
        {
            printf(" ");
        }
		for (unsigned int i = 0; i < DIM; i++)
		{
			printf("%c", board[j * DIM + i]);
		}
	}
	printf("\n");
}


/**
 * Locates a Ship pointed by the variables *x_ini and *y_ini by saving in *x_ini and *y_ini the initial coordinates of the ship
 * and saving in *x_end and *y_end the final coordinates of the ship.
 * This function expects correct coordinates.
 * This function always places the coordinates with lower values in *x_ini and *y_ini
 **/
void locateShip(char* board, unsigned int* x_ini, unsigned int* y_ini, unsigned int* x_end, unsigned int* y_end)
{
    *x_end = *x_ini;
    *y_end = *y_ini;
    int x_init = *x_ini, y_init = *y_ini;

    while (*x_end < DIM && (board[*y_end * DIM + *x_end] == SHIP || board[*y_end * DIM + *x_end] == SHOT_SHIP)) (*x_end)++;
    (*x_end)--;
    while (*y_end < DIM && (board[*y_end * DIM + *x_end] == SHIP || board[*y_end * DIM + *x_end] == SHOT_SHIP)) (*y_end)++;
    (*y_end)--;
    while (x_init >= 0 && (board[*y_ini * DIM + x_init] == SHIP || board[*y_ini * DIM + x_init] == SHOT_SHIP)) x_init--;
    x_init++;
    while (y_init >= 0 && (board[y_init * DIM + *x_ini] == SHIP || board[y_init * DIM + *x_ini] == SHOT_SHIP)) y_init--;
    y_init++;
    *x_ini = x_init;
    *y_ini = y_init;
}


/**
 * Fills with water all the positions surrounding the ship located by the coordinates x and y
 * @param board
 * @param x
 * @param y
 */
void floodSorroundings(char* board, unsigned int x, unsigned int y)
{
    unsigned int x_ini = x, y_ini = y, x_end, y_end;
    locateShip(board, &x_ini, &y_ini, &x_end, &y_end);
    if (x_ini > 0) x_ini--;
    if (y_ini > 0) y_ini--;
    if (x_end < DIM - 1) x_end++;
    if (y_end < DIM - 1) y_end++;
    for (unsigned int i = x_ini; i <= x_end; i++)
    {
        for (unsigned int j = y_ini; j <= y_end; j++)
        {
            if (board[j * DIM + i] != SHOT_SHIP && board[j * DIM + i] != SHIP) board[j * DIM + i] = WATER;
        }
    }
}


/**
 * Reserves memory space for a board of the given dimensions, initializes it with NOT_DISCOVERED_CELL and returns it.
 * @return
 */
char* startAttackBoard()
{
    char* attack_board = malloc(DIM * DIM * sizeof(char));
    for (unsigned int i = 0; i < DIM * DIM; i++)
    {
        attack_board[i] = NOT_DISCOVERED_CELL;
    }
    return attack_board;
}


/**
 * This functions updates the ship board with a shoot on a certain cell and informs the result of the shot:
 *  - Repeated cell (0)
 *  - Water (1)
 *  - Shot (2)
 *  - Shot & Sunk (3)
 * @param board
 * @param x
 * @param y
 * @return
 */
unsigned int shoot(char* board, unsigned int x, unsigned int y)
{
    switch (board[DIM * y + x])
    {
        case SHOT_WATER:
        {
            return 0;
        }
            break;
        case WATER:
        {
            board[DIM * y + x] = SHOT_WATER;
            return 1;
        }
        break;
        case SHIP:
        {
            board[DIM * y + x] = SHOT_SHIP;
            // Detect if ship is sunk and return consequently
        }
        break;
    }
    return 0; // by now to staisfy the compiler
}


/**
 * Locates the limit of the ship pointed by the coordinates x and y and then checks
 * if all the positions of the ship are shot, then return true; otherwise false
 * @param board
 * @param dim
 * @param x
 * @param y
 * @return
 */
bool isSunk(char* board, unsigned int x, unsigned int y)
{
    unsigned int x_ini, y_ini, x_end, y_end;
    locateShip(board, &x_ini, &y_ini, &x_end, &y_end);
    for (unsigned int i = x_ini; i <= x_end; i++)
    {
        for (unsigned int j = y_ini; j <= y_end; j++)
        {
            if (board[j * DIM + i] == SHIP) return false;
        }
    }
    return true;
}


/**
 * Returns true if the ship specified by parameters collisions wth any elements of the given board
 * @param defense_board
 * @param x_ini
 * @param y_ini
 * @param ship_size
 * @param orientation 1 for vertical orientations, 0 for horizontal
 * @return
 */
bool doesFit(char* defense_board, unsigned int x_ini, unsigned int y_ini, unsigned int* x_end, unsigned int* y_end, unsigned int ship_size, bool orientation)
{
    if (ship_size > 1)
    {
        if (orientation)
        {
            (*x_end) = x_ini + ship_size - 1;
            (*y_end) = y_ini;
        }
        else
        {
            (*y_end) = y_ini + ship_size - 1;
            (*x_end) = x_ini;
        }
    }
    else
    {
        (*x_end) = x_ini;
        (*y_end) = y_ini;
    }

    for (unsigned int i = x_ini; i <= (*x_end); i++)
    {
        for (unsigned int j = y_ini; j <= (*y_end); j++)
        {
            if (defense_board[j * DIM + i] != NOT_DISCOVERED_CELL) return false;
        }
    }
    return true;
}


/**
 * Writes a ship in the given position in the given board
 * @param defense_board
 * @param x_ini
 * @param y_ini
 * @param ship_size
 * @param orientation 1 for vertical orientations, 0 for horizontal
 * @return
 */
void initializeShip(char* defense_board, unsigned int x_ini, unsigned int y_ini, unsigned int x_end, unsigned int y_end, unsigned int ship_size, bool orientation)
{
    for (unsigned int i = x_ini; i <= x_end; i++)
    {
        for (unsigned int j = y_ini; j <= y_end; j++)
        {
            defense_board[j * DIM + i] = SHIP;
        }
    }
}


bool startDefenseBoardAutoDelegate(char* defense_board)
{
    unsigned int x_ini, y_ini, x_end, y_end, orientation, number_of_tries;
    for (unsigned int ship_size = 1; ship_size <= SHIP_MAX_SIZE; ship_size++)
    {
        for (unsigned int ship_counter = 0; ship_counter < NUM_SHIPS_BY_SIZE[ship_size - 1]; ship_counter++)
        {
            number_of_tries = 0;
            while (true)
            {
                x_ini = rand() % DIM;
                y_ini = rand() % DIM;
                if (ship_size > 1)
                {
                    orientation = rand() % 2;
                }
                if (doesFit(defense_board, x_ini, y_ini, &x_end, &y_end, ship_size, orientation))
                {
                    initializeShip(defense_board, x_ini, y_ini, x_end, y_end, ship_size, orientation);
                    floodSorroundings(defense_board, x_ini, y_ini);
                    break;
                }
                else
                {
                    number_of_tries++;
                    if (number_of_tries > 100000)
                    {
                        return false;
                    }
                }
            }

        }
    }
    return true;
}


/**
 * Reserves memory space for a board of the given dimensions, initializes it with WATER and the ships
 * and returns it.
 * @return pointer to the initialized board with dimensions DIM x DIM
 */
char* startDefenseBoardAuto()
{
    char* defense_board = malloc(DIM * DIM * sizeof(char));
    while (true)
    {
        for (unsigned int i = 0; i < DIM * DIM; i++)
        {
            defense_board[i] = NOT_DISCOVERED_CELL;  // To mark the free cells
        }

        if (!startDefenseBoardAutoDelegate(defense_board)) continue;
        else break;
    }
    for (unsigned int i = 0; i < DIM * DIM; i++)
    {
        if (defense_board[i] == NOT_DISCOVERED_CELL) defense_board[i] = WATER;
    }
    return defense_board;
}


/**
 * Returns two coordinates representing a cell that has been targeted as the next shot.
 * This function tries to optimize the number of calls needed to this function in order to finish the game as soon
 * as possible.
 *
 * Static parameters:
 * static unsigned int state;
 * static unsigned int tried_state;
 *
 * @param board
 * @param dim
 */
void computeNextMovement(char* board, unsigned int* x, unsigned int* y, int result_last_shot)
{
    static unsigned int state = 0;
    if (result_last_shot == RESULT_SHOT_AND_SUNK) state = 0;
    switch (state)
    {
        case 0:
        {

        }
    }
}


int main(int nargs, char* args[])
{
    srand(time(NULL));
	showBoard(startDefenseBoardAuto());

}

