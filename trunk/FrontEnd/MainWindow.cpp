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

#include "MainWindow.hpp"

#include "GLWidget.hpp"
#include "DataWidget.hpp"
#include "BodyModel.hpp"
#include "AboutDialog.hpp"

#include "ui_MainWindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>

CMainWindow::CMainWindow(CBodyModel* pModel, QWidget *parent)
    : QMainWindow(parent),
      m_ratio(1),
      m_pModel( pModel ),
      m_ui(new Ui::CMainWindow)
{
   m_ui->setupUi(this);

   m_ui->GLView->Initialize( m_pModel );
   m_ui->DataView->Init( m_pModel );



   m_ui->statusbar->addWidget( &m_frameSlip );

   QTimer *timer = new QTimer(this);
   QObject::connect( timer,
                     SIGNAL(timeout()),
                     m_ui->GLView,
                     SLOT(repaint()) );

   timer->start(50);
}

CMainWindow::~CMainWindow()
{
    
}

void CMainWindow::SetScaleLabel( float ratio )
{
    m_ratio = ratio;
}

void CMainWindow::ShowAboutWindow ()
{
   CAboutDialog aboutDialog;
   aboutDialog.exec();
}

void CMainWindow::Open( const QString& fileName )
{
   if( !m_pModel->Load( fileName ) )
   {
      QMessageBox::critical( this,
                             tr("File Error"),
                             fileName + tr("\n Could not be loaded") );
   }
}

void CMainWindow::FrameSlip( int slipTime )
{
   m_ui->statusbar->showMessage( QString().sprintf( "Frame Slip: %d", slipTime ), 3 );
}

void CMainWindow::on_actionPause_activated()
{
   static bool isPaused( false );
   if( !isPaused )
   {
      m_pModel->Pause();
      isPaused = true;
   }
   else
   {
      m_pModel->Resume();
      isPaused = false;
   }
}

void CMainWindow::on_actionSave_As_activated()
{
   QFileDialog::getSaveFileName( this,
                                 tr("Save As"),
                                 m_pModel->GetFileName(),
                                 tr("N-Body Simulation (*.nbs)" ) );
}

void CMainWindow::on_actionSave_activated()
{

}

void CMainWindow::on_actionOpen_activated()
{
   QString fileName = QFileDialog::getOpenFileName( this,
                        tr("Open File"),
                        tr(""),
                        tr("N-Body Simulation (*.nbs)" ) );
   if( fileName.size() > 0 )
   {
      emit Open( fileName );
   }
}

void CMainWindow::on_actionCascade_triggered()
{
   m_ui->mdiArea->cascadeSubWindows();
}

void CMainWindow::on_actionTile_triggered()
{
    m_ui->mdiArea->tileSubWindows();
}
