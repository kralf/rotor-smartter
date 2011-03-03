///
/// \brief   This class represents the gyro plot interface.
///
/// \file    GyroPlot.h
///
/// \author  Jerome Maye
///
/// \date    28/10/2009
///
/// \details This class represents the gyro plot interface.
///
/// Copyright (C) 2009 ETHZ-ASL All rights reserved.
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///

#ifndef GYROPLOT_H
#define GYROPLOT_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
// local includes


// system includes
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QEvent>
#include <QCoreApplication>


//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Main Class Declaration
//------------------------------------------------------------------------------
///
/// \class CGyroPlot
///
/// \brief Class for interfacing the gyro plot.
///
class CGyroPlot : public QWidget {
  Q_OBJECT

public:
  class UpdateEvent : public QEvent {
  public:
    UpdateEvent(double integratedTheta);

    double integratedTheta() const;
  protected:
    double _integratedTheta;
  };

  CGyroPlot(QWidget *pParent);
  ~CGyroPlot();

  void updateGyro(double integratedTheta);

protected:
  QLabel *mpIntegratedAngleLabel;
  QLabel *mpIntegratedAngleValue;
  QGridLayout *mpGridLayout;

  char macBuffer[128];

  virtual bool event(QEvent* event);
};

#endif // GYROPLOT_H


//------------------------------------------------------------------------------
// End of GyroPlot.h
//------------------------------------------------------------------------------

