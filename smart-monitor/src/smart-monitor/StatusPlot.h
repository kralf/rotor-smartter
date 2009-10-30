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
  ///
  /// \fn CStatusPlot()
  ///
  /// \brief Default constructor.
  ///
  CStatusPlot();

  ///
  /// \fn CStatusPlot(const CStatusPlot &other)
  ///
  /// \brief Copy constructor.
  ///
  CStatusPlot(const CStatusPlot &other);

  ///
  /// \brief Assignement operator.
  ///
  CStatusPlot& operator = (const CStatusPlot &other);

  /// \var QLabel *mpGasPedalLabel
  ///
  /// \brief Handle on the gas pedal value label.
  ///
  QLabel *mpGasPedalLabel;

  /// \var QLabel *mpGasPedalValue
  ///
  /// \brief Handle on the gas pedal value.
  ///
  QLabel *mpGasPedalValue;

  /// \var QLabel *mpGearLabel
  ///
  /// \brief Handle on the gear value label.
  ///
  QLabel *mpGearLabel;

  /// \var QLabel *mpGearValue
  ///
  /// \brief Handle on the gear value.
  ///
  QLabel *mpGearValue;

  /// \var QLabel *mpSteeringAngleLabel
  ///
  /// \brief Handle on the steering angle value label.
  ///
  QLabel *mpSteeringAngleLabel;

  /// \var QLabel *mpSteeringAngleValue
  ///
  /// \brief Handle on the steering angle value.
  ///
  QLabel *mpSteeringAngleValue;

  /// \var QLabel *mpTranslationalVelocityLabel
  ///
  /// \brief Handle on the translational velocity value label.
  ///
  QLabel *mpTranslationalVelocityLabel;

  /// \var QLabel *mpTranslationalVelocityValue
  ///
  /// \brief Handle on the translational velocity value.
  ///
  QLabel *mpTranslationalVelocityValue;

  /// \var QLabel *mpRotationalVelocityFrontRightLabel
  ///
  /// \brief Handle on the front right rotational velocity value label.
  ///
  QLabel *mpRotationalVelocityFrontRightLabel;

  /// \var QLabel *mpRotationalVelocityFrontRightValue
  ///
  /// \brief Handle on the front right rotational velocity value.
  ///
  QLabel *mpRotationalVelocityFrontRightValue;

  /// \var QLabel *mpRotationalVelocityFrontLeftLabel
  ///
  /// \brief Handle on the front left rotational velocity value label.
  ///
  QLabel *mpRotationalVelocityFrontLeftLabel;

  /// \var QLabel *mpRotationalVelocityFrontLeftValue
  ///
  /// \brief Handle on the front left rotational velocity value.
  ///
  QLabel *mpRotationalVelocityFrontLeftValue;

  /// \var QLabel *mpRotationalVelocityRearRightLabel
  ///
  /// \brief Handle on the rear right rotational velocity value label.
  ///
  QLabel *mpRotationalVelocityRearRightLabel;

  /// \var QLabel *mpRotationalVelocityRearRightValue
  ///
  /// \brief Handle on the rear right rotational velocity value.
  ///
  QLabel *mpRotationalVelocityRearRightValue;

  /// \var QLabel *mpRotationalVelocityRearLeftLabel
  ///
  /// \brief Handle on the rear left rotational velocity value label.
  ///
  QLabel *mpRotationalVelocityRearLeftLabel;

  /// \var QLabel *mpRotationalVelocityRearLeftValue
  ///
  /// \brief Handle on the rear left rotational velocity value.
  ///
  QLabel *mpRotationalVelocityRearLeftValue;

  /// \var QLabel *mpGyroAngleLabel
  ///
  /// \brief Handle on the gyro angle value label.
  ///
  QLabel *mpGyroAngleLabel;

  /// \var QLabel *mpGyroAngleValue
  ///
  /// \brief Handle on the gyro angle value.
  ///
  QLabel *mpGyroAngleValue;

  /// \var QGridLayout *mpGridLayout
  ///
  /// \brief Handle on the grid layout of our status plot.
  ///
  QGridLayout *mpGridLayout;
  
  /// \var char macBuffer[128]
  ///
  /// \brief Character array for displaying stuff.
  ///
  char macBuffer[128];


public:
  ///
  /// \fn CStatusPlot(QWidget * parent)
  ///
  /// \brief Constructor called automatically by QT.
  ///
  /// \param[in] pParent pointer to the parent's widget
  ///
  CStatusPlot(QWidget *pParent);

  ///
  /// \fn virtual ~CStatusPlot()
  ///
  /// \brief Destructor.
  ///
  ~CStatusPlot();

  ///
  /// \fn void updateStatus(double dGasPedalValue,
  ///                       int    iGearValue,
  ///                       double dSteeringAngleValue,
  ///                       double dTranslationalVelocity,
  ///                       double dRotationalVelocityFrontRight,
  ///                       double dRotationalVelocityFrontLeft,
  ///                       double dRotationalVelocityRearRight,
  ///                       double dRotationalVelocityRearLeft)
  ///
  /// \brief This function updates the status.
  ///
  /// \param[in] dGasPedalValue gas pedal position
  /// \param[in] iGearValue gear position
  /// \param[in] dSteeringAngleValue steering angle [rad]
  /// \param[in] dTranslationalVelocity translational velocity [m/s]
  /// \param[in] dRotationalVelocityFrontRight front right wheel rotational velocity [rps]
  /// \param[in] dRotationalVelocityFrontLeft front left wheel rotational velocity [rps]
  /// \param[in] dRotationalVelocityRearRight rear right wheel rotational velocity [rps]
  /// \param[in] dRotationalVelocityRearLeft rear left wheel rotational velocity [rps]
  /// \return void
  ///
  void updateStatus(double dGasPedalValue,
                    int    iGearValue,
                    double dSteeringAngleValue,
                    double dTranslationalVelocity,
                    double dRotationalVelocityFrontRight,
                    double dRotationalVelocityFrontLeft,
                    double dRotationalVelocityRearRight,
                    double dRotationalVelocityRearLeft);

  ///
  /// \fn void updateGyro(double dTheta)
  ///
  /// \brief This function updates the gyro angle.
  ///
  /// \param[in] dTheta integrated gyro angle
  ///
  /// \return void
  ///
  void updateGyro(double dTheta);


protected:
// accessible by inheritance

};

#endif // STATUSPLOT_H


//------------------------------------------------------------------------------
// End of StatusPlot.h
//------------------------------------------------------------------------------

