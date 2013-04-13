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

#include "Body.hpp"

#include "Constants.h"

#include <cmath>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const CBody& body)
{
    os <<  body.m_uid << "," << body.m_position << "," << body.m_velocity;
    return os;
}

CBody::CBody( Mass_t mass,
              const PositionVec_t& initPos,
              const VelocityVec_t& initVel,
              const AccelerationVec_t& initAccl /*= AccelerationVec_t()*/,
              Radius_t radius /*= 0 */ ):
   ITime(),
   IUid(),
   m_mass( mass ),
   m_radius( radius ),
   m_position( initPos ),
   m_velocity( initVel ),
   m_acceleration( initAccl ),
   m_interactingBodies()
{}

void CBody::AddInteraction( const CBody* pOtherBody )
{
   m_interactingBodies.push_back( pOtherBody );
}

bool CBody::operator ==( const CBody& other )
{
    return m_uid == other.m_uid;
}

bool CBody::operator !=( const CBody& other )
{
    return m_uid != other.m_uid;
}

void CBody::CalcInteraction()
{
   m_acceleration = AccelerationVec_t();
   for( unsigned int index(0); 
        index < m_interactingBodies.size(); 
        ++index )
   {
      PositionVec_t dist( m_position.DistanceTo( m_interactingBodies[index]->m_position ) );
      PositionVec_t::VecType_t distMag( dist.Magnitude() );
      AccelerationVec_t::VecType_t top( PlanetSim::G *  m_interactingBodies[index]->m_mass );
      m_acceleration += dist.UnitVector()  * ( top / std::pow( distMag, 2 ) );
   }
}

void CBody::PerformInteraction()
{
    static const Time_t cs_timeIncrement(1);
    m_velocity += ( m_acceleration * cs_timeIncrement );
    m_position += m_velocity * cs_timeIncrement ;
    m_time += cs_timeIncrement;
}

