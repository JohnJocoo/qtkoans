#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "types.hpp"

/*
 * We represent 2D point as column vectors.
 * Point2 and Point3 have 2 and 3 rows, respectively.
 * Point3[2] isn't used and is always set to 1.
 * Read affine transformation literature for more info.
 * We will use 2x2 and 3x3 matrices (Matrix2 and Matrix3).
 * The coordinate system in this example is a cortesian
 * (Y points up) and all rotation done counterclockwise.
 */

/**
 * @brief transformPoint applies transformation,
 * held by matrix to point.
 * @param point - original point
 * @param matrix - transformation matrix
 * @return transformed point
 */
Point2 transformPoint(Point2 point, Matrix2 matrix);

/**
 * @brief transformPoint applies transformation,
 * held by matrix to point.
 * @param point - original point
 * @param matrix - transformation matrix
 * @return transformed point
 */
Point3 transformPoint(Point3 point, Matrix3 matrix);

/**
 * @brief transformPointImpl same as transformPoint,
 * but requires you to write matrix multiplication by hand.
 * @param point - original point
 * @param matrix - transformation matrix
 * @return transformed point
 */
Point2 transformPointImpl(Point2 point, Matrix2 matrix);

/**
 * @brief transformPointImpl same as transformPoint,
 * but requires you to write matrix multiplication by hand.
 * @param point - original point
 * @param matrix - transformation matrix
 * @return transformed point
 */
Point3 transformPointImpl(Point3 point, Matrix3 matrix);

/**
 * @brief identity2
 * @return identity matrix.
 */
Matrix2 identity2();

/**
 * @brief identity3
 * @return identity matrix.
 */
Matrix3 identity3();

/**
 * @brief scaling creates scaling matrix.
 * @param scale_x
 * @param scale_y
 * @return scaling matrix.
 */
Matrix2 scaling(double scale_x, double scale_y);

/**
 * @brief reflection creates reflection matrix.
 * @param reflect_x
 * @param reflect_y
 * @return reflection matrix.
 */
Matrix2 reflection(bool reflect_x, bool reflect_y);

/**
 * @brief shearing creates shearing matrix.
 * @param shear_x
 * @param shear_y
 * @return shearing matrix.
 */
Matrix2 shearing(double shear_x, double shear_y);

/**
 * @brief unitSquire applies matrix to squire.
 * Actually this method is about introducing you
 * to matrix effect on unit squire.
 * @param squire
 * @param matrix
 * @return transformed squire.
 */
Squire2 unitSquire(Squire2 squire, Matrix2 matrix);

/**
 * @brief matrixFromUnitSquire creates matrix from
 * unit squire.
 * @param unit_squire
 * @return matrix.
 */
Matrix2 matrixFromUnitSquire(Squire2 unit_squire);

/**
 * @brief rotationOrigin creates matrix that
 * rotate around an origin of coordinates.
 * @param angle - rotation angle in radians (counterclockwise).
 * @return rotation matrix.
 */
Matrix2 rotationOrigin(double angle);

#endif // TRANSFORMATIONS_H
