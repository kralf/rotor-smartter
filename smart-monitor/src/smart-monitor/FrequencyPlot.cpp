///
/// \brief   This file contains the implementation of the frequency plot.
///
/// \file    FrequencyPlot.cpp
///
/// \author  Jerome Maye
///
/// \date    30/10/2009
///
/// \details This file contains the implementation of the frequency plot.
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
#include "FrequencyPlot.h"

// system includes
#include <qwt-qt4/qwt_plot_layout.h>


//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
const size_t LIST_DEFAULT_CAPACITY = 20;


//------------------------------------------------------------------------------
// Functions Implementations
//------------------------------------------------------------------------------
///
/// \fn CFrequencyPlot()
///
/// \brief Default constructor.
///
CFrequencyPlot::CFrequencyPlot() {
};

///
/// \fn CFrequencyPlot(const QwtText &rTitle, QWidget *pParent)
///
/// \brief Constructor called automatically by QT.
///
/// \param[in] rTitle title of the plot
/// \param[in] pParent pointer to the parent's widget
///
CFrequencyPlot::CFrequencyPlot(const QwtText &rTitle, QWidget *pParent) :
  QWidget(pParent),
  muFrequencyListCapacity(LIST_DEFAULT_CAPACITY) {
  // create the layout
  mpGridLayout = new QGridLayout(this);

  // create the QT plot
  mpPlot = new QwtPlot(rTitle, this);

  // set canvas
  mpPlot->setCanvasBackground(Qt::white);
  mpPlot->plotLayout()->setCanvasMargin(0);
  mpPlot->plotLayout()->setAlignCanvasToScales(true);

  // add plot to layout
  mpGridLayout->addWidget(mpPlot, 0, 0);
  mpPlot->replot();

  // set layout
  this->setLayout(mpGridLayout);
};

///
/// \fn CFrequencyPlot(const CFrequencyPlot &other)
///
/// \brief Copy constructor.
///
CFrequencyPlot::CFrequencyPlot(const CFrequencyPlot &other) {
};

///
/// \fn virtual ~CFrequencyPlot()
///
/// \brief Destructor.
///
CFrequencyPlot::~CFrequencyPlot() {
};

///
/// \brief Assignement operator.
///
CFrequencyPlot& CFrequencyPlot::operator = (const CFrequencyPlot &other) {
  if (this != &other) { // protect against invalid self-assignment
  }

  // by convention, always return *this
  return *this;
};

///
/// \fn void updateFigure()
///
/// \brief This function updates the plot.
///
/// \return void
///
void CFrequencyPlot::updateFigure() {

};

///
/// \fn void updateFrequency(double dFrequency)
///
/// \brief This function updates the frequency list to paint.
///
/// \return void
///
void CFrequencyPlot::updateFrequency(double dFrequency) {
  // add the new frequency at the back
  mFrequencyList.push_back(dFrequency);

  // if list capacity is exhausted, delete first element
  if (mFrequencyList.size() > muFrequencyListCapacity) {
    mFrequencyList.pop_front();
  }
};

///
/// \fn void setFrequencyListCapacity(size_t uCapacity)
///
/// \brief This function sets the capacity of the frequency list.
///
/// \param[in] uCapacity capacity of the list
///
/// \return void
///
void CFrequencyPlot::setFrequencyListCapacity(size_t uCapacity) {
  muFrequencyListCapacity = uCapacity;
};

///
/// \fn size_t getFrequencyListCapacity const ()
///
/// \brief This function sets the capacity of the frequency list.
///
/// \return capacity of the list
///
size_t CFrequencyPlot::getFrequencyListCapacity () const {
  return muFrequencyListCapacity;
};


//------------------------------------------------------------------------------
// End of FrequencyPlot.cpp
//------------------------------------------------------------------------------

