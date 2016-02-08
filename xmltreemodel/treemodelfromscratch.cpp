#include <QObject>
#include <QTest>
#include <QDomDocument>
#include <QList>
#include <QMetaObject>
#include <QMetaMethod>
#include <QFile>
#include <QVariant>

#include "xmltreemodel.h"

/**
 * @brief The TreeModelFromScratch class
 * test XmlTreeModel to behave as xml tree model.
 * Each entry is a tag, that may or may not
 * have children. Model itself should support 3 roles :
 *  TagNameRole - name of xml tag as QString.
 *  CustomIntRole - attribute "int_value" as int.
 *      If not present, or can't be converted to
 *      int, model must return QVariant().
 *  CustomStringRole - attribute "string_value" as QString.
 *      If not present, model must return QVariant().
 *
 * Also TagNameRole = Qt::DisplayRole.
 *
 * So for <div int_value="23" string_value="some data"/>
 *  TagNameRole = QString("div")
 *  CustomIntRole = int(23)
 *  CustomStringRole = QString("some data")
 *
 * Test data has following structure:
 *  <root>
 *      <tag1>
 *          <tag2>
 *          </tag2>
 *      </tag1>
 *      <tag3>
 *      </tag3>
 *  </root>
 * You should show children of root as your first level elements
 * (in this example tag1 and tag3).
 * Note that QDomDocument will show top level tags as it's children
 * (in this example root), so you'll need to use
 * QDomDocument.childNodes().item(0).childNodes() as your first level elements.
 *
 * You need to implement XmlTreeModel yourself.
 * Read http://doc.qt.io/qt-5/qabstractitemmodel.html#details
 * for information about Qt models.
 * Look at http://doc.qt.io/qt-5/model-view-programming.html
 * for info about Qt's understanding of MVC design pattern.
 *
 * This task main difference from "sringlistmodel" is that
 * you'll need to handle parenting hierarchy properly.
 * That is usually done with setting QModelIndex's
 * internalId or most likely internalPointer.
 * internalId and internalPointer are backed up by
 * one field of type quintptr (should be same as std::ptrdiff_t),
 * that can store integer or pointer, but only one at time
 * (you can't set both internalId and internalPointer).
 * You create index with createIndex(row, column, (void *)pointer_to_data_element).
 * Then receiving index you cast your pointer back
 * pointer_to_data_element = (DataElement *)index.internalPointer().
 * You'll need to think what you'll do with invalid indexes by yourself
 * (in many model methods you'll receive invalid index as parent,
 * meaning that you are at first level of your hierarchy).
 * Storing direct pointer to elements of some external data class
 * (not written by you) can be tricky ,and it is so for QDomDocument.
 * So here is some directions:
 *  - xml representation (QDomDocument) is actually a tree of QDomNodePrivate
 *      objects.
 *  - every QDom... class inherits from QDomNode,
 *      that has QDomNodePrivate * as impl.
 *  - QDomNode doesn't allow public access to QDomNodePrivate *,
 *      neither public construction from QDomNodePrivate *.
 *      Both QDomNodePrivate *impl and QDomNode(QDomNodePrivate *)
 *      are protected, hence this problem can be solved by inheritance,
 *      but be very precautious, as QDomNode use reference counting on
 *      it's QDomNodePrivate, so be sure to keep QDomNode's
 *      constructor/destructor cycles untouched by your override.
 *
 */
class TreeModelFromScratch : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void document();
    void index_data();
    void index();
    void parent_data();
    void parent();
    void rowCount_data();
    void rowCount();
    void columnCount_data();
    void columnCount();
    void data_data();
    void data();
    void setData_data();
    void setData();
    void merge_data();
    void merge();

    void document2();

public slots:
    void onForbiddenSignal();
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles);
    void onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
    void onRowsInserted(const QModelIndex &parent, int first, int last);

