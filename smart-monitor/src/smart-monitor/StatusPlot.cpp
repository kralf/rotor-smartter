///
/// \brief   This file contains the implementation of the status plot.
///
/// \file    StatusPlot.cpp
///
/// \author  Jerome Maye
///
/// \date    28/10/2009
///
/// \details This file contains the implementation of the status plot.
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

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
// local includes
#include "StatusPlot.h"

// system includes
#include <cmath>


//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------
///
/// \fn CStatusPlot()
///
/// \brief Default constructor.
///
CStatusPlot::CStatusPlot() {
};

///
/// \fn CStatusPlot(QWidget *pParent)
///
/// \brief Constructor called automatically by QT.
///
/// \param[in] pParent pointer to the parent's widget
///
CStatusPlot::CStatusPlot(QWidget *pParent) : QWidget(pParent) {
  // create the grid layout to put further objects in it
  mpGridLayout = new QGridLayout(this);

  // create all the labels and corresponding values that we want to display

  mpGasPedalLabel = new QLabel(this);
  mpGasPedalLabel->setText("Gas Pedal Position");
  mpGasPedalLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGasPedalLabel, 0, 0);

  mpGasPedalValue = new QLabel(this);
  mpGasPedalValue->setNum(0.0);
  mpGasPedalValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGasPedalValue, 0, 1);

  mpGearLabel = new QLabel(this);
  mpGearLabel->setText("Gear");
  mpGearLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGearLabel, 1, 0);

  mpGearValue = new QLabel(this);
  mpGearValue->setNum(0);
  mpGearValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGearValue, 1, 1);

  mpSteeringAngleLabel = new QLabel(this);
  mpSteeringAngleLabel->setText("Steering Angle");
  mpSteeringAngleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpSteeringAngleLabel, 2, 0);

  mpSteeringAngleValue = new QLabel(this);
  mpSteeringAngleValue->setNum(0.0);
  mpSteeringAngleValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpSteeringAngleValue, 2, 1);

  mpTranslationalVelocityLabel = new QLabel(this);
  mpTranslationalVelocityLabel->setText("Trans. Velocity");
  mpTranslationalVelocityLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpTranslationalVelocityLabel, 3, 0);

  mpTranslationalVelocityValue = new QLabel(this);
  mpTranslationalVelocityValue->setNum(0.0);
  mpTranslationalVelocityValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpTranslationalVelocityValue, 3, 1);

  mpRotationalVelocityFrontRightLabel = new QLabel(this);
  mpRotationalVelocityFrontRightLabel->setText("Front Right Rot. Velocity");
  mpRotationalVelocityFrontRightLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontRightLabel, 4, 0);

  mpRotationalVelocityFrontRightValue = new QLabel(this);
  mpRotationalVelocityFrontRightValue->setNum(0.0);
  mpRotationalVelocityFrontRightValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontRightValue, 4, 1);

  mpRotationalVelocityFrontLeftLabel = new QLabel(this);
  mpRotationalVelocityFrontLeftLabel->setText("Front Left Rot. Velocity");
  mpRotationalVelocityFrontLeftLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontLeftLabel, 5, 0);

  mpRotationalVelocityFrontLeftValue = new QLabel(this);
  mpRotationalVelocityFrontLeftValue->setNum(0.0);
  mpRotationalVelocityFrontLeftValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontLeftValue, 5, 1);

  mpRotationalVelocityRearRightLabel = new QLabel(this);
  mpRotationalVelocityRearRightLabel->setText("Rear Right Rot. Velocity");
  mpRotationalVelocityRearRightLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearRightLabel, 6, 0);

  mpRotationalVelocityRearRightValue = new QLabel(this);
  mpRotationalVelocityRearRightValue->setNum(0.0);
  mpRotationalVelocityRearRightValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearRightValue, 6, 1);

  mpRotationalVelocityRearLeftLabel = new QLabel(this);
  mpRotationalVelocityRearLeftLabel->setText("Rear Left Rot. Velocity");
  mpRotationalVelocityRearLeftLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearLeftLabel, 7, 0);

  mpRotationalVelocityRearLeftValue = new QLabel(this);
  mpRotationalVelocityRearLeftValue->setNum(0.0);
  mpRotationalVelocityRearLeftValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearLeftValue, 7, 1);
 
  mpGyroAngleLabel = new QLabel(this);
  mpGyroAngleLabel->setText("Gyro angle");
  mpGyroAngleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGyroAngleLabel, 8, 0);

  mpGyroAngleValue = new QLabel(this);
  mpGyroAngleValue->setNum(0.0);
  mpGyroAngleValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGyroAngleValue, 8, 1); 

  this->setLayout(mpGridLayout);
};

///
/// \fn CStatusPlot(const CStatusPlot &other)
///
/// \brief Copy constructor.
///
CStatusPlot::CStatusPlot(const CStatusPlot &other) {
};

///
/// \fn virtual ~CStatusPlot()
///
/// \brief Destructor.
///
CStatusPlot::~CStatusPlot() {
};

///
/// \brief Assignement operator.
///
CStatusPlot& CStatusPlot::operator = (const CStatusPlot &other) {
  if (this != &other) { // protect against invalid self-assignment
  }

  // by convention, always return *this
  return *this;
};

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
void CStatusPlot::updateStatus(double dGasPedalValue,
                               int    iGearValue,
                               double dSteeringAngleValue,
                               double dTranslationalVelocity,
                               double dRotationalVelocityFrontRight,
                               double dRotationalVelocityFrontLeft,
                               double dRotationalVelocityRearRight,
                               double dRotationalVelocityRearLeft) {
  
  // display all the incoming information
  sprintf(macBuffer, "%.2lf", dGasPedalValue);
  mpGasPedalValue->setText(macBuffer);
  
  mpGearValue->setNum(iGearValue);
  
  sprintf(macBuffer, "%.2lf", dSteeringAngleValue * 180.0 / M_PI);
  mpSteeringAngleValue->setText(macBuffer);
  
  sprintf(macBuffer, "%.2lf", dTranslationalVelocity);
  mpTranslationalVelocityValue->setText(macBuffer);
  
  sprintf(macBuffer, "%.2lf", dRotationalVelocityFrontRight);
  mpRotationalVelocityFrontRightValue->setText(macBuffer);
  
  sprintf(macBuffer, "%.2lf", dRotationalVelocityFrontRight);
  mpRotationalVelocityFrontLeftValue->setText(macBuffer);
  
  sprintf(macBuffer, "%.2lf", dRotationalVelocityRearRight);
  mpRotationalVelocityRearRightValue->setText(macBuffer);
  
  sprintf(macBuffer, "%.2lf", dRotationalVelocityRearLeft);
  mpRotationalVelocityRearLeftValue->setText(macBuffer);
};

///
/// \fn void updateGyro(double dTheta)
///
/// \brief This function updates the gyro angle.
///
/// \param[in] dTheta integrated gyro angle
///
/// \return void
///
void CStatusPlot::updateGyro(double dTheta)
{
  // display the gyro angle
  sprintf(macBuffer, "%.2lf", dTheta * 180.0 / M_PI);
  mpGyroAngleValue->setText(macBuffer);
};


//------------------------------------------------------------------------------
// End of StatusPlot.cpp
//------------------------------------------------------------------------------
