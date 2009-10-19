#-------------------------------------------------------------------------------
# Main Program
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )
file( GLOB PATH_FOLLOW_SRC pathFollow/* )

add_executable( pathFollow ${PATH_FOLLOW_SRC} )
target_link_libraries( pathFollow ${ROTOR_LIB})

sm_cpp_install( pathFollow )
