#ifndef STRINGLISTMODEL_H
#define STRINGLISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

/**
 * @brief The StringListModel class
 * http://doc.qt.io/qt-5/qabstractitemmodel.html#details
 */
class StringListModel : public QAbstractItemModel
{
public:
    explicit StringListModel(QStringList data, QObject *parent = NULL);
    ~StringListModel();

    /* direct access to data (needed for test).
     * You don't want to provide such kind of methods
     * in your real models.
     */
    QString operator[](int i) const;

    // QAbstractItemModel interface.
public:
    /* Here you create indexes that point to
     * elements of your data.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#details
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#createIndex-1
     */
    QModelIndex index(int row, int column = 0,
                      const QModelIndex &parent = QModelIndex()) const;
    /* Find parent for element.
     * In our model parent allways QModelIndex().
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#parent
     */
    QModelIndex parent(const QModelIndex &child) const;
    /* Number of rows.
     * In our model number of rows is equal size of data.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#rowCount
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /* Number of collumns.
     * In our model allways = 1.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#columnCount
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    /* Get data for some role.
     * In our model Qt::DisplayRole is only used.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#data
     */
    QVariant data(const QModelIndex &index, int role) const;
    /* Set data for some role.
     * In our model Qt::DisplayRole is only used as role
     * and QString as type of value.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#setData
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /* Handle elements appending.
     * You must emit rowsAboutToBeInserted(), append data and
     * emit rowsInserted() in right order.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#beginInsertRows
     * You don't need to overrite insertRows() in this example,
     * only implement following slots.
     */
public slots:
    void append(QString data);
    void append(QStringList data);
    void prepend(QString data);
    void prepend(QStringList data);
    /* insert at position.
     * ex. [val1, val2] -> insert(1, val3) -> [val1, val3, val2]
     * position == 0 -> prepend
     * position == rowCount -> append
     */
    void insert(int position, QString data);
    void insert(int position, QStringList data);
};

#endif // STRINGLISTMODEL_H
