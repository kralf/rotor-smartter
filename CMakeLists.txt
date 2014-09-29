cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  rotor-smartter
  VERSION 0.1
  RELEASE alpha
  SUMMARY "RoToR modules for the SmartTer."
  AUTHOR "Ralf Kaestner"
  AUTHOR "Dizan Vasquez"
  AUTHOR "Jerome Maye"
  AUTHOR "Martin Rufli"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://github.com/kralf/rotor-smartter"
  LICENSE "GNU Lesser General Public License (LGPL)"
  INSTALL /usr
  PREFIX rotor-
)

remake_add_directories(
  python
)
remake_add_directories(
  data
  COMPONENT data
)

remake_pack_deb(
  DEPENDS rotor-carmen libcv[0-9.]* libqwt5-qt4
)
remake_add_package(
  COMPONENT python
  DESCRIPTION "Python modules"
  DEPENDS rotor-python python-opencv
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  DEPENDS rotor-dev libeigen2-dev libcv-dev libqwt5-qt4-dev
)
remake_pack_deb(
  ARCH all
  COMPONENT data
  DESCRIPTION "costmap data"
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake rotor-dev rotor-carmen libeigen2-dev libcv-dev
    libhighgui-dev libcvaux-dev libqwt5-qt4-dev rotor-python
    python-opencv pkg-config doxygen
  PASS CMAKE_BUILD_TYPE ROTOR_SMARTTER_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION precise
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake rotor-dev rotor-carmen libeigen2-dev libcv2.3
    libcv-dev libopencv-contrib-dev libqwt5-qt4-dev rotor-python
    python-opencv pkg-config doxygen
  PASS CMAKE_BUILD_TYPE ROTOR_SMARTTER_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION trusty
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake rotor-dev rotor-carmen libeigen2-dev libcv2.4
    libopencv-dev libopencv-contrib-dev libqwt5-qt4-dev rotor-python
    python-opencv pkg-config doxygen
  PASS CMAKE_BUILD_TYPE ROTOR_SMARTTER_GIT_REVISION
)
