# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0

if (NOT PNG_DIR)
    find_package(PNG QUIET)
endif()
if (PNG_FOUND)
    include_directories(SYSTEM ${PNG_INCLUDE_DIR})

    add_executable(imageSynth2 "imageSynth.cpp")
    target_link_libraries(imageSynth2 ${PNG_LIBRARIES} ${SEEXPR_LIBRARIES})
    install(TARGETS imageSynth2 DESTINATION bin)

    add_executable(imageSynthPaint3d2 "imageSynthForPaint3d.cpp")
    target_link_libraries(imageSynthPaint3d2 ${PNG_LIBRARIES} ${SEEXPR_LIBRARIES})
    install(TARGETS imageSynthPaint3d2 DESTINATION bin)
    install(DIRECTORY examples DESTINATION share/KSeExpr/demos/imageSynth
            USE_SOURCE_PERMISSIONS)
endif()
