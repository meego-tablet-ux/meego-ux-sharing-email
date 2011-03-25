/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef EMAILSHARINGSERVICE_H
#define EMAILSHARINGSERVICE_H

#include <QObject>
#include <QMetaType>
#include <meegouxsharingservice.h>
#include <meegouxsharingcommon.h>

class EmailSharingService : public MeeGoUXSharingService
{
    Q_OBJECT
public:
    explicit EmailSharingService(MeeGoUXSharingServiceInfo serviceInfo, QObject *parent = 0);
    ~EmailSharingService();

public slots:
    //For ServiceAdaptor auto-invoking
    bool CanShareType(const QString &sharetype);
    bool CancelShare(int opid);
    uint GetCredsState();
    QString GetDisplayName();
    QString GetIconPath();
    bool GetServiceAvailable();
    QString GetServiceDesc();
    QString GetServiceName();
    QString GetServiceStateText();
    QString GetServiceType();
    QString GetSettingsURI(const QString &platform, const QString &product);
    QString GetUIName(const QString &widgettype, const QString &platform, const QString &product, const QString &sharetype, uint sharecount);
    int Share(const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage);

signals:
    //For ServiceAdaptor auto-connecting
    void ServiceChanged(const QString &service);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

private:
    QMap<int, QList<int> > mShareOpsMap;
    int mShareID;
    QMap<int, QList<QPair<int, int> > > mShareOpProgressMap;

};

Q_DECLARE_METATYPE(EmailSharingService *)

#endif // EMAILSHARINGSERVICE_H
