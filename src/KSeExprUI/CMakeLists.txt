# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0

if (EXISTS "/usr/share/apps/cmake/modules")
    # Needed for some versions of CMake, which only look in version-specific module path
    list(APPEND CMAKE_MODULE_PATH "/usr/share/apps/cmake/modules")
endif()

if (Qt5_FOUND)
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

    BuildParserScanner(KSeExprUI ExprSpecParserLex ExprSpecParser ExprSpec
                       editor_parser_cpp)

    set(EDITOR_MOC_HDRS
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
            Debug.h
            BasicExpression.h
            ControlSpec.h
            Editable.h
            EditableExpression.h
            ExprHelp.h
            ExprHighlighter.h
            ExprSpecType.h
            Utils.h
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
            Utils.cpp
    )

    qt5_wrap_cpp(EDITOR_MOC_SRCS ${EDITOR_MOC_HDRS})

    if (BUILD_TRANSLATIONS)
        include(ECMPoQmTools)
        ecm_install_po_files_as_qm("${CMAKE_SOURCE_DIR}/po")
        # ecm_create_qm_loader(KSeExprUI_QM_LOADER seexpr2_qt)
        set(KSeExprUI_QM_LOADER "ECMQmLoader-seexpr2_qt.cpp")
        set(EDITOR_CPPS
            ${EDITOR_CPPS}
            ${KSeExprUI_QM_LOADER})
    endif()

    add_library(KSeExprUI SHARED ${EDITOR_CPPS} ${EDITOR_MOC_SRCS}
                ${editor_parser_cpp})

    set_property(TARGET KSeExprUI PROPERTY VERSION ${KSeExpr_VERSION})
    set_property(TARGET KSeExprUI PROPERTY SOVERSION 3)
    set_property(TARGET KSeExprUI PROPERTY
                 INTERFACE_KSeExprUI_MAJOR_VERSION ${KSeExpr_MAJOR_VERSION})
    set_property(TARGET KSeExprUI APPEND PROPERTY
                 COMPATIBLE_INTERFACE_STRING ${KSeExpr_MAJOR_VERSION})

    target_include_directories(KSeExprUI
        PRIVATE ${CMAKE_BINARY_DIR}/src/KSeExpr
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        INTERFACE $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(KSeExprUI PUBLIC KSeExpr)

    target_link_libraries(KSeExprUI PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )

    ## Install library and includes
    install(TARGETS KSeExprUI EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(
        FILES ${EDITOR_MOC_HDRS} ${EDITOR_ADDITIONAL_HEADERS}
        DESTINATION "include/KSeExprUI"
    )

    if(USE_PYTHON)
        # Generate python module expreditor2, using sip
        find_program(SIP_EXEC sip)

        # Find python includes and libs. cmake doesn't seem to handle automatically
        find_package(PythonInterp)
        find_package(PythonLibs)

        macro(get_build_info NAME STORAGE)
            execute_process(
                COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/cmake/build-info ${NAME}
                OUTPUT_VARIABLE ${STORAGE}
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        endmacro()

        get_build_info(python-site PYTHON_SITE)
        get_build_info(python-inc PYTHON_INCLUDE_DIR)
        get_build_info(sip-inc SIP_INCLUDE_DIR)

        if (NOT DEFINED PYQT_SIP_FLAGS)
            get_build_info(pyqt5-sip-flags PYQT_SIP_FLAGS)
            separate_arguments(PYQT_SIP_FLAGS)
        endif()

        if (NOT DEFINED PYQT_SIP_DIR)
            message(FATAL_ERROR "PYQT_SIP_DIR must be defined")
        endif()

        set(CMAKE_INSTALL_PYTHON "${PYTHON_SITE}/KSeExpr" )

        include_directories(${SIP_INCLUDE_DIR}
                            ${PYQT_SIP_DIR}
                            ${PYTHON_INCLUDE_DIR})

        set(SIP_EXEC_COMMAND_EDITOR ${SIP_EXEC} -w -c .
                           ${PYQT_SIP_FLAGS}
                           -j 1
                           -I. -I${PYQT_SIP_DIR} -I${SIP_INCLUDE_DIR})

        add_custom_command(OUTPUT sipexpreditor2part0.cpp
                           DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/KSeExprUI.sip
                           COMMENT 'Processing KSeExprUI.sip'
                           COMMAND ${SIP_EXEC_COMMAND_EDITOR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/KSeExprUI.sip
                           VERBATIM)

        add_library(expreditor2 SHARED sipexpreditor2part0.cpp)
        target_include_directories(expreditor2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(expreditor2 KSeExprUI ${SEEXPR_LIBRARIES}
                              ${OPENGL_LIBRARY} ${GLUT_LIBRARY} ${PYTHON_LIBRARIES})

        target_link_libraries(expreditor2 KSeExprUI
                                Qt5::Core
                                Qt5::Gui
                                Qt5::Widgets
        )

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
