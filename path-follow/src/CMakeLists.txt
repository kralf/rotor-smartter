#-------------------------------------------------------------------------------
# Main Program
#-------------------------------------------------------------------------------

find_library( ROTOR_LIB rotor )
find_path( EIGEN2_INC Eigen /usr/include/eigen2 )
file( GLOB PATH_FOLLOW_SRC path-follow/* )

include_directories( ${EIGEN2_INC} )
add_executable( pathFollow ${PATH_FOLLOW_SRC} )
target_link_libraries( pathFollow ${ROTOR_LIB})

sm_cpp_install( pathFollow )
