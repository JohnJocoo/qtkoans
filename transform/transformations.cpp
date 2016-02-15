#include "transformations.h"

#include <math.h>

Point2 transformPoint(Point2 point, Matrix2 matrix)
{
    /*
     * Point transformation is done simply by multiplying
     * point (column vector) by transformation matrix.
     * Remember that order is important.
     */
    return Point2();
}

Point3 transformPoint(Point3 point, Matrix3 matrix)
{
    /*
     * Point transformation is done simply by multiplying
     * point (column vector) by transformation matrix.
     * Remember that order is important.
     */
    return Point3();
}

Point2 transformPointImpl(Point2 point, Matrix2 matrix)
{
    /*
     * The same as transformPoint(),
     * but here you need to implement multiplication
     * by hand.
     * Knowledge gained here will become handy in following
     * tasks.
     *
     * Lets assume R[n, p] = A[n, m] * B[m, p]
     * R(i, j) = sum( [k = 1..m; A(i, k) * B(k, j)] )
     * In our case:
     * R(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0)
     * R(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0)
     *
     * Now create and return transformed point.
     */
    return Point2();
}

Point3 transformPointImpl(Point3 point, Matrix3 matrix)
{
    /*
     * The same as transformPoint(),
     * but here you need to implement multiplication
     * by hand.
     * Knowledge gained here will become handy in following
     * tasks.
     *
     * Lets assume R[n, p] = A[n, m] * B[m, p]
     * R(i, j) = sum( [k = 1..m; A(i, k) * B(k, j)] )
     * In our case:
     * R(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0)
     * R(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0)
     * R(2, 0) = A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0)
     *
     * Now create and return transformed point.
     */
    return Point3();
}

Matrix2 identity2()
{
    /*
     * Identity matrix doesn't move points at all.
     *
     * Lets take a closer look:
     * Identity matrix for our case is:
     * [ 1, 0 ]
     * [ 0, 1 ]
     *
     * We had R[n, p] = A[n, m] * B[m, p]
     * R(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0)
     * R(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0)
     * that simplifies to:
     * R(0, 0) = 1 * B(0, 0) + 0 * B(1, 0)
     * R(1, 0) = 0 * B(0, 0) + 1 * B(1, 0)
     *
     * R(0, 0) = B(0, 0)
     * R(1, 0) = B(1, 0)
     *
     * Now just return identity matrix
     * (default constructor creates identity matrix).
     */
    return Matrix2(0.0, 0.0, 0.0, 0.0);
}

