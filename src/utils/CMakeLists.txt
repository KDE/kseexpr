# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

include_directories(${CMAKE_BINARY_DIR}/src/KSeExpr)

foreach(item eval listVar)
    add_executable("${item}" "${item}.cpp")
    target_link_libraries("${item}" ${SEEXPR_LIBRARIES})
    install(TARGETS "${item}" DESTINATION share/KSeExpr/utils)
endforeach()
