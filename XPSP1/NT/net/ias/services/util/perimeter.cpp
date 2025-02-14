///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1998, Microsoft Corp. All rights reserved.
//
// FILE
//
//    Perimeter.cpp
//
// SYNOPSIS
//
//    This file implements the class Perimeter.
//
// MODIFICATION HISTORY
//
//    09/04/1997    Original version.
//    09/30/1998    Fix bug with recursive LockExclusive calls.
//
///////////////////////////////////////////////////////////////////////////////

#include <ias.h>
#include <climits>
#include <Perimeter.h>

//////////
//  Large negative value used to block shared entry into the perimeter.
//////////
const LONG BLOCK_VALUE = (-LONG_MAX)/2;

Perimeter::Perimeter() throw ()
   : sharing(0),
     waiting(0),
     count(&sharing)
{
   InitializeCriticalSection(&exclusive);

   sharedOK =    CreateSemaphore(NULL, 0, LONG_MAX, NULL);
   exclusiveOK = CreateSemaphore(NULL, 0, 1, NULL);
}

Perimeter::~Perimeter() throw ()
{
   CloseHandle(exclusiveOK);
   CloseHandle(sharedOK);

   DeleteCriticalSection(&exclusive);
}

void Perimeter::Lock() throw ()
{
   // If this is less than zero, then an exlusive thread must have inserted
   // the BLOCK_VALUE, so ...
   if (InterlockedIncrement(count) <= 0)
   {
      // ... we have to wait until he's done.
      WaitForSingleObject(sharedOK, INFINITE);
   }
}

void Perimeter::LockExclusive() throw ()
{
   // This limits exclusive access to a single thread.
   EnterCriticalSection(&exclusive);

   // The first time through we have to wait for the sharers to finish.
   if (exclusive.RecursionCount == 1)
   {
      // Block any new shared threads.
      waiting = BLOCK_VALUE;
      InterlockedExchangePointer((PVOID *)&count, &waiting);

      // Find out how many shared threads are already in the perimeter ...
      LONG sharingNow = InterlockedExchangeAdd(&sharing, BLOCK_VALUE);

      if (sharingNow > 0)
      {
         // ... and wait until they're done.
         WaitForSingleObject(exclusiveOK, INFINITE);
      }

      // At this point there is no one left inside the perimeter.
      sharing = 0;
   }
}

void Perimeter::Unlock() throw ()
{
   // If sharing is zero, we must be an exclusive thread.
   if (!sharing)
   {
      // Are we about to release our last lock ?
      if (exclusive.RecursionCount == 1)
      {
         // Allow any new shared access attempts.
         InterlockedExchangePointer((PVOID *)&count, &sharing);

         // Find out how many threads are waiting on the semaphore ...
         LONG waitingNow = waiting - BLOCK_VALUE;

         if (waitingNow > 0)
         {
            // ... and let them go.
            InterlockedExchangeAdd(count, waitingNow);
            ReleaseSemaphore(sharedOK, waitingNow, NULL);
         }
      }

      // Release the exclusive lock.
      LeaveCriticalSection(&exclusive);
   }
   else if (InterlockedDecrement(&sharing) == BLOCK_VALUE)
   {
      // If we end up here, we must have been the last shared thread out of
      // the perimeter while an exlusive thread is waiting, so wake him up.
      ReleaseSemaphore(exclusiveOK, 1, NULL) ;
   }
}
