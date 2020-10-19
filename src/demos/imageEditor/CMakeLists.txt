# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

# Set application root dir for use inside the source code.
add_definitions(-DIMAGE_EDITOR_ROOT="${CMAKE_INSTALL_PREFIX}")

if (NOT PNG_DIR)
    find_package(PNG)
endif()

if(ENABLE_QT5 AND PNG_FOUND)
    set(imageEditor_MOC_HDRS ImageEditorDialog.h)
    set(imageEditor_CPPS imageEditor.cpp)

    qt5_wrap_cpp(imageEditor_MOC_SRCS ${imageEditor_MOC_HDRS})

    add_executable(imageEditor2 ${imageEditor_CPPS} ${imageEditor_MOC_SRCS})

    target_link_libraries(imageEditor2 Qt5::Core
                                       Qt5::Gui
                                       Qt5::Widgets
                                       ${SEEXPR_LIBRARIES}
                                       ${SEEXPR_EDITOR_LIBRARIES})

    include_directories(SYSTEM ${PNG_INCLUDE_DIR})
    target_link_libraries(imageEditor2 ${PNG_LIBRARIES})

    install(TARGETS imageEditor2 DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(FILES fbm.se noisecolor1.se noisecolor2.se noise.se raytrace.se sinc.se
        DESTINATION share/KSeExpr/expressions)
endif()
