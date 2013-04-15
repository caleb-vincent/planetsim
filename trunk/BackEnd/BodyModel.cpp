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

#include "BodyModel.hpp"

#include "Body.hpp"

#include <QFile>
#include <QTextStream>

#include <iostream>
#include <exception>

CBodyModel::CBodyModel( QObject * parent  ):
   QAbstractTableModel( parent ),
   m_bodies(0),
   m_mutex(),
   m_timer( this ),
   m_fileName(tr(""))
{
   QObject::connect( &m_timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(Iterate() ) );
}

CBodyModel::~CBodyModel()
{
   for( int index(0); index < m_bodies.size(); ++index )
   {
      delete m_bodies[index];
   }
}

bool CBodyModel::Load( QString fileName )
{
   QFile file( fileName );
   QVector<CBody*> bodies;
   if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
   {
      return false;
   }
   QTextStream in( &file );
   while( !in.atEnd() )
   {
      QString lineString( in.readLine() );
      QTextStream line( &lineString );
      CBody::Mass_t mass(0);
      PositionVec_t::VecType_t xPos(0), yPos(0), zPos(0);
      VelocityVec_t::VecType_t xVel(0), yVel(0), zVel(0);
      AccelerationVec_t::VecType_t xAccl(0), yAccl(0), zAccl(0);
      CBody::Radius_t radius(0);

      line >> mass >> xPos >> yPos >> zPos >> xVel >> yVel >> zVel >> xAccl >> yAccl >> zAccl >> radius;

      if( line.status() != QTextStream::Ok )
      {
         in.setStatus( QTextStream::ReadCorruptData );
         break;
      }
      CBody* pBody( new CBody( mass,
                               PositionVec_t( xPos, yPos, zPos ),
                               VelocityVec_t( xVel, yVel, zVel ),
                               AccelerationVec_t( xAccl, yAccl, zAccl ),
                               radius ) );
      bodies.append( pBody );
   }

   if( in.status() != QTextStream::Ok )
   {
      return false;
   }

   for( int first(0); first < bodies.size(); ++first )
   {
      for( int second(0); second < bodies.size(); ++second )
      {
          if( bodies[first] != bodies[second] )
          {
            bodies[first]->AddInteraction( bodies[second] );
          }
      }
   }
   Reset();
   emit layoutAboutToBeChanged();
   m_mutex.lock();
   m_bodies = bodies;
   m_mutex.unlock();
   emit layoutChanged();
   emit dataChanged( index(0,0), index( m_bodies.size(), COLUMN_COUNT ) );
   m_timer.start( 50 );
   m_fileName = fileName;
   return true;
}


bool CBodyModel::Save( QString fileName  /*=QString()*/ )
{
   if( fileName.size() != 0 )
   {
      fileName = m_fileName;
   }
   QFile file(fileName);
   if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
   {
      return false;
   }
   QTextStream out(&file);
   for( int index(0); index < m_bodies.size(); ++index )
   {
      out << m_bodies[index]->GetMass() << ' ';
      out << m_bodies[index]->GetPosition().m_x << ' ';
      out << m_bodies[index]->GetPosition().m_y << ' ';
      out << m_bodies[index]->GetPosition().m_z << ' ';
      out << m_bodies[index]->GetVelocity().m_x << ' ';
      out << m_bodies[index]->GetVelocity().m_y << ' ';
      out << m_bodies[index]->GetVelocity().m_z << ' ';
      out << m_bodies[index]->GetAcceleration().m_x << ' ';
      out << m_bodies[index]->GetAcceleration().m_y << ' ';
      out << m_bodies[index]->GetAcceleration().m_z << ' ';
      out << m_bodies[index]->GetRadius() << '\n';
   }
   m_fileName = fileName;
   return true;
}

bool CBodyModel::Reset()
{
   m_timer.stop();
   m_fileName = tr("");
   m_mutex.lock();
   for( int index(0); index < m_bodies.size(); ++index )
   {
      delete m_bodies[index];
   }
   m_mutex.unlock();
   m_timer.start();
   return true;
}

int CBodyModel::rowCount(const QModelIndex & ) const
{
   return m_bodies.size();
}

int CBodyModel::columnCount(const QModelIndex & ) const
{
   return COLUMN_COUNT;
}

