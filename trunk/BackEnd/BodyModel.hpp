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

#ifndef BODYMODEL_HPP
#define BODYMODEL_HPP

#include <QAbstractTableModel>
#include <QVector>
#include <QMutex>
#include <QTimer>

class CBody;

class CBodyModel : public QAbstractTableModel
{
   Q_OBJECT
   public:
      explicit CBodyModel( QObject * parent = 0 );
      virtual ~CBodyModel();

      inline QString GetFileName() const
      {
         return m_fileName;
      }

//************************************************************************************************************
//    QAbstractTableModel Overriden Functions
//************************************************************************************************************
      int rowCount(const QModelIndex & parent = QModelIndex()) const;
      int columnCount(const QModelIndex & parent = QModelIndex()) const;
      QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
      Qt::ItemFlags 	flags(const QModelIndex & index) const;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      bool 	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
//************************************************************************************************************
//    End: QAbstractTableModel Overriden Functions
//************************************************************************************************************

      enum
      {
         ID = 0,
         X_POSITION,
         Y_POSITION,
         Z_POSITION,
         X_VELOCITY,
         Y_VELOCITY,
         Z_VELOCITY,
         X_ACCELERATION,
         Y_ACCELERATION,
         Z_ACCELERATION,
         RADIUS,
         COLUMN_COUNT
      };

      enum
      {
         RenderRole = Qt::UserRole
      };

      bool Load( QString fileName );
      bool Save( QString fileName = QString() );
      bool Reset();

   signals:
      void Update( const QModelIndex & index );

   public slots:
      void Iterate();
      void SetIterationSpeed( int );

   private:
      bool Initialize();

      QVector<CBody*>   m_bodies;
      QMutex            m_mutex;
      QTimer            m_timer;
      QString           m_fileName;
};

#endif // BODYMODEL_HPP
