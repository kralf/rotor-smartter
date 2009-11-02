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
#include <QPaintEvent>
#include <QPainter>


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
CFrequencyPlot::CFrequencyPlot(const QString &rTitle, double dMaxFrequency,
  QWidget *pParent, size_t uFrequencyListCapacity) :
  QWidget(pParent),
  mTitle(rTitle),
  mdMaxFrequency(dMaxFrequency),
  muFrequencyListCapacity(uFrequencyListCapacity) {
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
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
  if (mFrequencyList.size() > muFrequencyListCapacity)
    mFrequencyList.pop_front();

  repaint();
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

QSize CFrequencyPlot::sizeHint() const {
  return fontMetrics().boundingRect(mTitle+" 000 Hz").size()+QSize(20, 25);
};

void CFrequencyPlot::paintEvent(QPaintEvent* event) {
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(event->rect(), palette().brush(QPalette::Base));

  painter.translate(0, height());
  painter.scale((double)width()/(double)muFrequencyListCapacity,
    -height()/mdMaxFrequency);
  size_t i = 0;
  QLinearGradient gradient(0, 0, 0, mdMaxFrequency);
  gradient.setColorAt(0, Qt::red);
  gradient.setColorAt(1, Qt::green);
  for (std::list<double>::const_iterator it = mFrequencyList.begin();
    it != mFrequencyList.end(); ++it, ++i)
    painter.fillRect(i, 0, 1, *it, gradient);

  painter.resetTransform();
  painter.drawText(QRect(4, 4,  width()-8, height()-8),
    Qt::AlignLeft | Qt::AlignTop, mTitle);

  double frequency = 0.0;
  QString frequencyString;
  if (!mFrequencyList.empty())
    frequency = mFrequencyList.back();
  painter.drawText(QRect(4, 4, width()-8, height()-8),
    Qt::AlignRight | Qt::AlignTop,
    frequencyString.sprintf("%3.0f Hz", frequency));
};

//------------------------------------------------------------------------------
// End of FrequencyPlot.cpp
//------------------------------------------------------------------------------
