#include "xmltreemodel.h"


XmlTreeModel::XmlTreeModel(QDomDocument tree, QObject *parent)
    : QAbstractItemModel(parent)
{

}

XmlTreeModel::~XmlTreeModel()
{

}

QDomDocument XmlTreeModel::document() const
{
    return QDomDocument();
}

QModelIndex XmlTreeModel::index(int row, int column,
                                const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex XmlTreeModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int XmlTreeModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int XmlTreeModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant XmlTreeModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

bool XmlTreeModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    return false;
}
