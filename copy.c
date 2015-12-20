/*
*  Author:	Kyler Witting
*  Date:	10/6/15
*  Class:	CS3411
*  Semester:	Fall 2015
*/	

//Used for recording how long the program took for stats.txt
#include <time.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main( int argc, char **argv) {
	
	//Used for recording how long the program took for stats.txt
	clock_t begin, end;
	float time_spent;
	begin = clock();
	

	//Initilizes variables for the entire program
	int oldFile, newFile;
	int blockSize, words, i, checkSum, *Buffer;
	ssize_t readBytes, writeBytes;
	char output[256], errorBlock[256], error[256];

	//if there is given blockSize, it sets blockSize to 1024
	//If it is given it will set blocksize to that
	//If not divisible by 4, it will round up. 
	if ( argc < 4 )
		blockSize = 1024;
	else {
		if ( ( atoi( argv[3] ) % 4 ) == 0 )
	        	blockSize = atoi( argv[3] );
		else {
			blockSize = 4 * ( ( atoi( argv[3] ) / 4 ) + 1 );
			sprintf( errorBlock, "The given blocksize was not divisible by 4. It has been rounded up.\n" );
			write( 2, errorBlock, strlen( errorBlock ) );
		}
	}

	if ( argc == 1 ) {
		sprintf( error, "No arguments were given.\n" );
		write( 2, error, strlen( error ) );
		return (-1);
	}

	else if ( argc == 2 ) {
		sprintf( error, "No file destination was given\n" );
		write( 2, error, strlen( error ) );
		return (-1);
	}

	else if ( argc >= 3 ) { 
		//opens the file to copy, checks for errors
		oldFile = open(argv[1], O_RDONLY);
		if ( oldFile == -1 ) {
			sprintf( error, "The file to copy could not be opened. \n" );
			write( 2, error, strlen( error ) );
			return oldFile;
		}
		//opens the file to copy to, checks for errors
		newFile = open(argv[2], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
		if ( newFile == -1 ) {
			sprintf( error, "The file to copy to could not be opened.\n" );
			write( 2, error, strlen( error ) );
			return newFile;
		}
		
		//mallocs Buffer for the copy
		Buffer = ( int* ) malloc( sizeof( int ) *blockSize );

		while ( readBytes > 0 ) {
			//Sets all of Buffer to 0
			for ( i = 0; i < blockSize; i++ )
				Buffer[i] = 0;
			//Sets the bytes to read
			readBytes = read( oldFile, (char *)Buffer, blockSize );
			//finds how many words 
			words = readBytes / 4;

			//calculates the checkSum and prints it out with a space after 
			checkSum = 0;
			for(i = 0; i < words; i++)
					checkSum = checkSum ^ Buffer[i];	
			sprintf(output,"%08X ",checkSum);
			write( 1, output, strlen( output ) );	

			//writes to newFile and checks if it errors
			if ( readBytes >= 0 ) {
				writeBytes == write( newFile, Buffer, blockSize );
				if ( writeBytes == -1 ) {
					sprintf( error, "Unable to write to the new file.\n" );
					write( 2, error, strlen( error ) );
					return writeBytes;
				}
			}
		}
	}

	//Used for recording how long the program took for stats.txt
	end = clock();
	time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
	sprintf(output,"\nTime Spent: %.9f \n",time_spent );
	write( 1, output, strlen( output ) );
	free(Buffer);
	return 0;
}