QVariant CBodyModel::data(const QModelIndex & index, int role) const
{
   QVariant result;
   if( role == Qt::EditRole ||
       role == RenderRole )
   {
      if( index.column() == ID )
      {
         result = index.row();
      }
      if( index.column() == MASS )
      {
         result = m_bodies[index.row()]->GetMass();
      }
      else if( index.column() == X_POSITION )
      {
         result = m_bodies[index.row()]->GetPosition().m_x;
      }
      else if( index.column() == Y_POSITION )
      {
         result = m_bodies[index.row()]->GetPosition().m_y;
      }
      else if( index.column() == Z_POSITION )
      {
         result = m_bodies[index.row()]->GetPosition().m_z;
      }
      else if( index.column() == X_VELOCITY )
      {
         result = m_bodies[index.row()]->GetVelocity().m_x;
      }
      else if( index.column() == Y_VELOCITY )
      {
         result = m_bodies[index.row()]->GetVelocity().m_y;
      }
      else if( index.column() == Z_VELOCITY )
      {
         result = m_bodies[index.row()]->GetVelocity().m_z;
      }
      else if( index.column() == X_ACCELERATION )
      {
         result = m_bodies[index.row()]->GetAcceleration().m_x;
      }
      else if( index.column() == Y_ACCELERATION )
      {
         result = m_bodies[index.row()]->GetAcceleration().m_y;
      }
      else if( index.column() == Z_ACCELERATION )
      {
         result = m_bodies[index.row()]->GetAcceleration().m_z;
      }
      else if( index.column() == RADIUS )
      {
         result = m_bodies[index.row()]->GetRadius();
      }
   }
   else if( role == Qt::DisplayRole )
   {
      QString formatted;
      if( index.column() == ID )
      {
         result = formatted.sprintf( "%u", index.row() );
      }
      else if( index.column() == MASS )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetMass() );
      }
      else if( index.column() == X_POSITION )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetPosition().m_x );
      }
      else if( index.column() == Y_POSITION )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetPosition().m_y );
      }
      else if( index.column() == Z_POSITION )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetPosition().m_z );
      }
      else if( index.column() == X_VELOCITY )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetVelocity().m_x );
      }
      else if( index.column() == Y_VELOCITY )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetVelocity().m_y );
      }
      else if( index.column() == Z_VELOCITY )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetVelocity().m_z );
      }
      else if( index.column() == X_ACCELERATION )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetAcceleration().m_x );
      }
      else if( index.column() == Y_ACCELERATION )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetAcceleration().m_y );
      }
      else if( index.column() == Z_ACCELERATION )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetAcceleration().m_z );
      }
      else if( index.column() == RADIUS )
      {
         result = formatted.sprintf( "%f", m_bodies[index.row()]->GetRadius() );
      }
   }
   return result;
}

Qt::ItemFlags CBodyModel::flags(const QModelIndex & index) const
{
   if( index.isValid() && index != QModelIndex() )
   {
      if( index.column() != ID && !m_timer.isActive() )
      {
         return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
      }
      else
      {
         return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
      }
   }
   return Qt::NoItemFlags;
}

QVariant CBodyModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
   if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
   {
      return QVariant();
   }
   else
   {
      switch( section )
      {
         case ID:
            return tr("ID");
         case MASS:
            return tr("Mass");
         case X_VELOCITY:
            return tr("X Velocity");
         case Y_VELOCITY:
            return tr("Y Velocity");
         case Z_VELOCITY:
            return tr("Z Velocity");
         case X_POSITION:
            return tr("X Position");
         case Y_POSITION:
            return tr("Y Position");
         case Z_POSITION:
            return tr("Z Position");
         case X_ACCELERATION:
            return tr("X Acceleration");
         case Y_ACCELERATION:
            return tr("Y Acceleration");
         case Z_ACCELERATION:
            return tr("Z Acceleration");
         case RADIUS:
            return tr("Radius");
         default:
            return tr("INVALID");
      }
   }
}

bool CBodyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   bool result( false );
   if( role == Qt::EditRole )
   {
      bool isOk( false );
      m_mutex.lock();
      switch( index.column() )
      {
         case MASS:
         {
            CBody::Mass_t mass(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetMass( mass );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case RADIUS:
         {
            CBody::Mass_t radius(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetRadius( radius );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case X_POSITION:
         {
            PositionVec_t::VecType_t xPos(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( PositionVec_t( xPos, 0, 0 ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case Y_POSITION:
         {
            PositionVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( PositionVec_t( 0, val, 0 ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case Z_POSITION:
         {
            PositionVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( PositionVec_t( 0, 0, val ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case X_VELOCITY:
         {
            VelocityVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( VelocityVec_t( val, 0, 0 ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case Y_VELOCITY:
         {
            VelocityVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( VelocityVec_t( 0, val, 0 ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case Z_VELOCITY:
         {
            VelocityVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( VelocityVec_t( 0, 0, val ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case X_ACCELERATION:
         {
            AccelerationVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( AccelerationVec_t( val, 0, 0 ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case Y_ACCELERATION:
         {
            AccelerationVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( AccelerationVec_t( 0, val, 0 ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         case Z_ACCELERATION:
         {
            AccelerationVec_t::VecType_t val(value.toDouble( &isOk ) );
            if( isOk )
            {
               m_bodies[index.row()]->SetPosition( AccelerationVec_t( 0, 0, val ) );
               result = true;
            }
            else
            {
               result = false;
            }
            break;
         }
         default:
         {
            break;
         }
      }
      m_mutex.unlock();
   }
   return result;
}

void CBodyModel::Iterate()
{

   for( int index(0); index < m_bodies.size(); ++index )
   {
      m_bodies[index]->CalcInteraction();
   }
   m_mutex.lock();
   for( int index(0); index < m_bodies.size(); ++index )
   {
      m_bodies[index]->PerformInteraction();
   }
   m_mutex.unlock();
   emit dataChanged( index(0,0), index( m_bodies.size(), COLUMN_COUNT ) );
}

void CBodyModel::SetIterationSpeed( int speed )
{
   m_timer.setInterval( speed );
}

void CBodyModel::Pause()
{
   m_timer.stop();
}

void CBodyModel::Resume()
{
   m_timer.start();
}
