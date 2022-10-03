# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020-2021 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

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
            ExprHighlighter.h
            ExprSpecType.h
            Utils.h
    )

    set(EDITOR_CPPS
            BasicExpression.cpp
            ControlSpec.cpp
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
        ecm_install_po_files_as_qm("${CMAKE_SOURCE_DIR}/poqm")

        if (KF5I18n_FOUND)
            add_definitions(-DKSeExpr_HAVE_I18N_FALLBACK_LANGUAGES_DETECTION)
        endif()

        # ecm_create_qm_loader(KSeExprUI_QM_LOADER seexpr2_qt)
        set(KSeExprUI_QM_LOADER "ECMQmLoader-seexpr2_qt.cpp")
        set(EDITOR_CPPS
            ${EDITOR_CPPS}
            ${KSeExprUI_QM_LOADER})
    endif()

    if (NOT WIN32)
        add_library(KSeExprUI SHARED ${EDITOR_CPPS} ${EDITOR_MOC_SRCS}
                    ${editor_parser_cpp})
    else ()
        add_library(KSeExprUI STATIC ${EDITOR_CPPS} ${EDITOR_MOC_SRCS}
                ${editor_parser_cpp})
    endif ()

    set_property(TARGET KSeExprUI PROPERTY VERSION ${KSeExpr_VERSION})
    set_property(TARGET KSeExprUI PROPERTY SOVERSION ${KSeExpr_VERSION_MAJOR})
    set_property(TARGET KSeExprUI PROPERTY
                 INTERFACE_KSeExprUI_MAJOR_VERSION ${KSeExpr_VERSION_MAJOR})
    set_property(TARGET KSeExprUI APPEND PROPERTY
                 COMPATIBLE_INTERFACE_STRING ${KSeExpr_VERSION_MAJOR})

    target_include_directories(KSeExprUI
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        INTERFACE $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(KSeExprUI PUBLIC KSeExpr)

    target_link_libraries(KSeExprUI PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    )

    if (KF5I18n_FOUND)
        target_link_libraries(KSeExprUI PUBLIC KF5::I18n)
    endif()

    ## Install library and includes
    install(TARGETS KSeExprUI EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(FILES
        ${EDITOR_MOC_HDRS}
        ${EDITOR_ADDITIONAL_HEADERS}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/KSeExprUI"
    )
endif()
