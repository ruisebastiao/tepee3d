#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QThread>
#include "ServiceInterface.h"
#include "manageBDD.h"
#include "DatabaseServiceUserInterface.h"

#define SERVICE_ID 2
#define SERVICE_VERSION "1.0.0"

class DatabaseThread : public QThread, Services::ServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(Services::ServiceInterface)
    Q_PLUGIN_METADATA(IID "com.tepee3d.Services.databaseservice")

public:
    DatabaseThread();
    ~DatabaseThread();
    void        run();
    void        stop();

    int               getServiceId() const;
    void            initLibraryConnection(QObject *parent);
    bool            connectServiceToUser(QObject *user);
    bool            disconnectServiceFromUser(QObject *user);
    QString      getServiceVersion() const;
    QObject*    getLibraryQObject();

private slots:
    void        transmitSQLResult(QList<QSqlRecord> result, QObject *receiver, int, void *data);
signals:
    void        executeSQLQuery(const QString& query, QObject *sender, int, const QString &, void *);
    void        threadInitialized();
};

#endif // DATABASETHREAD_H
