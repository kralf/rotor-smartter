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
#include <cstdio>


//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------

CGyroPlot::UpdateEvent::UpdateEvent(double integratedTheta) :
  QEvent(QEvent::User),
  _integratedTheta(integratedTheta) {
}

double CGyroPlot::UpdateEvent::integratedTheta() const {
  return _integratedTheta;
}

CGyroPlot::CGyroPlot(QWidget *pParent) : 
  QWidget(pParent) {
  // create the grid layout to put further objects in it
  mpGridLayout = new QGridLayout(this);
  mpGridLayout->setSpacing(2);

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

CGyroPlot::~CGyroPlot() {
};

void CGyroPlot::updateGyro(double integratedTheta) {
  QEvent* event = new UpdateEvent(integratedTheta);
  QCoreApplication::postEvent(this, event);
};

bool CGyroPlot::event(QEvent* event) {
  if (event->type() == QEvent::User) {
    UpdateEvent* updateEvent = (UpdateEvent*)event;

    sprintf(macBuffer, "%.2lf deg", 
      updateEvent->integratedTheta() * 180.0 / M_PI);
    mpIntegratedAngleValue->setText(macBuffer);

    return true;
  }
  else
    return QWidget::event(event);
}

//------------------------------------------------------------------------------
// End of GyroPlot.cpp
//------------------------------------------------------------------------------