private:
    struct DataChangedInfo
    {
        DataChangedInfo(QString path, QVector<int> roles)
            : path(path),
              roles(roles)
        {

        }

        bool operator==(const DataChangedInfo &other) const
        {
            if (other.roles.size() != roles.size())
                return false;
            foreach (int role, other.roles)
            {
                if (!roles.contains(role))
                    return false;
            }
            return path == other.path;
        }

        QString path;
        QVector<int> roles;
    };

    struct RowsInsertedInfo
    {
        RowsInsertedInfo(QString parent, bool about)
            : parent(parent),
              about(about)
        {

        }

        bool operator==(const RowsInsertedInfo &other) const
        {
            return parent == other.parent && about == other.about;
        }

        QString parent;
        bool about;
    };

    QDomDocument m_tree;
    XmlTreeModel *m_model;
    QString m_errorString;
    bool m_error;
    QList<DataChangedInfo> m_dataChanged;
    QList<RowsInsertedInfo> m_rowsInserted;
};

void recursiveCompareNodes(QDomNode node1, QDomNode node2, bool *res,
                           QString path)
{
    QVERIFY2(node1.nodeName() == node2.nodeName(),
             (path + " " + node1.nodeName() + " == " +
              node2.nodeName()).toStdString().c_str());

    QVERIFY2(node1.childNodes().size() == node2.childNodes().size(),
             (path + " " + node1.nodeName() + ".childNodes(" +
              QString::number(node1.childNodes().size()) +
              ") == " + node2.nodeName() + ".childNodes(" +
              QString::number(node2.childNodes().size()) +
              ")").toStdString().c_str());

    QVERIFY2(node1.attributes().size() == node2.attributes().size(),
             (path + " " + node1.nodeName() + ".attributes(" +
              QString::number(node1.attributes().size()) +
              ") == " + node2.nodeName() + ".attributes(" +
              QString::number(node2.attributes().size()) +
              ")").toStdString().c_str());

    QDomNamedNodeMap attributes1 = node1.attributes();
    QDomNamedNodeMap attributes2 = node2.attributes();

    for (int i = 0; i < attributes1.size(); ++i)
    {
        QDomNode attr1 = attributes1.item(i);
        QDomNode attr2 = attributes2.namedItem(attr1.nodeName());

        QVERIFY2(!attr2.isNull(),
                 (path + "/" + node1.nodeName() + " doesn't contain \"" +
                  attr1.nodeName() + "\" attribute").toStdString().c_str());

        QVERIFY2(attr1.nodeValue() == attr2.nodeValue(),
                 (path + "/" + node1.nodeName() + ":" + attr1.nodeName() +
                  " (" + attr1.nodeValue() + ") == (" +
                  attr2.nodeValue() + ")").toStdString().c_str());
    }

    QDomNodeList children1 = node1.childNodes();
    QDomNodeList children2 = node2.childNodes();

    for (int i = 0; i < children1.size(); ++i)
    {
        bool child_res = false;
        QDomNode c_node1 = children1.item(i);
        for (int j = 0; j < children2.size(); ++j)
        {
            QDomNode c_node2 = children2.item(j);
            if (c_node1.nodeName() == c_node2.nodeName())
            {
                recursiveCompareNodes(c_node1, c_node2, &child_res,
                                      path + "/" + node1.nodeName());
                if (!child_res)
                    return;
                break;
            }
        }
        if (!child_res)
        {
            QFAIL((path + " \"" + c_node1.nodeName() + "\" node can't be found")
                  .toStdString().c_str());
        }
    }

    *res = true;
}

