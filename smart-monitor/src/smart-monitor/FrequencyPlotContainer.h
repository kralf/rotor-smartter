///
/// \brief   This class represents a container of frequency plots.
///
/// \file    FrequencyPlotContainer.h
///
/// \author  Jerome Maye
///
/// \date    30/10/2009
///
/// \details This class represents a container of frequency plots.
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

#ifndef FREQUENCYPLOTCONTAINER_H
#define FREQUENCYPLOTCONTAINER_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
// local includes
#include "FrequencyPlot.h"

// system includes
#include <map>

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>

#include <rotor/Registry.h>

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Main Class Declaration
//------------------------------------------------------------------------------
///
/// \class CFrequencyPlotContainer
///
/// \brief Class for interfacing the status plot.
///
class CFrequencyPlotContainer : public QWidget {
  Q_OBJECT

  ///
  /// \fn CFrequencyPlotContainer()
  ///
  /// \brief Default constructor.
  ///
  CFrequencyPlotContainer();

  ///
  /// \fn CFrequencyPlotContainer(const CFrequencyPlotContainer &other)
  ///
  /// \brief Copy constructor.
  ///
  CFrequencyPlotContainer(const CFrequencyPlotContainer &other);

  ///
  /// \brief Assignement operator.
  ///
  CFrequencyPlotContainer& operator = (const CFrequencyPlotContainer &other);

  /// \var Rotor::Registry *mpRegistry
  ///
  /// \brief Handle on the Rotor registry.
  ///
  Rotor::Registry* mpRegistry;

  /// \var QGridLayout *mpGridLayout
  ///
  /// \brief Handle on the grid layout of our status plot.
  ///
  QVBoxLayout *mpVBoxLayout;

  /// \var QTimer *mpTimer
  ///
  /// \brief Handle on timer that will trigger update of the plot.
  ///
  QTimer *mpTimer;

  /// \var map<QString, CFrequencyPlot*> mpPlots
  ///
  /// \brief Handles on the frequecny plots.
  ///
  std::map<QString, CFrequencyPlot*> mpPlots;

public:
  ///
  /// \fn CFrequencyPlotContainer(QWidget *pParent)
  ///
  /// \brief Constructor called automatically by QT.
  ///
  /// \param[in] pParent pointer to the parent's widget
  ///
  CFrequencyPlotContainer(QWidget *pParent);

  ///
  /// \fn virtual ~CFrequencyPlotContainer()
  ///
  /// \brief Destructor.
  ///
  ~CFrequencyPlotContainer();

  ///
  /// \fn setRegistry(Rotor::Registry& rRegistry)
  ///
  /// \brief Set the frequency plot's Rotor registry.
  ///
  void setRegistry(Rotor::Registry& rRegistry);

  ///
  /// \fn addPlot(const QString& messageName, double dMaxFrequency)
  ///
  /// \brief Add a frequency plot for the message with the given name.
  ///
  void addPlot(const QString& messageName, double dMaxFrequency);

public slots:
  ///
  /// \fn void updatePlots()
  ///
  /// \brief This function updates the plots.
  ///
  /// \return void
  ///
  void updatePlots();

protected:
// accessible by inheritance

};

#endif // FREQUENCYPLOTCONTAINER_H


//------------------------------------------------------------------------------
// End of FrequencyPlotContainer.h
//------------------------------------------------------------------------------

