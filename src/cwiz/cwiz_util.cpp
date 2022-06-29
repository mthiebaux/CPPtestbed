//
//  cwiz_util.cpp
//  CppTestbed
//
//  Created by Marcus Thiebaux on 10/28/19.
//  Copyright © 2019 Marcus Thiebaux. All rights reserved.
//

#include "cwiz_util.hpp"

#include <unistd.h>
#include <cstdlib>

//#include <string>
//#include <vector>
//#include <future>

using namespace std;
using namespace cwiz;

//////////////////////////////////////////////////////////////////////////////////////////////////

vector<string> cwiz::gamblin_split( vector<string>& out_toks, const string& str, const string& delim ) {
/*
	Todd Gamblin
	https://stackoverflow.com/a/289365
	...here's a split() function I wrote to make my life easier. 
	It'll break a string into pieces using any of the chars in "delim" 
	as separators. Pieces are appended to the "parts" vector:
*/
	size_t start, end = 0;
	while (end < str.size()) {
  
		start = end;
		while (start < str.size() && (delim.find(str[start]) != string::npos)) {
			start++;  // skip initial whitespace
		}
	
		end = start;
		while (end < str.size() && (delim.find(str[end]) == string::npos)) {
			end++; // skip to end of word
		}
	
		if (end-start != 0) {  // just ignore zero-length strings.
			out_toks.push_back(string(str, start, end-start));
		}
	}
	
	return( out_toks );
}

vector <string> cwiz::string_split( vector <string> & out_toks, const string & input_str, const string & delims )	{
	
	char *input_dup = strdup( input_str.data() );
	char *str_ptr = nullptr;
	char *tok;
	
	for(
		tok = strtok_r( input_dup, delims.data(), &( str_ptr ) );
		tok != nullptr;
		tok = strtok_r( nullptr, delims.data(), &( str_ptr ) )
		) {
		out_toks.push_back( tok );
	}

/*
// // COMPACT
//	char *str_ptr = input_dup;
	while( ( tok = strtok_r( str_ptr, delims.data(), &( str_ptr ) ) ) ) {
		tokens.push_back( tok );
	}
*/
	
	free( input_dup );
	return( out_toks );
}

//////////////////////////////////////////////////////////////////////////////////////////////////


// this is the singleton resource

string cwiz::blocking_read_console_thread_function( void ) {

    string line;
    getline( cin, line );
    return line;
}


// deprecated

#if 0
bool cwiz::control_command_thread( string & s, bool resume )	{

	static future<string> thread_future = async( launch::async, blocking_read_console_thread_function );

	if( resume )	{
	
		thread_future = async( launch::async, blocking_read_console_thread_function );
		
		return( false );
	}

	if( thread_future.wait_for( chrono::seconds( 0 ) ) == future_status::ready )	{

		s = thread_future.get();
		
		return( true );
	}

	return( false );
}
#endif


