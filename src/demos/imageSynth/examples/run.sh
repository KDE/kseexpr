# SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
# SPDX-License-Identifier: LicenseRef-Apache-2.0
# SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
# SPDX-License-Identifier: GPL-3.0-or-later


RES=256
for i in *.se; do
    FNAME=`basename $i .se`.png;
    echo Generating $FNAME;
    imageSynth $FNAME $RES $RES $i;
done