void recursiveNodeContains(QDomNode node1, QDomNode node2, bool *res,
                           QString path)
{
    QVERIFY2(node1.nodeName() == node2.nodeName(),
             (path + " " + node1.nodeName() + " == " +
              node2.nodeName()).toStdString().c_str());

    QVERIFY2(node1.childNodes().size() >= node2.childNodes().size(),
             (path + " " + node1.nodeName() + ".childNodes(" +
              QString::number(node1.childNodes().size()) +
              ") == " + node2.nodeName() + ".childNodes(" +
              QString::number(node2.childNodes().size()) +
              ")").toStdString().c_str());

    QVERIFY2(node1.attributes().size() >= node2.attributes().size(),
             (path + " " + node1.nodeName() + ".attributes(" +
              QString::number(node1.attributes().size()) +
              ") == " + node2.nodeName() + ".attributes(" +
              QString::number(node2.attributes().size()) +
              ")").toStdString().c_str());

    QDomNamedNodeMap attributes1 = node1.attributes();
    QDomNamedNodeMap attributes2 = node2.attributes();

    for (int i = 0; i < attributes2.size(); ++i)
    {
        QDomNode attr2 = attributes2.item(i);
        QDomNode attr1 = attributes1.namedItem(attr2.nodeName());

        QVERIFY2(!attr1.isNull(),
                 (path + "/" + node1.nodeName() + " doesn't contain \"" +
                  attr2.nodeName() + "\" attribute").toStdString().c_str());

        QVERIFY2(attr1.nodeValue() == attr2.nodeValue(),
                 (path + "/" + node1.nodeName() + ":" + attr1.nodeName() +
                  " (" + attr1.nodeValue() + ") == (" +
                  attr2.nodeValue() + ")").toStdString().c_str());
    }

    QDomNodeList children1 = node1.childNodes();
    QDomNodeList children2 = node2.childNodes();

    for (int i = 0; i < children2.size(); ++i)
    {
        bool child_res = false;
        QDomNode c_node2 = children2.item(i);
        for (int j = 0; j < children1.size(); ++j)
        {
            QDomNode c_node1 = children1.item(j);
            if (c_node1.nodeName() == c_node2.nodeName())
            {
                recursiveNodeContains(c_node1, c_node2, &child_res,
                                      path + "/" + node1.nodeName());
                if (!child_res)
                    return;
                break;
            }
        }
        if (!child_res)
        {
            QFAIL((path + " \"" + c_node2.nodeName() + "\" node can't be found")
                  .toStdString().c_str());
        }
    }

    *res = true;
}

void recursiveMergeNodes(QDomNode node1, QDomNode node2, QString path)
{
    QVERIFY2(node1.nodeName() == node2.nodeName(),
             (path + " " + node1.nodeName() + " == " +
              node2.nodeName()).toStdString().c_str());

    QDomNamedNodeMap attributes1 = node1.attributes();
    QDomNamedNodeMap attributes2 = node2.attributes();

    for (int i = 0; i < attributes2.size(); ++i)
    {
        QDomNode attr_node = attributes1.namedItem(
                    attributes2.item(i).nodeName());
        if (attr_node.isNull())
            attributes1.setNamedItem(attributes2.item(i).cloneNode());
        else
            attr_node.setNodeValue(attributes2.item(i).nodeValue());
    }

    QDomNodeList children1 = node1.childNodes();
    QDomNodeList children2 = node2.childNodes();

    for (int i = 0; i < children2.size(); ++i)
    {
        QDomNode c_node2 = children2.item(i);
        bool insert = true;
        for (int j = 0; j < children1.size(); ++j)
        {
            QDomNode c_node1 = children1.item(j);
            if (c_node1.nodeName() == c_node2.nodeName())
            {
                recursiveMergeNodes(c_node1, c_node2,
                                      path + "/" + node1.nodeName());
                insert = false;
                continue;
            }
        }
        if (insert)
            node1.appendChild(c_node2.cloneNode());
    }
}

bool compareDocuments(QDomDocument doc1, QDomDocument doc2)
{
    bool res = false;
    recursiveCompareNodes(doc1, doc2, &res, QString(""));
    return res;
}

bool containsElements(QDomDocument doc1, QDomDocument doc2)
{
    bool res = false;
    recursiveNodeContains(doc1, doc2, &res, QString(""));
    return res;
}

void mergeDocuments(QDomDocument doc1, QDomDocument doc2)
{
    recursiveMergeNodes(doc1, doc2, QString(""));
}

