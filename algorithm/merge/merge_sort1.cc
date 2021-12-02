#include <algorithm>    // for sort function
#include <string>       // for strcpy 
#include <cstdio>       // for fscanf, fprintf, fopen     
#include <cstdlib>       // for fscanf, fprintf, fopen     
#include <ctime>        // for clock
#include <string.h>       // for strcpy 
#include <stdlib.h>
#include <stdio.h>
using namespace std;

/* define the constants used in this program */
const int MAX_RECORD_NUMBER = 10000000;     // max record number
const int BLOCK_SIZE =  500000;             // main memory block size
const int ROW_NUMBER = 95;                  // for record to fill the other 96 bytes
const int SUB_LIST_NUMBER = ( MAX_RECORD_NUMBER / BLOCK_SIZE );   // sub list number
const int MAX = 99999999;   // for function selectMinRecord to initialize the variable "min" 

/* the data structrue of a record */
typedef struct record           
{
	int key;                    // primary key
	char contents[ROW_NUMBER + 1];   // content
}Record;

Record subRecord[BLOCK_SIZE];         // main memory buffer
Record inputBuffer[BLOCK_SIZE + 1];   // input buffer      
Record outputBuffer[BLOCK_SIZE + 1];  // output buffer

/* generate a file of MAX_RECORD_NUMBER (= 10000000) records, 
   every record is 100 bytes */
void generateFile( string fileName )
{
	// calculate time
	printf("The records is now under generating ...\n");
	clock_t start, finish;
	double duration;
    start = clock();    // start time

	// open file
	FILE *fp = fopen( fileName.c_str(), "w" );
	printf("%s\n",fileName.c_str());  
	if ( !fp )     // open failed
	{
		printf("generateFile File could not be created!\n");
		fprintf( stderr, "generateFile File could not be created!\n" );
		exit( 1 );
	}

	// generate random integers and records
	srand( (unsigned)time( NULL ) );       // srand seed 
	for ( int i = 0; i < MAX_RECORD_NUMBER; i ++ )  // generate MAX_RECORD_NUMBER records
	{ 
		if ( i > 0 )
			fprintf( fp, "\n" );
		int key = rand();     // primary key, random integer, 4 bytes		

		// write record to disk, every record has 100 bytes
		fprintf( fp, "%d ", key );               // write key as the first 4 bytes
		for ( int j = 0; j < ROW_NUMBER; j ++ )  // write '0' for content as the other 96 bytes
			fprintf( fp, "%c", '0' );
	}
	fclose( fp );       // close output file 

    // calculate time
	finish = clock();     // finish time 
	duration = (double)( finish - start ) / CLOCKS_PER_SEC;   // run time 
	printf ( "It takes %f seconds to genetate the whole records.\n", duration );
}

/* use for phase 1 of two phase multiway merge sort
   compare two record by primary key, with ascending order */
