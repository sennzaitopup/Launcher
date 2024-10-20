#ifndef _ioSingletone_h_
#define _ioSingletone_h_

#include <assert.h>

/* Original version Copyright (C) Scott Bilas, 2000.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Bilas, 2000"
 */

#pragma warning (disable : 4311)
#pragma warning (disable : 4312)

template < typename T > class Singleton
{
protected:
	static T* ms_Singleton;

public:
	Singleton()
	{
		assert( !ms_Singleton );
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
	}

	~Singleton()
	{
		assert( ms_Singleton );
		ms_Singleton = 0;
	}

	static T& GetSingleton()
	{
		assert( ms_Singleton );
		return (*ms_Singleton);
	}

	static T* GetSingletonPtr()
	{
		return ( ms_Singleton );
	}
};

#endif
