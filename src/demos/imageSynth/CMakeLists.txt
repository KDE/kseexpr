# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

if (PNG_FOUND)
    add_executable(imageSynth2 "imageSynth.cpp")
    target_include_directories(imageSynth2 PRIVATE ${PNG_INCLUDE_DIR})
    target_link_libraries(imageSynth2 ${PNG_LIBRARIES} ${SEEXPR_LIBRARIES})
    install(TARGETS imageSynth2 DESTINATION bin)

    add_executable(imageSynthPaint3d2 "imageSynthForPaint3d.cpp")
    target_include_directories(imageSynthPaint3d2 PRIVATE ${PNG_INCLUDE_DIR})
    target_link_libraries(imageSynthPaint3d2 ${PNG_LIBRARIES} ${SEEXPR_LIBRARIES})
    install(TARGETS imageSynthPaint3d2 DESTINATION bin)
    install(DIRECTORY examples DESTINATION share/KSeExpr/demos/imageSynth
            USE_SOURCE_PERMISSIONS)
endif()
