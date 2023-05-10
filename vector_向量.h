#pragma once
#include<Windows.h>
#include<iostream>
#include <iomanip>
#include<cassert>
#include<string>
#include<math.h>
#include <directxmath.h>
#include<glm/glm.hpp>
#include<sstream>
#define PI 3.14159265358979323846
namespace vec {
    class Vector {
    public:
        double x;
        double y;
        double z;
        // Ĭ�Ϲ��캯������ʼ��Ϊ������
        Vector();
        Vector(double a);
        // �������Ĺ��캯�������ݸ����ķ�����ʼ������
        Vector(double x, double y, double z);
        // �����������ͷ��ڴ�
        ~Vector() {}
        // ����һ����̬��������������������ˣ��ڻ���������һ������ֵ
        static double dot(const Vector& v1, const Vector& v2) {
            return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
        }
        // ��ȡx������ֵ
        double GetX() const {
            return x;
        }
        // ��ȡy������ֵ
        double GetY() const {
            return y;
        }
        // ��ȡz������ֵ
        double GetZ() const {
            return z;
        }
        // ����x������ֵ
        void SetX(double x) {
            this->x = x;
        }
        // ����y������ֵ
        void SetY(double y) {
            this->y = y;
        }
        // ����z������ֵ
        void SetZ(double z) {
            this->z = z;
        }
        // ����������ģ�������ȣ�
        double Length() const {
            return sqrt(x * x + y * y + z * z);
        }
        // ������������֮��ļнǣ������ƣ�
        static double Angle(const Vector& v1, const Vector& v2) {
            return acos(dot(v1, v2) / (v1.Length() * v2.Length()));
        }
        // ������������֮��ľ��루ŷ�Ͼ��룩
        static double Distance(const Vector& v1, const Vector& v2) {
            return (v1 - v2).Length();
        }
        Vector Normalize()const;
        Vector Rotate(const Vector& v, const Vector& axis, double angle);
        // �����������ʵ�ּӼ�����˲�ˣ�������������λ��������Ȳ���
        Vector Rotate(const Vector& axis, double angle);
        // ����+�������ʵ������������ӣ�����һ���µ���������
        friend Vector operator+(const Vector& v1, const Vector& v2) {
            return Vector(v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z);
        }
        // ����+=�������ʵ�������������
        Vector operator+=(const Vector& v) {
            return *this = *this + v;
        }

        // ����-�������ʵ�������������������һ���µ���������
        friend Vector operator-(const Vector& v1, const Vector& v2) {
            return Vector(v1.x - v2.x,
                v1.y - v2.y,
                v1.z - v2.z);
        }
        // ����-=�������ʵ�������������
        Vector operator-=(const Vector& v) {
            return *this = *this - v;
        }
        // ����-�������ʵ������ȡ��������һ���µ���������
        friend Vector operator-(const Vector& v) {
            return !v;
        }

        // ����*�������ʵ��һ����������һ������������һ���µ���������
        friend double operator*(const Vector& v1, const Vector& v2) {
            return dot(v1, v2);
        }
        // ����*�������ʵ��һ����������һ������������һ���µ���������
        friend Vector operator*(const Vector& v, double k) {
            return Vector(v.x * k,
                v.y * k,
                v.z * k);
        }

        // ����*�������ʵ��һ����������һ������������һ���µ���������
        friend Vector operator*(double k, const Vector& v) {
            return (v * k);
        }

        // ����/�������ʵ��һ����������һ������������һ���µ���������
        friend Vector operator/(const Vector& v, double k) {
            if (k == 0) {
                throw "division by zero";
            }
            else {
                return (v * (1 / k));
            }
        }


        // �������
        double dot(const Vector& v) const {
            return x * v.x + y * v.y + z * v.z;
        }
        // ����^�������ʵ������������ˣ������������һ���µ���������
        friend Vector operator^(const Vector& v1, const Vector& v2) {
            return cross(v1, v2);
        }

