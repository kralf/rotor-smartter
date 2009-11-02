///
/// \brief   This file contains the implementation of the gyro plot.
///
/// \file    GyroPlot.cpp
///
/// \author  Jerome Maye
///
/// \date    28/10/2009
///
/// \details This file contains the implementation of the gyro plot.
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
#include "GyroPlot.h"

// system includes
#include <cmath>


//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------
///
/// \fn CGyroPlot()
///
/// \brief Default constructor.
///
CGyroPlot::CGyroPlot() {
};

///
/// \fn CGyroPlot(QWidget *pParent)
///
/// \brief Constructor called automatically by QT.
///
/// \param[in] pParent pointer to the parent's widget
///
CGyroPlot::CGyroPlot(QWidget *pParent) : QWidget(pParent) {
  // create the grid layout to put further objects in it
  mpGridLayout = new QGridLayout(this);

  // create all the labels and corresponding values that we want to display
  mpIntegratedAngleLabel = new QLabel(this);
  mpIntegratedAngleLabel->setText("Integrated angle");
  mpIntegratedAngleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpIntegratedAngleLabel, 0, 0);

  mpIntegratedAngleValue = new QLabel(this);
  mpIntegratedAngleValue->setText("n/a");
  mpIntegratedAngleValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpIntegratedAngleValue, 0, 1);

  mpGridLayout->setRowStretch(1, 1);

  this->setLayout(mpGridLayout);
};

///
/// \fn CGyroPlot(const CGyroPlot &other)
///
/// \brief Copy constructor.
///
CGyroPlot::CGyroPlot(const CGyroPlot &other) {
};

///
/// \fn virtual ~CGyroPlot()
///
/// \brief Destructor.
///
CGyroPlot::~CGyroPlot() {
};

///
/// \brief Assignement operator.
///
CGyroPlot& CGyroPlot::operator = (const CGyroPlot &other) {
  if (this != &other) { // protect against invalid self-assignment
  }

  // by convention, always return *this
  return *this;
};

///
/// \fn void updateGyro(double dIntegratedTheta)
///
/// \brief This function updates the gyro angles.
///
/// \param[in] dIntegratedTheta integrated gyro angle
///
/// \return void
///
void CGyroPlot::updateGyro(double dIntegratedTheta) {
  // display the gyro angle
  sprintf(macBuffer, "%.2lf deg", dIntegratedTheta * 180.0 / M_PI);
  mpIntegratedAngleValue->setText(macBuffer);
};


//------------------------------------------------------------------------------
// End of GyroPlot.cpp
//------------------------------------------------------------------------------

