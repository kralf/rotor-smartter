///
/// \brief   This class represents the gyro plot interface.
///
/// \file    ControlPlot.h
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

#ifndef CONTROLPLOT_H
#define CONTROLPLOT_H

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
/// \class CControlPlot
///
/// \brief Class for interfacing the control plot.
///
class CControlPlot : public QWidget {
  Q_OBJECT

public:
  class UpdateEvent : public QEvent {
  public:
    UpdateEvent(double steeringAngleValue,
                double translationalVelocity);

    double steeringAngleValue();
    double translationalVelocity();
  protected:
    double _steeringAngleValue;
    double _translationalVelocity;
  };

  CControlPlot(QWidget *pParent);
  ~CControlPlot();

  void updateControl(double steeringAngleValue,
                     double translationalVelocity);

protected:
  QLabel *mpSteeringAngleLabel;
  QLabel *mpSteeringAngleValue;
  QLabel *mpTranslationalVelocityLabel;
  QLabel *mpTranslationalVelocityValue;
  QGridLayout *mpGridLayout;

  char macBuffer[128];

  virtual bool event(QEvent* event);
};

#endif // CONTROLPLOT_H


//------------------------------------------------------------------------------
// End of ControlPlot.h
//------------------------------------------------------------------------------