        // ����һ����̬��������������������ˣ������������һ���µ���������
        static Vector cross(const Vector& v1, const Vector& v2) {
            return Vector(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
        }
        // ����һ����������������������ˣ������������һ���µ���������
        Vector Cross(const Vector& v1)const {
            return Vector(v1.y * z - v1.z * y,
                v1.z * x - v1.x * z,
                v1.x * y - v1.y * x);
        }
        // ����!�������ʵ������ȡ��������һ���µ���������
        friend Vector operator!(const Vector& v) {
            return Vector(-v.x, -v.y, -v.z);
        }

        // ����~�������ʵ��������λ��������һ���µ���������
        friend Vector operator~(const Vector& v) {
            double len = v.Length();
            if (len == 0) {
                throw "zero vector";
            }
            else {
                return (Vector(v / len));
            }
        }

        // ����һ����̬���� ���ر�ʾ����ĵ�λʸ

        static Vector unit(const Vector& v) {
            return ~v;
        }

        // ����==�������ʵ������������ȵ��жϣ�����һ������ֵ
        friend bool operator==(const Vector& v1, const Vector& v2) {
            return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
        }

        // ����!=�������ʵ�������������ȵ��жϣ�����һ������ֵ
        friend bool operator!=(const Vector& v1, const Vector& v2) {
            return !(v1 == v2);
        }

        // ����<<�������ʵ�������������ʽ��������һ�����������
        friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
            os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
            return os;
        }
        // ���ַ���׼�������
        friend std::wostream& operator<<(std::wostream& os, const Vector& v) {
            os << L"(" << std::setw(8) << std::fixed << std::setprecision(2) << v.x << L", "
                << std::setw(8) << std::fixed << std::setprecision(2) << v.y << L", "
                << std::setw(8) << std::fixed << std::setprecision(2) << v.z << L")";
            return os;
        }

        // ����>>�������ʵ�������������ʽ��������һ������������
        friend std::istream& operator>>(std::istream& is, Vector& v) {
            is >> v.x >> v.y >> v.z;
            return is;
        }
        DirectX::XMFLOAT3 ToXMFLOAT3() const
        {
            return DirectX::XMFLOAT3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
        }
        operator glm::vec3()const
        {
            return glm::vec3(x, y, z);
        }
    };

    typedef Vector Vector3;
    Vector3 str_vector(std::string);
    Vector3 str_vector(std::wstring svec);
    class Vector2 {
        // ˽�г�Ա����
    public:
        double x; // x����
        double y; // y����
        // ���г�Ա����

        // ���캯������ʼ��x��y
        Vector2(double x = 0, double y = 0) {
            this->x = x;
            this->y = y;
        }
        // ��ȡx�����ĺ���
        double getX() const {
            return x;
        }
        // ��ȡy�����ĺ���
        double getY() const {
            return y;
        }
        // ����x�����ĺ���
        void setX(double x) {
            this->x = x;
        }
        // ����y�����ĺ���
        void setY(double y) {
            this->y = y;
        }

        // ����������ģ�����ȣ��ĺ���
        double magnitude() const {
            return sqrt(x * x + y * y);
        }
        // ���������ķ��򣨽Ƕȣ��ĺ��������ػ���ֵ
        double angle() const {
            return atan2(y, x);
        }

        // ����+�������ʵ��������ӵĹ��ܣ�����һ���µ�Vector2D����
        Vector2 operator+(const Vector2& v) const {
            return Vector2(x + v.x, y + v.y);
        }

        // ����-�������ʵ����������Ĺ��ܣ�����һ���µ�Vector2D����
        Vector2 operator-(const Vector2& v) const {
            return Vector2(x - v.x, y - v.y);
        }

        // ����*�������ʵ�������������˵Ĺ��ܣ�����һ���µ�Vector2D����
        Vector2 operator*(double k) const {
            return Vector2(x * k, y * k);
        }

        // ����/�������ʵ���������������Ĺ��ܣ�����һ���µ�Vector2D���� 
        Vector2 operator/(double k) const {
            if (k != 0) {
                return Vector2(x / k, y / k);
            }
            else {
                throw std::runtime_error("Division by zero");
            }
        }

        // ����==��������ж����������Ƿ���ȣ��ֱ�Ƚ�x��y�Ƿ���ȣ�
        bool operator==(const Vector2& v) const {
            return (x == v.x && y == v.y);
        }