void updateAttrs(QDomNode node, int role, QString value, QDomDocument doc)
{
    if (XmlTreeModel::TagNameRole == role)
    {
        node.toElement().setTagName(value);
        return;
    }

    QString attr_name;
    if (XmlTreeModel::CustomIntRole == role)
    {
        attr_name = QString::fromLatin1("int_value");
    }
    else if (XmlTreeModel::CustomStringRole == role)
    {
        attr_name = QString::fromLatin1("string_value");
    }

    if (attr_name.isNull())
        return;
    QDomNode attr = node.attributes().namedItem(attr_name);
    if (attr.isNull())
    {
        attr = doc.createAttribute(attr_name);
        attr.setNodeValue(value);
        node.attributes().setNamedItem(attr);
    }
    else
    {
        attr.setNodeValue(value);
    }
}

void TreeModelFromScratch::initTestCase()
{
    m_error = false;

    QFile xml_in(":/xml/simple");
    xml_in.open(QIODevice::ReadOnly);
    m_tree.setContent(&xml_in);
    xml_in.close();

    QVERIFY(!m_tree.isNull());

    m_model = new XmlTreeModel(QDomDocument(m_tree.cloneNode().toDocument()), this);
    QVERIFY(connect(m_model, SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(columnsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(columnsInserted(QModelIndex,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(columnsRemoved(QModelIndex,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(modelAboutToBeReset()),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(modelReset()),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
                    this, SLOT(onForbiddenSignal())));
    QVERIFY(connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                    this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>))));
    QVERIFY(connect(m_model, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
                    this, SLOT(onRowsAboutToBeInserted(QModelIndex,int,int))));
    QVERIFY(connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
                    this, SLOT(onRowsInserted(QModelIndex,int,int))));
}

void TreeModelFromScratch::cleanupTestCase()
{
    cleanup();
    m_model->deleteLater();
}

void TreeModelFromScratch::cleanup()
{
    QVERIFY2(!m_error, m_errorString.toStdString().c_str());
    m_error = false;
    m_errorString = QString();
    m_dataChanged.clear();
    m_rowsInserted.clear();
}

void TreeModelFromScratch::document()
{
    QDomDocument doc = m_model->document();
    QVERIFY(!doc.isNull());
    compareDocuments(doc, m_tree);
}

void TreeModelFromScratch::index_data()
{
    QTest::addColumn<QString>("path");

    QTest::newRow("one_level") << "/simple";
    QTest::newRow("two_level") << "/with_children/child1";
    QTest::newRow("three_level") << "/node1/node2/node3";
    QTest::newRow("four_level") << "/node1/node2/node3/node4";
}

void TreeModelFromScratch::index()
{
    QFETCH(QString, path);

    QDomNode node = m_tree.childNodes().item(0);
    QStringList node_names = path.split("/", QString::SkipEmptyParts);
    QString cur_path("");
    while (!node_names.isEmpty())
    {
        QString cur_name = node_names.takeFirst();
        QModelIndex index;
        QDomNodeList children = node.childNodes();
        int i;
        for (i = 0; i < children.size(); ++i)
        {
            QDomNode tmp = children.item(i);
            if (tmp.nodeName() == cur_name)
            {
                node = tmp;
                index = m_model->index(i, 0, index);
                cur_path += QString::fromLatin1("/") + cur_name;
                break;
            }
        }
        QVERIFY2(i < children.size(), (QString::fromLatin1("can't find ") + cur_path)
                 .toStdString().c_str());
        QCOMPARE(index.row(), i);
        QCOMPARE(index.column(), 0);
    }
    QCOMPARE(cur_path, path);
}

void TreeModelFromScratch::parent_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("has_parent");

    QTest::newRow("one_level") << "/simple" << false;
    QTest::newRow("two_level") << "/with_children/child1" << true;
}

