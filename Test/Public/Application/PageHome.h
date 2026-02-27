#ifndef PAGEHOME_H
#define PAGEHOME_H

#include <QAbstractItemModel>
#include <QWidget>

#include <Interface/IRoutable.h>

struct HomeTreeNode
{
    quint32 id; // 节点 ID
    QString name; // 节点名称
    QList<HomeTreeNode*> children;
    HomeTreeNode* parent = nullptr;

    explicit HomeTreeNode(quint32 id, const QString& name) : id(id), name(name) {}

    ~HomeTreeNode()
    {
        qDeleteAll(children);
    }

    HomeTreeNode* addChild(HomeTreeNode* child)
    {
        if (child) {
            child->parent = this;
            children.append(child);
        }
        return this;
    }
};

class HomeTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit HomeTreeModel(HomeTreeNode* root, QObject* parent = nullptr);
    ~HomeTreeModel();

    // 必须实现的纯虚函数
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 工具函数：从 QModelIndex 获取 HomeTreeNode*
    HomeTreeNode* nodeFromIndex(const QModelIndex &index) const;

private:
    HomeTreeNode* m_rootNode;
};

namespace Ui {
class PageHome;
}

class PageHome : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)
public:
    explicit PageHome(QWidget *parent = nullptr);
    ~PageHome();

private:
    void init();

public:
    virtual void mounted() override;
    virtual void unmounted() override;

protected:
    virtual QRouteView* routeViews() override;

private slots:
    void onCurrentTreeViewChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::PageHome *ui;

    HomeTreeNode* m_rootNode = nullptr;
    HomeTreeModel* m_treeModel = nullptr;
};

#endif // PAGEHOME_H
