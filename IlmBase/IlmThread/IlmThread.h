///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005-2012, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_ILM_THREAD_H
#define INCLUDED_ILM_THREAD_H

//-----------------------------------------------------------------------------
//
//	class Thread
//
//	Class Thread is a portable interface to a system-dependent thread
//	primitive.  In order to make a thread actually do something useful,
//	you must derive a subclass from class Thread and implement the
//	run() function.  If the operating system supports threading then
//	the run() function will be executed int a new thread.
//
//	The actual creation of the thread is done by the start() routine
//	which then calls the run() function.  In general the start()
//	routine should be called from the constructor of the derived class.
//
//	The base-class thread destructor will join/destroy the thread.
//
//	IMPORTANT: Due to the mechanisms that encapsulate the low-level
//	threading primitives in a C++ class there is a race condition
//	with code resembling the following:
//
//	    {
//		WorkerThread myThread;
//	    } // myThread goes out of scope, is destroyed
//	      // and the thread is joined
//
//	The race is between the parent thread joining the child thread
//	in the destructor of myThread, and the run() function in the
//	child thread.  If the destructor gets executed first then run()
//	will be called with an invalid "this" pointer.
//
//	This issue can be fixed by using a Semaphore to keep track of
//	whether the run() function has already been called.  You can
//	include a Semaphore member variable within your derived class
//	which you post() on in the run() function, and wait() on in the
//	destructor before the thread is joined.  Alternatively you could
//	do something like this:
//
//	    Semaphore runStarted;
//
//	    void WorkerThread::run ()
//	    {
//		runStarted.post()
//		// do some work
//		...
//	    }
//
//	    {
//		WorkerThread myThread;
//		runStarted.wait ();    // ensure that we have started
//				       // the run function
//	    } // myThread goes out of scope, is destroyed
//	      // and the thread is joined
//
//-----------------------------------------------------------------------------

#include "IlmBaseConfig.h"
#include "IlmThreadExport.h"
#include "IlmThreadNamespace.h"

#if ILMBASE_FORCE_CXX03
#   if defined (_WIN32) || defined (_WIN64)
#   if !HAVE_PTHREAD
#       ifdef NOMINMAX
#          undef NOMINMAX
#       endif
#       define NOMINMAX
#       include <windows.h>
#       include <process.h>
#   else
#      include <pthread.h>
#   endif
#   endif
#else
#   if defined(__MINGW32__) || defined(__MINGW64__)
#      if ILMBASE_FORCE_CXX17
#         define SAFE
#         include "jthread.h"
#      else
#         include "mingw.thread.h"
#      endif
#   else
#      include <thread>
#   endif
#endif

ILMTHREAD_INTERNAL_NAMESPACE_HEADER_ENTER

//
// Query function to determine if the current platform supports
// threads AND this library was compiled with threading enabled.
//

ILMTHREAD_EXPORT bool supportsThreads ();


#if ILMBASE_FORCE_CXX17
class jthread
{
  public:

    ILMTHREAD_EXPORT jthread ();
    ILMTHREAD_EXPORT virtual ~jthread ();
#else
class Thread
{
  public:

    ILMTHREAD_EXPORT Thread ();
    ILMTHREAD_EXPORT virtual ~Thread ();
#endif

    ILMTHREAD_EXPORT void         start ();
    ILMTHREAD_EXPORT virtual void run () = 0;

    //
    // wait for thread to exit - must be called before deleting thread
    //
    void join();

  private:

#if ILMBASE_FORCE_CXX03
#   if defined (_WIN32) || defined (_WIN64)
#   if !HAVE_PTHREAD
	HANDLE _thread;
#   else
	pthread_t _thread;
#   endif
#   endif
    void operator = (const Thread& t);	// not implemented
    Thread (const Thread& t);		// not implemented
#else
#   if ILMBASE_FORCE_CXX17
    std::jthread _thread;

    jthread &operator= (const jthread& t) = delete;
    jthread &operator= (jthread&& t) = delete;
    jthread (const jthread& t) = delete;
    jthread (jthread&& t) = delete;
#   else
    std::thread _thread;

    Thread &operator= (const Thread& t) = delete;
    Thread &operator= (Thread&& t) = delete;
    Thread (const Thread& t) = delete;
    Thread (Thread&& t) = delete;
#   endif
#endif
};


ILMTHREAD_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_ILM_THREAD_H
