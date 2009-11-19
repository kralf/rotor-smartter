#-------------------------------------------------------------------------------
# Globals
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )

find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

#-------------------------------------------------------------------------------
# QtDesigner UI
#-------------------------------------------------------------------------------

qt4_wrap_ui( MAIN_WINDOW_UI_H ../ui/MainWindow.ui )

#-------------------------------------------------------------------------------
# Executable
#-------------------------------------------------------------------------------

file( GLOB PATH_CONTROL_SRC path-control/*.cpp )

set(
  PATH_CONTROL_MOC_HEADERS
  path-control/PathPlot.h
)

qt4_automoc( ${PATH_CONTROL_SRC} )
QT4_WRAP_CPP( PATH_CONTROL_MOC_SRC ${PATH_CONTROL_MOC_HEADERS} )


add_executable(
  pathControl
  ${PATH_CONTROL_SRC}
  ${MAIN_WINDOW_UI_H}
  ${PATH_CONTROL_MOC_SRC}
)

target_link_libraries(
  pathControl
  ${ROTOR_LIB}
  ${QT_LIBRARIES}
)

sm_cpp_install( pathControl )
