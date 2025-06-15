cmake_minimum_required(VERSION 3.16)

project(LAN_fileshare VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Network)
#target_link_libaries(appLAN_fileshare PRIVATE Qt6::Network)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appLAN_fileshare
    main.cpp
)

qt_add_qml_module(appLAN_fileshare
    URI LAN_fileshare
    VERSION 1.0
    SOURCES

    QML_FILES
        ui/Main.qml
        SOURCES Backend/Backend.h Backend/UDP_searchAround.cpp
        QML_FILES ui/Send_UI.qml
        QML_FILES ui/Receive_UI.qml
        SOURCES
        SOURCES
        SOURCES Backend/TCP_Sharing.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appLAN_fileshare PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appLAN_fileshare
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appLAN_fileshare
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appLAN_fileshare
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
