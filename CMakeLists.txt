cmake_minimum_required(VERSION 3.16)
project(LAN_fileshare VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Quick Network Widgets Core Gui Qml)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appLAN_fileshare
    main.cpp
)

qt_add_qml_module(appLAN_fileshare
    URI LAN_fileshare
    VERSION 1.0
    SOURCES
        Backend/Backend.h


        Backend/udpside.h

        Backend/FileDialogHelper.cpp
        Backend/UDP_Receiver.cpp
        Backend/TCP_Sharing.cpp
        Backend/UDP_Sender.cpp
    QML_FILES
        ui/Main.qml
        ui/Send_UI.qml
        ui/Receive_UI.qml
        ui/Icons/qml/ComputerIcon.qml
        ui/ConnectionRequest.qml
        ui/FileShare.qml
        SOURCES Backend/tcpside.h
        SOURCES Backend/Backend.cpp
)

qt_add_resources(appLAN_fileshare "resources"
    PREFIX /
    FILES
        ui/Icons/Computer4x.png
)

set_target_properties(appLAN_fileshare PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appLAN_fileshare
    PRIVATE Qt6::Quick Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS appLAN_fileshare
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
