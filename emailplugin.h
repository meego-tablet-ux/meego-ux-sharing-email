/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <meegouxsharingserviceplugininterface.h>
#include <QObject>
#include <QStringList>
#include <meegouxsharingservice.h>

#include "emailsharingservice.h"

class QTranslator;

class EmailPlugin : public MeeGoUXSharingServicePluginInterface
{
    Q_OBJECT
    Q_INTERFACES(MeeGoUXSharingServicePluginInterface)

public:
    EmailPlugin(QObject *parent = 0);
    ~EmailPlugin();
    bool init();
    QHash<QString, QString> metaData();
    QList<MeeGoUXSharingServiceInfo> getServices();

    int Share(QString serviceName, const QString &sharingType, ArrayOfShareItemStruct items, QString &errmessage);
    bool CancelShare(QString serviceName, int opid);

signals:
    void ServiceChanged(QString serviceName);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

private slots:
    void onServiceShareProgress(QString serviceName, int opid, int progress, QString message);

private:
    QList<MeeGoUXSharingServiceInfo> mServiceInfoList;
    QList<EmailSharingService *> mServices;
    QTranslator *mTranslator;
    int mShareID;
    QMap<int, int> mShareOpsMap;

};
