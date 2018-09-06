#ifndef SQLLITEOPERATOR_H
#define SQLLITEOPERATOR_H

#include "QSqlDatabase"
#include "QSqlQuery"

class SqlliteOperator
{
public:
    SqlliteOperator(QString connectionName = "");
    ~SqlliteOperator();
    bool OpenDBFile(QString filename);        
    void CloseDBFile();
    bool ExecSql(QString sql);
    bool ExecQuerySql(QString sql,QSqlQuery& query);
    QSqlDatabase& GetDB();

    //20161030
    bool OpenMySQL(QString iniFileName);
    bool OpenDataBase();

private:
    QSqlDatabase m_pdbconn;
    QString m_strFileName;

public:
    //20161030
    QString m_connName;
};

#endif // SQLLITEOPERATOR_H
