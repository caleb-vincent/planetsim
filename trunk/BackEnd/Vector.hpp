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

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <ostream>
#include <cmath>
#include <iostream>

template<typename type_t>
class CVector;

template<typename type_t>
std::ostream& operator<<(std::ostream& os, const CVector<type_t>& vect);

template<typename type_t>
class CVector
{
public:
    explicit CVector( type_t xPos = 0, type_t yPos = 0, type_t zPos = 0 ):
        m_x( xPos ),
        m_y( yPos ),
        m_z( zPos )
    {}

    CVector( const CVector& copyFrom ):
        m_x( copyFrom.m_x ),
        m_y( copyFrom.m_y ),
        m_z( copyFrom.m_z )
    {}

    type_t ScalarDistanceTo( const CVector& other ) const
    {
        return std::sqrt( std::pow( m_x - other.m_x, 2 ) +
                          std::pow( m_y - other.m_y, 2 ) +
                          std::pow( m_z - other.m_z, 2 ));
    }

    const CVector DistanceTo( const CVector& other ) const 
    {
        return CVector( other.m_x - m_x,
                        other.m_y - m_y,
                        other.m_z - m_z );
    }

    const type_t Magnitude() const
    {
        return std::sqrt( std::pow( m_x, 2 ) +
                          std::pow( m_y, 2 ) +
                          std::pow( m_z, 2 ));
    }

    const CVector UnitVector() const
    {
        type_t mag( Magnitude() );
        return CVector( m_x / mag,
                        m_y / mag,
                        m_z / mag );
    }

    const CVector Power( const int power ) const
    {
        CVector result( 1.0, 1.0, 1.0 );
        for( int i(0); i < std::abs(power); ++i )
        {
            result *= *this;
        }
        if( power <0 )
        {
            result = 1.0 / result;
        }
        return result;
    }

    CVector& operator =(const CVector& other )
    {
        if( *this != other )
        {
            m_x = other.m_x;
            m_y = other.m_y;
            m_z = other.m_z;
        }
        return *this;
    }

    bool operator==( const CVector& other)
    {
        return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
    }

    bool operator!=( const CVector& other)
    {
        return m_x != other.m_x || m_y != other.m_y || m_z != other.m_z;
    }

    CVector& operator *= (const CVector& other )
    {
      m_x = m_y * other.m_z - m_z * other.m_y;
      m_y = m_z * other.m_x - m_x * other.m_z;
      m_z = m_y * other.m_y - m_y * other.m_x;
      return *this;
    }

    CVector& operator /= (const CVector& other )
    {
      //! @todo Compute inverse Cross-Product
      std::cerr << "ERROR: Inverse Cross-Product not implemented" << std::endl;
      return *this;
    }

    CVector& operator += (const CVector& other )
    {
        m_x += other.m_x;
        m_y += other.m_y;
        m_z += other.m_z;
        return *this;
    }

    CVector& operator -= (const CVector& other )
    {
        m_x -= other.m_x;
        m_y -= other.m_y;
        m_z -= other.m_z;
        return *this;
    }

    CVector& operator *= (const type_t& value )
    {
        m_x *= value;
        m_y *= value;
        m_z *= value;
        return *this;
    }

    CVector& operator /= (const type_t& value )
    {
        m_x /= value;
        m_y /= value;
        m_z /= value;
        return *this;
    }

    CVector& operator += (const type_t& value )
    {
        m_x += value;
        m_y += value;
        m_z += value;
        return *this;
    }

    CVector& operator -= (const type_t& value )
    {
        m_x -= value;
        m_y -= value;
        m_z -= value;
        return *this;
    }

   const CVector operator *( const CVector& other ) const
   {
      CVector result( *this );
      result.m_x = m_y * other.m_z - m_z * other.m_y;
      result.m_y = m_z * other.m_x - m_x * other.m_z;
      result.m_z = m_y * other.m_y - m_y * other.m_x;
      return result;
   }

    const CVector operator /( const CVector& other ) const
    {
      //! @todo Compute inverse Cross-Product
      std::cerr << "ERROR: Inverse Cross-Product not implemented" << std::endl;
        return CVector(*this) /= other;
    }

    const CVector operator +( const CVector& other ) const
    {
        return CVector(*this) += other;
    }

    const CVector operator -( const CVector& other ) const
    {
        return CVector(*this) -= other;
    }

    const CVector operator *( const type_t& value ) const
    {
        return CVector(*this) *= value;
    }

    const CVector operator /( const type_t& value ) const
    {
        return CVector(*this) /= value;
    }

    const CVector operator +( const type_t& value ) const
    {
        return CVector(*this) += value;
    }

    const CVector operator -( const type_t& value ) const
    {
        return CVector(*this) -= value;
    }

/// Vector components
    type_t m_x;
    type_t m_y;
    type_t m_z;

    typedef type_t VecType_t;

};

template<typename type_t>
std::ostream& operator<<(std::ostream& os, const CVector<type_t>& vect)
{
    os << vect.m_x << "," << vect.m_y << "," << vect.m_z;
    return os;
}

template<typename type_t>
CVector<type_t> operator/( const type_t& lhs, const CVector<type_t>& rhs)
{
   return CVector<type_t>( lhs / rhs.m_x, lhs / rhs.m_y, lhs / rhs.m_z );
}

#endif // VECTOR_HPP
