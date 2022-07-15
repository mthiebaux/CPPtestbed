
#include <unistd.h>
#include <math.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <array>
#include <vector>
#include <list>
#include <forward_list>

#include "timer.hpp"

using namespace std;

/////////////////////////////////////////////////////////////

class node	{

	public:
		node( int id )	{
			identity = id;
			root_identity = id; // init to self
			visited = false;
		}
		~node()	{
		}
		// pass by value or reference: see #6: https://www.py4u.net/discuss/64291
		void connect( const shared_ptr< node >& snp )	{ // faster
			if( snp->identity != identity )	{ // self check, no err?
				edge_list.push_front( snp );
			}
		}
		void disconnect( const shared_ptr< node >& rem )	{

			for( auto& np: edge_list )	{
				if( np == rem )	{

					// check root_identity...

					edge_list.remove( rem );
					return;
				}
			}
		}
		bool edge_connected( int id )	{

			for( auto& snp: edge_list )	{
				if( snp->identity == id )	{
					return( true );
				}
			}
			return( false );
		}
#if 0
		bool path_connected( int id, int & root_id )	{

			if( root_id == root_identity ) // same root group
				return( true );
			if( id == identity ) // self check, no err
				return( true );
			visited = true;

			for( auto& snp: edge_list )	{
				if( snp->path_connected( id, root_id ) )	{

					// if( id == root_identity )
					//   return( root_id )

					if( root_id != root_identity )	{
						cout << "JOIN: " << root_identity << " -> " << root_id << endl;
					}

					return( true );
				}
			}
			return( false );
		}
#endif
#if 0
		bool path_connected( int id )	{ // recursion: DFS with system stack

			if( id == identity ) // self check, no err
				return( true );
			if( visited )
				return( false );
			visited = true;

			for( auto& snp: edge_list )	{
				if( snp->path_connected( id ) )	{
					return( true );
				}
			}
			return( false );
		}
#endif

		int identity;
		int root_identity; // connectivity id
		bool visited;
		forward_list< const shared_ptr< node > > edge_list;
};

/////////////////////////////////////////////////////////////

// Undirected Cyclic Graph

class graph	{