void TreeModelFromScratch::parent()
{
    QFETCH(QString, path);
    QFETCH(bool, has_parent);

    QDomNode node = m_tree.childNodes().item(0);
    QStringList node_names = path.split("/", QString::SkipEmptyParts);
    QModelIndex index;
    QList<int> indexes;
    while (!node_names.isEmpty())
    {
        QString cur_name = node_names.takeFirst();
        QDomNodeList children = node.childNodes();
        int i;
        for (i = 0; i < children.size(); ++i)
        {
            QDomNode tmp = children.item(i);
            if (tmp.nodeName() == cur_name)
            {
                node = tmp;
                index = m_model->index(i, 0, index);
                indexes.append(i);
                break;
            }
        }
    }
    QVERIFY(index.isValid());
    QVERIFY(!indexes.isEmpty());
    QCOMPARE(index.row(), indexes.last());
    QCOMPARE(index.parent().isValid(), has_parent);
    if (has_parent)
    {
        QVERIFY(indexes.size() >= 2);
        QCOMPARE(index.parent().row(), indexes[indexes.size() - 2]);
    }
}

void TreeModelFromScratch::rowCount_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("children");

    QTest::newRow("root") << "/" << 7;
    QTest::newRow("simple") << "/simple" << 0;
    QTest::newRow("with_children") << "/with_children" << 3;
    QTest::newRow("three_level") << "/node1/node2/node3" << 1;
}

void TreeModelFromScratch::rowCount()
{
    QFETCH(QString, path);
    QFETCH(int, children);

    QDomNode node = m_tree.childNodes().item(0);
    QStringList node_names = path.split("/", QString::SkipEmptyParts);
    QModelIndex index;
    while (!node_names.isEmpty())
    {
        QString cur_name = node_names.takeFirst();
        QDomNodeList children = node.childNodes();
        int i;
        for (i = 0; i < children.size(); ++i)
        {
            QDomNode tmp = children.item(i);
            if (tmp.nodeName() == cur_name)
            {
                node = tmp;
                index = m_model->index(i, 0, index);
                break;
            }
        }
    }

    if (path == "/")
    {
        QVERIFY(!index.isValid());
    }
    else
    {
        QVERIFY(index.isValid());
    }

    QCOMPARE(m_model->rowCount(index), children);
}

void TreeModelFromScratch::columnCount_data()
{
    QTest::addColumn<QString>("path");

    QTest::newRow("root") << "/";
    QTest::newRow("simple") << "/simple";
    QTest::newRow("with_children") << "/with_children";
    QTest::newRow("three_level") << "/node1/node2/node3";
}

void TreeModelFromScratch::columnCount()
{
    QFETCH(QString, path);

    QDomNode node = m_tree.childNodes().item(0);
    QStringList node_names = path.split("/", QString::SkipEmptyParts);
    QModelIndex index;
    while (!node_names.isEmpty())
    {
        QString cur_name = node_names.takeFirst();
        QDomNodeList children = node.childNodes();
        int i;
        for (i = 0; i < children.size(); ++i)
        {
            QDomNode tmp = children.item(i);
            if (tmp.nodeName() == cur_name)
            {
                node = tmp;
                index = m_model->index(i, 0, index);
                break;
            }
        }
    }

    if (path == "/")
    {
        QVERIFY(!index.isValid());
    }
    else
    {
        QVERIFY(index.isValid());
    }

    QCOMPARE(m_model->columnCount(index), 1);
}

