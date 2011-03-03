remake_qt4_ui(path_gui)
remake_qt4_moc(path_gui)
remake_add_library(path_gui LINK ${ROTOR_LIBRARY} ${QT_QTGUI_LIBRARY})
remake_add_headers()
