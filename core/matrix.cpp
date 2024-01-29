#include <core/matrix.h>
#include <core/assert.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const HomogeneousCoord& r1, const HomogeneousCoord& r2, const HomogeneousCoord& r3, const HomogeneousCoord& r4) {
    
    row[0] = r1;
    row[1] = r2;
    row[2] = r3;
    row[3] = r4;
}

HomogeneousCoord& Matrix::operator[](int idx) {
    rt_assert (idx>=0 && idx<=3);
    return row[idx];
}

HomogeneousCoord Matrix::operator[](int idx) const {
    rt_assert (idx>=0 && idx<=3);
    return row[idx];
}

Matrix Matrix::operator+(const Matrix& b) const {
    return Matrix(row[0]+b.row[0], row[1]+b.row[1], row[2]+b.row[2], row[3]+b.row[3]);
}

Matrix Matrix::operator-(const Matrix& b) const {
    return Matrix(row[0]-b.row[0], row[1]-b.row[1], row[2]-b.row[2], row[3]-b.row[3]);
}

Matrix Matrix::transpose() const {
    
    Matrix m;

    m[0] = HomogeneousCoord(row[0][0], row[1][0], row[2][0], row[3][0]);
    m[1] = HomogeneousCoord(row[0][1], row[1][1], row[2][1], row[3][1]);
    m[2] = HomogeneousCoord(row[0][2], row[1][2], row[2][2], row[3][2]);
    m[3] = HomogeneousCoord(row[0][3], row[1][3], row[2][3], row[3][3]);

    return m;
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    
    bool bool_r1 = (row[0]==b.row[0]);
    bool bool_r2 = (row[1]==b.row[1]);
    bool bool_r3 = (row[2]==b.row[2]);
    bool bool_r4 = (row[3]==b.row[3]);

    if (bool_r1 && bool_r2 && bool_r3 && bool_r4) return true;
    else return false;
}

bool Matrix::operator!=(const Matrix& b) const {
    return !(*this==b);
}

Matrix product(const Matrix& a, const Matrix& b) {
    
    Matrix m;
    Matrix b_T = b.transpose();

    for (int i=0; i<4; ++i){
        for (int j=0; j<4; ++j){
            m[i][j] = dot(a[i], b_T[j]);
        }
    }
    return m;
}

Matrix operator*(const Matrix& a, float scalar) {
    return Matrix(scalar*a.row[0], scalar*a.row[1], scalar*a.row[2], scalar*a.row[3]);
}

Matrix operator*(float scalar, const Matrix& a) {
    return Matrix(scalar*a.row[0], scalar*a.row[1], scalar*a.row[2], scalar*a.row[3]);
}

HomogeneousCoord Matrix::operator*(const HomogeneousCoord& b) const {
    return HomogeneousCoord(dot(row[0], b), dot(row[1], b), dot(row[2], b), dot(row[3], b));
}

Vector Matrix::operator*(const Vector& b) const {
    
    HomogeneousCoord b_h(b);
    return Vector((*this) * b_h);
}

Point Matrix::operator*(const Point& b) const {
    
    HomogeneousCoord b_h(b);
    return Point((*this) * b_h);
}

float Matrix::det() const {
    
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];

    return det;
}

Matrix Matrix::zero() {
    
    HomogeneousCoord z = HomogeneousCoord::rep(0.0f);
    return Matrix(z, z, z, z);
}

Matrix Matrix::identity() {
    
    Matrix m = Matrix::zero();

    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;

    return m;
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    return Matrix(HomogeneousCoord(e1), HomogeneousCoord(e2), HomogeneousCoord(e3), HomogeneousCoord(0.0f, 0.0f, 0.0f, 1.0f)).transpose();
}

}