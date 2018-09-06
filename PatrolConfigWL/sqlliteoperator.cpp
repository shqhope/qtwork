#include "sqlliteoperator.h"
#include <QMessageBox>
#include <QVariant>
#include <QSettings>
#include <QtSql>

extern const char *g_dbConfigFile;

SqlliteOperator::SqlliteOperator(QString connectionName)
{
    if(connectionName == "")
    {
       m_pdbconn = QSqlDatabase::addDatabase("QSQLITE");
    }
    else
    {
        m_pdbconn = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    }
}

SqlliteOperator::~SqlliteOperator()
{
    CloseDBFile();
}

QSqlDatabase& SqlliteOperator::GetDB()
{
    return m_pdbconn;
}

bool SqlliteOperator::OpenDBFile(QString filename)
{
    if(m_pdbconn.isOpen())
    {
        return false;
    }
    else
    {
        m_pdbconn.setDatabaseName(filename);
        if(m_pdbconn.open())
        {
            m_strFileName = filename;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void SqlliteOperator::CloseDBFile()
{
    if(m_pdbconn.isOpen())
    {
        m_pdbconn.close();
    }
}

bool SqlliteOperator::ExecSql(QString sql)
{
    QSqlQuery querytmp(m_pdbconn);
    querytmp.exec(sql);

    if(querytmp.isActive()&&querytmp.isSelect())
    {
        return true;
    }
    else
    {
        OpenDataBase();
        return false;
    }
}

bool SqlliteOperator::ExecQuerySql(QString sql,QSqlQuery& query)
{
    QSqlQuery querytmp(m_pdbconn);
    querytmp.exec(sql);
    if(querytmp.isActive()&&querytmp.isSelect())
    {
        query = querytmp;
        return true;
    }
    else
    {
        OpenDataBase();
        return false;
    }
}

bool SqlliteOperator::OpenDataBase()
{
    bool flag = false;

	QSettings setting(g_dbConfigFile,QSettings::IniFormat);
    setting.beginGroup("database");

    QString dbType = setting.value("dbType","SQLite").toString();

    QString dbname = setting.value("dbname","zhy630").toString();


    if(dbType == "MySQL")
    {
        flag = OpenMySQL(dbname);
    }
    else //SQLite
    {
        flag = OpenDBFile(dbname);
    }

    return flag;
}

bool SqlliteOperator::OpenMySQL(QString iniFileName)
{
    if(m_pdbconn.isOpen())
    {
        return false;
    }
    else
    {
        QString m_host,m_port,m_dbName,m_user,m_passwd;
		QSettings setting(g_dbConfigFile,QSettings::IniFormat);
        setting.beginGroup("database");
        m_host = setting.value("host","localhost").toString();
        m_port = setting.value("port","3306").toString();
        m_dbName = setting.value("dbname","zhy630").toString();
        m_user = setting.value("user","root").toString();
        m_passwd = setting.value("psw","root").toString();

        if(m_connName.isEmpty())
        {
        m_pdbconn = QSqlDatabase::addDatabase("QMYSQL","ShareMemInit");
        }
        else
        {
            m_pdbconn = QSqlDatabase::addDatabase("QMYSQL", m_connName);
        }

        m_pdbconn.setHostName(m_host);
        m_pdbconn.setPort(m_port.toInt());
        m_pdbconn.setDatabaseName(m_dbName);
        m_pdbconn.setUserName(m_user);
        m_pdbconn.setPassword(m_passwd);

        if(m_pdbconn.open())
        {
            m_strFileName = iniFileName;
            return true;
        }
        else
        {
            QSqlError err = m_pdbconn.lastError();
            m_pdbconn.close();
            QMessageBox::critical(NULL,QObject::trUtf8("提示"),err.text());

            return false;
        }
    }
}