	public:
		graph()	{
		}
		~graph()	{
		}
		bool insert( node* new_node_p )	{

			if( node_map.count( new_node_p->identity ) != 0 )	{ // duplicate insertion check/err
				cout << "graph::insert( " << new_node_p->identity << " ) DUPLICATE" << endl;
				// delete?
				return( false );
			}

			node_map.insert( make_pair( new_node_p->identity, shared_ptr< node >( new_node_p ) ) );
//			node_map.insert( make_pair( new_node_p->identity, node_map[ new_node_p->identity ] ) ); // not shared
			return( true );
		}
		void connect( int a, int b )	{

			if( a == b )	{ // self connect check err?
				return;
			}
			if( node_map[ a ]->edge_connected( b ) )	{ // duplicate connect
				return;
			}
			node_map[ a ]->connect( node_map[ b ] );
			node_map[ b ]->connect( node_map[ a ] );

			int root_a_id = find_root( node_map[ a ].get() );
			int root_b_id = find_root( node_map[ b ].get() );
			node_map[ root_a_id ]->root_identity = root_b_id;
		}
		void reset_roots( void )	{
			for( auto& np: node_map )	{
				np.second->root_identity = np.second->identity;
			}
		}
#if 1
		int find_root( node* node_p )	{

			if( node_p->root_identity == node_p->identity )	{
				return( node_p->root_identity );
			}
			node_p->root_identity = find_root(  // path compression
				node_map[ node_p->root_identity ].get()
			);
			return( node_p->root_identity );
		}
		int find_root( int id )	{
			return( find_root( node_map[ id ].get() ) );
		}
#endif
		void reset_visits( void )	{
			for( auto& np: node_map )	{
				np.second->visited = false;
			}
		}
#if 0
		bool connected( int a, int b )	{ // recursion: DFS with system stack
			reset_visits();
			return( node_map[ a ]->path_connected( b ) );
		}
#elif 1
		bool connected( int a, int b )	{ // iteration: DFS with std::stack

			reset_visits();
			stack< node* > dfs_stack;
			dfs_stack.push( node_map[ a ].get() ); // use raw pointers locally

			node* target_node_p = node_map[ b ].get();

			while( ! dfs_stack.empty() )	{

				node* node_p = dfs_stack.top();
				dfs_stack.pop();

				if( node_p->identity == target_node_p->identity )
					return( true );
				if( find_root( target_node_p ) == find_root( node_p ) )
					return( true );
				node_p->visited = true;

				for( auto& snp: node_p->edge_list )	{
					if( ! snp->visited )	{
						dfs_stack.push( snp.get() ); // only unvisited nodes are pushed
					}
				}
			}
			return( false );
		}
#else
		bool connected( int a, int b )	{ // iteration: BFS with std::list OR std::queue

			reset_visits();
			list< node* > bfs_list;
			bfs_list.push_back( node_map[ a ].get() );

			node* target_node_p = node_map[ b ].get();

			while( ! bfs_list.empty() )	{

				node* node_p = bfs_list.front();
				bfs_list.pop_front();

				if( node_p->identity == target_node_p->identity )
					return( true );
				if( find_root( target_node_p ) == find_root( node_p ) )
					return( true );
				node_p->visited = true;

				for( auto& snp: node_p->edge_list )	{
					if( ! snp->visited )	{
						bfs_list.push_back( snp.get() );
					}
				}
			}
			return( false );
		}
#endif
		void remove( int id )	{

			shared_ptr< node > remp = node_map[ id ];
			for( auto& np: node_map )	{

				remp->disconnect( np.second );
				np.second->disconnect( remp );

				// needs to reset root_identity... traverse entire map for same root, set each to self
			}
			node_map.erase( id );
		}

		char alphanumeric_filter( int i )	{
			static string ANF = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ*";
			size_t len = ANF.size();
			return( ANF[ i % len ] );
		}
		void print_nodes( void )	{

			for( auto& np: node_map )	{

				cout << "node [" << alphanumeric_filter( np.second->identity ) << "]";
				if( np.second->identity == np.second->root_identity )	{
					if( np.second->edge_list.empty() )
						cout << " - ";
					else
						cout << " * ";
				}
				else
					cout << " > " ;
				cout << alphanumeric_filter( np.second->root_identity ) << " { ";
				bool f = true; // first pass
				for( auto& e: np.second->edge_list )	{

					if( f ) { f = false; } else { cout << ", "; }
					cout << alphanumeric_filter( e->identity );
				}
				cout << " }" << endl;
			}
		}
		void print_paths( void )	{

			size_t size = node_map.size();
			vector< int > id_vec;
			cout << " x[ ";
			for( auto& np: node_map )	{
				id_vec.push_back( np.second->identity );
				cout << alphanumeric_filter( np.second->identity ) << " ";
			}
			cout << "]" << endl;
			for( auto& np: node_map )	{

				cout << "[" << alphanumeric_filter( np.second->identity ) << "] ";
				bool f = true; // first
				for( int c=0; c<size; c++ )	{

					if( f ) { f = false; } else { cout << " "; }

					if( np.second->identity == id_vec[ c ] )	{
						if( np.second->edge_list.empty() )	{
#if 0
							cout << "o";
#else
							cout << alphanumeric_filter( np.second->root_identity );
#endif
						}
						else	{
							cout << " ";
						}
					}
					else
					if( connected( np.second->identity, id_vec[ c ] ) )	{
#if 0
						if( np.second->edge_connected( id_vec[ c ] ) )	{
							cout << "O";
						}
						else	{
							cout << "+";
						}
#else
						cout << alphanumeric_filter( np.second->root_identity );
#endif
					}
					else	{
						cout << " ";
					}
				}
				cout << endl;
			}
		}

#if 1
		map< int, shared_ptr< node > > // ordered for print out
			node_map; // Fast range iter. Slow [key]. Search, removal, insertion: logarithmic complexity.
#else
		unordered_map< int, shared_ptr< node > >
			node_map; // Slow range iter. Fast [key]. Search, removal, insertion: average constant-time complexity.
#endif
};

