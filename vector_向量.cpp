#include "vector_向量.h"
vec::Vector::Vector() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}
vec::Vector::Vector(double a) {
    x = a;
    y = a;
    z = a;
}
vec::Vector::Vector(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
//定义一个旋转函数
vec::Vector vec::Vector::Rotate(const Vector& v, const Vector& axis, double angle) {
    //计算旋转公式中的三角函数值
    double cos_theta = cos(angle);
    double sin_theta = sin(angle);
    //计算旋转后的向量
    Vector rotated = v * cos_theta + (axis ^ v) * sin_theta + axis * (axis * v) * (1 - cos_theta);
    //返回旋转后的向量
    return rotated;
}
vec::Vector vec::Vector::Rotate(const Vector& axis, double angle) {
    //计算旋转公式中的三角函数值
    double cos_theta = cos(angle);
    double sin_theta = sin(angle);
    //计算旋转后的向量
    *this = *this * cos_theta + (axis ^ *this) * sin_theta + axis * (axis * *this) * (1 - cos_theta);
    //返回旋转后的向量
    return *this;
}
vec::Vector vec::Vector::Normalize()const
{
    return *this / Length();
}
namespace vec {
    Matrix4x4::Matrix4x4() {
        // 默认构造函数将矩阵初始化为单位矩阵
        m_data[0][0] = 1.0;
        m_data[1][1] = 1.0;
        m_data[2][2] = 1.0;
        m_data[3][3] = 1.0;
    }

    void Matrix4x4::SetScale(const Vector3& scale) {
        // 根据给定的缩放向量设置缩放矩阵
        m_data[0][0] = scale.GetX();
        m_data[1][1] = scale.GetY();
        m_data[2][2] = scale.GetZ();
    }

    void Matrix4x4::SetRotation(const Vector3& axis, double angle) {
        // 根据给定的旋转轴和旋转角度设置旋转矩阵
        double rad = angle * PI / 180.0;
        double cosA = cos(rad);
        double sinA = sin(rad);
        double x = axis.GetX();
        double y = axis.GetY();
        double z = axis.GetZ();

        m_data[0][0] = cosA + (1 - cosA) * x * x;
        m_data[0][1] = (1 - cosA) * x * y - sinA * z;
        m_data[0][2] = (1 - cosA) * x * z + sinA * y;

        m_data[1][0] = (1 - cosA) * x * y + sinA * z;
        m_data[1][1] = cosA + (1 - cosA) * y * y;
        m_data[1][2] = (1 - cosA) * y * z - sinA * x;

        m_data[2][0] = (1 - cosA) * x * z - sinA * y;
        m_data[2][1] = (1 - cosA) * y * z + sinA * x;
        m_data[2][2] = cosA + (1 - cosA) * z * z;
    }

    Vector3 Matrix4x4::GetTranslation() const {
        // 获取矩阵的平移向量
        return Vector3(m_data[3][0], m_data[3][1], m_data[3][2]);
    }

    void Matrix4x4::SetTranslation(const Vector3& pos) {
        // 根据给定的平移向量设置平移矩阵
        m_data[3][0] = pos.GetX();
        m_data[3][1] = pos.GetY();
        m_data[3][2] = pos.GetZ();
    }

    Vector3 Matrix4x4::GetScale() const {
        // 获取矩阵的缩放向量
        return Vector3(m_data[0][0], m_data[1][1], m_data[2][2]);
    }

    Matrix4x4 Matrix4x4::GetInverse() const {
        // 计算矩阵的逆矩阵
        double det =
            m_data[0][0] * (m_data[1][1] * m_data[2][2] - m_data[2][1] * m_data[1][2])
            - m_data[1][0] * (m_data[0][1] * m_data[2][2] - m_data[2][1] * m_data[0][2])
            + m_data[2][0] * (m_data[0][1] * m_data[1][2] - m_data[1][1] * m_data[0][2]);

        if (det == 0.0) {
            // 如果矩阵不可逆，则返回单位矩阵
            return Matrix4x4();
        }

        Matrix4x4 inv;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                double cofactor =
                    ((i + j) % 2 == 0 ? 1 : -1) *
                    (m_data[(j + 1) % 4][(i + 1) % 4] * m_data[(j + 2) % 4][(i + 2) % 4] * m_data[(j + 3) % 4][(i + 3) % 4] - m_data[(j + 1) % 4][(i + 2) % 4] * m_data[(j + 2) % 4][(i + 1) % 4] * m_data[(j + 3) % 4][(i + 3) % 4]);
                inv.m_data[j][i] = cofactor / det;
            }
        }

        return inv;
    }

    void Matrix4x4::SetPerspective(double fov, double aspect, double nearClip, double farClip) {
        // 根据给定的视野、宽高比、近剪裁面和远剪裁面设置透视投影矩阵
        double f = 1.0 / tan(fov * PI / 360.0);
        double zDiff = nearClip - farClip;

        m_data[0][0] = f / aspect;
        m_data[1][1] = f;
        m_data[2][2] = (nearClip + farClip) / zDiff;
        m_data[3][2] = 2.0 * nearClip * farClip / zDiff;
        m_data[2][3] = -1.0;
        m_data[3][3] = 0.0;
    }
    double DegToRad(double Deg)
    {
        return Deg / 180 * PI;
    }
    Vector3 Transform(const Vector3& v, const Matrix4x4& worldToCamera)
    {
        // 将向量从世界坐标系转换到相机坐标系
        Vector4 v4(v.x, v.y, v.z, 1.0f);
        Vector4 vCamera = v4 * worldToCamera;
        return Vector3(vCamera.x, vCamera.y, vCamera.z);
    }

    Quaternion::Quaternion() : r(1.0), i(0.0), j(0.0), k(0.0) {}

    Quaternion::Quaternion(double r, double i, double j, double k)
        : r(r), i(i), j(j), k(k) {}

    Quaternion::Quaternion(const Quaternion& other)
        : r(other.r), i(other.i), j(other.j), k(other.k) {}

    Quaternion::~Quaternion() {}

    Quaternion Quaternion::operator+(const Quaternion& other) const {
        return Quaternion(r + other.r, i + other.i, j + other.j, k + other.k);
    }

    Quaternion Quaternion::operator-(const Quaternion& other) const {
        return Quaternion(r - other.r, i - other.i, j - other.j, k - other.k);
    }

    Quaternion Quaternion::operator*(const Quaternion& other) const {
        return Quaternion(
            r * other.r - i * other.i - j * other.j - k * other.k,
            r * other.i + i * other.r + j * other.k - k * other.j,
            r * other.j - i * other.k + j * other.r + k * other.i,
            r * other.k + i * other.j - j * other.i + k * other.r
        );
    }

    Quaternion Quaternion::operator*(double scalar) const {
        return Quaternion(r * scalar, i * scalar, j * scalar, k * scalar);
    }

    Quaternion Quaternion::operator/(double scalar) const {
        return Quaternion(r / scalar, i / scalar, j / scalar, k / scalar);
    }

    Quaternion Quaternion::operator-() const {
        return Quaternion(-r, -i, -j, -k);
    }

    Quaternion& Quaternion::operator+=(const Quaternion& other) {
        r += other.r;
        i += other.i;
        j += other.j;
        k += other.k;
        return *this;
    }

    Quaternion& Quaternion::operator-=(const Quaternion& other) {
        r -= other.r;
        i -= other.i;
        j -= other.j;
        k -= other.k;
        return *this;
    }

    Quaternion& Quaternion::operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }

    Quaternion& Quaternion::operator*=(double scalar) {
        r *= scalar;
        i *= scalar;
        j *= scalar;
        k *= scalar;
        return *this;
    }

    Quaternion& Quaternion::operator/=(double scalar) {
        r /= scalar;
        i /= scalar;
        j /= scalar;
        k /= scalar;
        return *this;
    }

    bool Quaternion::operator==(const Quaternion& other) const {
        return (r == other.r) && (i == other.i) && (j == other.j) && (k == other.k);
    }

    bool Quaternion::operator!=(const Quaternion& other) const {
        return !(*this == other);
    }

    void Quaternion::normalize() {
        double n = norm();
        if (n > 0.0) {
            r /= n;
            i /= n;
            j /= n;
            k /= n;
        }
    }

    double Quaternion::norm() const {
        return std::sqrt(r * r + i * i + j * j + k * k);
    }

    double Quaternion::normSquared() const {
        return r * r + i * i + j * j + k * k;
    }

    void Quaternion::conjugate() {
        i = -i;
        j = -j;
        k = -k;
    }

    Quaternion Quaternion::conjugated() const {
        return Quaternion(r, -i, -j, -k);
    }

    void Quaternion::invert() {
        double nSq = normSquared();
        if (nSq > 0.0) {
            *this = conjugated() / nSq;
        }
    }

    Quaternion Quaternion::inverted() const {
        double nSq = normSquared();
        if (nSq > 0.0) {
            return conjugated() / nSq;
        }
        else {
            return Quaternion();
        }
    }

    double Quaternion::dot(const Quaternion& other) const {
        return r * other.r + i * other.i + j * other.j + k * other.k;
    }

    Quaternion Quaternion::lerp(const Quaternion& other, double t) const {
        Quaternion q1 = *this;
        Quaternion q2 = other;
        return   q1 * (1.0 - t) + q2 * t;
    }

    Quaternion Quaternion::slerp(const Quaternion& other, double t) const {
        Quaternion q1 = *this;
        Quaternion q2 = other;
        double dotProd = q1.dot(q2);
        if (std::abs(dotProd) >= 1.0) {
            return q1;
        }
        else if (std::abs(dotProd) <= 0.0001) {
            return q1.lerp(q2, t);
        }
        else {
            double theta_0 = std::acos(dotProd);
            double theta = theta_0 * t;
            Quaternion q3 = q2 - q1 * dotProd;
            q3.normalize();
            return q1 * std::cos(theta) + q3 * std::sin(theta);
        }
    }

    Quaternion Quaternion::fromEulerAngles(double pitch, double yaw, double roll) {
        double cy = std::cos(yaw * 0.5);
        double sy = std::sin(yaw * 0.5);
        double cr = std::cos(roll * 0.5);
        double sr = std::sin(roll * 0.5);
        double cp = std::cos(pitch * 0.5);
        double sp = std::sin(pitch * 0.5);

        return Quaternion(
            cy * cr * cp + sy * sr * sp,
            cy * sr * cp - sy * cr * sp,
            cy * cr * sp + sy * sr * cp,
            sy * cr * cp - cy * sr * sp
        );
    }

    Quaternion Quaternion::fromAxisAngle(const Vector& axis, double angle) {
        double halfAngle = angle * 0.5;
        double sinHalfAngle = std::sin(halfAngle);
        Vector normAxis = axis.Normalize();
        return Quaternion(
            std::cos(halfAngle),
            normAxis.x * sinHalfAngle,
            normAxis.y * sinHalfAngle,
            normAxis.z * sinHalfAngle
        );
    }
}