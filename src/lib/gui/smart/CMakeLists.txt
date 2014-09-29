remake_qt4_ui(smart_gui)
remake_qt4_moc(smart_gui)
remake_add_library(smart_gui
  LINK ${ROTOR_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
    ${QWT_QT4_LIBRARY})
remake_add_headers()
