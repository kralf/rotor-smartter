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

file( GLOB SMART_MONITOR_SRC smart-monitor/*.cpp )

set( SMART_MONITOR_MOC_HEADERS smart-monitor/LocalizationPlot.h )

qt4_automoc( ${SMART_MONITOR_SRC} )
QT4_WRAP_CPP( SMART_MONITOR_MOC_SRC ${SMART_MONITOR_MOC_HEADERS} )


add_executable( 
  smartMonitor 
  ${SMART_MONITOR_SRC} 
  ${MAIN_WINDOW_UI_H} 
  ${SMART_MONITOR_MOC_SRC} 
)

target_link_libraries( 
  smartMonitor
  ${ROTOR_LIB}
  ${QT_LIBRARIES}
)