/////////////////////////////////////////////////////////////

#if 1

void test_graph_node_perf( int size, double prob, int reps = -1, bool print_out = false )	{

	cout << "TEST: test_graph_node_perf: " << size << " prob: " << prob << " reps: " << reps << endl;
	Timer T_full, T_part;
	double dt_construct = 0.0;
	double dt_stat = 0.0;
	double dt_search = 0.0;
	T_full.reset();

	cout << "Construction:" << endl;
	T_part.reset();

	graph G;
	vector< int > id_vec; // This is the id protection.
	for( int i=0; i<size; i++ )	{
		G.insert( new node( i ) );
		id_vec.push_back( i );
	}
	size_t G_size = G.node_map.size(); // actual size after insertions

	unsigned seed = static_cast <unsigned> ( time(0) ) % 1000;
#if 0
	seed = 2;
	seed = 914;  // p=0.7 : Bench: 21 -> 27 -> 37 % connected
	seed = 305;
	seed = 197;
#endif
	cout << " seed: " << seed << endl;
	srand( seed );

	for( int c=0; c< G_size; c++ )	{
		if( prob > (double) rand() / ((double)RAND_MAX + 1.0 ) )	{

			int ar = id_vec[ rand() % G_size ];
			int br = id_vec[ rand() % G_size ];
			G.connect( ar, br );
		}
	}
	dt_construct = T_part.elapsed();
	cout << " Elapsed: " << dt_construct << " sec" << endl;

	cout << "Statistics:" << endl;
	T_part.reset();

	int num_edges = 0;
	int max_edges = 0;
	int num_orphans = 0;
	for( auto& np: G.node_map )	{
		int len = (int)distance( np.second->edge_list.begin(), np.second->edge_list.end() );
		num_edges += len;
		if( len > max_edges ) max_edges = len;
		if( len == 0 ) num_orphans++;
	}
	cout << " avg edges: " << (double)( num_edges ) / (double)G_size << endl;
	cout << " max edges: " << max_edges << endl;
	cout << " orphans:   " << 100.0 * (double)num_orphans / (double)G_size << "%" << endl;
	dt_stat = T_part.elapsed();
	cout << " Elapsed: " << dt_stat << " sec" << endl;

	if( print_out )	{
		cout << "--" << endl;
		G.print_nodes();
		cout << "--" << endl;
	}

	cout << "Search:" << endl;
	T_part.reset();

	long long int connection_count = 0;
	if( reps < 0 )	{ // test all pairs

		reps = 0;
		int prog = 0;
		for( auto& np: G.node_map )	{
			for( int c=0; c<G_size; c++ )	{

				if( G.connected( np.second->identity, id_vec[ c ] ) )	{
					connection_count++;
				}
				reps++;
			}
			prog++;
//			cout << "  pass: " << reps << ": " << (double)prog / (double)G_size << "%" << endl;
		}
	}
	else	{ // fixed number of passes

		if( reps == 0 ) reps = (int)G_size;

		for( int c=0; c<reps; c++ )	{

			int ar = id_vec[ rand() % G_size ];
			int br = id_vec[ rand() % G_size ];
			if( G.connected( ar, br ) )	{
				connection_count++;
			}
 //			cout << "  pass: " << c << endl;
		}
	}
	cout << " connections:  " << connection_count << ": " << 100.0 * (double)connection_count / (double)reps << "%" << endl;
	dt_search = T_part.elapsed();
	cout << " Elapsed: " << dt_search << " sec" << endl;

	cout << "Parts Elapsed: " << dt_construct << " : " << dt_stat << " : " << dt_search << endl;
	cout << "Total Elapsed: " << T_full.elapsed() << " sec" << endl;
	if( print_out )	{
		cout << "==" << endl;
		G.print_nodes();
		cout << "--" << endl;
		G.print_paths();
		cout << "--" << endl;
	}
}

