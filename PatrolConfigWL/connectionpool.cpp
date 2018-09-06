#include "connectionpool.h"
#include <QSettings>

QMutex ConnectionPool::m_mutex;
QMutex ConnectionPool::m_mutexPool;
QWaitCondition ConnectionPool::m_waitConnection;
ConnectionPool* ConnectionPool::m_instance = NULL;
extern const char *g_dbConfigFile;

ConnectionPool::ConnectionPool()
{
	QSettings setting(g_dbConfigFile, QSettings::IniFormat);

    setting.beginGroup("database");

    m_databaseType = setting.value("dbType").toString();
    if(m_databaseType == "MySQL")
    {
        m_databaseType = "QMYSQL";
    }

    m_hostName = setting.value("host").toString();
    m_port = setting.value("port").toString();
    m_databaseName = setting.value("dbname").toString();
    m_userName = setting.value("user").toString();
    m_passwd = setting.value("psw").toString();

    m_maxWaitTime = setting.value("MaxWaitTime").toInt();
    m_waitInterval = setting.value("WaitInterval").toInt();
    m_maxConnectionCount = setting.value("MaxConnectionCount").toInt();

    setting.endGroup();

    m_testOnBorrow = true;
    m_testOnBorrowSql = "select 1";

}

ConnectionPool::~ConnectionPool()
{
}

ConnectionPool& ConnectionPool::getInstance()
{
    if(NULL == m_instance)
    {
        QMutexLocker locker(&m_mutexPool);

        if(NULL == m_instance)
        {
            m_instance = new ConnectionPool();
        }
    }

    return *m_instance;
}

void ConnectionPool::release()
{
    //QMutexLocker locker(&m_mutex);

    delete m_instance;

    m_instance = NULL;
}

QSqlDatabase ConnectionPool::openConnection()
{
    ConnectionPool& pool = ConnectionPool::getInstance();

    QMutexLocker locker(&m_mutex);
    for(int i = pool.m_sqlConnections.size(); i < pool.m_maxConnectionCount; i++)
    {
        DatabaseConnection sqlConnectionTmp;
        sqlConnectionTmp.m_connectionName =
                QString("PatrolConnection_%1").arg(i + 1);
        pool.createConnection(sqlConnectionTmp.m_connectionName);

        pool.m_sqlConnections.insert(sqlConnectionTmp.m_connectionName, sqlConnectionTmp);
    }

    DatabaseConnection sqlConnection;

    int count = 0;
    bool connectionFlag = false;
    while(1)
    {
        foreach(DatabaseConnection sqlConnectionTmp, pool.m_sqlConnections)
        {
            if(!sqlConnectionTmp.m_enable)
            {
                connectionFlag = true;
                sqlConnectionTmp.m_enable = true;
                pool.m_sqlConnections.insert(sqlConnectionTmp.m_connectionName, sqlConnectionTmp);
                sqlConnection = sqlConnectionTmp;
                break;
            }
        }

        if(!connectionFlag/* && count++ < 10*/)
        {
            ///
            locker.unlock();
            if(count == 10)

            locker.relock();
            continue;
        }

        break;
    }

    QSqlDatabase db = QSqlDatabase::database(sqlConnection.m_connectionName);

    if(pool.m_testOnBorrow)
    {
        QSqlQuery query(pool.m_testOnBorrowSql, db);
        if(query.lastError().type() != QSqlError::NoError
                && !db.open())
        {
            printf("error opendatabase\n");
            return db;
        }
    }

    return db;
}

void ConnectionPool::closeConnection(QSqlDatabase &sqlDatabase)
{
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = sqlDatabase.connectionName();
    QMutexLocker locker(&m_mutex);
    QMap<QString, DatabaseConnection>::iterator pdb = pool.m_sqlConnections.find(connectionName);
    if (pdb != pool.m_sqlConnections.end())
    {
        DatabaseConnection databaseConnection;
        databaseConnection.m_enable = false;
        databaseConnection.m_connectionName = connectionName;
        pool.m_sqlConnections.insert(connectionName, databaseConnection);
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName)
{    
    QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseType, connectionName);
    db.setHostName(m_hostName);
    db.setPort(m_port.toInt());
    db.setDatabaseName(m_databaseName);
    db.setUserName(m_userName);
    db.setPassword(m_passwd);

    if(!db.open())
    {

        return QSqlDatabase();
    }

    return db;
}

bool ConnectionPool::execSQL(QString sql, QSqlQuery &query)
{
    bool flag = true;

    QSqlDatabase sqlDatabase = ConnectionPool::getInstance().openConnection();

    QSqlQuery queryTmp(sqlDatabase);

    flag = queryTmp.exec(sql);
    if(!flag)
    {   
    }

    query = queryTmp;

    ConnectionPool::getInstance().closeConnection(sqlDatabase);

    return flag;
}
