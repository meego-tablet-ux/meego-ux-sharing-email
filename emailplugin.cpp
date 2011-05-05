/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "emailplugin.h"
#include "meegouxsharingdbustypes.h"

#include <QDebug>
#include <QTranslator>
#include <QLibraryInfo>
#include <QCoreApplication>

EmailPlugin::EmailPlugin(QObject *parent):
        MeeGoUXSharingServicePluginInterface(parent),
        mTranslator(new QTranslator(this)),
        mShareID(0)
{
    //TODO: Maybe hook up to QMF and only make it available if an email account is setup?
}

EmailPlugin::~EmailPlugin()
{
}

bool EmailPlugin::init()
{
    qDebug() << "Email Plugin initialization";

    MeeGoUXSharingServiceInfo info;
    info.serviceName = "email";

    //Email can share *anything!*
    info.serviceTypes << MEEGO_SHARE_TYPE_IMAGE;
    info.serviceTypes << MEEGO_SHARE_TYPE_VIDEO;
    info.serviceTypes << MEEGO_SHARE_TYPE_AUDIO;
    info.serviceTypes << MEEGO_SHARE_TYPE_TEXT;
    info.serviceTypes << MEEGO_SHARE_TYPE_VCARD;
    mServiceInfoList.append(info);
    EmailSharingService *shareSvc = new EmailSharingService(info, this);
    mServices.append(shareSvc);

    connect(shareSvc,
            SIGNAL(ShareProgress(QString,int,int,QString)),
            this,
            SLOT(onServiceShareProgress(QString,int,int,QString)));


    QString catName = QString("%1_%2").arg("meego-ux-sharing-email", QLocale::system().name());
    mTranslator->load(catName, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::installTranslator(mTranslator);

    return true;
}

QHash<QString, QString> EmailPlugin::metaData()
{
    QHash<QString, QString> info;
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_NAME]    = QString("meego-ux-sharing-email");
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_VERSION] = QString("0.2.2");
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_DESC]    = QString("email plugin for meego-ux-sharing");
    return info;
}

QList<MeeGoUXSharingServiceInfo> EmailPlugin::getServices()
{
    return mServiceInfoList;
}

int EmailPlugin::Share(QString serviceName, const QString &sharingType, ArrayOfShareItemStruct items, QString &errmessage)
{
    Q_UNUSED(serviceName);
    int i = -1;
    i = mServices[0]->Share(sharingType, items, errmessage);
    if (i != -1) {
        mShareOpsMap[mShareID] = i;
        return mShareID++;
    } else {
        return -1;
    }
}

bool EmailPlugin::CancelShare(QString serviceName, int opid)
{
    Q_UNUSED(serviceName);
    if (!mShareOpsMap.contains(opid))
        return false;
    return mServices[0]->CancelShare(mShareOpsMap[opid]);
}

//Private slots:

void EmailPlugin::onServiceShareProgress(QString serviceName, int opid, int progress, QString message)
{
    int i, j = -1;
    for (i = 0; i < mShareOpsMap.count(); ++i) {
        if (mShareOpsMap[i] == opid) {
            j = i;
            break;
        }
    }
    if (j == -1) {
        qDebug() << QString("Unmapped opid %1 for service %2 in onServiceShareProgress!").arg(QString::number(opid), serviceName);
        return;
    }
    emit this->ShareProgress(serviceName, j, progress, message);
}



Q_EXPORT_PLUGIN2(EmailPlugin, EmailPlugin);

