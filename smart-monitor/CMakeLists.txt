project( smart-monitor )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.1 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Monitoring tool for elrob" )
set( PACKAGE_DESCRIPTION "Visualization tool for the smartter platorm" )
set( PACKAGE_DEPENDS rotor,libqtgui4,libqwt5-qt4 )

include( SimpleCMake )
sm_configure_project()
