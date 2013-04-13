/** @copyright
 * Copyright (c) 2013, Caleb Vincent caleb.i.m.vincent@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the PlanetSim nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#ifndef TIME_HPP
#define TIME_HPP

#include <cstdint>

typedef uint64_t Time_t;

/*!
*  Gives every subclass a standard interface for timekeeping
*     and access to a global clock
*  @todo incrementing the global clock should probably trigger
*     checks/updates of all classes inheriting from ITime
*/
class ITime
{
   public:
      ITime( Time_t initTime = 0 );
   
   protected:
      Time_t m_time; //!< Object's timekeeping value
      
      /*!
      *  Gets the global timekeeping value
      */
      Time_t GetGlobalTime() const;
      
      /*!
      *  Increment the Global Timekeeping
      *  @todo There should probably be some notification/checks here
      */
      Time_t IncrementGlobalTime();
   
   private:
      static Time_t ms_globalTime;  //!< Global timekeeping value
};

#endif
