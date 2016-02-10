#include <QObject>
#include <QTest>
#include <QList>
#include <QMetaObject>
#include <QMetaMethod>

#include "stringlistmodel.h"

/**
 * @brief The ListModelFromScratch class
 * test StringListModel to behave as simple one-level
 * list model of QStrings.
 * You need to implement StringListModel yourself.
 * Read http://doc.qt.io/qt-5/qabstractitemmodel.html#details
 * for information about Qt models.
 * You can look at http://doc.qt.io/qt-5/qabstractlistmodel.html
 * to see some concepts of list models.
 * Look at http://doc.qt.io/qt-5/model-view-programming.html
 * for info about Qt's understanding of MVC design pattern.
 */
class ListModelFromScratch : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void direct();
    void index();
    void parent();
    void rowCount();
    void columnCount();
    void data_data();
    void data();
    void setData_data();
    void setData();
    void append();
    void appendList_data();
    void appendList();
    void prepend();
    void prependList_data();
    void prependList();
    void insert_data();
    void insert();
    void insertList_data();
    void insertList();

    void rowCount2();
    void direct2();

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
        DataChangedInfo(int from, int to, QVector<int> roles)
            : from(from),
              to(to),
              roles(roles)
        {

        }

        int from;
        int to;
        QVector<int> roles;
    };

    struct RowsInsertedInfo
    {
        RowsInsertedInfo(int first, int last, bool about)
            : first(first),
              last(last),
              about(about)
        {

        }

        int first;
        int last;
        bool about;
    };

    QStringList m_data;
    StringListModel *m_model;
    QString m_errorString;
    bool m_error;
    QList<DataChangedInfo> m_dataChanged;
    QList<RowsInsertedInfo> m_rowsInserted;
};

