///
/// \brief   This class represents the gyro plot interface.
///
/// \file    GyroPlot.h
///
/// \author  Jerome Maye
///
/// \date    28/10/2009
///
/// \details This class represents the gyro plot interface.
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

#ifndef GYROPLOT_H
#define GYROPLOT_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
// local includes


// system includes
#include <QWidget>
#include <QLabel>
#include <QGridLayout>


//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Main Class Declaration
//------------------------------------------------------------------------------
///
/// \class CGyroPlot
///
/// \brief Class for interfacing the gyro plot.
///
class CGyroPlot : public QWidget {
  Q_OBJECT

  ///
  /// \fn CGyroPlot()
  ///
  /// \brief Default constructor.
  ///
  CGyroPlot();

  ///
  /// \brief Assignement operator.
  ///
  CGyroPlot& operator = (const CGyroPlot &other);

  /// \var QLabel *mpIntegrateAngleLabel
  ///
  /// \brief Handle on the integrated angle value label.
  ///
  QLabel *mpIntegratedAngleLabel;

  /// \var QLabel *mpIntegratedAngleValue
  ///
  /// \brief Handle on the integrated angle value.
  ///
  QLabel *mpIntegratedAngleValue;

  /// \var QGridLayout *mpGridLayout
  ///
  /// \brief Handle on the grid layout of our gyro plot.
  ///
  QGridLayout *mpGridLayout;

  /// \var char macBuffer[128]
  ///
  /// \brief Character array for displaying stuff.
  ///
  char macBuffer[128];

public:
  ///
  /// \fn CGyroPlot(QWidget *pParent)
  ///
  /// \brief Constructor called automatically by QT.
  ///
  /// \param[in] pParent pointer to the parent's widget
  ///
  CGyroPlot(QWidget *pParent);

  ///
  /// \fn virtual ~CGyroPlot()
  ///
  /// \brief Destructor.
  ///
  ~CGyroPlot();

  ///
  /// \fn void updateGyro(double dIntegratedTheta)
  ///
  /// \brief This function updates the gyro angles.
  ///
  /// \param[in] dIntegratedTheta integrated gyro angle
  ///
  /// \return void
  ///
  void updateGyro(double dIntegratedTheta);

protected:
// accessible by inheritance

};

#endif // GYROPLOT_H


//------------------------------------------------------------------------------
// End of GyroPlot.h
//------------------------------------------------------------------------------

