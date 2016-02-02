#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>

/**
 * @brief The StringListModel class
 * http://doc.qt.io/qt-5/qabstractitemmodel.html#details
 * Don't show "root" element (QDomDocument.childNodes().item(0)),
 * your first level elements are QDomDocument.childNodes().item(0).childNodes().
 */
class XmlTreeModel : public QAbstractItemModel
{
public:
    enum Roles
    {
        // return QString - tag name
        TagNameRole = Qt::DisplayRole,
        // return int - attribute "int_value"
        CustomIntRole = Qt::UserRole,
        // return QString - attribute "string_value"
        CustomStringRole
    };

public:
    // You wanna keep tree as member and only bridge all calls to it
    explicit XmlTreeModel(QDomDocument tree, QObject *parent = NULL);
    ~XmlTreeModel();

    /* Direct access to your data needed by test.
     * You don't want to provide such kind of methods
     * in your real models.
     */
    QDomDocument document() const;

    // QAbstractItemModel interface
public:
    /* Here you create indexes that point to
     * elements of your data.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#details
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#createIndex-1
     */
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    /* Find parent for element.
     * QModelIndex() for first level and valid index for other.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#parent
     */
    QModelIndex parent(const QModelIndex &child) const;
    /* Number of rows.
     * Will depend on parent obviously.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#rowCount
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /* Number of collumns.
     * In our model allways = 1.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#columnCount
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    /* Get data for some role.
     * In our model only TagNameRole, CustomIntRole, CustomStringRole are used.
     * Btw TagNameRole = Qt::DisplayRole, but you need to check only for
     * TagNameRole obviously.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#data
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    /* Set data for some role.
     * http://doc.qt.io/qt-5/qabstractitemmodel.html#setData
     */
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::DisplayRole);
};

#endif // XMLTREEMODEL_H
