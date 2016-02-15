#ifndef TYPES_HPP
#define TYPES_HPP

#include <QGenericMatrix>
#include <QStringBuilder>

class QGenericMatrix12 : public QGenericMatrix<1, 2, double>
{
public:
    QGenericMatrix12()
    {

    }

    QGenericMatrix12(double m1, double m2)
    {
        (*this)(0, 0) = m1;
        (*this)(1, 0) = m2;
    }

    QGenericMatrix12(const QGenericMatrix<1, 2, double> &other)
        : QGenericMatrix<1, 2, double>(other)
    {

    }

};

class QGenericMatrix13 : public QGenericMatrix<1, 3, double>
{
public:
    QGenericMatrix13()
    {

    }

    QGenericMatrix13(double m1, double m2)
    {
        (*this)(0, 0) = m1;
        (*this)(1, 0) = m2;
        (*this)(2, 0) = 1;
    }

    QGenericMatrix13(double m1, double m2, double m3)
    {
        (*this)(0, 0) = m1;
        (*this)(1, 0) = m2;
        (*this)(2, 0) = m3;
    }

    QGenericMatrix13(const QGenericMatrix<1, 3, double> &other)
        : QGenericMatrix<1, 3, double>(other)
    {

    }

    /* QGenericMatrix12 and QGenericMatrix13
     * represents 2D points with third component
     * in QGenericMatrix13 not used (we set it to 1,
     * read affine transformation literature for more info).
     * We can make cast from QGenericMatrix12
     * to QGenericMatrix13.
     */
    QGenericMatrix13(const QGenericMatrix<1, 2, double> &other)
    {
        (*this)(0, 0) = other(0, 0);
        (*this)(1, 0) = other(1, 0);
        (*this)(2, 0) = 1;
    }
};

class QGenericMatrix22 : public QGenericMatrix<2, 2, double>
{
public:
    QGenericMatrix22()
    {

    }

    QGenericMatrix22(double m11, double m12,
                     double m21, double m22)
    {
        (*this)(0, 0) = m11;
        (*this)(0, 1) = m12;
        (*this)(1, 0) = m21;
        (*this)(1, 1) = m22;
    }

    QGenericMatrix22(const QGenericMatrix<2, 2, double> &other)
        : QGenericMatrix<2, 2, double>(other)
    {

    }
};

class QGenericMatrix33 : public QGenericMatrix<3, 3, double>
{
public:
    QGenericMatrix33()
    {

    }

    QGenericMatrix33(double m11, double m12, double m13,
                     double m21, double m22, double m23)
    {
        (*this)(0, 0) = m11;
        (*this)(0, 1) = m12;
        (*this)(0, 2) = m13;
        (*this)(1, 0) = m21;
        (*this)(1, 1) = m22;
        (*this)(1, 2) = m23;
        (*this)(2, 0) = 0;
        (*this)(2, 1) = 0;
        (*this)(2, 2) = 1;
    }

    QGenericMatrix33(double m11, double m12, double m13,
                     double m21, double m22, double m23,
                     double m31, double m32, double m33)
    {
        (*this)(0, 0) = m11;
        (*this)(0, 1) = m12;
        (*this)(0, 2) = m13;
        (*this)(1, 0) = m21;
        (*this)(1, 1) = m22;
        (*this)(1, 2) = m23;
        (*this)(2, 0) = m31;
        (*this)(2, 1) = m32;
        (*this)(2, 2) = m33;
    }

    QGenericMatrix33(const QGenericMatrix<3, 3, double> &other)
        : QGenericMatrix<3, 3, double>(other)
    {

    }

    /* QGenericMatrix22 and QGenericMatrix33
     * represents 2D transformation matrices.
     * QGenericMatrix22 is able to represent
     * scaling, rotation, reflection, shearing.
     * QGenericMatrix33 is also capable of
     * translation and perspective projection,
     * m13 and m23 used for translation,
     * m31, m32 and m33 used for perspective
     * (we will allways set them as 0, 0, 1 while
     * using only affine transformations).
     * Read affine transformation literature for more info.
     * We can make cast from QGenericMatrix22
     * to QGenericMatrix33, setting transformation
     * and perspective to default values.
     */
    QGenericMatrix33(const QGenericMatrix<2, 2, double> &other)
    {
        setToIdentity();
        (*this)(0, 0) = other(0, 0);
        (*this)(0, 1) = other(0, 1);
        (*this)(1, 0) = other(1, 0);
        (*this)(1, 1) = other(1, 1);
    }
};

typedef QGenericMatrix12 Point2;
typedef QGenericMatrix13 Point3;
typedef QGenericMatrix22 Matrix2;
typedef QGenericMatrix33 Matrix3;
typedef QGenericMatrix<4, 2, double> Squire2;

template <int N, int M, class T>
struct MatrixUtils
{
    static inline QString toString(const QGenericMatrix<N, M, T> &m)
    {
        QString builder;
        for (int i = 0; i < M; ++i)
        {
            if (i)
                builder += QString::fromUtf8("\n");
            builder += QString::fromUtf8("[ ");
            for (int j = 0; j < N; ++j)
            {
                if (j)
                    builder += QString::fromUtf8(", ");
                builder += QString::number(m(i, j));
            }
            builder += QString::fromUtf8(" ]");
        }
        return builder;
    }

    static inline bool equals(const QGenericMatrix<N, M, T> &m1,
                              const QGenericMatrix<N, M, T> &m2)
    {
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (!qFuzzyCompare(m1(i, j), m2(i, j)))
                    return false;
            }
        }
        return true;
    }
};

template <int M, class T>
struct MatrixUtils<1, M, T>
{
    static inline QString toString(const QGenericMatrix<1, M, T> &m)
    {
        QString builder;
        builder += QString::fromUtf8("[ ");
        for (int i = 0; i < M; ++i)
        {
            if (i)
                builder += QString::fromUtf8(", ");
            builder += QString::number(m(i, 0));
        }
        builder += QString::fromUtf8(" ]'");
        return builder;
    }

    static inline bool equals(const QGenericMatrix<1, M, T> &m1,
                              const QGenericMatrix<1, M, T> &m2)
    {
        for (int i = 0; i < M; ++i)
        {
            if (!qFuzzyCompare(m1(i, 0), m2(i, 0)))
                return false;
        }
        return true;
    }
};

Q_DECLARE_METATYPE(Point2)
Q_DECLARE_METATYPE(Point3)
Q_DECLARE_METATYPE(Matrix2)
Q_DECLARE_METATYPE(Matrix3)
Q_DECLARE_METATYPE(Squire2)

#endif // TYPES_HPP
