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

#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <QGLWidget>
#include <QTimer>


class QAbstractItemModel;
class CFrontEndData;
class QModelIndex;

namespace Ui {
   class CGLWidget;
}

class CGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit CGLWidget(QWidget *parent = 0);

      void Initialize( QAbstractItemModel* pModel );

protected:
    void paintEvent( QPaintEvent *event );

signals:
    void FrameSlip( QString slipTime, int displayTime );

public slots:
    void SetZoom( double zoom );
    void SetHorz( double horz );
    void SetVert( double vert );
    void SetTargetFramerate( int speed );
    void SetLockStep( bool set );
    void SetBodyZoom(double);
    void dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight );
    void SetFocus( int row );

private:
    QBrush              m_backgroundBrush;
    QBrush              m_foregroundBrush;
    QPen                m_foregroundPen;
    QAbstractItemModel* m_pModel;
    Ui::CGLWidget*      m_ui;
    QTimer              m_timer;
    double              m_zoom;
    double              m_horz;
    double              m_vert;
    double              m_bodyZoom;
    int                 m_focus;

};

#endif // GLWIDGET_HPP
