
#include <unistd.h>
#include <math.h>

#include <iostream>
#include <string>
//#include <algorithm>
#include <map>
#include <unordered_map>
//#include <set>
//#include <unordered_set>
#include <queue>
#include <stack>
#include <array>
#include <vector>
#include <list>
#include <forward_list>

#include "timer.hpp"

using namespace std;

// MAXIMUM: double max = std::numeric_limits<double>::max();
// INFINITY: double inf = std::numeric_limits<double>::infinity();

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class node	{

	public:
	node( int id )	{
		identity = id;
		component = id;
//		processing = false;
		visited = false;
	}

	void connect( int id, double w = 1.0 )	{
		edges.push_back( id );
		weights.push_back( w );
	}

	int identity;
	int component;
//	bool processing;
	bool visited;

	vector< int > edges;
	vector< double > weights;

//	forward_list< int > edges;
//	forward_list< double > weights;
};

class edge	{

	public:
	edge( int src, int dst, double w )	{

	}
};

/*
	vector<string> vs;
	...
	for (const auto &s : vs)	{

        cout << s << endl;
    }
    for (Foo &i: theContainer)	{

	     // Do stuff with i
	}
*/

class graph	{

	public:
	graph( int n, bool d = false )	{

		for( int i=0; i<n; i++ )	{
			nodes.push_back( new node( i ) );
		}

		directed = d;
	}

	void print_node( int i )	{
		cout << "node: " << nodes[ i ]->identity << " -> " << nodes[ i ]->component << endl;
	}

	int root( int i )	{

		while( nodes[ i ]->component != i )	{

			int c = nodes[ i ]->component;
			nodes[ i ]->component = nodes[ c ]->component; // compress

			i = nodes[ i ]->component;
		}
		return( i );
	}

	void connect( int fr, int to, double w = 1.0 )	{

	// store edge, weight

		nodes[ fr ]->connect( to, w );
		if( directed == false )	{

			nodes[ to ]->connect( fr, w );

			nodes[ root( fr ) ]->component = root( to );
		}
	}

	bool connected_UF( int fr, int to )	{

		return( root( fr ) == root( to ) );
	}

	bool connected_DFS( int fr, int to )	{

		for( int i=0; i< nodes.size(); i++ )	{
			nodes[ i ]->visited = false;
		}

		stack< node* > S;
		S.push( nodes[ fr ] );

		while( S.empty() == false )	{

			node* node_p = S.top();
			node_p->visited = true;
			S.pop();

			for( int i=0; i< node_p->edges.size(); i++ )	{

				int id = node_p->edges[ i ];
				if( id == to )	{

					return( true );
				}
				if( nodes[ id ]->visited == false )	{

					S.push( nodes[ id ] );
				}
			}
		}
		return( false );
	}

	bool connected_BFS( int fr, int to )	{

		for( int i=0; i< nodes.size(); i++ )	{
			nodes[ i ]->visited = false;
		}

		queue< node* > Q;
		Q.push( nodes[ fr ] );

		while( Q.empty() == false )	{

			node* node_p = Q.front();
			node_p->visited = true;
			Q.pop();

			for( int i=0; i< node_p->edges.size(); i++ )	{

				int id = node_p->edges[ i ];
				if( id == to )	{

					return( true );
				}
				if( nodes[ id ]->visited == false )	{

					Q.push( nodes[ id ] );
				}
			}
		}
		return( false );
	}

	vector< node* > nodes;
	bool directed;
};

