/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "emailsharingservice.h"

#include <QFile>
#include <QProcess>

#include <meegouxsharingcommon.h>

EmailSharingService::EmailSharingService(MeeGoUXSharingServiceInfo serviceInfo,
                                   QObject *parent) :
    MeeGoUXSharingService(serviceInfo, parent),
    mShareID(0)
{
    qDebug() << QString("Starting up EmailSharingService with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
}

EmailSharingService::~EmailSharingService()
{
}

bool EmailSharingService::CanShareType(const QString &sharetype)
{
    Q_UNUSED(sharetype);
    //Email can share *anything*!
    return true;
}

bool EmailSharingService::CancelShare(int opid)
{
    Q_UNUSED(opid);
    qDebug() << QString("Cannot cancel email sharing uploads!");
    return false;
}

uint EmailSharingService::GetCredsState()
{
    return CredsStateValid;
}

QString EmailSharingService::GetDisplayName()
{
    return tr("MeeGo Email");
}

QString EmailSharingService::GetIconPath()
{
    return QString();
}

bool EmailSharingService::GetServiceAvailable()
{
    return true;
}

QString EmailSharingService::GetServiceDesc()
{
    return tr("Sharing via Email");
}

QString EmailSharingService::GetServiceName()
{
    return mServiceInfo.serviceName;
}

QString EmailSharingService::GetServiceStateText()
{
    return QString();
}

QString EmailSharingService::GetServiceType()
{
    return tr("Email");
}

QString EmailSharingService::GetSettingsURI(const QString &platform,
                                       const QString &product)
{
    //TODO: put in actual settings launcher method once it exists...
    return QString("exec echo \"Launching settings for service %1, on platform %2, product %3\"").arg(mServiceInfo.serviceName, platform, product);
}

QString EmailSharingService::GetUIName(const QString &widgettype,
                                  const QString &platform,
                                  const QString &product,
                                  const QString &sharetype,
                                  uint sharecount)
{

    QString type;
    QString mult;
    if (widgettype == "QML") {
        if (sharetype == MEEGO_SHARE_TYPE_IMAGE) {
            type = "image";
        } else if (sharetype == MEEGO_SHARE_TYPE_VIDEO) {
            type = "video";
        } else if (sharetype == MEEGO_SHARE_TYPE_AUDIO) {
            type = "audio";
        } else if (sharetype == MEEGO_SHARE_TYPE_TEXT) {
            type = "text";
        } else if (sharetype == MEEGO_SHARE_TYPE_VCARD) {
            type = "vcard";
        } else {
            type = QString(sharetype).replace(QString("/"), QString("_"));   //Custom type support
        }

        //Even though this plugin itself only provides a single standard email.qml,
        //we can allow platform/product customization just by placing additional
        //qml files into the email directory...
        mult = (sharecount > 1 ? "multi" : "single");
        QString filename = QString("%1/%2/%3_%4_%5_%6_%7").arg(QML_TARGET_BASE_PATH, "email", mServiceInfo.serviceName, platform, product, type, mult);
        //If we don't have a file for this product/platform
        if (!QFile::exists(filename + ".qml")) {
            //Try just this product
            filename = QString("%1/%2/%3_%4_%5_%6").arg(QML_TARGET_BASE_PATH, "email", mServiceInfo.serviceName, platform, type, mult);
            if (!QFile::exists(filename + ".qml")) {
                //Try email_$type_$sharecount
                filename = QString("%1/%2/%3_%4_%5").arg(QML_TARGET_BASE_PATH, "email", mServiceInfo.serviceName, type, mult);
                if (!QFile::exists(filename + ".qml")) {
                    //Try email_$platform
                    filename = QString("%1/%2/%3_%4").arg(QML_TARGET_BASE_PATH, "email", mServiceInfo.serviceName, platform);
                    if (!QFile::exists(filename + ".qml")) {
                        //We *know* this one exists, as it's provided w/ the plugin...
                        filename = QString("%1/%2/%3").arg(QML_TARGET_BASE_PATH, "email", mServiceInfo.serviceName);
                    }
                }
            }
        }
        return filename;

    } else {
        //Handle other UI types here...
        return QString();
    }
}

int EmailSharingService::Share(const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage)
{
    //We should never actually hit this, currently, as the qml file actually invokes the
    //command-line to bring up an email compose window w/ attachments
    qDebug() << "Got to Share in meego-ux-sharing-email - shouldn't have! This is only a stub!";
    return -1;
    if (!CanShareType(sharetype)) {
        errmessage = QString("Invalid share type %1!").arg(sharetype);
	return -1;
    }
    int i = -1;
    if (items.count() < 1) {
        errmessage = "No items to share!";
        return -1;
    }

    foreach(ShareItemStruct sis, items) {
        qDebug() << QString("Received file %1!").arg(sis.shareURI);
        qDebug() << sis.params;

        if (sharetype == MEEGO_SHARE_TYPE_IMAGE) {
        } else if (sharetype == MEEGO_SHARE_TYPE_VIDEO) {
        } else if (sharetype == MEEGO_SHARE_TYPE_AUDIO) {
        }
        if (i != -1) {
            mShareOpsMap[mShareID].append(i);
            mShareOpProgressMap[mShareID].append(QPair<int, int>(i, 0));
        }
    }

    return mShareID++;
}


