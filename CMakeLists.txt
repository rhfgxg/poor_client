cmake_minimum_required(VERSION 3.5)

project(poor_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)    # Network



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(poor_client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        # 用户管理界面
        ui/user/user_login_account.h ui/user/user_login_account.cpp
        ui/user/user_login_phone.h ui/user/user_login_phone.cpp
        ui/user/user_login_phone_code.h ui/user/user_login_phone_code.cpp ui/user/user_login_phone_code.ui

        # 工具界面
        ui/tools/tools.h ui/tools/tools.cpp
            ui/tools/convertor/convertor.h ui/tools/convertor/convertor.cpp
            ui/tools/convertor/timestamp/timestamp_ui.h ui/tools/convertor/timestamp/timestamp_ui.cpp

        # 文件管理界面
        ui/file/fileuploads.h ui/file/fileuploads.cpp

        # 终端界面
        ui/terminal/terminal.h ui/terminal/terminal.cpp

        # 功能实现
        feature/tools/convertor/timestamp/timestamp.h feature/tools/convertor/timestamp/timestamp.cpp
        feature/tools/encipher/md5.h feature/tools/encipher/md5.cpp
        feature/tools/encipher/base64.h feature/tools/encipher/base64.cpp

        feature/terminal/command.h feature/terminal/command.cpp

        # 库文件重写
        overloading/terminal/plain_text_edit.h overloading/terminal/plain_text_edit.cpp

        # 服务器通信
        server/client_network.h server/client_network.cpp
            server/user/user_manager.h server/user/user_manager.cpp
            server/uploads/useruploadsmanager.h server/uploads/useruploadsmanager.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET poor_client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(poor_client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(poor_client
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(poor_client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(poor_client PRIVATE Qt6::Network) # Network

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.poor_client)
endif()
set_target_properties(poor_client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS poor_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(poor_client)
endif()
