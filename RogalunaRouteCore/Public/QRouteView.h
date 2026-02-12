#ifndef QROUTEVIEW_H
#define QROUTEVIEW_H

#include <RogalunaRouteCore_Global.h>
#include <QWidget>

/**
 * @brief The QRouteView class
 * 路由视图控件
 * 创建一个子视图挂载点，传递的路由将解析为视图对象
 */
class ROGALUNAROUTECORE_EXPORT QRouteView : public QWidget
{
    Q_OBJECT
public:
    explicit QRouteView(QWidget* parent = nullptr);
    ~QRouteView();

    /**
     * @brief setWidget
     * @param view
     * 将目标视图设置到容器内
     */
    void setWidget(QWidget* view);

    /**
     * @brief unsetWidget
     * 解除当前视图与容器的关系，以便 GC 处理
     */
    void unsetWidget();

    /**
     * @brief clearWidget
     * 解除当前视图与容器的关系，并立刻删除视图对象
     */
    void clearWidget();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QWidget* m_widget = nullptr;                        // 子视图对象
};

#endif // QROUTEVIEW_H
