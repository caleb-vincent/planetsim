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

#ifndef BODY_HPP
#define BODY_HPP

#include "Vector.hpp"
#include "Time.hpp"
#include "Uid.hpp"

#include <vector>
#include <ostream>

typedef CVector<double> PositionVec_t;
typedef CVector<double> VelocityVec_t;
typedef CVector<double> AccelerationVec_t;

class CBody;

extern std::ostream& operator<<(std::ostream& os, const CBody& body);

class CBody : public ITime, public IUid
{
   friend int main( int argc, char* argv[] );
   friend std::ostream&operator<<(std::ostream& os, const CBody& body);
   public:
      typedef double Mass_t;
      typedef double Radius_t;
      
      CBody( Mass_t mass,
             const PositionVec_t& initPos,
             const VelocityVec_t& initVel,
             const AccelerationVec_t& initAccl = AccelerationVec_t(),
             Radius_t radius = 0 );
      
      void AddInteraction( const CBody* pOtherBody );
      void CalcInteraction();
      void PerformInteraction();

      bool operator ==( const CBody& other );
      bool operator !=( const CBody& other );

      inline Mass_t GetMass() const
      {
         return m_mass;
      }

      inline Mass_t GetRadius() const
      {
         return m_radius;
      }

      inline PositionVec_t GetPosition() const
      {
         return m_position;
      }

      inline VelocityVec_t GetVelocity() const
      {
         return m_velocity;
      }

      inline AccelerationVec_t GetAcceleration() const
      {
         return m_acceleration;
      }
      
   protected:
      typedef std::vector<const CBody*> Interactions_t;
      
      Mass_t            m_mass;
      Radius_t          m_radius;
      PositionVec_t     m_position;
      VelocityVec_t     m_velocity;
      AccelerationVec_t m_acceleration;
      Interactions_t    m_interactingBodies;

   private:
      CBody( const CBody& );
      CBody& operator=(const CBody& );
};

#endif
