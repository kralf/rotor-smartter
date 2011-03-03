cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  "RoToR SmartTer"
  VERSION 0.1
  RELEASE alpha
  SUMMARY "RoToR modules for the SmartTer."
  AUTHOR "Ralf Kaestner"
  AUTHOR "Dizan Vasquez"
  AUTHOR "Jerome Maye"
  AUTHOR "Martin Rufli"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://aslforge.ethz.ch/projects/rotor-smartter"
  LICENSE "GNU Lesser General Public License (LGPL)"
  INSTALL /usr
  PREFIX rotor-
)

remake_python()
remake_add_directories(
  data
  COMPONENT data
)

remake_pack_deb(
  rotor
)
remake_add_package(
  COMPONENT python
  DESCRIPTION "Python modules"
  rotor-python
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  rotor-dev
)
remake_pack_deb(
  ARCH all
  COMPONENT data
  DESCRIPTION "costmap data"
)