void test_primitive_graph( void )	{

	graph G( 6 );

	G.connect( 0, 1 );
	G.connect( 2, 3 );
	G.connect( 2, 4 );
	G.connect( 2, 5 );

	for( int i=0; i< G.nodes.size(); i++ )	{
		G.print_node( i );
	}

#if 1
	cout << "( 0, 1 ): " << G.connected_UF( 0, 1 ) << endl;
	cout << "( 0, 2 ): " << G.connected_UF( 0, 2 ) << endl;
	cout << "( 3, 4 ): " << G.connected_UF( 3, 4 ) << endl;
	cout << "( 3, 1 ): " << G.connected_UF( 3, 1 ) << endl;

	cout << "( 0, 1 ): " << G.connected_DFS( 0, 1 ) << endl;
	cout << "( 0, 2 ): " << G.connected_DFS( 0, 2 ) << endl;
	cout << "( 3, 4 ): " << G.connected_DFS( 3, 4 ) << endl;
	cout << "( 3, 1 ): " << G.connected_DFS( 3, 1 ) << endl;

	cout << "( 0, 1 ): " << G.connected_BFS( 0, 1 ) << endl;
	cout << "( 0, 2 ): " << G.connected_BFS( 0, 2 ) << endl;
	cout << "( 3, 4 ): " << G.connected_BFS( 3, 4 ) << endl;
	cout << "( 3, 1 ): " << G.connected_BFS( 3, 1 ) << endl;
#endif

	for( int i=0; i< G.nodes.size(); i++ )	{
		G.print_node( i );
	}
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class Node	{

	public:
		Node( int id )	{
			identity = id;
			root_identity = id; // init to self
			visited = false;
		}
		~Node()	{
		}

		// https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value
		// 	pass by const reference

		void connect( const shared_ptr< Node >& snp )	{

			if( snp->identity != identity )	{ // self check, no err?

				edge_list.push_front( snp );
//				edge_list.push_front( make_pair( snp, 1.0 ) );
			}
		}

		void disconnect( const shared_ptr< Node >& rem )	{ // breaks connected components

			for( auto& np: edge_list )	{
				if( np == rem )	{

					// check root_identity...

					edge_list.remove( rem );
					return;
				}
			}
		}

		bool adjacent( int id )	{

			for( auto& snp: edge_list )	{

				if( snp->identity == id )	{
					return( true );
				}
			}
			return( false );
		}

		int identity;
		int root_identity; // connected component id

		bool visited;

		forward_list< const shared_ptr< Node > > edge_list;
//		forward_list< const shared_ptr< pair< Node, double > > > edge_list;
};

/////////////////////////////////////////////////////////////

class Graph	{

	public:

		Graph()	{
		}
		~Graph()	{
		}

#if 1
		map< int, shared_ptr< Node > > // ordered for print out
			node_map; // Fast range iter. Slow [key]. Search, removal, insertion: logarithmic complexity.
#else
		unordered_map< int, shared_ptr< Node > >
			node_map; // Slow range iter. Fast [key]. Search, removal, insertion: average constant-time complexity.
#endif

		bool insert( Node* new_node_p )	{

			if( node_map.count( new_node_p->identity ) != 0 )	{ // duplicate insertion check/err
				cout << "Graph::insert( " << new_node_p->identity << " ) DUPLICATE" << endl;
				// delete?
				return( false );
			}

			node_map.insert( make_pair( new_node_p->identity, shared_ptr< Node >( new_node_p ) ) );
			return( true );
		}

		bool insert( int id )	{
			return( insert( new Node( id ) ) );
		}

		// bidirectional
		void connect( int a, int b )	{

			if( a == b )	{ // self connect check err?
				return;
			}
			if( node_map[ a ]->adjacent( b ) )	{ // duplicate connect
				return;
			}
			node_map[ a ]->connect( node_map[ b ] );
			node_map[ b ]->connect( node_map[ a ] );

			int root_a_id = find_root( node_map[ a ].get() ); // move to calc_components iteration?
			int root_b_id = find_root( node_map[ b ].get() );
			node_map[ root_a_id ]->root_identity = root_b_id;
		}

		void reset_roots( void )	{
			for( auto& np: node_map )	{
				np.second->root_identity = np.second->identity;
			}
		}

		void reset_root( int root_id )	{ // only specified component
			for( auto& np: node_map )	{
				if( np.second->root_identity == root_id )	{
					np.second->root_identity = np.second->identity;
				}
			}
		}

#if 1
		// convert recursive to iterative...

		int find_root( Node* node_p )	{

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

#if 1
		bool connected( int a, int b )	{ // iteration: DFS with std::stack

			reset_visits();

			stack< Node* > dfs_stack;
			dfs_stack.push( node_map[ a ].get() ); // use raw pointers locally

			Node* target_node_p = node_map[ b ].get();

			while( ! dfs_stack.empty() )	{

				Node* node_p = dfs_stack.top();
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

			list< Node* > bfs_list;
			bfs_list.push_back( node_map[ a ].get() );

			Node* target_node_p = node_map[ b ].get();

			while( ! bfs_list.empty() )	{

				Node* node_p = bfs_list.front();
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

			shared_ptr< Node > remp = node_map[ id ];
			for( auto& np: node_map )	{

				remp->disconnect( np.second );
				np.second->disconnect( remp );

				// needs to reset root_identity...
				// traverse entire map for same root, set each to self
			}
			node_map.erase( id );
		}

////////////////////////////

		char alphanumeric_filter( int i )	{
			static string ANF = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ*";
			size_t len = ANF.size();
			return( ANF[ i % len ] );
		}

		void print_nodes( void )	{

			for( auto& np: node_map )	{

				cout << "Node [" << alphanumeric_filter( np.second->identity ) << "]";
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
						if( np.second->adjacent( id_vec[ c ] ) )	{
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
};

/////////////////////////////////////////////////////////////

void test_graph_node_class()	{

	Graph G;

	G.insert( 0 );
	G.insert( 1 );
	G.insert( 2 );
	G.insert( 3 );
	G.insert( 4 );
	G.insert( 5 );

	G.connect( 0, 5 );
	G.connect( 1, 2 );
	G.connect( 4, 3 );
	G.connect( 3, 5 );
//	G.connect( 5, 1 );

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

//	G.insert( new Node( 1 ) );
	G.insert( 1 );
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

void test_graph_perf( int size, double prob, int reps = -1, bool print_out = false )	{

	cout << "TEST: test_graph_node_perf: " << size << " prob: " << prob << " reps: " << reps << endl;
	Timer T_full, T_part;
	double dt_construct = 0.0;
	double dt_stat = 0.0;
	double dt_search = 0.0;
	T_full.reset();

	cout << "Construction:" << endl;
	T_part.reset();

	Graph G;
	vector< int > id_vec; // This is the id protection.
	for( int i=0; i<size; i++ )	{
//		G.insert( new Node( i ) );
		G.insert( i );
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

int test_graph_perf_scenarios( void ) {

	double p = 0.65;

		test_graph_perf( 10, p, -1, true );
//		test_graph_perf( 50, p, -1, true );

//		test_graph_perf( 10000, p, 10000 );  // 1.62
//		test_graph_perf( 20000, p, 10000 );  // 3.6
//		test_graph_perf( 40000, p, 10000 );  // 12.2
//		test_graph_perf( 80 * 1000, p, 10000 );  // 25.7
//		test_graph_perf( 160 * 1000, p, 10000 );  // 57.0
//		test_graph_perf( 320 * 1000, p, 10000 );  // 115.2
//		test_graph_perf( 640 * 1000, p, 10000 );  // 461
//		test_graph_perf( 1000 * 1000, p, 10000 );  // 697 (11.6 min)
//		test_graph_perf( 1280 * 1000, p, 10000 );  // stack blown

    return 0;
}

/////////////////////////////////////////////////////////////

void test_graph_algorithms( void )	{

	test_primitive_graph();

//	test_graph_node_class();

//	test_graph_perf_scenarios();
}

