# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

if(ENABLE_QT6)
    set(imageEditor_MOC_HDRS ImageEditorDialog.h)
    set(imageEditor_CPPS imageEditor.cpp)

    qt6_wrap_cpp(imageEditor_MOC_SRCS ${imageEditor_MOC_HDRS})

    add_executable(imageEditor2 ${imageEditor_CPPS} ${imageEditor_MOC_SRCS})

    target_link_libraries(imageEditor2 Qt6::Core
                                       Qt6::Gui
                                       Qt6::Widgets
                                       ${SEEXPR_LIBRARIES}
                                       ${SEEXPR_EDITOR_LIBRARIES})

    # Set application root dir for use inside the source code.
    target_compile_definitions(imageEditor2 PRIVATE IMAGE_EDITOR_ROOT="${CMAKE_INSTALL_PREFIX}")
    install(TARGETS imageEditor2 DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(FILES fbm.se noisecolor1.se noisecolor2.se noise.se raytrace.se sinc.se
        DESTINATION share/KSeExpr/expressions)
endif()
