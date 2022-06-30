#include <cstdio>	// for printf
#include <iostream> // cin/cout

#include <vector>
#include <unordered_map>

#include "cwiz_util.hpp"

#include "algos.h"

using namespace std;

//////////////////////////////////////////////////////////////

vector<int> icecreamParlor( int money, vector<int> iceCreamPrices )	{

    unordered_map<int,int> complements;
    for( int i = 0; i < int( iceCreamPrices.size() ); ++i )	{

		auto iter_p = complements.find( money - iceCreamPrices[ i ] );
		if( iter_p == complements.end() )	{

			complements.insert( make_pair( iceCreamPrices[ i ], i ) );
		}
		else	{
			auto ordered = minmax( i, iter_p->second );
			return { ordered.first + 1, ordered.second + 1 };
		}
    }
	return { 0, 0 };
}

void test_icecream_parlor(void)	{

	int money = 8;
	vector<int> prices = { 1, 2, 4, 5, 6, 8 };
//	vector<int> prices = { 1, 2, 4, 5, 9, 8 };

	vector<int> result = icecreamParlor( money, prices );

	cout << "result: " << result[ 0 ] << ", " << result[ 1 ] << endl;
}

//////////////////////////////////////////////////////////////

class app_state	{

	public:

	app_state( void )	{
		count = 0;
	}

	int count;
};

int test_command( vector <string> & args, cwiz::state_executor <app_state> & state )	{

	state.count++;

	cout << "count: " << state.count << endl;

	return( 0 );
}

void test_console_executor( void )	{

	cwiz::console_executor <app_state> console;

	console.callbacks[ "test" ] = test_command;

	console.resume(); // initialize control

	cout << "> " << flush; // indicate ready

	while( console.quit() == false )	{

//		usleep( 10000 ); // microseconds
		this_thread::sleep_for( chrono::milliseconds( 10 ) );

		if( console.command() )	{

			cout << "> " << flush; // indicate command consumed
		}
	}
}

//////////////////////////////////////////////////////////////

int main( int argc, char *argv[] ) {

	// char * p = "foobar"; does not allow conversion from string literal to 'char *'
/*
	cout << "argc == " << argc << endl;
	std::printf( "argc == %d\n", argc );

	test_icecream_parlor();
	test_algorithms();
*/

	test_console_executor();

	return( EXIT_SUCCESS );
}

