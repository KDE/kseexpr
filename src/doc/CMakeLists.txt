# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(Doxygen)
if (DOXYGEN_FOUND AND NOT WIN32)
    configure_file("Doxyfile.in" "Doxyfile" @ONLY)
    file(GLOB (DOCUMENTED_FILES ../KSeExpr/*.h))

    set(DOXYGEN ${DOXYGEN_EXECUTABLE})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen.txt
        COMMAND ${DOXYGEN} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile >doxygen.txt
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS Doxyfile.in)

    add_custom_target(doc ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen.txt)

    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/" DESTINATION ${DOC_DIR})
    install(FILES userdoc.txt DESTINATION ${DOC_DIR} RENAME SeExpressions.html)

    file(GLOB "*.png" PNG_FILES)
    install(FILES ${PNG_FILES} DESTINATION ${DOC_DIR})
endif()