bool cmp( const Record &r1, const Record &r2 )
{
	return r1.key < r2.key;
}
int my_itoa(int val, char* buf)
{
    const unsigned int radix = 10;

    char* p;
    unsigned int a;        //every digit
    int len;
    char* b;            //start of the digit char
    char temp;
    unsigned int u;

    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    u = (unsigned int)val;

    b = p;

    do
    {
        a = u % radix;
        u /= radix;

        *p++ = a + '0';

    } while (u > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return len;
}
/* give an integer, to generate a file name */
string generateFileName( int i )
{
	char str[20];             // temporary charater array
	string temp = "";         // temporary string

	my_itoa(i+1, str);       // store integer k+1 to array str
	temp = str;               // convert array str to temporary string 
	temp = "/home/keyvalue/code/HopeGit/algorithm/" + temp + ".txt";  // form the file name
	return temp;  // return the temporary string of file name 
}

/* phase 1 of two phase multiway merge sort
   read record with maximum block size to main memory 
   and sort them by primary key */
void phase1( string fileName )
{
	// open file
	FILE *infp = fopen( fileName.c_str(), "r" );   
	if ( !infp )            // open failed
	{
		printf( "File %s could not be opened!\n", fileName.c_str() );
		fprintf( stderr, "File %s could not be opened!\n", fileName.c_str() );  
		exit( 1 );
	}

	string temp = "";         // temporary string
	int i = 0, j = 0;

	// calculate time
	printf( "The sorted list of records is now under generating ...\n" );
	clock_t start, finish;
	double duration;
    start = clock();    // start time

	char str[ROW_NUMBER + 10];

	// read all records to main memory
	for ( int k = 0; k < SUB_LIST_NUMBER; k ++ )
	{
		// read records of a block size to main memory 
		for ( i = 0; i < BLOCK_SIZE; i ++ ) 
		{
			fgets( str, ROW_NUMBER + 10, infp );
			sscanf( str, "%d %s", &subRecord[i].key, subRecord[i].contents );
		}

		// use STL algorithm sort to sort records
		sort( subRecord, subRecord + BLOCK_SIZE, cmp );   

		temp = generateFileName( k );  // sorted list name
		FILE *outfp = fopen( temp.c_str(), "w" );   // open output file
		if ( !outfp )                 // open failed
		{
			printf( "File %s could not be opened!\n", temp.c_str());
			fprintf( stderr, "File %s could not be opened!\n", temp.c_str() );
			exit( 1 );
		}

		// write the sorted records to sub list file
		for ( i = 0; i < BLOCK_SIZE; i ++ )
		{
			if ( i > 0 )
				fprintf( outfp, "\n" );

			fprintf( outfp, "%d %s", subRecord[i].key, subRecord[i].contents );
		}
		printf( "The sorted list %s generated successfully!\n", temp.c_str() );
		fclose( outfp );       // close output stream
	}
	fclose( infp );         // close input file

	// calculate time
	finish = clock();     // finish time 
	duration = (double)( finish - start ) / CLOCKS_PER_SEC;   // run time 
	printf( "It takes %f seconds to genetate the sorted list of records.\n", duration );
}

/* copy record r2 to record r1 */
void copyRecord( Record &r1, Record &r2 )
{
	r1.key = r2.key;
	strcpy( r1.contents, r2.contents );
}

/* copy a record to input buffer */
void copyToInputBuffer( FILE *fp, int index )
{
	char str[ROW_NUMBER + 10];
	fgets( str, ROW_NUMBER + 10, fp );
	sscanf( str, "%d %s", &inputBuffer[index].key, inputBuffer[index].contents );
}

/* write the records in output buffer to disk
   when the output buffer is full */
void writeToDisk( FILE *fp )
{
	// flush output buffer to disk
	for ( int j = 0; j < BLOCK_SIZE; j ++ )
	{
		fprintf( fp, "%d %s\n", outputBuffer[j].key, outputBuffer[j].contents );
	}
}

/* select the minimum record in input buffer 
   by primary key */
int selectMinRecord( int size )
{
	int min = MAX;     
	int index = 0;
	for ( int i = 0; i < size; i ++ )
	{
		if ( inputBuffer[i].key < min )
		{
			min = inputBuffer[i].key;
			index = i;
		}
	}
	return index;
}

/* phase 2 of two phase multiway merge sort
   merge the sorted sub list to a sorted result file
   of ascending order */
void phase2()
{
	// open output file to store the sorted records
	FILE *outfp = fopen( "/home/keyvalue/code/HopeGit/algorithm/result.txt", "w" ); 
	if ( !outfp )      // open failed
	{
		printf( "Output file could not be created!\n" );
		fprintf( stderr, "Output file could not be created!\n" );
		exit( 1 );
	}

	string temp = "";         // temporary string
	int count = 0;            // the used output buffer size
	int total = 0;            // the record number written to disk
	int i = 0, j = 0;

	// array of input stream object, to open sub list of sorted records
	FILE * *infp = new FILE*[ SUB_LIST_NUMBER ]; 

	// calculate time
	printf( "Merge all of the sorted lists of records ...\n" );
	clock_t start, finish;
	double duration;
    start = clock();     // start time

	char str[ROW_NUMBER + 10];

	// read all of the sublist's first record to input buffer
	for ( int k = 0; k < SUB_LIST_NUMBER; k ++ )
	{
		temp = generateFileName( k );
		infp[k] = fopen( temp.c_str(), "r" );    // open sorted list file
		if ( !infp[k] )                         // open failed
		{
			printf( "File %s could not be created!\n", temp.c_str() );
			fprintf( stderr, "File %s could not be created!\n", temp.c_str() );
			exit( 1 );
		}          

		// read record to input buffer
		fgets( str, ROW_NUMBER + 10, infp[k] );
		sscanf( str, "%d %s", &inputBuffer[k].key, inputBuffer[k].contents );
	}
	
	// mark whether all the sored list have been read
	bool flag = false;       

	// merge the sorted list
	while ( total < MAX_RECORD_NUMBER )
	{
		//  select the smallest record
		int index = selectMinRecord( SUB_LIST_NUMBER );
		int newIndex = index;

		// write record to output buffer
		copyRecord( outputBuffer[count], inputBuffer[index] );
		count ++;
		total ++;

		// output buffer is full, write to disk
		if ( count >= BLOCK_SIZE )
		{
			count = 0;               // reset count
			writeToDisk( outfp );
		}

		if ( feof( infp[index] ) )     // the whole sub file hase been resd
		{
			// select a file that has more record to be read
			for ( i = 0; i < SUB_LIST_NUMBER; i ++ )
			{
				if ( !feof( infp[i] ) )
				{
					newIndex = i;
					break;
				}
			}

			// all sorted file have been read
			if ( i >= SUB_LIST_NUMBER )
				flag = true;
		}

		if ( !flag )     // not all sublist have been read 
			copyToInputBuffer( infp[newIndex], index  );
		else              // all sublist have been read 
		{
			// handle the last number of size records 
			sort( inputBuffer, inputBuffer + SUB_LIST_NUMBER, cmp );    
			for ( i = 1; i < SUB_LIST_NUMBER; i ++ )
			{
				// copy to output buffer
				copyRecord( outputBuffer[BLOCK_SIZE - SUB_LIST_NUMBER + i], inputBuffer[i] );
				count ++;
				total ++;
			} 
			writeToDisk( outfp );    // write to disk
			break;
		}
	}

    // close all input file
	for ( i = 0; i < SUB_LIST_NUMBER; i ++ )
		fclose( infp[i] );
	fclose( outfp );        // close output file

	// calculate time
	finish = clock();     // finish time 
	duration = (double)( finish - start ) / CLOCKS_PER_SEC;   // run time 
	printf( "It takes %f seconds to merge the sorted list of records.\n", duration );
}

/* the entrance of the program */
int main()
{
	// generate record file
	string fileName = "/home/keyvalue/code/HopeGit/algorithm/record.txt";
	generateFile(fileName);

	// calculate time
	clock_t start, finish;
	double duration;
    start = clock();

	// phase1 and phase2
	phase1(fileName);
	phase2();
	// calculate time
	finish = clock();     // finish time 
	duration = (double)( finish - start ) / CLOCKS_PER_SEC;   // run time 
	printf( "It takes %f seconds to sort the original records.\n", duration );

	return 0;
}