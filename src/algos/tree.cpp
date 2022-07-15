#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

/////////////////////////////////////////////////////////////

// https://www.interviewkickstart.com/learn/tree-traversals-inorder-preorder-and-postorder
//
//	in-order, pre-order, post-order, level-order

template <typename T> class BinaryTree	{

	public:

		BinaryTree( void )	{
			root = nullptr;
		}

		void insert( T value )	{
			root = insert( root, value );
		}

		int height( void )	{
			return( height( root ) );
		}

		bool find( T value )	{
			return( find( root, value ) );
		}

		void print( void )	{

			cout << "height: " << height() << endl;

			cout << "preorder recurr:" << endl;
			print_preorder_recurr( root, 0 );

			cout << "inorder recurr:" << endl;
			print_inorder_recurr( root, 0 );

			cout << "preorder iter: " << endl;
			print_preorder_iter();

			cout << "levelorder iter:" << endl;
			print_levelorder_iter();
		}

	private:

		class Node	{

			public:

				Node( T v ) {
					value = v;
					left = nullptr;
					right = nullptr;
				}

				T value;
				Node * left;
				Node * right;
		};

		Node * root;

		Node* insert( Node* parent, int value )	{

			if( parent == nullptr )
				return( new Node( value ) );

			if( value <= parent->value )	{
				parent->left = insert( parent->left, value );
			}
			else	{
				parent->right = insert( parent->right, value );
			}
			return( parent );
		}

		int height( Node* parent )	{

			if( parent == nullptr )
				return( 0 );

			int hleft = height( parent->left );
			int hright = height( parent->right );

			return( max( hleft, hright ) + 1 );
		}

		bool find( Node* parent, T value )	{

			if( parent == nullptr )
				return( false );
			if( value == parent->value )
				return( true );

			if( value < parent->value )
				return( find( parent->left, value ) );
			return( find( parent->right, value ) );
		}

		void print_preorder_recurr( Node* parent, int level )	{

			for( int i=0; i< level; i++ ) cout << "  ";

			if( parent == nullptr )	{
				cout << "*" << endl;
				return;
			}

			cout << parent->value << endl;

			if( parent->left || parent->right )	{
				print_preorder_recurr( parent->left, level + 1 );
				print_preorder_recurr( parent->right, level + 1 );
			}
		}

		void print_inorder_recurr( Node* parent, int level )	{

			if( parent == nullptr )	{
				return;
			}

			print_inorder_recurr( parent->left, level + 1 );

			for( int i=0; i< level; i++ ) cout << "  ";
			cout << parent->value << endl;

			print_inorder_recurr( parent->right, level + 1 );
		}

		void print_preorder_iter( void )	{ // DFS

			if( root == nullptr ) return;

			stack< Node* > node_stack;
			node_stack.push( root );

			stack< int > level_stack;
			level_stack.push( 0 );

			while( node_stack.size() )	{

				Node* node = node_stack.top();
				node_stack.pop();

				int level = level_stack.top();
				level_stack.pop();

				for( int i=0; i< level; i++ ) cout << "  ";
				cout << node->value << endl;

				if( node->right )	{
					node_stack.push( node->right );
					level_stack.push( level + 1 );
				}
				if( node->left )	{
					node_stack.push( node->left );
					level_stack.push( level + 1 );
				}
			}
		}

		void print_levelorder_iter( void )	{ // BFS

			if( root == nullptr ) return;

			queue< Node* > node_queue;
			node_queue.push( root );

			queue< int > level_queue;
			level_queue.push( 0 );

			while( node_queue.size() )	{

				Node* node = node_queue.front();
				node_queue.pop();

				int level = level_queue.front();
				level_queue.pop();

				for( int i=0; i< level; i++ ) cout << "  ";
				cout << node->value << endl;

				if( node->left )	{
					node_queue.push( node->left );
					level_queue.push( level + 1 );
				}
				if( node->right )	{
					node_queue.push( node->right );
					level_queue.push( level + 1 );
				}
			}
		}
};

/////////////////////////////////////////////////////////////

void test_tree_algorithms( void )	{

	cout << "test_tree_algorithms:" << endl;

	vector<int> vals =
//		{ 6, 3, 7, 9, 1, 0, 5, 4, 2, 8 };
		{ 12, 17, 6, 10, 3, 7, 18, 14, 19, 9, 16, 1, 11, 0, 5, 13, 4, 2, 15, 8 };

	BinaryTree<int> tree;

	for( int v: vals )	{
		tree.insert( v );
	}

	tree.print();

	cout << "find 5: " << tree.find( 5 ) << endl;
	cout << "find 10: " << tree.find( 10 ) << endl;
	cout << "find 20: " << tree.find( 20 ) << endl;
}