        // ����!=��������ж����������Ƿ񲻵ȣ�ȡ==���������ķ�ֵ��
        bool operator!=(const Vector2& v) const {
            return !(*this == v);
        }
        DirectX::XMFLOAT2 ToXMFLOAT2() const
        {
            return DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y));
        }
    };
    class Vector4 {
    public:
        double x;
        double y;
        double z;
        double w;

        // ���캯��
        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vector4(double a, double b, double c, double d) : x(a), y(b), z(c), w(d) {}

        // ��ֵ�����
        Vector4& operator=(const Vector4& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        // �����ӷ�
        Vector4 operator+(const Vector4& other) const {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        // ��������
        Vector4 operator-(const Vector4& other) const {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        // �������
        double dot(const Vector4& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        // ������ˣ���������ά������
        Vector4 cross(const Vector4& other) const {
            return Vector4(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x,
                0.0f
            );
        }

        // ��������
        double length() const {
            return sqrt(dot(*this));
        }

        // ������һ��
        void normalize() {
            double len = length();
            if (len > 0.0f) {
                x /= len;
                y /= len;
                z /= len;
                w /= len;
            }
        }
    };


#include <iostream>

    class Matrix4 {
    private:
        double m_data[4][4];

    public:
        // ���캯��
        Matrix4();
        Matrix4(double a, double b, double c, double d,
            double e, double f, double g, double h,
            double i, double j, double k, double l,
            double mm, double n, double o, double p) {
            m_data[0][0] = a; m_data[0][1] = b; m_data[0][2] = c; m_data[0][3] = d;
            m_data[1][0] = e; m_data[1][1] = f; m_data[1][2] = g; m_data[1][3] = h;
            m_data[2][0] = i; m_data[2][1] = j; m_data[2][2] = k; m_data[2][3] = l;
            m_data[3][0] = mm; m_data[3][1] = n; m_data[3][2] = o; m_data[3][3] = p;
        }
        Matrix4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4)
        {
            m_data[0][0] = v1.x; m_data[0][1] = v1.y; m_data[0][2] = v1.z; m_data[0][3] = v1.w;
            m_data[1][0] = v2.x; m_data[1][1] = v2.y; m_data[1][2] = v2.z; m_data[1][3] = v2.w;
            m_data[2][0] = v3.x; m_data[2][1] = v3.y; m_data[2][2] = v3.z; m_data[2][3] = v3.w;
            m_data[3][0] = v4.x; m_data[3][1] = v4.y; m_data[3][2] = v4.z; m_data[3][3] = v4.w;
        }
        Matrix4(double arr[4][4])
        {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    m_data[i][j] = arr[i][j];
                }
            }
        }

        Matrix4(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& v4)
        {
            m_data[0][0] = v1.x; m_data[0][1] = v1.y; m_data[0][2] = v1.z; m_data[0][3] = 0.0;
            m_data[1][0] = v2.x; m_data[1][1] = v2.y; m_data[1][2] = v2.z; m_data[1][3] = 0.0;
            m_data[2][0] = v3.x; m_data[2][1] = v3.y; m_data[2][2] = v3.z; m_data[2][3] = 0.0;
            m_data[3][0] = v4.x; m_data[3][1] = v4.y; m_data[3][2] = v4.z; m_data[3][3] = 1.0;
        }



        // ����Ԫ��ֵ
        void Set(int row, int col, double value) {
            m_data[row][col] = value;
        }

        // ��ȡԪ��ֵ
        double Get(int row, int col) const {
            return m_data[row][col];
        }

        // �ǳ����汾���±������
        double* operator[](int row) {
            return m_data[row];
        }

        // �����汾���±������
        const double* operator[](int row) const {
            return m_data[row];
        }

        // ����ӷ�
        Matrix4 operator+(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result[i][j] = m_data[i][j] + other[i][j];
                }
            }
            return result;
        }

        // ����˷�
        Matrix4 operator*(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    double sum = 0.0;
                    for (int k = 0; k < 4; k++) {
                        sum += m_data[i][k] * other[k][j];
                    }
                    result[i][j] = sum;
                }
            }
            return result;
        }
        friend Vector3 operator*(const Matrix4& mat, const Vector3& vec) {
            double x = mat.m_data[0][0] * vec.x + mat.m_data[0][1] * vec.y + mat.m_data[0][2] * vec.z + mat.m_data[0][3];
            double y = mat.m_data[1][0] * vec.x + mat.m_data[1][1] * vec.y + mat.m_data[1][2] * vec.z + mat.m_data[1][3];
            double z = mat.m_data[2][0] * vec.x + mat.m_data[2][1] * vec.y + mat.m_data[2][2] * vec.z + mat.m_data[2][3];
            double w = mat.m_data[3][0] * vec.x + mat.m_data[3][1] * vec.y + mat.m_data[3][2] * vec.z + mat.m_data[3][3];

            if (w == 0.0) {
                return Vector3(x, y, z);
            }
            else {
                return Vector3(x / w, y / w, z / w);
            }
        }
        friend Vector4 operator*(const Matrix4& mat, const Vector4& vec) {
            double x = mat.m_data[0][0] * vec.x + mat.m_data[0][1] * vec.y + mat.m_data[0][2] * vec.z + mat.m_data[0][3] * vec.w;
            double y = mat.m_data[1][0] * vec.x + mat.m_data[1][1] * vec.y + mat.m_data[1][2] * vec.z + mat.m_data[1][3] * vec.w;
            double z = mat.m_data[2][0] * vec.x + mat.m_data[2][1] * vec.y + mat.m_data[2][2] * vec.z + mat.m_data[2][3] * vec.w;
            double w = mat.m_data[3][0] * vec.x + mat.m_data[3][1] * vec.y + mat.m_data[3][2] * vec.z + mat.m_data[3][3] * vec.w;

            return Vector4(x, y, z, w);
        }
        friend Vector4 operator*(const Vector4& vec, const Matrix4& mat) {
            double x = vec.x * mat.m_data[0][0] + vec.y * mat.m_data[1][0] + vec.z * mat.m_data[2][0] + vec.w * mat.m_data[3][0];
            double y = vec.x * mat.m_data[0][1] + vec.y * mat.m_data[1][1] + vec.z * mat.m_data[2][1] + vec.w * mat.m_data[3][1];
            double z = vec.x * mat.m_data[0][2] + vec.y * mat.m_data[1][2] + vec.z * mat.m_data[2][2] + vec.w * mat.m_data[3][2];
            double w = vec.x * mat.m_data[0][3] + vec.y * mat.m_data[1][3] + vec.z * mat.m_data[2][3] + vec.w * mat.m_data[3][3];

            return Vector4(x, y, z, w);
        }

        // �������
        Matrix4 Inverse() const {
            // TODO: ʵ�����������㷨
            // ����ֻ�Ƿ���һ���յľ���
            return Matrix4();
        }

        // ��ת�þ���
        Matrix4 Transpose() const {
            Matrix4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result[j][i] = m_data[i][j];
                }
            }
            return result;
        }
        // �������׼�����
        friend std::ostream& operator<<(std::ostream& os, const Matrix4& matrix) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    os << matrix[i][j] << " ";
                }
                os << std::endl;
            }
            return os;
        }
        operator glm::mat4() const {
            return glm::mat4(
                m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0],
                m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1],
                m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2],
                m_data[0][3], m_data[1][3], m_data[2][3], m_data[3][3]
            );
        }
        // ����λ������
        void SetArrTranslation(double x, double y, double z) {
            m_data[0][3] = x;
            m_data[1][3] = y;
            m_data[2][3] = z;
        }

        // ������ת����
        void SetArrRotation(double angle, double x, double y, double z) {
            double cosA = cos(angle);
            double sinA = sin(angle);
            double u = x, v = y, w = z;
            double len = sqrt(u * u + v * v + w * w);
            u /= len;
            v /= len;
            w /= len;

            double rotMat[3][3] = { {cosA + u * u * (1 - cosA), u * v * (1 - cosA) - w * sinA, u * w * (1 - cosA) + v * sinA},
                                  {v * u * (1 - cosA) + w * sinA, cosA + v * v * (1 - cosA), v * w * (1 - cosA) - u * sinA},
                                  {w * u * (1 - cosA) - v * sinA, w * v * (1 - cosA) + u * sinA, cosA + w * w * (1 - cosA)} };

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    m_data[i][j] = rotMat[i][j];
                }
            }
        }

        // ������������
        void SetArrScale(double sx, double sy, double sz) {
            m_data[0][0] = sx;
            m_data[1][1] = sy;
            m_data[2][2] = sz;
        }
        // ����λ������
        void SetRowTranslation(double x, double y, double z) {
            m_data[3][0] = x;
            m_data[3][1] = y;
            m_data[3][2] = z;
        }

        // ������ת����
        void SetRowRotation(double angle, double x, double y, double z) {
            double cosA = cos(angle);
            double sinA = sin(angle);
            double u = x, v = y, w = z;
            double len = sqrt(u * u + v * v + w * w);
            u /= len;
            v /= len;
            w /= len;

            double rotMat[3][3] = { {cosA + u * u * (1 - cosA), u * v * (1 - cosA) - w * sinA, u * w * (1 - cosA) + v * sinA},
                                    {v * u * (1 - cosA) + w * sinA, cosA + v * v * (1 - cosA), v * w * (1 - cosA) - u * sinA},
                                    {w * u * (1 - cosA) - v * sinA, w * v * (1 - cosA) + u * sinA, cosA + w * w * (1 - cosA)} };

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    m_data[i][j] = rotMat[i][j];
                }
            }
        }

        // ������������
        void SetRowScale(double sx, double sy, double sz) {
            m_data[0][0] = sx;
            m_data[1][1] = sy;
            m_data[2][2] = sz;
        }
        //static DirectX::XMFLOAT4X4 ToXMFLOAT4X4(const Matrix4x4& matrix){
        //    DirectX::XMFLOAT4X4 result;
        //    for (int i = 0; i < 4; i++) {
        //        for (int j = 0; j < 4; j++) {
        //            result.m[i][j] = static_cast<float>(matrix[i][j]);
        //        }
        //    }
        //    return result;
        //}
        //operator DirectX::XMFLOAT4X4()const
        //{
        //    return ToXMFLOAT4X4(*this);
        //};
    };
    // ��Ԫ����
    class Quaternion {
    public:
        // Ĭ�Ϲ��캯��
        Quaternion();

        // �������Ĺ��캯������ r, i, j, k ��˳��
        Quaternion(double r, double i, double j, double k);

        // �������캯��
        Quaternion(const Quaternion& other);

        // ��������
        ~Quaternion();

        // ���������
        Quaternion operator+(const Quaternion& other) const;
        Quaternion operator-(const Quaternion& other) const;
        Quaternion operator*(const Quaternion& other) const;
        Quaternion operator*(double scalar) const;
        Vector operator*(const Vector&)const;
        Quaternion operator/(double scalar) const;
        Quaternion operator-() const;
        Quaternion& operator+=(const Quaternion& other);
        Quaternion& operator-=(const Quaternion& other);
        Quaternion& operator*=(const Quaternion& other);
        Quaternion& operator*=(double scalar);
        Quaternion& operator/=(double scalar);
        bool operator==(const Quaternion& other) const;
        bool operator!=(const Quaternion& other) const;

        // ������Ա����
        Quaternion Normalize() const;// ����Ԫ����һ��
        double norm() const; // ������Ԫ����ģ��
        double normSquared() const; // ������Ԫ����ģ����ƽ��
        void conjugate(); // ����Ԫ���Ĺ���
        Quaternion conjugated() const; // ������Ԫ���Ĺ���
        void invert(); // ����Ԫ������
        Quaternion inverted() const; // ������Ԫ������
        double dot(const Quaternion& other) const; // ������Ԫ���ĵ��
        Quaternion lerp(const Quaternion& other, double t) const; // ��������Ԫ��֮��������Բ�ֵ
        Quaternion slerp(const Quaternion& other, double t) const; // ��������Ԫ��֮������������Բ�ֵ
        Vector imag()const;
        // ������Ԫ��������ת���Ⱥ���ת��
        //void QuaternionToAxisAngle(const Quaternion& q, Vector3& axis, float& angle);
        Vector GetAxis();
        void RotateAxis(const Vector3& newAxis);
        // ���ص�ǰ��Ԫ����ʾ�Ļ���
        double getAngle() const {
            // ������Ԫ����ģ��
            double norm = std::sqrt(w * w + x * x + y * y + z * z);

            // ���ģ���ӽ����㣬����Ϊ��ת�Ƕȷǳ�С��ֱ�ӷ���0
            if (norm < 1e-6) {
                return 0.0;
            }

            // ����Ԫ�����е�λ��
            double w_prime = w / norm;
            double x_prime = x / norm;
            double y_prime = y / norm;
            double z_prime = z / norm;

            // �����鲿�ĳ���
            double s = std::sqrt(x_prime * x_prime + y_prime * y_prime + z_prime * z_prime);

            // ������ת�Ƕ�
            double theta = 2.0 * std::atan2(s, w_prime);

            return theta;
        }
        // ������Ԫ��������ת����
        static Matrix4 QuaternionToMatrix(const Quaternion& q);

        // ����ŷ���ǹ�����Ԫ��
        static Quaternion fromEulerAngles(double pitch, double yaw, double roll); 
        // �����������ת�ǹ�����Ԫ��
        static Quaternion fromAxisAngle(const Vector& axis, double angle); 
        union {
            struct {
                double r, i, j, k;
            };
            struct {
                double w, x, y, z;
            };
        };
    };
    class Rotation {
    public:
        double angle;       // ��ת�Ƕȣ����ȣ�
        Vector3 axis;     // ��ת������

        // ��ʼ����ת
        Rotation(double angle, Vector3 axis) : angle(angle), axis(axis.Normalize()) {}
        Rotation() : angle(0) ,axis(1,0,0) {}
        // ��ȡ��ת����
        void getMatrix(double mat[][3]) {
            double c = std::cos(angle);
            double s = std::sin(angle);
            double t = 1 - c;
            Vector Axis = axis.Normalize();
            double x = Axis.x;
            double y = Axis.y;
            double z = Axis.z;

            mat[0][0] = t * x * x + c;
            mat[0][1] = t * x * y + s * z;
            mat[0][2] = t * x * z - s * y;
            mat[1][0] = t * x * y - s * z;
            mat[1][1] = t * y * y + c;
            mat[1][2] = t * y * z + s * x;
            mat[2][0] = t * x * z + s * y;
            mat[2][1] = t * y * z - s * x;
            mat[2][2] = t * z * z + c;
        }
        // ��ת����
        Rotation compose(Rotation r)const {
            // ����ǰ��ת�ʹ������ת��ת��Ϊ��Ԫ����ʽ
            Quaternion q1 = Quaternion::fromAxisAngle(axis, angle);
            Quaternion q2 = Quaternion::fromAxisAngle(r.axis, r.angle);

            // ��������Ԫ����˻����õ����Ӻ����Ԫ��
            Quaternion q = q2 * q1;

            // �����Ӻ����Ԫ��ת��Ϊ��Ǳ�ʾ
            double angle = 2 * std::acos(q.w);
            Vector3 axis;
            if (std::fabs(std::sin(angle / 2)) < 1e-8) {
                axis = Vector3(1, 0, 0);
            }
            else {
                axis = Vector3(q.x, q.y, q.z).Normalize();
            }

            return Rotation(angle, axis);
        }



        // ��ת�����
        void decompose(double& angle, Vector3& axis) {
            angle = this->angle;
            axis = this->axis;
        }

        // ��ת���ȷ���
        double radian() {
            return this->angle;
        }

        // �����µ������ת��Ϣ
        void setRotation(double angle, Vector3 axis) {
            this->angle = angle;
            this->axis = axis;
        }
        Rotation getRelativeRotation(Rotation worldRotation, Rotation parentRotation) {
            // ��������ת�͸�������ת��ת��Ϊ��Ԫ��
            Quaternion qWorld = Quaternion::fromAxisAngle(worldRotation.axis, worldRotation.angle);
            Quaternion qParent = Quaternion::fromAxisAngle(parentRotation.axis, parentRotation.angle);

            // ���㸸�����������ת��Ԫ��
            Quaternion qParentWorld = qParent * qWorld;

            // ����ǰģ�͵�������ת��Ԫ��ת��Ϊ��Ǳ�ʾ
            double angle = 2 * std::acos(qParentWorld.w);
            Vector3 axis;
            if (std::fabs(std::sin(angle / 2)) < 1e-8) {
                axis = Vector3(1, 0, 0);
            }
            else {
                axis = Vector3(qParentWorld.x, qParentWorld.y, qParentWorld.z).Normalize();
            }

            // ������ת�����ת�Ƕȹ��ɵ� Rotation ���Ͷ���
            return Rotation(angle, axis);
        }

    };
    //���ܽǶ�
    Quaternion FromAxisAngle(const Vector3& axis, float angle);
    Vector3 Transform(const Vector3& v, const Matrix4& worldToCamera);
    double DegToRad(double Deg);
    double RadToDeg(double Rad);
};

