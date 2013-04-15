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

#include "GLWidget.hpp"
#include "BodyModel.hpp"
#include "ui_GLWidget.h"

#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPaintEvent>
#include <QAbstractItemModel>



CGLWidget::CGLWidget(QAbstractItemModel* pModel, QWidget *parent) :
   QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_backgroundBrush( Qt::black ),
    m_foregroundBrush( Qt::green ),
    m_foregroundPen( Qt::green ),
    m_pModel( pModel ),
    m_ui(new Ui::CGLWidget),
    m_zoom(1),
    m_horz(0),
    m_vert(0),
    m_bodyZoom(1)
{
   m_ui->setupUi(this);
   setAutoFillBackground( false );
   m_foregroundPen.setWidth(1);
   m_timer.start( 1000 / m_ui->frameRateSpinBox->value() );
}

void CGLWidget::paintEvent(QPaintEvent *event)
{
   QTime paintTime;
   paintTime.start();
   QPainter painter;

   painter.begin(this);
   painter.setRenderHint(QPainter::Antialiasing);
   painter.fillRect( event->rect(), m_backgroundBrush );
   painter.translate( (event->rect().width()/2) + m_horz, (event->rect().height()/2) + m_vert );

   painter.save();
   painter.setBrush(m_foregroundBrush);
   painter.setPen(m_foregroundPen);
   for( int i(0); i < m_pModel->rowCount(); ++i )
   {
      QModelIndex xIndex( m_pModel->index( i, CBodyModel::X_POSITION ));
      QModelIndex yIndex( m_pModel->index( i, CBodyModel::Y_POSITION ));
      int radius( m_pModel->data( m_pModel->index( i, CBodyModel::RADIUS ),
                                  CBodyModel::RenderRole ).toInt() * m_zoom );
      painter.drawEllipse( m_pModel->data( xIndex, CBodyModel::RenderRole ).toInt() * m_zoom,
                           m_pModel->data( yIndex, CBodyModel::RenderRole ).toInt() * m_zoom,
                           radius * m_bodyZoom,
                           radius * m_bodyZoom );
   }
   painter.restore();
   painter.end();

   int runTime( paintTime.elapsed() - m_timer.interval() );
   if( runTime < 0 )
   {
      emit FrameSlip( runTime );
   }
}

void CGLWidget::SetZoom( double zoom )
{
   m_zoom = zoom;
}

void CGLWidget::SetHorz( double horz )
{
   m_horz = horz;
}

void CGLWidget::SetVert( double vert )
{
   m_vert = vert;
}

void CGLWidget::SetTargetFramerate( int frameRate )
{
   m_timer.setInterval( frameRate );
}

void CGLWidget::SetLockStep( bool set )
{
   m_ui->frameRateSpinBox->setEnabled( !set );
   if( set )
   {
      m_timer.stop();
      connect( m_pModel,
               SIGNAL(dataChanged(QModelIndex,QModelIndex)),
               this,
               SLOT(dataChanged(QModelIndex,QModelIndex)) );
   }
   else
   {
      m_timer.start( 1000 / m_ui->frameRateSpinBox->value() );
      disconnect(this,
                 SLOT(dataChanged(QModelIndex,QModelIndex)));
   }
}

void CGLWidget::SetBodyZoom( double zoom )
{
   m_bodyZoom = zoom;
}

void CGLWidget::dataChanged( const QModelIndex &, const QModelIndex & )
{
   repaint();
}
