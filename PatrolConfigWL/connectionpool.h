#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QtSql>

enum DatabaseType
{
    QSQLITE = 0,
    QMYSQL,
    QDB2,
    QIBASE,
    QOCI,
    QODBC,
    QPSQL,
    QSQLITE2,
    QTDS
};

class DatabaseConnection
{
public:
    DatabaseConnection()
    {
        m_enable = false;
    }

public:
    QString m_connectionName;
    bool m_enable;
};

class ConnectionPool
{
public:
    void release();  //关闭所有的数据库连接
    QSqlDatabase openConnection();  //获取数据库连接
    void closeConnection(QSqlDatabase &sqlDatabase);  //释放数据库连接回连接池
    static bool execSQL(QString sql, QSqlQuery &query);

private:
    static ConnectionPool& getInstance();

    ConnectionPool();
    ConnectionPool(const ConnectionPool& other);
    ConnectionPool& operator =(const ConnectionPool &other);
    ~ConnectionPool();
    QSqlDatabase createConnection(const QString& connectionName);

    //数据库信息
    QString m_hostName;
    QString m_port;
    QString m_databaseName;
    QString m_userName;
    QString m_passwd;
    QString m_databaseType;

    bool m_testOnBorrow;
    QString m_testOnBorrowSql;

    int m_maxWaitTime;  //获取连接最大等待时间
    int m_waitInterval;  //尝试获取连接时等待间隔时间
    int m_maxConnectionCount;  //最大连接数

    static QMutex m_mutex;
    static QMutex m_mutexPool;
    static QWaitCondition m_waitConnection;
    static ConnectionPool* m_instance;

    QMap<QString, DatabaseConnection> m_sqlConnections;
};

#endif // CONNECTIONPOOL_H
