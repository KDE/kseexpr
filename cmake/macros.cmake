# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later

macro(localize_path SRC DST)
  file(READ 
    ${SRC}
    _TMP_FILE
  )
  string(REPLACE
    "@@PATH@@"
    "${CMAKE_SOURCE_DIR}"
    _TMP_FILE_2
    "${_TMP_FILE}"
  )
  file(GENERATE
    OUTPUT ${DST}
    CONTENT "${_TMP_FILE_2}"
  )
endmacro()

macro(BuildParserScanner TGT FLEX_L_PREFIX BISON_Y_PREFIX PARSER_PREFIX GENERATED_CPPS)
  if(USE_PREGENERATED_FILES OR
      NOT BISON_FOUND OR
      NOT FLEX_FOUND OR
      (SED_EXE STREQUAL "SED_EXE-NOTFOUND")
  )
    message(STATUS "Using pregenerated parser files for ${TGT}")
    # don't have flex/bison/sed, use pregenerated versions
    localize_path(
      ${CMAKE_SOURCE_DIR}/generated/${TGT}/${FLEX_L_PREFIX}.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/${FLEX_L_PREFIX}.cpp
    )
    localize_path(
      ${CMAKE_SOURCE_DIR}/generated/${TGT}/${FLEX_L_PREFIX}In.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/${FLEX_L_PREFIX}In.cpp
    )
    localize_path(
      ${CMAKE_SOURCE_DIR}/generated/${TGT}/${BISON_Y_PREFIX}.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/${BISON_Y_PREFIX}.cpp
    )
    localize_path(
      ${CMAKE_SOURCE_DIR}/generated/${TGT}/${BISON_Y_PREFIX}.tab.h
      ${CMAKE_CURRENT_BINARY_DIR}/${BISON_Y_PREFIX}.tab.h
    )

    set (${GENERATED_CPPS} ${FLEX_L_PREFIX}.cpp ${BISON_Y_PREFIX}.cpp)
  else ()
    ## build the parser from the flex/yacc sources
    
    ADD_CUSTOM_COMMAND(
      SOURCE "${FLEX_L_PREFIX}.l"
      COMMAND "flex"
      ARGS "-o${FLEX_L_PREFIX}In.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${FLEX_L_PREFIX}.l"
      OUTPUT ${FLEX_L_PREFIX}In.cpp
      DEPENDS ${FLEX_L_PREFIX}.l
      )
    
    ADD_CUSTOM_COMMAND(
      SOURCE "${FLEX_L_PREFIX}In.cpp"
      COMMAND "sed"
      ARGS -e "'s/${PARSER_PREFIX}wrap(n)/${PARSER_PREFIX}wrap()/g'" -e "'s/yy/${PARSER_PREFIX}/g'" -e "'s/YY/${PARSER_PREFIX}YY/g'"  ${FLEX_L_PREFIX}In.cpp | tee ${FLEX_L_PREFIX}.cpp ${CMAKE_CURRENT_SOURCE_DIR}/generated/${FLEX_L_PREFIX}.cpp > /dev/null
      OUTPUT ${FLEX_L_PREFIX}.cpp
      DEPENDS ${FLEX_L_PREFIX}In.cpp
      )
    
    ADD_CUSTOM_COMMAND(
      SOURCE "${BISON_Y_PREFIX}.y"
      COMMAND "bison"
      ARGS "--defines" "--verbose" "--fixed-output-files" "-p" "${PARSER_PREFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${BISON_Y_PREFIX}.y"
      OUTPUT y.tab.c y.tab.h
      DEPENDS ${BISON_Y_PREFIX}.y
      )
    
    ADD_CUSTOM_COMMAND(
      SOURCE "y.tab.h"
      COMMAND "sed"
      ARGS -e "'s/yy/${PARSER_PREFIX}/g'" -e "'s/YY/${PARSER_PREFIX}YY/g'" y.tab.h | tee  ${BISON_Y_PREFIX}.tab.h ${CMAKE_CURRENT_SOURCE_DIR}/generated/${BISON_Y_PREFIX}.tab.h > /dev/null
      OUTPUT ${BISON_Y_PREFIX}.tab.h
      DEPENDS y.tab.h
      )
    
    ADD_CUSTOM_COMMAND(
      SOURCE "y.tab.c"
      COMMAND "sed"
      ARGS -e "'s/yy/${PARSER_PREFIX}/g'" -e "'s/YY/${PARSER_PREFIX}YY/g'" y.tab.c | tee ${BISON_Y_PREFIX}.cpp  "${CMAKE_CURRENT_SOURCE_DIR}/generated/${BISON_Y_PREFIX}.cpp" > /dev/null
      OUTPUT ${BISON_Y_PREFIX}.cpp
      DEPENDS y.tab.c ${BISON_Y_PREFIX}.tab.h
      )
    
    ## set build files
    set (${GENERATED_CPPS} ${FLEX_L_PREFIX}.cpp ${BISON_Y_PREFIX}.cpp)
    message(STATUS "${TGT} parser files: ${${GENERATED_CPPS}}")
  endif()
endmacro()
