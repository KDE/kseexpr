# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

# Set application root dir for use inside the source code.
add_definitions(-DIMAGE_EDITOR_ROOT="${CMAKE_INSTALL_PREFIX}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

if (NOT PNG_DIR)
    find_package(PNG QUIET)
endif()

if(PNG_FOUND)
    set(imageEditor_MOC_HDRS ImageEditorDialog.h)
    set(imageEditor_CPPS imageEditor.cpp)

    qt6_wrap_cpp(imageEditor_MOC_SRCS ${imageEditor_MOC_HDRS})
    add_executable(imageEditor2 ${imageEditor_CPPS} ${imageEditor_MOC_SRCS})

    target_link_libraries(imageEditor2 
        Qt6::Core 
        Qt6::Gui 
        Qt6::Widgets
        ${SEEXPR_LIBRARIES}
        ${SEEXPR_EDITOR_LIBRARIES}
        PNG::PNG
    )

    install(TARGETS imageEditor2 DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

install(FILES fbm.se noisecolor1.se noisecolor2.se noise.se raytrace.se sinc.se
        DESTINATION share/SeExpr2/expressions)