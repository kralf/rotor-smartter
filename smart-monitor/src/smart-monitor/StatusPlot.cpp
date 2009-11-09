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
#include <cstdio>

//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------

CStatusPlot::CStatusPlot() {
};

CStatusPlot::CStatusPlot(QWidget *pParent) : QWidget(pParent) {
  // create the grid layout to put further objects in it
  mpGridLayout = new QGridLayout(this);
  mpGridLayout->setSpacing(2);

  // create all the labels and corresponding values that we want to display
  mpGasPedalLabel = new QLabel(this);
  mpGasPedalLabel->setText("Gas pedal position");
  mpGasPedalLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGasPedalLabel, 0, 0);

  mpGasPedalValue = new QLabel(this);
  mpGasPedalValue->setText("n/a");
  mpGasPedalValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGasPedalValue, 0, 1);

  mpGearLabel = new QLabel(this);
  mpGearLabel->setText("Gear");
  mpGearLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGearLabel, 1, 0);

  mpGearValue = new QLabel(this);
  mpGearValue->setText("n/a");
  mpGearValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpGearValue, 1, 1);

  mpSteeringAngleLabel = new QLabel(this);
  mpSteeringAngleLabel->setText("Steering angle");
  mpSteeringAngleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpSteeringAngleLabel, 2, 0);

  mpSteeringAngleValue = new QLabel(this);
  mpSteeringAngleValue->setText("n/a");
  mpSteeringAngleValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpSteeringAngleValue, 2, 1);

  mpTranslationalVelocityLabel = new QLabel(this);
  mpTranslationalVelocityLabel->setText("Trans. velocity");
  mpTranslationalVelocityLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpTranslationalVelocityLabel, 3, 0);

  mpTranslationalVelocityValue = new QLabel(this);
  mpTranslationalVelocityValue->setText("n/a");
  mpTranslationalVelocityValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpTranslationalVelocityValue, 3, 1);

  mpRotationalVelocityFrontRightLabel = new QLabel(this);
  mpRotationalVelocityFrontRightLabel->setText("Front right rot. velocity");
  mpRotationalVelocityFrontRightLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontRightLabel, 4, 0);

  mpRotationalVelocityFrontRightValue = new QLabel(this);
  mpRotationalVelocityFrontRightValue->setText("n/a");
  mpRotationalVelocityFrontRightValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontRightValue, 4, 1);

  mpRotationalVelocityFrontLeftLabel = new QLabel(this);
  mpRotationalVelocityFrontLeftLabel->setText("Front left rot. velocity");
  mpRotationalVelocityFrontLeftLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontLeftLabel, 5, 0);

  mpRotationalVelocityFrontLeftValue = new QLabel(this);
  mpRotationalVelocityFrontLeftValue->setText("n/a");
  mpRotationalVelocityFrontLeftValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityFrontLeftValue, 5, 1);

  mpRotationalVelocityRearRightLabel = new QLabel(this);
  mpRotationalVelocityRearRightLabel->setText("Rear right rot. velocity");
  mpRotationalVelocityRearRightLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearRightLabel, 6, 0);

  mpRotationalVelocityRearRightValue = new QLabel(this);
  mpRotationalVelocityRearRightValue->setText("n/a");
  mpRotationalVelocityRearRightValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearRightValue, 6, 1);

  mpRotationalVelocityRearLeftLabel = new QLabel(this);
  mpRotationalVelocityRearLeftLabel->setText("Rear left rot. velocity");
  mpRotationalVelocityRearLeftLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearLeftLabel, 7, 0);

  mpRotationalVelocityRearLeftValue = new QLabel(this);
  mpRotationalVelocityRearLeftValue->setText("n/a");
  mpRotationalVelocityRearLeftValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpRotationalVelocityRearLeftValue, 7, 1);

  mpGridLayout->setRowStretch(8, 1);

  this->setLayout(mpGridLayout);
};

CStatusPlot::~CStatusPlot() {
};

CStatusPlot& CStatusPlot::operator = (const CStatusPlot &other) {
  if (this != &other) { // protect against invalid self-assignment
  }

  // by convention, always return *this
  return *this;
};

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

  if (iGearValue == 0)
    mpGearValue->setText("N");
  else if (iGearValue < 6)
    mpGearValue->setNum(iGearValue);
  else if (iGearValue == 6)
    mpGearValue->setText("R");


  sprintf(macBuffer, "%.2lf deg", dSteeringAngleValue * 180.0 / M_PI);
  mpSteeringAngleValue->setText(macBuffer);

  sprintf(macBuffer, "%.2lf m/s", dTranslationalVelocity);
  mpTranslationalVelocityValue->setText(macBuffer);

  sprintf(macBuffer, "%.2lf rps", dRotationalVelocityFrontRight);
  mpRotationalVelocityFrontRightValue->setText(macBuffer);

  sprintf(macBuffer, "%.2lf rps", dRotationalVelocityFrontLeft);
  mpRotationalVelocityFrontLeftValue->setText(macBuffer);

  sprintf(macBuffer, "%.2lf rps", dRotationalVelocityRearRight);
  mpRotationalVelocityRearRightValue->setText(macBuffer);

  sprintf(macBuffer, "%.2lf rps", dRotationalVelocityRearLeft);
  mpRotationalVelocityRearLeftValue->setText(macBuffer);
};

//------------------------------------------------------------------------------
// End of StatusPlot.cpp
//------------------------------------------------------------------------------
