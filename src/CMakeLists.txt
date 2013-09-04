remake_find_package(opencv CONFIG)
remake_find_package(rotor CONFIG)
remake_find_file(eigen2/Eigen/Core PACKAGE eigen2)
remake_find_library(qwt-qt4 qwt-qt4/qwt.h PACKAGE qwt-qt4)

remake_qt4_include()
remake_include(${EIGEN2_PATH}/eigen2)
remake_add_directories()

remake_pkg_config_generate(REQUIRES opencv rotor)