Matrix3 identity3()
{
    /*
     * Identity matrix doesn't move points at all.
     *
     * Identity matrix for our case is:
     * [ 1, 0, 0 ]
     * [ 0, 1, 0 ]
     * [ 0, 0, 1 ]
     *
     * We had R[n, p] = A[n, m] * B[m, p]
     * Similarly to identity2() you can prove that:
     *
     * R(0, 0) = B(0, 0)
     * R(1, 0) = B(1, 0)
     * R(2, 0) = B(2, 0)
     *
     * Now just return identity matrix
     * (default constructor creates identity matrix).
     */
    return Matrix3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

Matrix2 scaling(double scale_x, double scale_y)
{
    /*
     * Scaling matrix is created setting
     * M(0, 0) and M(1, 1) to x and y scale coefficients.
     *
     * So scale matrix is:
     * [ scale_x,      0 ]
     * [       0, scale_y]
     *
     * Lets take a closer look now:
     * We have R[n, p] = A[n, m] * B[m, p]
     * And before we've shown that:
     * R(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0)
     * R(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0)
     * and in our case:
     * R(0, 0) = scale_x * B(0, 0) +       0 * B(1, 0)
     * R(1, 0) =       0 * B(0, 0) + scale_y * B(1, 0)
     *
     * R(0, 0) = scale_x * B(0, 0)
     * R(1, 0) = scale_y * B(1, 0)
     *
     * Now return scaling matrix.
     */
    return Matrix2();
}

Matrix2 reflection(bool reflect_x, bool reflect_y)
{
    /*
     * Reflection matrix is created setting
     * M(0, 0) (if you want to reflect x) or
     * M(1, 1) (if you want to reflect y) or
     * both to -1.
     *
     * Suppose we want to reflect both:
     * [ -1,  0]
     * [  0, -1]
     *
     * Using the same technic as in scaling() we can
     * show that for:
     * R[n, p] = A[n, m] * B[m, p]
     *
     * R(0, 0) = -1 * B(0, 0)
     * R(1, 0) = -1 * B(1, 0)
     *
     * Now return reflection matrix.
     */
    return Matrix2();
}

Matrix2 shearing(double shear_x, double shear_y)
{
    /*
     * Shearing matrix is created setting
     * M(0, 1) and M(1, 0) to x and y shearing coefficients.
     *
     * So shear matrix is:
     * [       1, shear_x ]
     * [ shear_y,       1 ]
     *
     * Lets take a closer look now:
     * We have R[n, p] = A[n, m] * B[m, p]
     * And before we've shown that:
     * R(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0)
     * R(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0)
     * and in our case:
     * R(0, 0) =       1 * B(0, 0) + shear_x * B(1, 0)
     * R(1, 0) = shear_y * B(0, 0) +       1 * B(1, 0)
     *
     * R(0, 0) = B(0, 0) + shear_x * B(1, 0)
     * R(1, 0) = B(1, 0) + shear_y * B(0, 0)
     *
     * Now return shearing matrix.
     */
    return Matrix2();
}

Squire2 unitSquire(Squire2 squire, Matrix2 matrix)
{
    /*
     * Unit squire is a squire with side length = 1.0.
     * Suppose we have an unit squire S with points p, q, r, s.
     * p = [ 0, 0 ] (placed in origin)
     * q = [ 1, 0 ]
     * r = [ 1, 1 ]
     * s = [ 0, 1 ]
     *
     * or [ 0, 1, 1, 0 ]
     *    [ 0, 0, 1, 1 ]
     *
     * and a matrix A:
     * [ a, b ]
     * [ c, d ]
     *
     * Multiplying A * S we have
     * [ 0, a, a + b, b ]
     * [ 0, c, c + d, d ]
     *
     * Now look trasformation does't affect p (in origin) at all.
     * So origin invariant under A (any 2x2 matrix).
     *
     * Also:
     * q' = [ a, c ]'
     * s' = [ b, d ]'
     * Hence A can be determined by q' and s' (q and s in transformed squire).
     *
     * Now just apply matrix on unit squire.
     */
    return Squire2();
}

Matrix2 matrixFromUnitSquire(Squire2 unit_squire)
{
    /*
     * In previous task we've learned that.
     * q = [ a, c ]
     * s = [ b, d ]
     * with matrix A:
     * [ a, b ]
     * [ c, d ]
     * and squire S = [ p, q, r, s ].
     *
     * Now create matrix from transformed unit squire.
     */
    return Matrix2();
}

Matrix2 rotationOrigin(double angle)
{
    /*
     * Now open rotate_origin.png (in "transform" folder)
     * for better comprehension.
     * As you see we rotated unit squire about origin of
     * coordinate system at angle a.
     * Lets show that rotation about origin can be done by 2x2 matrix.
     *
     * From previous tasks (unitSquire and matrixFromUnitSquire)
     * we know that matrix can be created from unit squire
     * (actually only q and s points are needed).
     * Unit squire S = [ p, q, r, s ]
     * Matrix A:
     * [ a, b ]
     * [ c, d ]
     *
     * q = [ a, c ]
     * s = [ b, d ]
     *
     * Now look at the picture. q and s can be found
     * with simple trigonometric functions:
     * q = [      cos(a), sin(a) ]
     * s = [ -1 * sin(a), cos(a) ]
     *
     * So rotation matrix is:
     * [ cos(a), -1 * sin(a) ]
     * [ sin(a),      cos(a) ]
     *
     * Now create and return rotation matrix.
     */
    return Matrix2();
}
