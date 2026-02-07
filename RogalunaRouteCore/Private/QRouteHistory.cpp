#include "QRouteHistory.h"

QRouteHistory::QRouteHistory(QObject *parent)
    : QObject{parent},
    m_maxStackLen(10),
    m_bCanOpera(true)
{

}

void QRouteHistory::push(const QString &entry)
{
    m_pastPath.push_back(entry);
    m_futurePath.clear();
}

void QRouteHistory::next(QString &entry)
{
    if (m_bCanOpera == false) return;

    if (m_futurePath.size() == 0) return;
    QString historyPath = QString();
    historyPath = m_futurePath.last();
    m_pastPath.push_back(historyPath);
    m_futurePath.removeLast();
}

void QRouteHistory::pre(QString &entry)
{
    if (m_bCanOpera == false) return;

    if (m_pastPath.size() == 0) return;
    QString historyPath = QString();
    historyPath = m_pastPath.last();
    m_futurePath.push_back(historyPath);
    m_pastPath.removeLast();
}
