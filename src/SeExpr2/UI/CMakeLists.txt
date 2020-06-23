# Copyright Disney Enterprises, Inc.  All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License
# and the following modification to it: Section 6 Trademarks.
# deleted and replaced with:
#
# 6. Trademarks. This License does not grant permission to use the
# trade names, trademarks, service marks, or product names of the
# Licensor and its affiliates, except as required for reproducing
# the content of the NOTICE file.
#
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0

if (EXISTS "/usr/share/apps/cmake/modules")
    # Needed for some versions of CMake, which only look in version-specific module path
    list(APPEND CMAKE_MODULE_PATH "/usr/share/apps/cmake/modules")
endif()

if (Qt5_FOUND OR QT4_FOUND)
    # Adding Krita's definitions to make headers automatically compatible. -amyspark
    add_definitions(
        -DQT_USE_QSTRINGBUILDER
        -DQT_STRICT_ITERATORS
        -DQT_NO_SIGNALS_SLOTS_KEYWORDS
        -DQT_NO_URL_CAST_FROM_STRING
        -DQT_USE_FAST_CONCATENATION
        -DQT_USE_FAST_OPERATOR_PLUS
    )

    if (CMAKE_COMPILER_IS_GNUCXX)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-suggest-override> -Wextra)
    endif()

    BuildParserScanner(ExprSpecParserLex ExprSpecParser ExprSpec
                       editor_parser_cpp)

    option(ENABLE_DEEPWATER "Whether to build controls for displaying a deep water spectrum" FALSE)
    option(ENABLE_ANIMCURVE "Whether to build controls for anim curves" FALSE)
    option(ENABLE_EXPRMAIN "Whether to build the ExprMain entrypoint" FALSE)
    option(ENABLE_OPENGL_DIALOGS "Whether to build legacy OpenGL widgets (ExprShortEdit, ExprDialog, and ExprGrapher2d)" FALSE)

    set(EDITOR_MOC_HDRS
            Debug.h
            ErrorMessages.h
            ExprAddDialog.h
            ExprBrowser.h
            ExprColorCurve.h
            ExprColorSwatch.h
            ExprCompletionModel.h
            ExprControlCollection.h
            ExprControl.h
            ExprCurve.h
            ExprEditor.h
            ExprFileDialog.h
            ExprPopupDoc.h
            ExprTextEdit.h
        )

    set(EDITOR_ADDITIONAL_HEADERS
            BasicExpression.h
            ControlSpec.h
            Editable.h
            EditableExpression.h
            ExprHelp.h
            ExprHighlighter.h
            ExprPopupDoc.h
            ExprShortEdit.h
            ExprSpecType.h
    )

    set(EDITOR_CPPS
            BasicExpression.cpp
            Debug.cpp
            Editable.cpp
            EditableExpression.cpp
            ErrorMessages.cpp
            ExprAddDialog.cpp
            ExprBrowser.cpp
            ExprColorCurve.cpp
            ExprColorSwatch.cpp
            ExprCompletionModel.cpp
            ExprControl.cpp
            ExprControlCollection.cpp
            ExprCurve.cpp
            ExprEditor.cpp
            ExprFileDialog.cpp
            ExprHighlighter.cpp
            ExprPopupDoc.cpp
            ExprTextEdit.cpp
    )
    
    if (ENABLE_DEEPWATER)
        add_definitions(-DSEEXPR_ENABLE_DEEPWATER)
        set(EDITOR_MOC_HDRS ${EDITOR_MOC_HDRS} ExprDeepWater.h)
        set(EDITOR_CPPS ${EDITOR_CPPS} ExprDeepWater.cpp)
        set(PYQT_EXPORT_DEEPWATER "")
    else()
        set(PYQT_EXPORT_DEEPWATER "-x SEEXPR_ENABLE_DEEPWATER")
    endif()

    if (ENABLE_ANIMCURVE)
        add_definitions(-DSEEXPR_ENABLE_ANIMCURVE)
    endif()

    if (ENABLE_EXPRMAIN)
        set(EDITOR_CPPS ${EDITOR_CPPS} ExprMain.cpp)
    endif()

    if (ENABLE_OPENGL_DIALOGS)
        # Other package dependencies...
        find_package(OpenGL)
        set(EDITOR_MOC_HDRS ${EDITOR_MOC_HDRS}
                ExprDialog.h
                ExprGrapher2d.h
                ExprShortEdit.h
        )
        set(EDITOR_CPPS ${EDITOR_CPPS}
                ExprDialog.cpp
                ExprGrapher2d.cpp
                ExprShortEdit.cpp
        )
        set(PYQT_EXPORT_OPENGL_DIALOGS "")
    else()
        set(PYQT_EXPORT_OPENGL_DIALOGS "-x SEEXPR_ENABLE_OPENGL_DIALOGS")
    endif()

    if (ENABLE_QT5)
        qt5_wrap_cpp(EDITOR_MOC_SRCS ${EDITOR_MOC_HDRS})
    else()
        qt4_wrap_cpp(EDITOR_MOC_SRCS ${EDITOR_MOC_HDRS})
    endif()

    set(ANIMLIB_SRCS "")
    if (DEFINED ANIMLIB_DIR)
        set(CE_MOC_HDRS CE/CECurveListUI.h CE/CEDragHandlers.h CE/CEGraphCurve.h
            CE/CEGraphKey.h CE/CEGraphSeg.h CE/CEGraphUI.h CE/CEMainUI.h
            CE/CESegEditUI.h CE/CETool.h)

        set(CE_CPPS CE/CECurveListUI.cpp CE/CEDragHandlers.cpp CE/CEGraphCurve.cpp
            CE/CEGraphKey.cpp CE/CEGraphSeg.cpp CE/CEGraphUI.cpp CE/CEMainUI.cpp
            CE/CESegEditUI.cpp CE/CETool.cpp)

        if (ENABLE_QT5)
            qt5_wrap_cpp(CE_MOC_SRCS ${CE_MOC_HDRS})
        else()
            qt4_wrap_cpp(CE_MOC_SRCS ${CE_MOC_HDRS})
        endif()
        set(ANIMLIB_SRCS "${CE_CPPS} ${CE_MOC_SRCS}")
    endif()

    if (BUILD_TRANSLATIONS)
        include(ECMPoQmTools)
        ecm_install_po_files_as_qm("${CMAKE_SOURCE_DIR}/po")
        ecm_create_qm_loader(SeExpr2Editor_QM_LOADER seexpr2_qt)
        set(EDITOR_CPPS
            ${EDITOR_CPPS}
            ${SeExpr2Editor_QM_LOADER})
    endif()

    if (WIN32)
        add_library(SeExpr2Editor ${EDITOR_CPPS} ${EDITOR_MOC_SRCS}
                    ${ANIMLIB_SRCS}
                    ${editor_parser_cpp})
        generate_export_header(SeExpr2Editor
                               BASE_NAME SeExpr2Editor
                               EXPORT_MACRO_NAME SeExpr2Editor
                               EXPORT_FILE_NAME SeExpr2Editor_Export.h
                               STATIC_DEFINE SeExpr2Editor_BUILT_AS_STATIC )
    else()
        add_library(SeExpr2Editor SHARED ${EDITOR_CPPS} ${EDITOR_MOC_SRCS}
                    ${ANIMLIB_SRCS}
                    ${editor_parser_cpp})
    endif()

    set_property(TARGET SeExpr2Editor PROPERTY VERSION ${SeExpr2_VERSION})
    set_property(TARGET SeExpr2Editor PROPERTY SOVERSION 3)
    set_property(TARGET SeExpr2Editor PROPERTY
                 INTERFACE_SeExpr2Editor_MAJOR_VERSION ${SeExpr2_MAJOR_VERSION})
    set_property(TARGET SeExpr2Editor APPEND PROPERTY
                 COMPATIBLE_INTERFACE_STRING ${SeExpr2_MAJOR_VERSION})

    target_include_directories(SeExpr2Editor
        PRIVATE ${CMAKE_BINARY_DIR}/src/SeExpr2
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        INTERFACE $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(SeExpr2Editor PUBLIC SeExpr2)

    if (ENABLE_QT5)
        target_link_libraries(SeExpr2Editor PRIVATE
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
        )
    else()
        target_link_libraries(SeExpr2Editor PRIVATE
            ${QT_QTCORE_LIBRARY}
            ${QT_QTGUI_LIBRARY}
        )
    endif()

    if (ENABLE_OPENGL_DIALOGS)
        if (ENABLE_QT5)
            target_link_libraries(SeExpr2 PRIVATE Qt5::OpenGL)
        else()
            target_link_libraries(SeExpr2 PRIVATE ${QT_QTOPENGL_LIBRARY})
        endif()
        target_link_libraries(SeExpr2Editor PRIVATE ${OPENGL_LIBRARY} ${GLUT_LIBRARY})
        if (WIN32)
            target_link_libraries(SeExpr2Editor PRIVATE opengl32)
        endif()
    endif()

    if (DEFINED ANIMLIB_DIR)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CE)
        target_link_libraries(SeExpr2Editor PRIVATE animlib)
    endif()

    if (DEFINED DERR_DIR)
        target_link_libraries(SeExpr2Editor PRIVATE DErr)
    endif()

    if (DEFINED DMSG_DIR)
        target_link_libraries(SeExpr2Editor PRIVATE DMsg)
    endif()

    if (DEFINED QDGUI_DIR)
        target_link_libraries(SeExpr2Editor PRIVATE qdcolorwidgets)
    endif()

    ## Install library and includes
    install(TARGETS SeExpr2Editor EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(
        FILES ${EDITOR_MOC_HDRS} ${EDITOR_ADDITIONAL_HEADERS}
        DESTINATION ${INCLUDE_DIR}/UI
    )

    if(USE_PYTHON)
        # Generate python module expreditor2, using sip
        find_program(SIP_EXEC sip)

        # Find python includes and libs. cmake doesn't seem to handle automatically
        find_package(PythonInterp)
        find_package(PythonLibs)

        macro(get_build_info NAME STORAGE)
            execute_process(
                COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/build/build-info ${NAME}
                OUTPUT_VARIABLE ${STORAGE}
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        endmacro()

        get_build_info(python-site PYTHON_SITE)
        get_build_info(python-inc PYTHON_INCLUDE_DIR)
        get_build_info(sip-inc SIP_INCLUDE_DIR)

        if (NOT DEFINED PYQT_SIP_FLAGS)
            if (ENABLE_QT5)
                get_build_info(pyqt5-sip-flags PYQT_SIP_FLAGS)
            else()
                get_build_info(pyqt4-sip-flags PYQT_SIP_FLAGS)
            endif()
            separate_arguments(PYQT_SIP_FLAGS)
        endif()

        if (NOT DEFINED PYQT_SIP_DIR)
            get_build_info(pyqt4-sip PYQT_SIP_DIR)
            if (NOT DEFINED PYQT_SIP_DIR)
               message(FATAL_ERROR "PYQT_SIP_DIR must be defined")
            endif()
        endif()

        set(CMAKE_INSTALL_PYTHON "${PYTHON_SITE}/SeExpr2" )

        set(CMAKE_CXX_FLAGS "-std=c++11")
        include_directories(${SIP_INCLUDE_DIR}
                            ${PYQT_SIP_DIR}
                            ${PYTHON_INCLUDE_DIR})

        add_custom_command(OUTPUT sipexpreditor2part0.cpp
                           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/SeExpr2Editor.sip
                           COMMENT 'Processing SeExpr2Editor.sip'
                           COMMAND ${SIP_EXEC} -w -c .
                           ${PYQT_SIP_FLAGS}
                           -j 1
                           -I. -I${PYQT_SIP_DIR} -I${SIP_INCLUDE_DIR}
                           ${PYQT_EXPORT_DEEPWATER}
                           ${PYQT_EXPORT_OPENGL_DIALOGS}
                           ${CMAKE_CURRENT_SOURCE_DIR}/SeExpr2Editor.sip
                           VERBATIM)

        add_library(expreditor2 SHARED sipexpreditor2part0.cpp)
        target_link_libraries(expreditor2 SeExpr2Editor ${SEEXPR_LIBRARIES}
                              ${OPENGL_LIBRARY} ${GLUT_LIBRARY} ${PYTHON_LIBRARIES})

        if (ENABLE_QT5)
            target_link_libraries(expreditor2 SeExpr2Editor
                                  Qt5::Core
                                  Qt5::Gui
                                  Qt5::Widgets
            )
        else()
            target_link_libraries(expreditor2 SeExpr2Editor
                                  ${QT_QTCORE_LIBRARY}
                                  ${QT_QTGUI_LIBRARY}
            )
        endif()

        if (ENABLE_EXPRDIALOG)
            if (ENABLE_QT5)
                target_link_libraries(expreditor2 PRIVATE Qt5::OpenGL)
            else()
                target_link_libraries(expreditor2 PRIVATE ${QT_QTOPENGL_LIBRARY})
            endif()
        endif()

        # No prefix on python module name
        set_target_properties(expreditor2 PROPERTIES PREFIX "")
        # Python modules require a ".so" suffix on macOS
        if (APPLE)
            set_target_properties(expreditor2 PROPERTIES SUFFIX ".so")
        endif()

        install(TARGETS expreditor2 EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_PYTHON})
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
                DESTINATION ${CMAKE_INSTALL_PYTHON})
    endif()
endif()
