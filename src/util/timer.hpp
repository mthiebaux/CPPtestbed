//
//  timer.hpp
//  CppDemo
//
//  Created by Marcus Thiebaux on 1/13/20.
//  Copyright © 2020 Marcus Thiebaux. All rights reserved.
//

#ifndef timer_hpp
#define timer_hpp

#include <chrono>

/*
	Chrono
	see also:
	https://www.techiedelight.com/measure-elapsed-time-program-chrono-library/
*/

class Timer	{

	public:
		Timer() : begin( hr_clock::now() ) {}

		void reset() { begin = hr_clock::now(); }

		double elapsed() const {
			return( std::chrono::duration_cast< std::chrono::milliseconds > ( hr_clock::now() - begin ).count() / 1000.0 );
		}

	private:
		typedef std::chrono::high_resolution_clock hr_clock;
		std::chrono::time_point< hr_clock > begin;
};

#endif /* timer_hpp */

