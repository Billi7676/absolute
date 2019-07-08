// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2014-2019 The Dash Core developers
// Copyright (c) 2018-2019 The Absolute Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/absolute-config.h"
#endif

#include "chainparams.h"
#include "key.h"
#include "rpcnestedtests.h"
#include "util.h"
#include "uritests.h"
#include "compattests.h"
#include "trafficgraphdatatests.h"

#ifdef ENABLE_WALLET
#include "paymentservertests.h"
#include "wallettests.h"
#endif

#include <QApplication>
#include <QObject>
#include <QTest>

#include <openssl/ssl.h>

#if defined(QT_STATICPLUGIN)
#include <QtPlugin>
#if defined(QT_QPA_PLATFORM_MINIMAL)
Q_IMPORT_PLUGIN(QMinimalIntegrationPlugin);
#endif
#if defined(QT_QPA_PLATFORM_XCB)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_WINDOWS)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_COCOA)
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#endif
#endif

extern void noui_connect();
// This is all you need to run all the tests
int main(int argc, char *argv[])
{
    ECC_Start();
    SetupEnvironment();
    SetupNetworking();
    SelectParams(CBaseChainParams::MAIN);
    noui_connect();
    bool fInvalid = false;

    // Don't remove this, it's needed to access
    // QApplication:: and QCoreApplication:: in the tests
    QApplication app(argc, argv);
    app.setApplicationName("Absolute-Qt-test");

    SSL_library_init();

    URITests test1;
    if (QTest::qExec(&test1) != 0)
        fInvalid = true;
#ifdef ENABLE_WALLET
    PaymentServerTests test2;
    if (QTest::qExec(&test2) != 0)
        fInvalid = true;
#endif
    RPCNestedTests test3;
    if (QTest::qExec(&test3) != 0)
        fInvalid = true;
    CompatTests test4;
    if (QTest::qExec(&test4) != 0)
        fInvalid = true;

    TrafficGraphDataTests test5;
    if (QTest::qExec(&test5) != 0)
        fInvalid = true;
    ECC_Stop();

    return fInvalid;
}
