//
//  cwiz_util_defs.hpp
//  CppTestbed
//
//  Created by Marcus Thiebaux on 10/28/19.
//  Copyright © 2019 Marcus Thiebaux. All rights reserved.
//

#ifndef cwiz_util_defs_hpp
#define cwiz_util_defs_hpp

namespace cwiz {

	/////////////////////////////////
	// Template command execution:

	template <typename T>
	bool cwiz::state_executor<T>::command( std::vector <std::string> & tokens )	{

		bool prompt = false;

		if( tokens.size() > 0 )	{

			std::string cmd = tokens[ 0 ];
			tokens.erase( tokens.begin() ); // remove from front

			if( cmd == quit_key )	{

				quit( true ); // don't prompt


			}
			else	{

				if( callbacks.find( cmd ) != callbacks.end() )	{ // if found


			// ignore: nested quit?

					int ignore = callbacks[ cmd ]( tokens, *this );

					if( ignore )	{
						std::cout << "ignored" << std::endl;
					}
				}

				prompt = !quit(); // prompt here
			}
		}

		return( prompt );
	}

	template <typename T>
	bool cwiz::state_executor<T>::command( std::string & line )	{

		std::vector <std::string> tokens;
		string_split( tokens, line, whitespace_delim );

		if( tokens.size() == 0 )	{

			tokens.push_back( "" ); // ensure empty command generates prompt
		}

		bool prompt = command( tokens );

		return( prompt );
	}

	/////////////////////////////////
	// console execution:

	// this is the raw blocking singleton resource
	std::string
	blocking_read_console_thread_function( void );

#if 0
	// this is the singleton static thread handle, launch()
	template <typename T>
	std::future<std::string> cwiz::console_executor<T>::console_future =
	std::async( std::launch::async, blocking_read_console_thread_function );
#endif

	// singleton control
	template <typename T>
	bool cwiz::console_executor<T>::ready( void ) {
		if( console_future.valid() )	{
			return( console_future.wait_for( std::chrono::seconds( 0 ) ) == std::future_status::ready );
		}
		return( false );
	}

	template <typename T>
	bool cwiz::console_executor<T>::execute( void ) {
		std::string line = console_future.get();
		return( cwiz::state_executor<T>::command( line ) );
	}

	template <typename T>
	void cwiz::console_executor<T>::resume( void ) {
//		console_future = std::async( std::launch::async, blocking_read_console_thread_function );
		console_future = launch();
	}

	template <typename T>
	std::future <std::string> cwiz::console_executor<T>::launch( void ) {
		return( std::async( std::launch::async, blocking_read_console_thread_function ) );
	}

	template <typename T>
	bool cwiz::console_executor<T>::command( void )	{

#if 1
		if( ready() )	{
			if( execute() )	{
				resume();
				return( true );
			}
		}
		return( false );
#else
		bool prompt = false;
		if( ready() )	{

			prompt = execute();
			if( prompt )	{
				resume();
			}
		}
		return( prompt );
#endif
	}

#if 0
	template <typename T>
	bool cwiz::console_executor<T>::command( void )	{

		bool prompt = false;

		// if thread result is ready
		if( console_future.wait_for( std::chrono::seconds( 0 ) ) == std::future_status::ready )	{

			std::string line = console_future.get();

			prompt = cwiz::state_executor<T>::command( line );

			if( prompt )	{

				// resume
				console_future = std::async( std::launch::async, blocking_read_console_thread_function );
			}
		}

		return( prompt );
	}
#endif

} // namespace cwiz

#endif /* cwiz_util_defs_hpp */
