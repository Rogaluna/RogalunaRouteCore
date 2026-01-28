#ifndef QROUTEHISTORY_H
#define QROUTEHISTORY_H

#include <QObject>
#include <QVariantMap>
#include <Define.h>

/**
 * @brief 路由历史记录类
 * - 类中仅保存路由路径，页面元数据在其他地方进行保存、处理
 * - 路由历史仅保存路由成功的路径
 */
class QRouteHistory : public QObject
{
    Q_OBJECT

    friend class QRouter;
public:
    explicit QRouteHistory(QObject *parent = nullptr);

    bool getCanOperaState() const { return m_bCanOpera; };
    void setCanOperaState(bool state) { m_bCanOpera = state; };

private:
    /**
     * @brief push 将路径写入过去队列中，写入新的路由路径后，前向页面路由会清空
     * @param entry
     */
    void push(const QString& entry);

    /**
     * @brief next 将顶部的过去页面路由取出
     * @param entry
     */
    void next(QString& entry);

    /**
     * @brief pre 将顶部的可向前路由取出
     * @param entry
     */
    void pre(QString& entry);


private:
    QVector<QString> m_pastPath;    // 过去经过的页面路由
    QVector<QString> m_futurePath;    // 能够向前的页面路由

    int m_maxStackLen;  // 栈最大长度，历史记录能够存储的大小，默认为 20
    uint8_t m_bCanOpera : 1; // 标记是否可以操作历史栈
};

#endif // QROUTEHISTORY_H
