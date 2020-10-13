// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#ifndef StringUtils_h
#define StringUtils_h

#include <string>

//! Unescape a few common special characters in the input @p string and return
//! the result as a new one.
inline std::string unescapeString(const std::string& string) {
    std::string output(string);
    int index = 0;
    bool special = false;
    for (char c : string) {
        if (special == true) {
            special = false;
            switch (c) {
                case 'n':   output[index++] = '\n'; break;
                case 'r':   output[index++] = '\r'; break;
                case 't':   output[index++] = '\t'; break;
                case '\\':  output[index++] = '\\'; break;
                case '"':   output[index++] = '\"'; break;
                default:
                    // leave the escape sequence as it was
                    output[index++] = '\\';
                    output[index++] = c;
            }
        } else {
            if (c == '\\') {
                special = true;
            } else {
                output[index++] = c;
            }
        }
    }
    output.resize(index);
    return output;
}

#endif
