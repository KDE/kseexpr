# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

@PACKAGE_INIT@

set(KSeExpr_VERSION @KSeExpr_VERSION@)

set_and_check(KSeExpr_DIR @PACKAGE_CMAKE_INSTALL_PREFIX@)
set_and_check(KSeExpr_LIBRARY_DIRS @PACKAGE_CMAKE_INSTALL_LIBDIR@)
set_and_check(KSeExpr_INCLUDE_DIRS @PACKAGE_CMAKE_INSTALL_INCLUDEDIR@)

include("${CMAKE_CURRENT_LIST_DIR}/KSeExprTargets.cmake")

set(KSeExpr_FOUND TRUE)
