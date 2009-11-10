///
/// \brief   This class represents the status plot interface.
///
/// \file    StatusPlot.h
///
/// \author  Jerome Maye
///
/// \date    28/10/2009
///
/// \details This class represents the status plot interface.
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

#ifndef STATUSPLOT_H
#define STATUSPLOT_H

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
/// \class CStatusPlot
///
/// \brief Class for interfacing the status plot.
///
class CStatusPlot : public QWidget {
  Q_OBJECT

public:
  class UpdateEvent : public QEvent {
  public:
    UpdateEvent(double gasPedalValue,
                int    gearValue,
                double steeringAngleValue,
                double translationalVelocity,
                double rotationalVelocityFrontRight,
                double rotationalVelocityFrontLeft,
                double rotationalVelocityRearRight,
                double rotationalVelocityRearLeft);

    double gasPedalValue();
    int    gearValue();
    double steeringAngleValue();
    double translationalVelocity();
    double rotationalVelocityFrontRight();
    double rotationalVelocityFrontLeft();
    double rotationalVelocityRearRight();
    double rotationalVelocityRearLeft();
  protected:
    double _gasPedalValue;
    int    _gearValue;
    double _steeringAngleValue;
    double _translationalVelocity;
    double _rotationalVelocityFrontRight;
    double _rotationalVelocityFrontLeft;
    double _rotationalVelocityRearRight;
    double _rotationalVelocityRearLeft;
  };

  CStatusPlot(QWidget *pParent);
  ~CStatusPlot();

  void updateStatus(double gasPedalValue,
                    int    gearValue,
                    double steeringAngleValue,
                    double translationalVelocity,
                    double rotationalVelocityFrontRight,
                    double rotationalVelocityFrontLeft,
                    double rotationalVelocityRearRight,
                    double rotationalVelocityRearLeft);

protected:
  QLabel *mpGasPedalLabel;
  QLabel *mpGasPedalValue;
  QLabel *mpGearLabel;
  QLabel *mpGearValue;
  QLabel *mpSteeringAngleLabel;
  QLabel *mpSteeringAngleValue;
  QLabel *mpTranslationalVelocityLabel;
  QLabel *mpTranslationalVelocityValue;
  QLabel *mpRotationalVelocityFrontRightLabel;
  QLabel *mpRotationalVelocityFrontRightValue;
  QLabel *mpRotationalVelocityFrontLeftLabel;
  QLabel *mpRotationalVelocityFrontLeftValue;
  QLabel *mpRotationalVelocityRearRightLabel;
  QLabel *mpRotationalVelocityRearRightValue;
  QLabel *mpRotationalVelocityRearLeftLabel;
  QLabel *mpRotationalVelocityRearLeftValue;
  QGridLayout *mpGridLayout;

  char macBuffer[128];

  virtual bool event(QEvent* event);
};

#endif // STATUSPLOT_H


//------------------------------------------------------------------------------
// End of StatusPlot.h
//------------------------------------------------------------------------------

