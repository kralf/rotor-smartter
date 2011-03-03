///
/// \brief   This file contains the implementation of the control plot.
///
/// \file    ControlPlot.cpp
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
#include "ControlPlot.h"

// system includes
#include <cmath>
#include <cstdio>

//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------

CControlPlot::UpdateEvent::UpdateEvent(
  double steeringAngleValue,
  double translationalVelocity) :
  QEvent(QEvent::User),
  _steeringAngleValue(steeringAngleValue),
  _translationalVelocity(translationalVelocity) {
}

double CControlPlot::UpdateEvent::steeringAngleValue() {
  return _steeringAngleValue;
}

double CControlPlot::UpdateEvent::translationalVelocity() {
  return _translationalVelocity;
}

CControlPlot::CControlPlot(QWidget *pParent) :
  QWidget(pParent) {
  // create the grid layout to put further objects in it
  mpGridLayout = new QGridLayout(this);
  mpGridLayout->setSpacing(2);

  // create all the labels and corresponding values that we want to display
  mpSteeringAngleLabel = new QLabel(this);
  mpSteeringAngleLabel->setText("Steering angle");
  mpSteeringAngleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpSteeringAngleLabel, 0, 0);

  mpSteeringAngleValue = new QLabel(this);
  mpSteeringAngleValue->setText("n/a");
  mpSteeringAngleValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpSteeringAngleValue, 0, 1);

  mpTranslationalVelocityLabel = new QLabel(this);
  mpTranslationalVelocityLabel->setText("Trans. velocity");
  mpTranslationalVelocityLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpTranslationalVelocityLabel, 1, 0);

  mpTranslationalVelocityValue = new QLabel(this);
  mpTranslationalVelocityValue->setText("n/a");
  mpTranslationalVelocityValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mpGridLayout->addWidget(mpTranslationalVelocityValue, 1, 1);

  mpGridLayout->setRowStretch(2, 1);

  this->setLayout(mpGridLayout);
};

CControlPlot::~CControlPlot() {
};

void CControlPlot::updateControl(
  double steeringAngleValue,
  double translationalVelocity) {
  QEvent* event = new UpdateEvent(
    steeringAngleValue,
    translationalVelocity);

  QCoreApplication::postEvent(this, event);
};

bool CControlPlot::event(QEvent* event) {
  if (event->type() == QEvent::User) {
    UpdateEvent* updateEvent = (UpdateEvent*)event;

    sprintf(macBuffer, "%.2lf deg",
      updateEvent->steeringAngleValue() * 180.0 / M_PI);
    mpSteeringAngleValue->setText(macBuffer);
    sprintf(macBuffer, "%.2lf m/s",
      updateEvent->translationalVelocity());
    mpTranslationalVelocityValue->setText(macBuffer);

    return true;
  }
  else
    return QWidget::event(event);
}

//------------------------------------------------------------------------------
// End of ControlPlot.cpp
//------------------------------------------------------------------------------
