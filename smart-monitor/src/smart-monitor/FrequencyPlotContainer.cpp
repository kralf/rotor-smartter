///
/// \brief   This file contains the implementation of the frequency plot
///          container.
///
/// \file    FrequencyPlotContainer.cpp
///
/// \author  Jerome Maye
///
/// \date    30/10/2009
///
/// \details This file contains the implementation of the frequency plot
///          container.
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
#include "FrequencyPlotContainer.h"

// system includes
#include <qwt-qt4/qwt_text.h>


//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------
///
/// \fn CFrequencyPlotContainer()
///
/// \brief Default constructor.
///
CFrequencyPlotContainer::CFrequencyPlotContainer() {
};

///
/// \fn CFrequencyPlotContainer(QWidget * parent)
///
/// \brief Constructor called automatically by QT.
///
/// \param[in] pParent pointer to the parent's widget
///
CFrequencyPlotContainer::CFrequencyPlotContainer(QWidget *pParent) : QWidget(pParent) {
  // create the layout
  mpGridLayout = new QGridLayout(this);

  // set layout
  this->setLayout(mpGridLayout);

  // create alasca frequency plot and add it to the layout
  mpAxtFreqPlot = new CFrequencyPlot(QwtText("Alasca"), this);
  mpGridLayout->addWidget(mpAxtFreqPlot, 0, 0);

  // create gyro frequency plot and add it to the layout
  mpGyroFreqPlot = new CFrequencyPlot(QwtText("Gyro"), this);
  mpGridLayout->addWidget(mpGyroFreqPlot, 1, 0);

  // create odometry frequency plot and add it to the layout
  //mpOdometryFreqPlot = new CFrequencyPlot(this);
  //mpGridLayout->addWidget(mpOdometryFreqPlot, 2, 0);

  // create localize frequency plot and add it to the layout
  //mpLocalizeFreqPlot = new CFrequencyPlot(this);
  //mpGridLayout->addWidget(mpLocalizeFreqPlot, 3, 0);
  
  // create update timer
  mpTimer = new QTimer(this);

  /// update function of the plot is called periodically
  connect(mpTimer, SIGNAL(timeout()), this, SLOT(updatePlots()));

  // start update timer
  mpTimer->start(500);
};

///
/// \fn CFrequencyPlotContainer(const CFrequencyPlotContainer &other)
///
/// \brief Copy constructor.
///
CFrequencyPlotContainer::CFrequencyPlotContainer(const CFrequencyPlotContainer &other) {
};

///
/// \fn virtual ~CFrequencyPlotContainer()
///
/// \brief Destructor.
///
CFrequencyPlotContainer::~CFrequencyPlotContainer() {
};

///
/// \brief Assignement operator.
///
CFrequencyPlotContainer& CFrequencyPlotContainer::operator = (const CFrequencyPlotContainer &other) {
  if (this != &other) { // protect against invalid self-assignment
  }

  // by convention, always return *this
  return *this;
};

///
/// \fn void updatePlots()
///
/// \brief This function updates the plots.
///
/// \return void
///
void CFrequencyPlotContainer::updatePlots() {
  // trigger the update figure of the frequency plots
  mpAxtFreqPlot->updateFigure();
  mpGyroFreqPlot->updateFigure();
};


//------------------------------------------------------------------------------
// End of FrequencyPlotContainer.cpp
//------------------------------------------------------------------------------

