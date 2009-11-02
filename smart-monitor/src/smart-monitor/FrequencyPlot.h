///
/// \brief   This class represents the frequency plot interface.
///
/// \file    FrequencyPlot.h
///
/// \author  Jerome Maye
///
/// \date    30/10/2009
///
/// \details This class represents the frequency plot interface.
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

#ifndef FREQUENCYPLOT_H
#define FREQUENCYPLOT_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
// local includes


// system includes
#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <list>


//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Main Class Declaration
//------------------------------------------------------------------------------
///
/// \class CFrequencyPlot
///
/// \brief Class for interfacing the status plot.
///
class CFrequencyPlot : public QWidget {
  Q_OBJECT

  ///
  /// \fn CFrequencyPlot()
  ///
  /// \brief Default constructor.
  ///
  CFrequencyPlot();

  ///
  /// \fn CFrequencyPlot(const CFrequencyPlot &other)
  ///
  /// \brief Copy constructor.
  ///
  CFrequencyPlot(const CFrequencyPlot &other);

  ///
  /// \brief Assignement operator.
  ///
  CFrequencyPlot& operator = (const CFrequencyPlot &other);

  /// \var QString muTitle
  ///
  /// \brief Title of the frequency plot.
  ///
  QString mTitle;

  /// \var double mdMaxFrequency
  ///
  /// \brief Maximum frequency of the frequency plot.
  ///
  double mdMaxFrequency;

  /// \var std::list<double> mFrequencyList
  ///
  /// \brief List of frequencies for painting.
  ///
  std::list<double> mFrequencyList;

  /// \var size_t muFrequencyListCapacity
  ///
  /// \brief Capacity of the frequency list.
  ///
  size_t muFrequencyListCapacity;

public:
  ///
  /// \fn CFrequencyPlot(const QString &rTitle, double mdMaxFrequency, QWidget *pParent)
  ///
  /// \brief Constructor called automatically by QT.
  ///
  /// \param[in] rTitle title of the plot
  /// \param[in] dMaxFrequency maximum frequency of the plot
  /// \param[in] pParent pointer to the parent's widget
  /// \param[in] uFrequencyListCapacity frequency queue capacity
  ///
  CFrequencyPlot(const QString &rTitle, double dMaxFrequency,
    QWidget *pParent, size_t uFrequencyListCapacity = 100);

  ///
  /// \fn virtual ~CFrequencyPlot()
  ///
  /// \brief Destructor.
  ///
  ~CFrequencyPlot();

  ///
  /// \fn void updateFrequency(double dFrequency)
  ///
  /// \brief This function updates the frequency list to paint.
  ///
  /// \return void
  ///
  void updateFrequency(double dFrequency);

  ///
  /// \fn void setFrequencyListCapacity(size_t uCapacity)
  ///
  /// \brief This function sets the capacity of the frequency list.
  ///
  /// \param[in] uCapacity capacity of the list
  ///
  /// \return void
  ///
  void setFrequencyListCapacity(size_t uCapacity);

  ///
  /// \fn size_t getFrequencyListCapacity const ()
  ///
  /// \brief This function sets the capacity of the frequency list.
  ///
  /// \return capacity of the list
  ///
  size_t getFrequencyListCapacity() const;

  virtual QSize sizeHint() const;
protected:
// accessible by inheritance

  void paintEvent(QPaintEvent* event);
};

#endif // FREQUENCYPLOT_H


//------------------------------------------------------------------------------
// End of FrequencyPlot.h
//------------------------------------------------------------------------------

