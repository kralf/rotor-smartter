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
#include <QWidget>
#include <QGridLayout>
#include <QTimer>


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

  /// \var QGridLayout *mpGridLayout
  ///
  /// \brief Handle on the grid layout of our status plot.
  ///
  QGridLayout *mpGridLayout;

  /// \var QTimer *mpTimer
  ///
  /// \brief Handle on timer that will trigger update of the plot.
  ///
  QTimer *mpTimer;

  /// \var CFrequencyPlot *mpAxtFreqPlot
  ///
  /// \brief Handle on the alasca frequency plot.
  ///
  CFrequencyPlot *mpAxtFreqPlot;

  /// \var CFrequencyPlot *mpGyroFreqPlot
  ///
  /// \brief Handle on the gyro frequency plot.
  ///
  CFrequencyPlot *mpGyroFreqPlot;

  /// \var CFrequencyPlot *mpStatusFreqPlot
  ///
  /// \brief Handle on the status frequency plot.
  ///
  CFrequencyPlot *mpStatusFreqPlot;

  /// \var CFrequencyPlot *mpSmartVelocityFreqPlot
  ///
  /// \brief Handle on the smart velocity frequency plot.
  ///
  CFrequencyPlot *mpSmartVelocityFreqPlot;

  /// \var CFrequencyPlot *mpCarmenVelocityFreqPlot
  ///
  /// \brief Handle on the carmen velocity frequency plot.
  ///
  CFrequencyPlot *mpCarmenVelocityFreqPlot;

  /// \var CFrequencyPlot *mpPathFreqPlot
  ///
  /// \brief Handle on the path frequency plot.
  ///
  CFrequencyPlot *mpPathFreqPlot;

  /// \var CFrequencyPlot *mpLocalizeFreqPlot
  ///
  /// \brief Handle on the localize frequency plot.
  ///
  CFrequencyPlot *mpLocalizeFreqPlot;

  /// \var CFrequencyPlot *mpOdometryFreqPlot
  ///
  /// \brief Handle on the odometry frequency plot.
  ///
  CFrequencyPlot *mpOdometryFreqPlot;

  /// \var CFrequencyPlot *mpPointFreqPlot
  ///
  /// \brief Handle on the point frequency plot.
  ///
  CFrequencyPlot *mpPointFreqPlot;

  /// \var CFrequencyPlot *mpLaserConfigFreqPlot
  ///
  /// \brief Handle on the laser config frequency plot.
  ///
  CFrequencyPlot *mpLaserConfigFreqPlot;

  /// \var CFrequencyPlot *mpLaserLaserFreqPlot
  ///
  /// \brief Handle on the laser laser frequency plot.
  ///
  CFrequencyPlot *mpLaserLaserFreqPlot;

  /// \var CFrequencyPlot *mpRobotLaserFreqPlot
  ///
  /// \brief Handle on the robot laser frequency plot.
  ///
  CFrequencyPlot *mpRobotLaserFreqPlot;

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

