#include "QRouteHistory.h"

QRouteHistory::QRouteHistory(QObject *parent)
    : QObject{parent},
    m_maxStackLen(10),
    m_bCanOpera(true)
{

}

void QRouteHistory::push(const QString &entry)
{
    if (m_bCanOpera)
    {
        // 避免写入相同的历史路径
        if (m_pastPath.isEmpty() || m_pastPath.last() != entry) {
            m_pastPath.push_back(entry);
            m_futurePath.clear();
        }
    }

    // 重置操作状态
    setCanOperaState(true);
}

void QRouteHistory::next(QString &entry)
{
    if (m_bCanOpera == false) return;

    if (m_futurePath.size() == 0) return;
    entry = m_futurePath.last();
    m_pastPath.push_back(entry);
    m_futurePath.removeLast();
}

void QRouteHistory::pre(QString &entry)
{
    if (m_bCanOpera == false) return;

    if (m_pastPath.size() == 1) return;
    m_futurePath.push_back(QString(m_pastPath.last()));
    m_pastPath.removeLast();

    entry = m_pastPath.last();
}

void QRouteHistory::current(QString &entry)
{
    entry = m_pastPath.last();
}
