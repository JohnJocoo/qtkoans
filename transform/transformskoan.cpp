#include <QObject>
#include <QTest>

#include "transformations.h"
#include "types.hpp"

/**
 * This koan is pretty different from other koans,
 * as you will be asked to do some tasks not using
 * Qt classes that are able to do it, as I hope it
 * will help you understand basics better.
 * First of all you will need to read some on affine
 * transformations, and it's representation in
 * linear algebra.
 * Much of this task was written as a follow-up to this exact lecture
 * https://www.cs.utexas.edu/~fussell/courses/cs384g/lectures/affine.pdf
 * by Don Fussell (fussell@cs.utexas.edu). So it might be
 * great material to start with.
 */
class Transforms : public QObject
{
    Q_OBJECT

private slots:
    void transformPoint2_data();
    void transformPoint2();
    void transformPoint3_data();
    void transformPoint3();
    void transformPointImpl2_data();
    void transformPointImpl2();
    void transformPointImpl3_data();
    void transformPointImpl3();
    void identity2();
    void identity3();
    void scaling_data();
    void scaling();
    void reflection_data();
    void reflection();
    void shearing_data();
    void shearing();
    void unitSquire_data();
    void unitSquire();
    void matrixFromUnitSquire_data();
    void matrixFromUnitSquire();
    void rotationOrigin_data();
    void rotationOrigin();

};

template <int N, int M, class T>
bool compareTransformed(const QGenericMatrix<N, M, T> &result,
                        const QGenericMatrix<N, M, T> &expected,
                        const QGenericMatrix<N, M, T> &original,
                        const QGenericMatrix<M, M, T> &matrix)
{
    typedef MatrixUtils<N, M, T> MU;

    if (MU::equals(result, expected))
        return true;

    QTest::qFail((QString::fromLatin1("\n") +
                  MatrixUtils<M, M, T>::toString(matrix) + "\n * \n" +
                  MU::toString(original) + "\n = \n" +
                  MU::toString(result) + "\nExpected:\n" +
                  MU::toString(expected)).toLatin1().data(),
                  __FILE__, __LINE__);
    return false;
}

Squire2 createSquire2(double x1, double x2, double x3, double x4,
                      double y1, double y2, double y3, double y4)
{
    double arr[8];
    arr[0] = x1;
    arr[1] = x2;
    arr[2] = x3;
    arr[3] = x4;
    arr[4] = y1;
    arr[5] = y2;
    arr[6] = y3;
    arr[7] = y4;
    return Squire2(arr);
}

