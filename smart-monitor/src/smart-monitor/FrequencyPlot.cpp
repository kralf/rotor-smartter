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

CFrequencyPlot::CFrequencyPlot() {
};

CFrequencyPlot::CFrequencyPlot(const QString &rTitle, double dMaxFrequency,
  QWidget *pParent, size_t uFrequencyListCapacity) :
  QWidget(pParent),
  mTitle(rTitle),
  mdMaxFrequency(dMaxFrequency),
  muFrequencyListCapacity(uFrequencyListCapacity) {
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
};

CFrequencyPlot::~CFrequencyPlot() {
};

CFrequencyPlot& CFrequencyPlot::operator = (const CFrequencyPlot &other) {
  if (this != &other) { // protect against invalid self-assignment
  }

  // by convention, always return *this
  return *this;
};

void CFrequencyPlot::updateFrequency(double dFrequency) {
  // add the new frequency at the back
  mFrequencyList.push_back(dFrequency);

  // if list capacity is exhausted, delete first element
  if (mFrequencyList.size() > muFrequencyListCapacity)
    mFrequencyList.pop_front();

  repaint();
};

void CFrequencyPlot::setFrequencyListCapacity(size_t uCapacity) {
  muFrequencyListCapacity = uCapacity;
};

size_t CFrequencyPlot::getFrequencyListCapacity () const {
  return muFrequencyListCapacity;
};

void CFrequencyPlot::setMaxFrequency(double dMaxFrequency) {
  mdMaxFrequency = dMaxFrequency;
  repaint();
}

double CFrequencyPlot::getMaxFrequency() const {
  return mdMaxFrequency;
}

QSize CFrequencyPlot::sizeHint() const {
  return fontMetrics().boundingRect(mTitle+" 000 Hz").size()+QSize(8, 18);
};

void CFrequencyPlot::paintEvent(QPaintEvent* event) {
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(event->rect(), palette().brush(QPalette::Base));

  painter.translate(0, height());
  painter.scale((double)width()/(double)muFrequencyListCapacity,
    -height()/(mdMaxFrequency*1.2));
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
