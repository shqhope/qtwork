#include "terminalinfodll.h"

TerminalInfoDLL::TerminalInfoDLL(QSqlDatabase db) :
    m_db(db)
{
}

void TerminalInfoDLL::GetTerminals(QList<Terminal> & tList)
{
    if(tList.count() > 0)
        tList.clear();
    QSqlQuery query(m_db);
    if(!query.exec("select * from terminalinfo"))
    {
        qDebug() << QString("select terminalinfo error");
        qDebug() << query.lastError();
        return;
    }
    while(query.next())
    {
        Terminal ter;
        ter.m_tId = query.record().value("T_ID").toInt();
        ter.m_tName = query.record().value("T_Name").toString();
        tList.append(ter);
    }
}

bool TerminalInfoDLL::GetTerminalsPort(QList<Terminal> &tList, InfoPort *pPortInfo)
{
    if (pPortInfo == NULL || tList.count() <= 0)
        return false;

    Terminal *pTerminal = NULL;

    for (int i = 0; i < pPortInfo->m_CountNum; i++)
    {
        if (pTerminal == NULL || pTerminal->m_tId != pPortInfo[i].m_nDevID)
        {
            for (int j = 0; j < tList.count(); j++)
            {
                pTerminal = &tList[j];
                if (pPortInfo[i].m_nDevID == pTerminal->m_tId)
                    break;
            }
        }
        if (pTerminal && pPortInfo[i].m_nDevID == pTerminal->m_tId)
        {
            pTerminal->m_tPorts.append((pPortInfo+i));
        }
    }

    return true;
}

void TerminalInfoDLL::GetGradeColor(QHash<int, QRgb> &gradeColor)
{
    if (gradeColor.count() > 0)
        gradeColor.clear();
    QSqlQuery query(m_db);
    query.exec("select id, alarmColor from alarm_grade");
    while (query.next())
    {
        gradeColor[query.record().value(0).toInt()] = QRgb(query.record().value(1).toUInt());
    }
}
