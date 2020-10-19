// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#if 0
header

    QTextBrowser* helpBrowser;

constructor

    QWidget* browserspace = new QWidget(tab);
    helpBrowser = new QTextBrowser(browserspace);
    tab->addTab(browserspace, "Help");
    char * path = getenv("RP_KSeExpr");
    if (path)
        helpBrowser->setSource(QString("%1/share/doc/KSeExpr/SeExpressions.html").arg(path));

    QPushButton* backPb = new QPushButton("Back");
    backPb->setEnabled(false);
    QPushButton* forwardPb = new QPushButton("Forward");
    forwardPb->setEnabled(false);

    QVBoxLayout * helpLayout = new QVBoxLayout(browserspace);
    QHBoxLayout * helpPbLayout = new QHBoxLayout;
    helpLayout->addLayout(helpPbLayout);
    helpPbLayout->addWidget(backPb);
    helpPbLayout->addWidget(forwardPb);
    helpPbLayout->addItem(new QSpacerItem(0,0, QSizePolicy::MinimumExpanding,
                QSizePolicy::Minimum));
    helpLayout->addWidget(helpBrowser, /*stretch*/ true);
    helpBrowser->setMinimumHeight(120);

    // wire up help browser forward/back buttons
    connect(backPb, SIGNAL(clicked()), helpBrowser, SLOT(backward()));
    connect(forwardPb, SIGNAL(clicked()), helpBrowser, SLOT(forward()));
    connect(helpBrowser, SIGNAL(backwardAvailable(bool)), backPb, SLOT(setEnabled(bool)));
    connect(helpBrowser, SIGNAL(forwardAvailable(bool)), forwardPb, SLOT(setEnabled(bool)));
#endif