void TreeModelFromScratch::data_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("role");
    QTest::addColumn<QVariant>("value");

    QTest::newRow("simple_tag") << "/simple" << (int) XmlTreeModel::TagNameRole
                                << QVariant(QString::fromLatin1("simple"));
    QTest::newRow("simple_dis") << "/simple" << (int) Qt::DisplayRole
                                << QVariant(QString::fromLatin1("simple"));
    QTest::newRow("simple_int") << "/simple" << (int) XmlTreeModel::CustomIntRole
                                << QVariant();
    QTest::newRow("simple_str") << "/simple" << (int) XmlTreeModel::CustomStringRole
                                << QVariant();
    QTest::newRow("simple_inv") << "/simple" << (int) Qt::EditRole
                                << QVariant();

    QTest::newRow("with_attributes_tag") << "/with_attributes"
                                         << (int) XmlTreeModel::TagNameRole
                                         << QVariant(QString::fromLatin1("with_attributes"));
    QTest::newRow("with_attributes_int") << "/with_attributes"
                                         << (int) XmlTreeModel::CustomIntRole
                                         << QVariant(1);
    QTest::newRow("with_attributes_str") << "/with_attributes"
                                         << (int) XmlTreeModel::CustomStringRole
                                         << QVariant(QString::fromLatin1("string value"));

    QTest::newRow("int_attribute_tag") << "/int_attribute"
                                       << (int) XmlTreeModel::TagNameRole
                                       << QVariant(QString::fromLatin1("int_attribute"));
    QTest::newRow("int_attribute_int") << "/int_attribute"
                                       << (int) XmlTreeModel::CustomIntRole
                                       << QVariant(43);
    QTest::newRow("int_attribute_str") << "/int_attribute"
                                       << (int) XmlTreeModel::CustomStringRole
                                       << QVariant();

    QTest::newRow("string_attribute_tag") << "/string_attribute"
                                          << (int) XmlTreeModel::TagNameRole
                                          << QVariant(QString::fromLatin1("string_attribute"));
    QTest::newRow("string_attribute_int") << "/string_attribute"
                                          << (int) XmlTreeModel::CustomIntRole
                                          << QVariant();
    QTest::newRow("string_attribute_str") << "/string_attribute"
                                          << (int) XmlTreeModel::CustomStringRole
                                          << QVariant(QString::fromLatin1("some data here"));

    QTest::newRow("node4_tag") << "/node1/node2/node3/node4"
                               << (int) XmlTreeModel::TagNameRole
                               << QVariant(QString::fromLatin1("node4"));
    QTest::newRow("node4_int") << "/node1/node2/node3/node4"
                               << (int) XmlTreeModel::CustomIntRole
                               << QVariant(2);
    QTest::newRow("node4_str") << "/node1/node2/node3/node4"
                               << (int) XmlTreeModel::CustomStringRole
                               << QVariant(QString::fromLatin1(""));
    QTest::newRow("node4_inv") << "/node1/node2/node3/node4"
                               << (int) (Qt::UserRole + 100)
                               << QVariant();

    QTest::newRow("wrong_attributes_tag") << "/node5/wrong_attributes"
                                          << (int) XmlTreeModel::TagNameRole
                                          << QVariant(QString::fromLatin1("wrong_attributes"));
    QTest::newRow("wrong_attributes_int") << "/node5/wrong_attributes"
                                          << (int) XmlTreeModel::CustomIntRole
                                          << QVariant();
    QTest::newRow("wrong_attributes_str") << "/node5/wrong_attributes"
                                          << (int) XmlTreeModel::CustomStringRole
                                          << QVariant();

    QTest::newRow("int_attribute_malformed_tag") << "/node5/int_attribute_malformed"
                                                 << (int) XmlTreeModel::TagNameRole
                                                 << QVariant(QString::fromLatin1("int_attribute_malformed"));
    QTest::newRow("int_attribute_malformed_int") << "/node5/int_attribute_malformed"
                                                 << (int) XmlTreeModel::CustomIntRole
                                                 << QVariant();
    QTest::newRow("int_attribute_malformed_str") << "/node5/int_attribute_malformed"
                                                 << (int) XmlTreeModel::CustomStringRole
                                                 << QVariant();
}

void TreeModelFromScratch::data()
{
    QFETCH(QString, path);
    QFETCH(int, role);
    QFETCH(QVariant, value);

    QDomNode node = m_tree.childNodes().item(0);
    QStringList node_names = path.split("/", QString::SkipEmptyParts);
    QModelIndex index;
    while (!node_names.isEmpty())
    {
        QString cur_name = node_names.takeFirst();
        QDomNodeList children = node.childNodes();
        int i;
        for (i = 0; i < children.size(); ++i)
        {
            QDomNode tmp = children.item(i);
            if (tmp.nodeName() == cur_name)
            {
                node = tmp;
                index = m_model->index(i, 0, index);
                break;
            }
        }
    }
    QVERIFY(index.isValid());
    if (value.isNull())
    {
        QVERIFY(index.data(role).isNull());
    }
    else
    {
        QVariant res = index.data(role);
        QVERIFY(!res.isNull());
        QCOMPARE(res.type(), value.type());
        QCOMPARE(res, value);
    }
}

