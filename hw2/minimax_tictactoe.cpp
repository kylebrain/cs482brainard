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

	// debug info
	//printf( "input file: [%s]\n", inputfilename);
	//printf( "output file: [%s]\n", outputfilename);

	// read from file
	read_from_file( inputfilename, board );

	// debug
	//printf( "starting tictactoe\n");

	//TODO: MAKE MOVE HERE
	//sleep(5);

	//debug into
	//printf( "finishing tictactoe\n");
	
	print_move_to_screen( m );
	print_board_to_screen( board) ;

	//write board state to file and exit
	return write_to_file(outputfilename, board);
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