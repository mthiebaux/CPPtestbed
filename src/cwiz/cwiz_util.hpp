//
//  cwiz_util.hpp
//  CppTestbed
//
//  Created by Marcus Thiebaux on 10/28/19.
//  Copyright © 2019 Marcus Thiebaux. All rights reserved.
//

#ifndef cwiz_util_hpp
#define cwiz_util_hpp

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <future>

namespace cwiz {

	/////////////////////////////////
	// Utilities:
	
	std::vector <std::string>
	gamblin_split( std::vector <std::string> & out_toks, const std::string & str, const std::string & delim );
	
	std::vector <std::string>
	string_split( std::vector <std::string> & out_toks, const std::string & input_str, const std::string & delims );
	
	const std::string
	whitespace_delim = " \t\n\r";

	/////////////////////////////////
	// Template console command execution class:
	
	class empty_state {}; // empty class for default state
	
	template <typename T>
	class state_executor; // forward declaration of state derived class below

	template <typename T>
	using state_callback_type =
	int (*)( std::vector <std::string> &, state_executor <T> & ); // callback signature

	template <typename T>
	using state_callback_map_type =
	std::unordered_map< std::string, state_callback_type <T> >; // hashmap signature

//	template <typename state_type = empty_state> // default to empty base state
	template <typename T = empty_state> // default to empty base state
	class state_executor : public T	{ // derive from client supplied base state class
		
		bool quit_flag;

		public:
		
		state_executor( void )	{

			quit_flag = false;
			quit_key = "q";
		}
		
		std::string quit_key;
		bool quit( bool set = false ) { if( set ) { quit_flag = true; } return( quit_flag ); }

		state_callback_map_type <T> callbacks;

		bool command( std::vector <std::string> & tokens );
		bool command( std::string & line );
	};
	
	template <typename T = empty_state> // default to empty
	class console_executor : public state_executor <T>	{ // derive from state class
		
		// static not needed
		std::future <std::string> console_future; // console_future.valid() == false

		public:
		
		console_executor( void ) {
//			std::cout << "valid: " <<  console_future.valid() << std::endl;
		}

		bool ready( void );
		bool execute( void );
		void resume( void );
		std::future <std::string> launch( void );

		bool command( void ); // check console, execute, and resume
	};
	
} // namespace cwiz

#include "cwiz_util_defs.hpp" // for template class member definitions

#endif /* cwiz_util_hpp */
