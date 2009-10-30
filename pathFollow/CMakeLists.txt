project( path-follow )

cmake_minimum_required( VERSION 2.6 )

set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_VERSION 0.0.1 )
set( PACKAGE_RELEASE 0 )
set( PACKAGE_MAINTAINER_NAME "Dizan Vasquez" )
set( PACKAGE_MAINTAINER_EMAIL "vasquez@mavt.ethz.ch" )
set( PACKAGE_DESCRIPTION_SUMMARY "Simple path follow module for the Smartter" )
set( PACKAGE_DESCRIPTION "Arc controller based path follow module used on the Smartter autonomous vehicle" )
set( PACKAGE_DEPENDS )

execute_process(
  COMMAND dpkg --print-architecture
  OUTPUT_VARIABLE DEBIAN_ARCHITECTURE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

include( SimpleCMake )
sm_configure_project()
