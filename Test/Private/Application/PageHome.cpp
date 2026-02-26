#include "Application/PageHome.h"
#include "./ui_PageHome.h"

#include <QGlobalVariable.h>

#include <QDebug>

HomeTreeModel::HomeTreeModel(HomeTreeNode *root, QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootNode(root)
{

}

HomeTreeModel::~HomeTreeModel()
{

}

QModelIndex HomeTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    HomeTreeNode* parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return QModelIndex();

    if (row < parentNode->children.size())
    {
        HomeTreeNode* childNode = parentNode->children.at(row);
        return createIndex(row, column, childNode); // internalPointer = childNode
    }

    return QModelIndex();
}

QModelIndex HomeTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    HomeTreeNode* childNode = static_cast<HomeTreeNode*>(child.internalPointer());
    if (!childNode || childNode == m_rootNode)
        return QModelIndex();

    HomeTreeNode* parentNode = childNode->parent;
    if (!parentNode || parentNode == m_rootNode)
        return QModelIndex();

    // 找到 parentNode 在其父节点中的行号
    HomeTreeNode* grandParent = parentNode->parent;
    if (!grandParent)
        return createIndex(0, 0, parentNode); // 根的直接子节点

    int row = grandParent->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int HomeTreeModel::rowCount(const QModelIndex &parent) const
{
    HomeTreeNode* parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return 0;
    return parentNode->children.size();
}

int HomeTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1; // 单列
}

QVariant HomeTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        HomeTreeNode* node = static_cast<HomeTreeNode*>(index.internalPointer());
        return node ? node->name : QVariant();
    }

    return QVariant();
}

HomeTreeNode *HomeTreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootNode;
    return static_cast<HomeTreeNode*>(index.internalPointer());
}

PageHome::PageHome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageHome)
{
    ui->setupUi(this);

    init();
}

PageHome::~PageHome()
{
    delete ui;
}

void PageHome::init()
{
    // 1. 构建节点树
    m_rootNode = new HomeTreeNode(10000, "Root");
    auto* qrouterNode = new HomeTreeNode(10100, "QRouter");
    m_rootNode->addChild(qrouterNode);
        qrouterNode->addChild(new HomeTreeNode(10101, QStringLiteral("概述")));

    // 2. 创建模型
    m_treeModel = new HomeTreeModel(m_rootNode, this);
    ui->treeView->setModel(m_treeModel);

    // 3. 隐藏 header（列名）
    ui->treeView->header()->hide();

    // 4. 连接 currentChanged 信号
    connect(ui->treeView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &PageHome::onCurrentTreeViewChanged);

    // 5. 展开所有（可选）
    ui->treeView->expandAll();
}

void PageHome::mounted()
{
    qDebug() << __FUNCTION__ << "Home page";

}

void PageHome::unmounted()
{
    qDebug() << __FUNCTION__ << "Home page";
}

QRouteView *PageHome::routeViews()
{
    return ui->container;
}

void PageHome::onCurrentTreeViewChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    HomeTreeNode* node = m_treeModel->nodeFromIndex(current);
    if (node)
    {
        qDebug() << "Selected node:" << node->name << ", id:" << node->id;

        switch(node->id)
        {
        case 10100: // QRouter
        case 10101:
            {
                QGlobalVariables::instance()->getRouter()->push("manual?id=10101", this);
            }
            break;
        }
    }
}


