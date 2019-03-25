#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>

// This class may seem a little strange if you have never seen a
// singleton before. But it is a very useful, and increasingly
// common data structure, particularly in games.

// You can think of a singleton as a nice way to have a "global"
// object, without the hassle of actually making the variable global
// and then externing it all over the place.

// If you derive your class from CSingleton, and then create a single
// instance of the object in the source file, you can then access that
// object from anywhere by calling the GetSingleton method of your class

template <typename T> class CSingleton
{
	// The one and only instance of this class
	static T * ms_Singleton;

public:
	// Sets up the one and only instance
	CSingleton()
	{
		assert(!ms_Singleton);
		int offset = (int)(T*)1 - (int)(CSingleton<T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
	}

	// Destroys the one and only instance
	~CSingleton()
	{
		assert(ms_Singleton);
		ms_Singleton = 0;
	}

	// Returns the singleton
	static T& GetSingleton()
	{
		assert(ms_Singleton);
		return (*ms_Singleton);
	}

	// Returns a pointer to the singleton
	static T* GetSingletonPtr()
	{
		return ms_Singleton;
	}
};

// This might look a bit confusing, but all it does is for each new singleton type that is created
// it initialises it's static T * ms_Singleton member.
template <typename T> T * CSingleton <T>::ms_Singleton = 0;

#endif