void Transforms::transformPoint2_data()
{
    QTest::addColumn<Point2>("point");
    QTest::addColumn<Matrix2>("matrix");
    QTest::addColumn<Point2>("expected");

    QTest::newRow("[1,0;0,1]*[0,0]")
            << Point2(0.0, 0.0)
            << Matrix2(1.0, 0.0, 0.0, 1.0)
            << Point2(0.0, 0.0);
    QTest::newRow("[1,0;0,1]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(1.0, 0.0, 0.0, 1.0)
            << Point2(1.0, 1.0);
    QTest::newRow("[1.5,0;0,1.5]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(1.5, 0.0, 0.0, 1.5)
            << Point2(1.5, 1.5);
    QTest::newRow("[0.5,0;0,0.5]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(0.5, 0.0, 0.0, 0.5)
            << Point2(0.5, 0.5);
    QTest::newRow("[-1,0;0,1]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(-1.0, 0.0, 0.0, 1.0)
            << Point2(-1.0, 1.0);
    QTest::newRow("[1,0.5;0,1]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(1.0, 0.5, 0.0, 1.0)
            << Point2(1.5, 1.0);
    QTest::newRow("[-0,23,0.19;-0.19,-0.23]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(-0.229813332935693,  0.192836282905962,
                       -0.192836282905962, -0.229813332935693)
            << Point2(-0.0369770500297316, -0.4226496158416552);
    QTest::newRow("[-0,23,0.19;-0.19,-0.23]*[0,0]")
            << Point2(0.0, 0.0)
            << Matrix2(-0.229813332935693,  0.192836282905962,
                       -0.192836282905962, -0.229813332935693)
            << Point2(0.0, 0.0);
    QTest::newRow("[1,56,-0.9;0.9,1.56]*[1,1]")
            << Point2(1.0, 1.0)
            << Matrix2(1.558845726811990, -0.900000000000000,
                       0.900000000000000,  1.558845726811990)
            << Point2(0.658845726811990, 2.458845726811989);
}

void Transforms::transformPoint2()
{
    QFETCH(Point2, point);
    QFETCH(Matrix2, matrix);
    QFETCH(Point2, expected);

    Point2 result = ::transformPoint(point, matrix);
    QVERIFY(compareTransformed(result, expected, point, matrix));
}

void Transforms::transformPoint3_data()
{
    QTest::addColumn<Point3>("point");
    QTest::addColumn<Matrix3>("matrix");
    QTest::addColumn<Point3>("expected");

    QTest::newRow("[1,0,0;0,1,0;0,0,1]*[0,0,1]")
            << Point3(0.0, 0.0)
            << Matrix3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
            << Point3(0.0, 0.0);
    QTest::newRow("[1,0,0;0,1,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
            << Point3(1.0, 1.0);
    QTest::newRow("[1.5,0,0;0,1.5,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(1.5, 0.0, 0.0, 0.0, 1.5, 0.0)
            << Point3(1.5, 1.5);
    QTest::newRow("[0.5,0,0;0,0.5,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(0.5, 0.0, 0.0, 0.0, 0.5, 0.0)
            << Point3(0.5, 0.5);
    QTest::newRow("[-1,0,0;0,1,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
            << Point3(-1.0, 1.0);
    QTest::newRow("[1,0.5,0;0,1,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(1.0, 0.5, 0.0, 0.0, 1.0, 0.0)
            << Point3(1.5, 1.0);
    QTest::newRow("[-0,23,0.19,0;-0.19,-0.23,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(-0.229813332935693,  0.192836282905962, 0.0,
                       -0.192836282905962, -0.229813332935693, 0.0)
            << Point3(-0.0369770500297316, -0.4226496158416552);
    QTest::newRow("[-0,23,0.19,0;-0.19,-0.23,0;0,0,1]*[0,0,1]")
            << Point3(0.0, 0.0)
            << Matrix3(-0.229813332935693,  0.192836282905962, 0.0,
                       -0.192836282905962, -0.229813332935693, 0.0)
            << Point3(0.0, 0.0);
    QTest::newRow("[1,56,-0.9,0;0.9,1.56,0;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(1.558845726811990, -0.900000000000000, 0.0,
                       0.900000000000000,  1.558845726811990, 0.0)
            << Point3(0.658845726811990, 2.458845726811989);
    QTest::newRow("[1,0,1;0,1,1;0,0,1]*[0,0,1]")
            << Point3(0.0, 0.0)
            << Matrix3(1.0, 0.0, 1.0, 0.0, 1.0, 1.0)
            << Point3(1.0, 1.0);
    QTest::newRow("[1,0,-0.5;0,1,-0.4;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(1.0, 0.0, -0.5, 0.0, 1.0, -0.4)
            << Point3(0.5, 0.6);
    QTest::newRow("[0.52,-0.3,1.13;0.3,0.52,2.25;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(0.519615242270663, -0.3, 1.128653347947322,
                       0.3, 0.519615242270663, 2.245115057222525)
            << Point3(1.34826859021798, 3.06473029949319);
    QTest::newRow("[1.04,-0.6,3.5;0.6,1.04,2.3;0,0,1]*[1,1,1]")
            << Point3(1.0, 1.0)
            << Matrix3(1.039230484541326, -0.6, 3.5,
                       0.6, 1.039230484541326, 2.3)
            << Point3(3.93923048454133, 3.93923048454133);
}

void Transforms::transformPoint3()
{
    QFETCH(Point3, point);
    QFETCH(Matrix3, matrix);
    QFETCH(Point3, expected);

    Point3 result = ::transformPoint(point, matrix);
    QVERIFY(compareTransformed(result, expected, point, matrix));
}

void Transforms::transformPointImpl2_data()
{
    transformPoint2_data();
}

void Transforms::transformPointImpl2()
{
    transformPoint2();
}

void Transforms::transformPointImpl3_data()
{
    transformPoint3_data();
}

void Transforms::transformPointImpl3()
{
    transformPoint3();
}

void Transforms::identity2()
{
    Matrix2 matrix = ::identity2();
    QCOMPARE(matrix(0, 0), 1.0);
    QCOMPARE(matrix(0, 1), 0.0);
    QCOMPARE(matrix(1, 0), 0.0);
    QCOMPARE(matrix(1, 1), 1.0);
}

void Transforms::identity3()
{
    Matrix3 matrix = ::identity3();
    QCOMPARE(matrix(0, 0), 1.0);
    QCOMPARE(matrix(0, 1), 0.0);
    QCOMPARE(matrix(0, 2), 0.0);
    QCOMPARE(matrix(1, 0), 0.0);
    QCOMPARE(matrix(1, 1), 1.0);
    QCOMPARE(matrix(1, 2), 0.0);
    QCOMPARE(matrix(2, 0), 0.0);
    QCOMPARE(matrix(2, 1), 0.0);
    QCOMPARE(matrix(2, 2), 1.0);
}

void Transforms::scaling_data()
{
    QTest::addColumn<double>("scale_x");
    QTest::addColumn<double>("scale_y");

    QTest::newRow("0,0") << 0.0 << 0.0;
    QTest::newRow("1,1") << 1.0 << 1.0;
    QTest::newRow("0.5,0.5") << 0.5 << 0.5;
    QTest::newRow("1.5,2.3") << 1.5 << 2.3;
}

void Transforms::scaling()
{
    QFETCH(double, scale_x);
    QFETCH(double, scale_y);

    Matrix2 result = ::scaling(scale_x, scale_y);
    QCOMPARE(result(0, 0), scale_x);
    QCOMPARE(result(0, 1), 0.0);
    QCOMPARE(result(1, 0), 0.0);
    QCOMPARE(result(1, 1), scale_y);
}

void Transforms::reflection_data()
{
    QTest::addColumn<bool>("mirror_x");
    QTest::addColumn<bool>("mirror_y");

    QTest::newRow("1,1") << false << false;
    QTest::newRow("1,-1") << false << true;
    QTest::newRow("-1,1") << true << false;
    QTest::newRow("-1,-1") << true << true;
}

void Transforms::reflection()
{
    QFETCH(bool, mirror_x);
    QFETCH(bool, mirror_y);

    Matrix2 result = ::reflection(mirror_x, mirror_y);
    QCOMPARE(result(0, 0), mirror_x ? -1.0 : 1.0);
    QCOMPARE(result(0, 1), 0.0);
    QCOMPARE(result(1, 0), 0.0);
    QCOMPARE(result(1, 1), mirror_y ? -1.0 : 1.0);
}

void Transforms::shearing_data()
{
    QTest::addColumn<double>("shear_x");
    QTest::addColumn<double>("shear_y");

    QTest::newRow("0,0") << 0.0 << 0.0;
    QTest::newRow("1,1") << 1.0 << 1.0;
    QTest::newRow("0.5,0.5") << 0.5 << 0.5;
    QTest::newRow("1.5,2.3") << 1.5 << 2.3;
}

void Transforms::shearing()
{
    QFETCH(double, shear_x);
    QFETCH(double, shear_y);

    Matrix2 result = ::shearing(shear_x, shear_y);
    QCOMPARE(result(0, 0), 1.0);
    QCOMPARE(result(0, 1), shear_x);
    QCOMPARE(result(1, 0), shear_y);
    QCOMPARE(result(1, 1), 1.0);
}

void Transforms::unitSquire_data()
{
    QTest::addColumn<Squire2>("squire");
    QTest::addColumn<Matrix2>("matrix");
    QTest::addColumn<Squire2>("expected");

    QTest::newRow("[1,0;0,1]*unitSquire")
            << createSquire2(0.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 1.0, 1.0)
            << Matrix2(1.0, 0.0, 0.0, 1.0)
            << createSquire2(0.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 1.0, 1.0);
    QTest::newRow("[1.3,0;0,1.7]*unitSquire")
            << createSquire2(0.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 1.0, 1.0)
            << Matrix2(1.3, 0.0, 0.0, 1.7)
            << createSquire2(0.0, 1.3, 1.3, 0.0,
                             0.0, 0.0, 1.7, 1.7);
    QTest::newRow("[0.87,-0.5;0.5,0.87]*unitSquire")
            << createSquire2(0.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 1.0, 1.0)
            << Matrix2(0.866025403784439, -0.5,
                       0.5, 0.866025403784439)
            << createSquire2(0.0, 0.866025403784439, 0.366025403784439, -0.5,
                             0.0, 0.5, 1.366025403784439, 0.866025403784439);
}

void Transforms::unitSquire()
{
    QFETCH(Squire2, squire);
    QFETCH(Matrix2, matrix);
    QFETCH(Squire2, expected);

    Squire2 result = ::unitSquire(squire, matrix);
    QVERIFY(compareTransformed(result, expected, squire, matrix));
}

void Transforms::matrixFromUnitSquire_data()
{
    QTest::addColumn<Squire2>("squire");

    QTest::newRow("[1,0;0,1]*unitSquire")
            << createSquire2(0.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 1.0, 1.0);
    QTest::newRow("[1.3,0;0,1.7]*unitSquire")
            << createSquire2(0.0, 1.3, 1.3, 0.0,
                             0.0, 0.0, 1.7, 1.7);
    QTest::newRow("[0.87,-0.5;0.5,0.87]*unitSquire")
            << createSquire2(0.0, 0.866025403784439, 0.366025403784439, -0.5,
                             0.0, 0.5, 1.366025403784439, 0.866025403784439);
}

void Transforms::matrixFromUnitSquire()
{
    QFETCH(Squire2, squire);

    Matrix2 matrix = ::matrixFromUnitSquire(squire);
    QCOMPARE(matrix(0, 0), squire(0, 1));
    QCOMPARE(matrix(0, 1), squire(0, 3));
    QCOMPARE(matrix(1, 0), squire(1, 1));
    QCOMPARE(matrix(1, 1), squire(1, 3));
}

void Transforms::rotationOrigin_data()
{
    QTest::addColumn<double>("angle");
    QTest::addColumn<Matrix2>("expected");

    QTest::newRow("0")
            << 0.0
            << Matrix2(1.0, 0.0, 0.0, 1.0);
    QTest::newRow("15")
            << 0.261799387799149
            << Matrix2(0.965925826289068, -0.258819045102521,
                       0.258819045102521, 0.965925826289068);
    QTest::newRow("30")
            << 0.523598775598299
            << Matrix2(0.866025403784439, -0.5,
                       0.5, 0.866025403784439);
    QTest::newRow("45")
            << 0.785398163397448
            << Matrix2(0.707106781186548, -0.707106781186547,
                       0.707106781186547, 0.707106781186548);
    QTest::newRow("90")
            << 1.57079632679490
            << Matrix2(0.0, -1.0, 1.0, 0.0);
    QTest::newRow("180")
            << 3.14159265358979
            << Matrix2(-1.0, 0.0, 0.0, -1.0);
    QTest::newRow("360")
            << 6.28318530717959
            << Matrix2(1.0, 0.0, 0.0, 1.0);
    QTest::newRow("375")
            << 6.54498469497874
            << Matrix2(0.965925826289068, -0.258819045102521,
                       0.258819045102521, 0.965925826289068);
}

void Transforms::rotationOrigin()
{
    QFETCH(double, angle);
    QFETCH(Matrix2, expected);

    Matrix2 result = ::rotationOrigin(angle);
    // see fuzzy comparison with 0.0
    // http://doc.qt.io/qt-5/qtglobal.html#qFuzzyCompare
    QCOMPARE(result(0, 0) + 2.0, expected(0, 0) + 2.0);
    QCOMPARE(result(0, 1) + 2.0, expected(0, 1) + 2.0);
    QCOMPARE(result(1, 0) + 2.0, expected(1, 0) + 2.0);
    QCOMPARE(result(1, 1) + 2.0, expected(1, 1) + 2.0);
}


QTEST_MAIN(Transforms)
#include "transformskoan.moc"
