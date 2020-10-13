# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0

# Set application root dir for use inside the source code.
add_definitions(-DIMAGE_EDITOR_ROOT="${CMAKE_INSTALL_PREFIX}")

if (ENABLE_QT5)
    find_package(Qt5 COMPONENTS Core Gui Widgets) # find and setup Qt5 for this project
    include_directories(${Qt5Core_INCLUDE_DIRS}
                        ${Qt5Gui_INCLUDE_DIRS}
                        ${Qt5OpenGL_INCLUDE_DIRS})
endif()

if (NOT PNG_DIR)
    find_package(PNG QUIET)
endif()

if(Qt5_FOUND)
if(PNG_FOUND)
    set(imageEditor_MOC_HDRS ImageEditorDialog.h)
    set(imageEditor_CPPS imageEditor.cpp)

if (ENABLE_QT5)
    qt5_wrap_cpp(imageEditor_MOC_SRCS ${imageEditor_MOC_HDRS})
endif()

    add_executable(imageEditor2 ${imageEditor_CPPS} ${imageEditor_MOC_SRCS})

if (ENABLE_QT5)
    target_link_libraries(imageEditor2 Qt5::Core)
    target_link_libraries(imageEditor2 Qt5::Gui)
    target_link_libraries(imageEditor2 Qt5::Widgets)
endif()

    target_link_libraries(imageEditor2 ${SEEXPR_LIBRARIES})
    target_link_libraries(imageEditor2 ${SEEXPR_EDITOR_LIBRARIES})

    include_directories(SYSTEM ${PNG_INCLUDE_DIR})
    target_link_libraries(imageEditor2 ${PNG_LIBRARIES})
    install(TARGETS imageEditor2 DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
endif()

install(FILES fbm.se noisecolor1.se noisecolor2.se noise.se raytrace.se sinc.se
        DESTINATION share/KSeExpr/expressions)
