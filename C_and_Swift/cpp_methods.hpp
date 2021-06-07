//
//  cpp_methods.hpp
//  C_and_Swift
//
//  Created by Andrew Trotman on 6/06/21.
//

#ifndef cpp_methods_hpp
#define cpp_methods_hpp

#include <stdio.h>

class something
	{
	public:
		int get_int(int x, int y);
	};

extern "C"
	{
const void *something_construct()
	{
	return (const void *)new something();
	}

void something_destruct(const void *object)
	{
	delete (something *)object;
	}

int something_add(const void *object, int a, int b)
	{
	return ((something *)object)->get_int(a, b);
	}

}

#endif /* cpp_methods_hpp */