#endif

/////////////////////////////////////////////////////////////

void test_graph_node_class()	{

	graph G;
	G.insert( new node( 0 ) );
	G.insert( new node( 1 ) );
	G.insert( new node( 2 ) );
	G.insert( new node( 3 ) );
	G.insert( new node( 4 ) );
	G.insert( new node( 5 ) );

	G.connect( 0, 5 );
	G.connect( 1, 2 );
	G.connect( 4, 3 );
	G.connect( 3, 5 );
	G.connect( 5, 1 );

	G.print_nodes();
	cout << "==" << endl;

	G.print_paths();
	cout << "--" << endl;

#if 0
//	G.find_root( 0 );
	G.find_root( 1 );
//	G.find_root( 2 );
#endif

	G.print_nodes();
	cout << "--" << endl;

#if 0
	G.remove( 1 );
	G.print_paths();
	cout << "--" << endl;

	G.insert( new node( 1 ) );
	G.print_paths();
	cout << "--" << endl;

	G.connect( 0, 1 );
	G.print_paths();
	cout << "--" << endl;

	G.connect( 1, 2 );
	G.print_paths();
	cout << "--" << endl;
#endif

}

/////////////////////////////////////////////////////////////

#if 1

int graph_perf_scenarios( void ) {

#if 1
	double p = 0.65;

		test_graph_node_perf( 10, p, -1, true );
//		test_graph_node_perf( 50, p, -1, true );

	// sys-stack recursion on node:

//		test_graph_node_perf( 10000, p, 10000 );  // 1.62
//		test_graph_node_perf( 20000, p, 10000 );  // 3.6
//		test_graph_node_perf( 40000, p, 10000 );  // 12.2
//		test_graph_node_perf( 80 * 1000, p, 10000 );  // 25.7
//		test_graph_node_perf( 160 * 1000, p, 10000 );  // 57.0
//		test_graph_node_perf( 320 * 1000, p, 10000 );  // 115.2
//		test_graph_node_perf( 640 * 1000, p, 10000 );  // 461
//		test_graph_node_perf( 1000 * 1000, p, 10000 );  // 697 (11.6 min)
//		test_graph_node_perf( 1280 * 1000, p, 10000 );  // stack blown

p = 0.6;
//		test_graph_node_perf( 10000, p, 10000 );  // 0.77
//		test_graph_node_perf( 20 * 1000, p, 10000 );  // 1.6
//		test_graph_node_perf( 40 * 1000, p, 10000 );  // 5.2
//		test_graph_node_perf( 80 * 1000, p, 10000 );  // 14.5
//		test_graph_node_perf( 160 * 1000, p, 10000 );  // 33.5
//		test_graph_node_perf( 320 * 1000, p, 10000 );  // 71.5

	// DFS:
p = 0.8;
//		test_graph_node_perf( 1000 * 1000, p, 10000 );  // (est 10 min) 642 (10.7 min)
//		test_graph_node_perf( 2000 * 1000, p, 10000 );  // (est 20 min) 1484 (25 min)


p = 0.6;
//		test_graph_node_perf( 10000, p, 10000 );  // 0.79
//		test_graph_node_perf( 20000, p, 10000 );  // 1.7
//		test_graph_node_perf( 40000, p, 10000 );  // 5.5
//		test_graph_node_perf( 80000, p, 10000 );  // 12.2
//		test_graph_node_perf( 160 * 1000, p, 10000 );  // 29.8
//		test_graph_node_perf( 320 * 1000, p, 10000 );  // 66.7
//		test_graph_node_perf( 640 * 1000, p, 10000 );  // 99.6
//		test_graph_node_perf( 1280 * 1000, p, 10000 );  // 454.0 (7.6 min)
//		test_graph_node_perf( 2560 * 1000, p, 10000 );  // 656.0 (11 min)
//		test_graph_node_perf( 2560 * 1000, p, 100 );  // 7.3
//		test_graph_node_perf( 5120 * 1000, p, 100 );  // 13.9
//		test_graph_node_perf( 10 * 1000 * 1000, p, 100 );  // 31.1
//		test_graph_node_perf( 20 * 1000 * 1000, p, 100 );  // 56.1
//		test_graph_node_perf( 40 * 1000 * 1000, p, 100 );  // 157.6

//		test_graph_node_perf( 100 * 1000, p, 1000 );  // 3.25
//		test_graph_node_perf( 200 * 1000, p, 1000 );  // 8.28
//		test_graph_node_perf( 400 * 1000, p, 1000 );  // 18.2
//		test_graph_node_perf( 800 * 1000, p, 1000 );  // 64.0

p = 0.8;
//		test_graph_node_perf( 400 * 1000, p, 100 );  // 1.63
//		test_graph_node_perf( 800 * 1000, p, 100 );  // 4.65
//		test_graph_node_perf( 1600 * 1000, p, 100 );  // 13.9
//		test_graph_node_perf( 3200 * 1000, p, 100 );  // 24.2

//		test_graph_node_perf( 5 * 1000 * 1000, p, 10 );  // 3.8 : 0.25 : 5.0
//		test_graph_node_perf( 10 * 1000 * 1000, p, 10 ); // 8.1 : 0.5 : 9.2
//		test_graph_node_perf( 20 * 1000 * 1000, p, 10 ); // 16.1 : 1.0 : 12.5
//		test_graph_node_perf( 40 * 1000 * 1000, p, 10 ); // 32.5 : 2.7 : 29.4
//		test_graph_node_perf( 80 * 1000 * 1000, p, 10 ); // 76.4 : 20.2 : 157.7

	// BFS:

//		test_graph_node_perf( 100 * 1000, p, 100 ); // 0.05 : 0.004 : 1.25
//		test_graph_node_perf( 200 * 1000, p, 100 ); // 2.95
//		test_graph_node_perf( 400 * 1000, p, 100 ); // 8.04
//		test_graph_node_perf( 800 * 1000, p, 100 ); // 16.0
//		test_graph_node_perf( 1600 * 1000, p, 100 ); // 31.1
//		test_graph_node_perf( 3200 * 1000, p, 100 ); // 70.5

//		test_graph_node_perf( 1 * 1000 * 1000, p, 10 ); // 0.72 : 0.04 : 1.9
//		test_graph_node_perf( 2 * 1000 * 1000, p, 10 ); // 1.5 : 0.1 : 2.7
//		test_graph_node_perf( 4 * 1000 * 1000, p, 10 ); // 3.0 : 0.2 : 10.9
//		test_graph_node_perf( 5 * 1000 * 1000, p, 10 ); // 3.7 : 0.23 : 4.4
//		test_graph_node_perf( 10 * 1000 * 1000, p, 10 ); // 7.8 : 0.47 : 30.2

p = 1.0;

//		test_graph_node_perf( 1 * 1000 * 1000, p, 20 ); 		// 2.4
//		test_graph_node_perf( 2 * 1000 * 1000, p, 20 ); 		// 4.5
//		test_graph_node_perf( 4 * 1000 * 1000, p, 20 ); 		// 8.6
//		test_graph_node_perf( 8 * 1000 * 1000, p, 20 ); 		// 16.0
//		test_graph_node_perf( 16 * 1000 * 1000, p, 20 ); 		// 44.2
//		test_graph_node_perf( 32 * 1000 * 1000, p, 20 ); 		// 97.7
//		test_graph_node_perf( 64 * 1000 * 1000, p, 20 ); 		// 59.2 : 297.4
//		test_graph_node_perf( 128 * 1000 * 1000, p, 20 ); 	// 2020.86 (33.7 min) : (2.5 hrs? KILLED)

#endif

    return 0;
}

#endif

/////////////////////////////////////////////////////////////

void test_graph_algorithms()	{

	test_graph_node_class();


}

