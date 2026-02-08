#include "QGlobalVariable.h"

QGlobalVariables *QGlobalVariables::m_instance = nullptr;

QGlobalVariables *QGlobalVariables::instance()
{
    if (!m_instance) m_instance = new QGlobalVariables;

    return m_instance;
}

QGlobalVariables::QGlobalVariables(QObject *parent)
    : QObject{parent}
{
    init();
}

void QGlobalVariables::init()
{
    m_router = new QRouter;
}
