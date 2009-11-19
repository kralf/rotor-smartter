#-------------------------------------------------------------------------------
# Main Program
#-------------------------------------------------------------------------------

find_library( CV_LIB cv )
find_library( ROTOR_LIB rotor )
find_library( RANA_LIB rana-graph )
file( GLOB PATH_PLANNER_SRC pathPlanner/* )

add_executable( pathPlanner ${PATH_PLANNER_SRC} )
target_link_libraries( pathPlanner ${CV_LIB} ${ROTOR_LIB} ${RANA_LIB})

sm_cpp_install( pathPlanner )
