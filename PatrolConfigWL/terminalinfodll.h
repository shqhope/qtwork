#ifndef TERMINALINFODLL_H
#define TERMINALINFODLL_H

#include "Common.h"

class Terminal              // 单个设备对象
{
public:
    Terminal():m_tId(0),m_tName("") {}
    int m_tId;
    QString m_tName;
	QList<InfoPort *> m_tPorts;
};

class TerminalInfoDLL       // 设备统计类
{
public:
    TerminalInfoDLL(QSqlDatabase db);

    void GetTerminals(QList<Terminal> &);

	bool GetTerminalsPort(QList<Terminal> &tList, InfoPort *pPortInfo);

    void GetGradeColor(QHash<int, QRgb> &gradeColor);

private:

    QSqlDatabase m_db;
};

#endif // TERMINALINFODLL_H