void ListModelFromScratch::initTestCase()
{
    //this test must never fail, unless build is broken

    m_error = false;

    m_data << QString::fromLatin1("The");
    m_data << QString::fromLatin1("QCOMPARE");
    m_data << QString::fromLatin1("macro");
    m_data << QString::fromLatin1("compares");
    m_data << QString::fromLatin1("an");
    m_data << QString::fromLatin1("actual");
    m_data << QString::fromLatin1("value");
    m_data << QString::fromLatin1("to");
    m_data << QString::fromLatin1("an");
    m_data << QString::fromLatin1("expected");
    m_data << QString::fromLatin1("value");
    m_data << QString::fromLatin1("using");
    m_data << QString::fromLatin1("the");
    m_data << QString::fromLatin1("equals");
    m_data << QString::fromLatin1("operator");

    m_model = new StringListModel(m_data, this);
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

void ListModelFromScratch::cleanupTestCase()
{
    cleanup();
    m_model->deleteLater();
}

void ListModelFromScratch::cleanup()
{
    // check that no error messages are left by test
    QVERIFY2(!m_error, m_errorString.toStdString().c_str());

    m_error = false;
    m_errorString = QString();
    m_dataChanged.clear();
    m_rowsInserted.clear();
}

void ListModelFromScratch::direct()
{
    for (int i = 0; i < m_data.size(); ++i)
    {
        QCOMPARE((*m_model)[i], m_data[i]);
    }
    QVERIFY(m_dataChanged.isEmpty());
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::index()
{
    {
        QModelIndex index = m_model->index(0);
        QVERIFY(index.isValid());
        QCOMPARE(index.row(), 0);
        QCOMPARE(index.column(), 0);
    }
    {
        QModelIndex index = m_model->index(5);
        QVERIFY(index.isValid());
        QCOMPARE(index.row(), 5);
        QCOMPARE(index.column(), 0);
    }
    QVERIFY(m_dataChanged.isEmpty());
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::parent()
{
    QVERIFY(!m_model->parent(m_model->index(0)).isValid());
    QVERIFY(!m_model->parent(QModelIndex()).isValid());
    QVERIFY(m_dataChanged.isEmpty());
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::rowCount()
{
    QCOMPARE(m_model->rowCount(), m_data.size());
    QCOMPARE(m_model->rowCount(m_model->index(0)), 0);
    QCOMPARE(m_model->rowCount(m_model->index(5)), 0);
    QVERIFY(m_dataChanged.isEmpty());
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::columnCount()
{
    QCOMPARE(m_model->columnCount(), 1);
    QVERIFY(m_dataChanged.isEmpty());
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::data_data()
{
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("value");

    for (int i = 0; i < m_data.size(); ++i)
    {
        QTest::newRow((QString::number(i) + "(" + m_data[i] + ")")
                      .toStdString().c_str()) << i << m_data[i];
    }
}

void ListModelFromScratch::data()
{
    QFETCH(int, position);
    QFETCH(QString, value);

    QCOMPARE(m_model->data(m_model->index(position), Qt::DisplayRole)
             .toString(), value);
    QVERIFY(m_dataChanged.isEmpty());
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::setData_data()
{
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("value");

    QTest::newRow("0") << 0 << QString::fromLatin1("new data");
    QTest::newRow("2") << 2 << QString::fromLatin1("new data");
    QTest::newRow("last") << m_data.size() - 1 << QString::fromLatin1("new data");
}

void ListModelFromScratch::setData()
{
    QFETCH(int, position);
    QFETCH(QString, value);

    QVERIFY(m_model->setData(m_model->index(position), value, Qt::DisplayRole));
    m_data[position] = value;
    // check that dataChanged was emitted
    QCOMPARE(m_dataChanged.size(), 1);
    QCOMPARE(m_dataChanged[0].from, position);
    QCOMPARE(m_dataChanged[0].to, position);
    QCOMPARE(m_dataChanged[0].roles.size(), 1);
    QCOMPARE(m_dataChanged[0].roles[0], (int)Qt::DisplayRole);
    // confirm actual changes
    QCOMPARE((*m_model)[position], value);
    QVERIFY(m_rowsInserted.isEmpty());
}

void ListModelFromScratch::append()
{
    QString str = QString::fromLatin1("appended data");

    m_model->append(str);

    // check that appended to the end
    QCOMPARE(m_model->rowCount(), m_data.size() + 1);
    QCOMPARE((*m_model)[m_data.size()], str);
    // check that rowsAboutToBeInserted and rowsInserted was emitted
    QCOMPARE(m_rowsInserted.size(), 2);
    QCOMPARE(m_rowsInserted[0].about, true);
    QCOMPARE(m_rowsInserted[0].first, m_data.size());
    QCOMPARE(m_rowsInserted[0].last, m_data.size());
    QCOMPARE(m_rowsInserted[1].about, false);
    QCOMPARE(m_rowsInserted[1].first, m_data.size());
    QCOMPARE(m_rowsInserted[1].last, m_data.size());
    QVERIFY(m_dataChanged.isEmpty());

    m_data.append(str);
}

void ListModelFromScratch::appendList_data()
{
    QTest::addColumn<QStringList>("list");

    QTest::newRow("QStringList()") << QStringList();
    QTest::newRow("value1") << QStringList(QString::fromLatin1("value1"));
    QTest::newRow("value1, value2, value3") << (QStringList()
                                                << QString::fromLatin1("value1")
                                                << QString::fromLatin1("value2")
                                                << QString::fromLatin1("value3"));
}

void ListModelFromScratch::appendList()
{
    QFETCH(QStringList, list);

    m_model->append(list);

    // check that appended to the end
    QCOMPARE(m_model->rowCount(), m_data.size() + list.size());
    for (int i = 0; i < list.size(); ++i)
    {
        QCOMPARE((*m_model)[m_data.size() + i], list[i]);
    }
    // check that rowsAboutToBeInserted and rowsInserted was emitted
    if (list.isEmpty())
    {
        QCOMPARE(m_rowsInserted.size(), 0);
    }
    else
    {
        QCOMPARE(m_rowsInserted.size(), 2);
        QCOMPARE(m_rowsInserted[0].about, true);
        QCOMPARE(m_rowsInserted[0].first, m_data.size());
        QCOMPARE(m_rowsInserted[0].last, m_data.size() + list.size() - 1);
        QCOMPARE(m_rowsInserted[1].about, false);
        QCOMPARE(m_rowsInserted[1].first, m_data.size());
        QCOMPARE(m_rowsInserted[1].last, m_data.size() + list.size() - 1);
    }
    QVERIFY(m_dataChanged.isEmpty());

    m_data.append(list);
}

void ListModelFromScratch::prepend()
{
    QString str = QString::fromLatin1("prepended data");

    m_model->prepend(str);

    // check that inserted at 0
    QCOMPARE(m_model->rowCount(), m_data.size() + 1);
    QCOMPARE((*m_model)[0], str);
    // check that rowsAboutToBeInserted and rowsInserted was emitted
    QCOMPARE(m_rowsInserted.size(), 2);
    QCOMPARE(m_rowsInserted[0].about, true);
    QCOMPARE(m_rowsInserted[0].first, 0);
    QCOMPARE(m_rowsInserted[0].last, 0);
    QCOMPARE(m_rowsInserted[1].about, false);
    QCOMPARE(m_rowsInserted[1].first, 0);
    QCOMPARE(m_rowsInserted[1].last, 0);
    QVERIFY(m_dataChanged.isEmpty());

    m_data.prepend(str);
}

void ListModelFromScratch::prependList_data()
{
    QTest::addColumn<QStringList>("list");

    QTest::newRow("QStringList()") << QStringList();
    QTest::newRow("value1") << QStringList(QString::fromLatin1("value1"));
    QTest::newRow("value1, value2, value3") << (QStringList()
                                                << QString::fromLatin1("value1")
                                                << QString::fromLatin1("value2")
                                                << QString::fromLatin1("value3"));
}

void ListModelFromScratch::prependList()
{
    QFETCH(QStringList, list);

    m_model->prepend(list);

    // check that inserted at 0
    QCOMPARE(m_model->rowCount(), m_data.size() + list.size());
    for (int i = 0; i < list.size(); ++i)
    {
        QCOMPARE((*m_model)[i], list[i]);
    }
    // check that rowsAboutToBeInserted and rowsInserted was emitted
    if (list.isEmpty())
    {
        QCOMPARE(m_rowsInserted.size(), 0);
    }
    else
    {
        QCOMPARE(m_rowsInserted.size(), 2);
        QCOMPARE(m_rowsInserted[0].about, true);
        QCOMPARE(m_rowsInserted[0].first, 0);
        QCOMPARE(m_rowsInserted[0].last, list.size() - 1);
        QCOMPARE(m_rowsInserted[1].about, false);
        QCOMPARE(m_rowsInserted[1].first, 0);
        QCOMPARE(m_rowsInserted[1].last, list.size() - 1);
    }
    QVERIFY(m_dataChanged.isEmpty());

    while (!list.isEmpty())
        m_data.prepend(list.takeLast());
}

void ListModelFromScratch::insert_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<int>("position");

    QTest::newRow(QString::number(m_data.size()).toStdString().c_str())
            << QString::fromLatin1("value1") << m_data.size();
    QTest::newRow("0") << QString::fromLatin1("value2") << 0;
    QTest::newRow("4") << QString::fromLatin1("value3") << 4;
    QTest::newRow("7") << QString::fromLatin1("value4") << 7;
}

void ListModelFromScratch::insert()
{
    QFETCH(QString, value);
    QFETCH(int, position);

    m_model->insert(position, value);

    // check that inserted at i
    QCOMPARE(m_model->rowCount(), m_data.size() + 1);
    QCOMPARE((*m_model)[position], value);
    // check that rowsAboutToBeInserted and rowsInserted was emitted
    QCOMPARE(m_rowsInserted.size(), 2);
    QCOMPARE(m_rowsInserted[0].about, true);
    QCOMPARE(m_rowsInserted[0].first, position);
    QCOMPARE(m_rowsInserted[0].last, position);
    QCOMPARE(m_rowsInserted[1].about, false);
    QCOMPARE(m_rowsInserted[1].first, position);
    QCOMPARE(m_rowsInserted[1].last, position);
    QVERIFY(m_dataChanged.isEmpty());

    m_data.insert(position, value);
}

void ListModelFromScratch::insertList_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<int>("position");

    QTest::newRow((QString::number(0)
            + QString::fromLatin1(" QStringList()")).toStdString().c_str())
            << QStringList() << 0;
    QTest::newRow((QString::number(m_data.size())
            + QString::fromLatin1(" value1")).toStdString().c_str())
            << QStringList(QString::fromLatin1("value1")) << m_data.size();
    QTest::newRow((QString::number(0)
            + QString::fromLatin1(" value2")).toStdString().c_str())
            << QStringList(QString::fromLatin1("value2")) << 0;
    QTest::newRow((QString::number(0)
            + QString::fromLatin1(" value3, value4")).toStdString().c_str())
            << (QStringList() << QString::fromLatin1("value3")
                              << QString::fromLatin1("value4")) << 0;
    QTest::newRow((QString::number(4)
            + QString::fromLatin1(" value5")).toStdString().c_str())
            << QStringList(QString::fromLatin1("value5")) << 4;
    QTest::newRow((QString::number(9)
            + QString::fromLatin1(" value6, value7")).toStdString().c_str())
            << (QStringList() << QString::fromLatin1("value6")
                              << QString::fromLatin1("value7")) << 9;
}

void ListModelFromScratch::insertList()
{
    QFETCH(QStringList, list);
    QFETCH(int, position);

    m_model->insert(position, list);

    // check that inserted at i
    QCOMPARE(m_model->rowCount(), m_data.size() + list.size());
    for (int i = 0; i < list.size(); ++i)
    {
        QCOMPARE((*m_model)[position + i], list[i]);
    }
    // check that rowsAboutToBeInserted and rowsInserted was emitted
    if (list.isEmpty())
    {
        QCOMPARE(m_rowsInserted.size(), 0);
    }
    else
    {
        QCOMPARE(m_rowsInserted.size(), 2);
        QCOMPARE(m_rowsInserted[0].about, true);
        QCOMPARE(m_rowsInserted[0].first, position);
        QCOMPARE(m_rowsInserted[0].last, position + list.size() - 1);
        QCOMPARE(m_rowsInserted[1].about, false);
        QCOMPARE(m_rowsInserted[1].first, position);
        QCOMPARE(m_rowsInserted[1].last, position + list.size() - 1);
    }
    QVERIFY(m_dataChanged.isEmpty());

    while (!list.isEmpty())
        m_data.insert(position, list.takeLast());
}

void ListModelFromScratch::rowCount2()
{
    rowCount();
}

void ListModelFromScratch::direct2()
{
    direct();
}

void ListModelFromScratch::onForbiddenSignal()
{
    m_error = true;
    m_errorString = QString::fromLatin1("forbidden signal was emitted: ")
            + QString::fromLatin1(this->sender()->metaObject()->method(
                                      this->senderSignalIndex()).name());
}

void ListModelFromScratch::onDataChanged(const QModelIndex &topLeft,
                   const QModelIndex &bottomRight,
                   const QVector<int> &roles)
{
    if (topLeft.parent().isValid())
    {
        m_error = true;
        m_errorString = QString::fromLatin1(
                    "onDataChanged topLeft.parent() must be invalid");
        return;
    }
    if (bottomRight.parent().isValid())
    {
        m_error = true;
        m_errorString = QString::fromLatin1(
                    "onDataChanged bottomRight.parent() must be invalid");
        return;
    }
    m_dataChanged.append(DataChangedInfo(topLeft.row(), bottomRight.row(),
                                         roles));
}

void ListModelFromScratch::onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    if (parent.isValid())
    {
        m_error = true;
        m_errorString = QString::fromLatin1(
                    "onRowsAboutToBeInserted parent must be invalid");
        return;
    }

    m_rowsInserted.append(RowsInsertedInfo(start, end, true));

    // at this point elements not inserted
    if (m_model->rowCount() != m_data.size())
    {
        m_error = true;
        m_errorString = QString::fromLatin1(
                    "onRowsAboutToBeInserted element was already inserted");
    }
}

void ListModelFromScratch::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    if (parent.isValid())
    {
        m_error = true;
        m_errorString = QString::fromLatin1(
                    "onRowsInserted parent must be invalid");
        return;
    }

    m_rowsInserted.append(RowsInsertedInfo(first, last, false));

    // at this point elements is inserted
    if (m_model->rowCount() != (m_data.size() + last - first + 1))
    {
        m_error = true;
        m_errorString = QString::fromLatin1(
                    "onRowsInserted unexpected rowCount (")
                + QString::number(m_model->rowCount())
                + QString::fromLatin1("), then (")
                + QString::number(m_data.size())
                + QString::fromLatin1(" + ")
                + QString::number(last - first + 1)
                + QString::fromLatin1(") was expected");
    }
}

QTEST_MAIN(ListModelFromScratch)
#include "listmodelfromscratch.moc"
