remake_find_library(rotor rotor/Rotor.h PACKAGE rotor)
remake_find_file(eigen2/Eigen/Core PACKAGE eigen2)

remake_include(${EIGEN2_FILE}/eigen2)
remake_add_directories()
