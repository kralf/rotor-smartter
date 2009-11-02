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
CFrequencyPlotContainer::CFrequencyPlotContainer(QWidget *pParent) :
  QWidget(pParent),
  mpRegistry(0),
  mpVBoxLayout(new QVBoxLayout(this)),
  mpTimer(new QTimer(this)) {
  setLayout(mpVBoxLayout);
  mpVBoxLayout->addStretch(1);

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
/// \fn setRegistry(Rotor::Registry& rRegistry)
///
/// \brief Set the frequency plot's Rotor registry.
///
void CFrequencyPlotContainer::setRegistry(Rotor::Registry& rRegistry) {
  mpRegistry = &rRegistry;
};

///
/// \fn addPlot(const QString& messageName, double dMaxFrequency)
///
/// \brief Add a frequency plot for the message with the given name.
///
void CFrequencyPlotContainer::addPlot(const QString& messageName,
  double dMaxFrequency) {
  std::map<QString, CFrequencyPlot*>::const_iterator it =
    mpPlots.find(messageName);

  if (it == mpPlots.end()) {
    CFrequencyPlot* mpPlot = new CFrequencyPlot(messageName,
      dMaxFrequency, this);
    mpVBoxLayout->insertWidget(mpVBoxLayout->count()-1, mpPlot);

    mpPlots[messageName] = mpPlot;
  }
};

///
/// \fn void updatePlots()
///
/// \brief This function updates the plots.
///
/// \return void
///
void CFrequencyPlotContainer::updatePlots() {
  if (mpRegistry) {
    for (std::map<QString, CFrequencyPlot*>::const_iterator it = mpPlots.begin();
      it != mpPlots.end(); ++it)
      mpRegistry->messageFrequency(it->first.toLatin1().constData());
  }
};


//------------------------------------------------------------------------------
// End of FrequencyPlotContainer.cpp
//------------------------------------------------------------------------------

