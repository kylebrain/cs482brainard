#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <string.h>

struct Move {
    int x;
    int y;
};

/*
    read_from_file: reads the board from a file
    args:
        char* inputfilename: filename to be read
        int board[][3]: array to store board
    returns: 0 if all is well; -2 if the file cannot be opened for reading
*/
int read_from_file( char* inputfilename, int board[3][3] );

/*
    write_to_file: writes the board from a file
    args:
        char* outputfilename: filename to be written
        int board[4][4][4]: array to store board
    returns: 0 if all is well; -2 if the file cannot be opened for writing
*/
int write_to_file( char* outputfilename, int board[3][3] );

/*
    write_to_file: prints the board to the screen
    args:
        int board[3][3]: array used for the board
    returns: void
*/
void print_board_to_screen( int board[3][3] );


/*
    print_move_to_screen: prints the move chosen to the screen
    args:
        Move m: struct storing the x,y coordinate of the square to move to
    returns: void
*/
void print_move_to_screen( Move m);

Move minimax_move(int board[3][3], int player);
int minimax_score(int board[3][3], int player, Move * move = NULL);
Move from_index(int index);
bool terminal_state(int board[3][3], int * score);


int main( int argc, char* argv[] )
{
    char* inputfilename = NULL, *outputfilename = NULL;
    int board[3][3];
    Move m = {0,0};

    // parse command-line arguments
    for( int i = 1; i < argc; i++ )
    {
        // input file
        if( !strcmp(argv[i], "-i" ) )
        {
            inputfilename = argv[i+1];
            i++;
        }
        // output file
        else if( !strcmp( argv[i], "-o") )
        {
            outputfilename = argv[i+1];
            i++;
        }
    }

    // check to make sure command-line arguments have been specified
    if( inputfilename == NULL || outputfilename == NULL )
    {
        printf( "input and output filenames need to be specified on the command line (-i <filename> -o <filename>\n");
        return -1;
    }

    // read from file
    read_from_file( inputfilename, board );

    int winner;
    int player = -1;
    while(!terminal_state(board, &winner))
    {
        m = minimax_move(board, player);

        board[m.x][m.y] = player;
        
        print_move_to_screen( m );
        print_board_to_screen( board);

        player = -player;
    }

    //write board state to file and exit
    int success = write_to_file(outputfilename, board);
    if(success != 0)
    {
        return success;
    }

    return 0;
}

Move minimax_move(int board[3][3], int player)
{
    for(int i = 0; i < 9; i++)
    {
        Move m = from_index(i);
        if(board[m.x][m.y] == 0)
        {
            board[m.x][m.y] = player;
            int winner;
            if(terminal_state(board, &winner) && winner == player)
            {
                board[m.x][m.y] = 0;
                return m;
            }

            board[m.x][m.y] = 0;
        }
    }

    Move m;
    minimax_score(board, player, &m);
    return m;

}

// Returns the minimax score for a given board
// Player must be 0 or 1. 1 is max. 0 is min.
// Move will be populated with the best move available associated with the best score
int minimax_score(int board[3][3], int player, Move * move)
{
    // If board is terminating return score for winning player / tie
    int score = 0;
    if(terminal_state(board, &score))
    {
        return score;
    }

    // set the initial score to the first open move available
    int starting_index;
    Move best_move;
    for(starting_index = 0; starting_index < 9; starting_index++)
    {
        Move m = from_index(starting_index);
        if(board[m.x][m.y] == 0)
        {
            board[m.x][m.y] = player;
            score = minimax_score(board, -player);
            best_move = m;
            board[m.x][m.y] = 0;
            break;
        }
    }

    // loop through each of the remaining open moves and pick the min/max
    for(int i = starting_index + 1; i < 9; i ++)
    {
        Move m = from_index(i);
        if(board[m.x][m.y] == 0)
        {
            board[m.x][m.y] = player;
            int current_score = minimax_score(board, -player);
            if(player == 1)
            {
                if(current_score > score)
                {
                    score = current_score;
                    best_move = m;
                }
            } else if(current_score < score)
            {
                score = current_score;
                best_move = m;
            }
            board[m.x][m.y] = 0;
        }
    }

    if(move != NULL)
    {
        *move = best_move;
    }
    return score;
}

bool terminal_state(int board[3][3], int * score)
{
    // rows
    for (int i = 0; i < 3; i++)
    {
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][0]!=0)
        {
            *score = board[i][0];
            return true;
        }
    }

    // columns
    for(int i = 0; i < 3; i++)
    {
        if (board[0][i]==board[1][i] && board[1][i]==board[2][i] && board[0][i]!=0)
        {
            *score = board[0][i];
            return true;
        }
    }

    // diagonals
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[0][0]!=0)
    {
        *score = board[0][0];
        return true;
    }

    if(board[0][2]==board[1][1] && board[1][1]==board[2][0] && board [0][2]!=0)
    {
        *score = board[0][2];
        return true;
    }

    // if no winner, check if there is an empty space
    for(int i= 0 ; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(board[i][j]==0)
            {
                return false;
            }
        }
    }

    // board is full with no winner
    *score = 0;
    return true;
}

Move from_index(int index)
{
    return {index / 3, index % 3};
}

int read_from_file( char* inputfilename, int board[3][3] )
{
    FILE *ifile = fopen( inputfilename, "r" );
    if( !ifile )
    {
        printf( "could not open input file [%s] for reading\n", inputfilename );
        return -2;
    }

    for( int i = 0; i < 3; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            char c = '.';
            fscanf(ifile, " %c", &c );
            switch( c )
            {
                case '.': board[i][j] = 0; break;
                case 'x': board[i][j] = -1; break;
                case 'o': board[i][j] = 1; break;
                default: board[i][j] = 0; break;
            }

        }
    }

    fclose( ifile );
    return 0;
}

int write_to_file( char* outputfilename, int board[3][3] )
{
    FILE *ofile = fopen( outputfilename, "w" );
    if( !ofile )
    {
        printf( "could not open output file [%s] for writing\n", outputfilename );
        return -2;
    }

    // iterate through all squares on the board
    for( int i = 0; i < 3; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            // write the appropriate character to the file
            switch( board[i][j])
            {
                case 0: fprintf( ofile, "%c", '.'); break;
                case 1: fprintf( ofile, "%c", 'o'); break;
                case -1: fprintf( ofile, "%c", 'x'); break;
                default: fprintf( ofile, "%c", '.'); break;
            }
        }
        fprintf( ofile, "\n");
    }

    // close the output file
    fclose (ofile);
    return 0;
}

void print_board_to_screen( int board[3][3] )
{
    // iterate through all squares on the board
    for( int i = 0; i < 3; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            // print the appropriate character to the screen
            switch( board[i][j])
            {
                case 0: printf( "%c", '.'); break;
                case 1: printf( "%c", 'o'); break;
                case -1: printf( "%c", 'x'); break;
                default: printf( "%c", '.'); break;
            }
        }
        printf( "\n");
    }

}

void print_move_to_screen( Move m)
{
    printf( "move chosen: %d %d\n\n", m.x, m.y);
}