#ifndef QGLOBALVARIABLE_H
#define QGLOBALVARIABLE_H

#include <QObject>
#include <QGlobalStatic>

#include <QRouter.h>

class QGlobalVariables : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QGlobalVariables)
public:
    static QGlobalVariables* instance();

private:
    static QGlobalVariables* m_instance;

    explicit QGlobalVariables(QObject *parent = nullptr);
    ~QGlobalVariables() override = default;

    friend class QGlobalStatic<QGlobalVariables>;

    void init();

// =================== Variables ==================

private:
    QRouter* m_router = nullptr;
public:
    QRouter* getRouter() { return m_router; };

};

#endif // QGLOBALVARIABLE_H