void TreeModelFromScratch::setData_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("role");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("success");
    QTest::addColumn<QString>("res_path");

    QTest::newRow("simple_int") << "/simple" << (int) XmlTreeModel::CustomIntRole
                                << QVariant(1) << true << "/simple";
    QTest::newRow("simple_str") << "/simple" << (int) XmlTreeModel::CustomStringRole
                                << QVariant("new string") << true << "/simple";

    QTest::newRow("with_attributes_int") << "/with_attributes"
                                         << (int) XmlTreeModel::CustomIntRole
                                         << QVariant("three")
                                         << false
                                         << "/with_attributes";
    QTest::newRow("with_attributes_str") << "/with_attributes"
                                         << (int) XmlTreeModel::CustomStringRole
                                         << QVariant(QString::fromLatin1("string value"))
                                         << true
                                         << "/with_attributes";

    QTest::newRow("node4_tag") << "/node1/node2/node3/node4"
                               << (int) XmlTreeModel::TagNameRole
                               << QVariant(QString::fromLatin1("renamed_node"))
                               << true
                               << "/node1/node2/node3/renamed_node";
}

void TreeModelFromScratch::setData()
{
    QFETCH(QString, path);
    QFETCH(int, role);
    QFETCH(QVariant, value);
    QFETCH(bool, success);
    QFETCH(QString, res_path);

    QDomNode node = m_tree.childNodes().item(0);
    QStringList node_names = path.split("/", QString::SkipEmptyParts);
    QModelIndex index;
    while (!node_names.isEmpty())
    {
        QString cur_name = node_names.takeFirst();
        QDomNodeList children = node.childNodes();
        int i;
        for (i = 0; i < children.size(); ++i)
        {
            QDomNode tmp = children.item(i);
            if (tmp.nodeName() == cur_name)
            {
                node = tmp;
                index = m_model->index(i, 0, index);
                break;
            }
        }
    }
    QVERIFY(index.isValid());
    QVERIFY(!node.isNull());
    QVERIFY(node != m_tree.childNodes().item(0));
    QCOMPARE(m_model->setData(index, value, role), success);
    if (success)
    {
        QCOMPARE(m_dataChanged.size(), 1);
        QCOMPARE(m_dataChanged[0].path, res_path);
        QCOMPARE(m_dataChanged[0].roles.size(), 1);
        QCOMPARE(m_dataChanged[0].roles[0], role);

        updateAttrs(node, role, value.toString(), m_tree);
    }
    else
    {
        QVERIFY(m_dataChanged.isEmpty());
    }
}

void TreeModelFromScratch::merge_data()
{
    typedef QList<QPair<QString, QVector<int> > > ChangesType;

    QTest::addColumn<QString>("path");
    QTest::addColumn<QStringList>("insertions");
    QTest::addColumn<ChangesType>("changes");

    QTest::newRow("insertion") << QString(":/xml/insertion")
                               << (QStringList()
                                   << QString("/")
                                   << QString("/node1/node2/node3")
                                   << QString("/node1/node2/node3/parent_inserted_node"))
                               << QList<QPair<QString, QVector<int> > >();
    {
        QList<QPair<QString, QVector<int> > > changes;
        changes.append(QPair<QString, QVector<int> >(
                           QString("/with_attributes"),
                           QVector<int>() << XmlTreeModel::CustomIntRole));
        changes.append(QPair<QString, QVector<int> >(
                           QString("/node1/node2/node3"),
                           QVector<int>() << XmlTreeModel::CustomStringRole));
        QTest::newRow("modification") << QString(":/xml/modification")
                                      << QStringList()
                                      << changes;
    }
    {
        QList<QPair<QString, QVector<int> > > changes;
        changes.append(QPair<QString, QVector<int> >(
                           QString("/with_children/child1"),
                           QVector<int>() << XmlTreeModel::CustomIntRole));
        QTest::newRow("insertion_modification") << QString(":/xml/insertion_modification")
                                                << (QStringList()
                                                    << QString("/with_children/child3"))
                                                << changes;
    }
}

