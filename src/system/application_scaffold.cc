// EPOS Application Scaffold and Application Component Implementation

#include <system.h>

__BEGIN_SYS

// Application class attributes
char Application::_preheap[];
Heap * Application::_heap;

__END_SYS

__BEGIN_UTIL

OStream::Begl begl;
OStream::Endl endl;
OStream::Hex hex;
OStream::Dec dec;
OStream::Oct oct;
OStream::Bin bin;
OStream::Err error;

__END_UTIL

__BEGIN_API

// Global objects
OStream cout;
OStream cerr;

__END_API

