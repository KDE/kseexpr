# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0


RES=256
for i in *.se; do
    FNAME=`basename $i .se`.png;
    echo Generating $FNAME;
    imageSynth $FNAME $RES $RES $i;
done