void TreeModelFromScratch::merge()
{
    typedef QList<QPair<QString, QVector<int> > > ChangesType;
    typedef QPair<QString, QVector<int> > ChangeType;

    QFETCH(QString, path);
    QFETCH(QStringList, insertions);
    QFETCH(ChangesType, changes);

    QDomDocument doc;
    QFile xml_in(path);
    xml_in.open(QIODevice::ReadOnly);
    doc.setContent(&xml_in);
    xml_in.close();

    m_model->merge(doc.cloneNode().toDocument());
    QVERIFY(containsElements(m_model->document(), doc));
    QCOMPARE(m_rowsInserted.size(), insertions.size() * 2);
    foreach (QString insertion, insertions)
    {
        QVERIFY(m_rowsInserted.contains(RowsInsertedInfo(insertion, true)));
        QVERIFY(m_rowsInserted.contains(RowsInsertedInfo(insertion, false)));
    }
    QCOMPARE(m_dataChanged.size(), changes.size());
    foreach (ChangeType change, changes)
    {
        QVERIFY(m_dataChanged.contains(DataChangedInfo(change.first, change.second)));
    }
    mergeDocuments(m_tree, doc);
}

void TreeModelFromScratch::document2()
{
    {
        QFile xml_out("/Users/anton/Temporary/test.xml");
        xml_out.open(QIODevice::WriteOnly);
        xml_out.write(m_tree.toString().toLatin1());
        xml_out.close();
    }
    {
        QFile xml_out("/Users/anton/Temporary/model.xml");
        xml_out.open(QIODevice::WriteOnly);
        xml_out.write(m_model->document().toString().toLatin1());
        xml_out.close();
    }

    document();
}

void TreeModelFromScratch::onForbiddenSignal()
{
    m_error = true;
    m_errorString = QString::fromLatin1("forbidden signal was emitted: ")
            + QString::fromLatin1(this->sender()->metaObject()->method(
                                      this->senderSignalIndex()).name());
}

void TreeModelFromScratch::onDataChanged(const QModelIndex &topLeft,
                                         const QModelIndex &bottomRight,
                                         const QVector<int> &roles)
{
    QModelIndex index = topLeft;
    QString path;
    while (index.isValid())
    {
        path = QString::fromLatin1("/") +
                index.data(XmlTreeModel::TagNameRole).toString() + path;
        index = index.parent();
    }

    m_dataChanged.append(DataChangedInfo(path, roles));

    Q_UNUSED(bottomRight);
}

void TreeModelFromScratch::onRowsAboutToBeInserted(const QModelIndex &parent,
                                                   int start, int end)
{
    QModelIndex index = parent;
    QString path;
    if (index.isValid())
    {
        while (index.isValid())
        {
            path = QString::fromLatin1("/") +
                    index.data(XmlTreeModel::TagNameRole).toString() + path;
            index = index.parent();
        }
    }
    else
    {
        path = QString::fromLatin1("/");
    }

    m_rowsInserted.append(RowsInsertedInfo(path, true));

    Q_UNUSED(start);
    Q_UNUSED(end);
}

void TreeModelFromScratch::onRowsInserted(const QModelIndex &parent,
                                          int first, int last)
{
    QModelIndex index = parent;
    QString path;
    if (index.isValid())
    {
        while (index.isValid())
        {
            path = QString::fromLatin1("/") +
                    index.data(XmlTreeModel::TagNameRole).toString() + path;
            index = index.parent();
        }
    }
    else
    {
        path = QString::fromLatin1("/");
    }

    m_rowsInserted.append(RowsInsertedInfo(path, false));

    Q_UNUSED(first);
    Q_UNUSED(last);
}

QTEST_MAIN(TreeModelFromScratch)
#include "treemodelfromscratch.moc"
