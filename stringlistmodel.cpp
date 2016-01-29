#include "stringlistmodel.h"

StringListModel::StringListModel(QStringList data, QObject *parent)
    : QAbstractItemModel(parent)
{

}

StringListModel::~StringListModel()
{

}

QString StringListModel::operator[](int i) const
{
    return QString();
}

QModelIndex StringListModel::index(int row, int column,
                                   const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex StringListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int StringListModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int StringListModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

bool StringListModel::setData(const QModelIndex &index, const QVariant &value,
                              int role)
{
    return false;
}

void StringListModel::append(QString data)
{

}

void StringListModel::append(QStringList data)
{

}

void StringListModel::prepend(QString data)
{

}

void StringListModel::prepend(QStringList data)
{

}

void StringListModel::insert(int position, QString data)
{

}

void StringListModel::insert(int position, QStringList data)
{

}

