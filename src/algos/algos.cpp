#include <iostream>
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////

void print_set( vector< char > set )	{

	cout << "set: ";
	for( char c: set )	{

		cout << c << " ";
	}
	cout << endl;
}

void print_sset( vector< vector< char > > sset )	{

	cout << "sset:" << endl;
	int i = 0;

	for( vector< char > set: sset )	{

		cout << "set[ " << i++ << " ]: ";
		for( char c: set )	{

			cout << c << " ";
		}
		cout << endl;
	}
}

void test_powerset_iteration( vector< char > set )	{

	cout << "powerset:" << endl;

	vector< vector< char > > powset = { {} };

	for( char ch: set )	{

		vector< vector< char > > dupset;

		for( vector< char > v: powset )	{

			v.push_back( ch );
			dupset.push_back( v );
		}
		for( vector< char >& v: dupset )	{

			powset.push_back( v );

			print_set( v );
		}
	}

	print_sset( powset );
}

void test_permutation_iteration( vector< char > set )	{

	cout << "permutation:" << endl;

	vector< vector< char > > permset = { {} };
	int size = set.size();

	for( int c=0; c< size; c++ )	{

		int len = permset.size();

		for( int i=0; i < c; i++ )	{

			for( int j=0; j< len; j++ )	{

				permset.push_back( permset[ j ] );
			}
		}

		for( int i=0; i <= c; i++ )	{

			for( int j=0; j< len; j++ )	{

				int n = len * i + j;
				permset[ n ].insert( permset[ n ].begin() + i, set[ c ] );

				if( c == ( size - 1 ) )	{ // final pass
					print_set( permset[ n ] );
				}
			}
		}
	}

//	print_sset( permset );
}

void test_powerperm_iteration( vector< char > set )	{

	cout << "POWERPERM:" << endl;

	vector< vector< char > > powset = { {} };

	for( char ch: set )	{

		vector< vector< char > > dupset;

		for( vector< char > v: powset )	{

			v.push_back( ch );
			dupset.push_back( v );
		}
		for( vector< char >& v: dupset )	{

			powset.push_back( v );

			test_permutation_iteration( v );
		}
	}

//	print_sset( powset );
}

void test_set_family_iteration( void )	{

// 2, 6, 21, 88, 445, 2676

//	vector< char > set = { 'A' };
//	vector< char > set = { 'A', 'B' };
	vector< char > set = { 'A', 'B', 'C' };
//	vector< char > set = { 'A', 'B', 'C', 'D' };
//	vector< char > set = { 'A', 'B', 'C', 'D', 'E' };
//	vector< char > set = { 'A', 'B', 'C', 'D', 'E', 'F' };

	test_powerset_iteration( set );

	test_permutation_iteration( set );

	test_powerperm_iteration( set );
}

/////////////////////////////////////////////////////////////

void test_algorithms( void )	{

	cout << "test_algorithms:" << endl;

	test_set_family_iteration();
}
