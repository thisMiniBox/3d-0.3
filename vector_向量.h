#pragma once
#include<iostream>
#include <iomanip>
#include<cassert>
#include<math.h>
#include <directxmath.h>
#include<glm/glm.hpp>
#define PI 3.14159265358979323846
namespace vec {
    class Vector {
    public:
        double x;
        double y;
        double z;
        // 默认构造函数，初始化为零向量
        Vector();
        Vector(double a);
        // 带参数的构造函数，根据给定的分量初始化向量
        Vector(double x, double y, double z);
        // 析构函数，释放内存
        ~Vector() {}
        // 定义一个静态函数，计算两个向量点乘（内积），返回一个标量值
        static double dot(const Vector& v1, const Vector& v2) {
            return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
        }
        // 获取x分量的值
        double GetX() const {
            return x;
        }
        // 获取y分量的值
        double GetY() const {
            return y;
        }
        // 获取z分量的值
        double GetZ() const {
            return z;
        }
        // 设置x分量的值
        void SetX(double x) {
            this->x = x;
        }
        // 设置y分量的值
        void SetY(double y) {
            this->y = y;
        }
        // 设置z分量的值
        void SetZ(double z) {
            this->z = z;
        }
        // 计算向量的模长（长度）
        double Length() const {
            return sqrt(x * x + y * y + z * z);
        }
        // 计算两个向量之间的夹角（弧度制）
        static double Angle(const Vector& v1, const Vector& v2) {
            return acos(dot(v1, v2) / (v1.Length() * v2.Length()));
        }
        // 计算两个向量之间的距离（欧氏距离）
        static double Distance(const Vector& v1, const Vector& v2) {
            return (v1 - v2).Length();
        }
        Vector Normalize()const;
        Vector Rotate(const Vector& v, const Vector& axis, double angle);
        // 重载运算符，实现加减，点乘叉乘，倍数伸缩，单位向量换算等操作
        Vector Rotate(const Vector& axis, double angle);
        // 重载+运算符，实现两个向量相加，返回一个新的向量对象
        friend Vector operator+(const Vector& v1, const Vector& v2) {
            return Vector(v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z);
        }
        // 重载+=运算符，实现两个向量相加
        Vector operator+=(const Vector& v) {
            return *this = *this + v;
        }

        // 重载-运算符，实现两个向量相减，返回一个新的向量对象
        friend Vector operator-(const Vector& v1, const Vector& v2) {
            return Vector(v1.x - v2.x,
                v1.y - v2.y,
                v1.z - v2.z);
        }
        // 重载-=运算符，实现两个向量相加
        Vector operator-=(const Vector& v) {
            return *this = *this - v;
        }
        // 重载-运算符，实现向量取反，返回一个新的向量对象
        friend Vector operator-(const Vector& v) {
            return !v;
        }

        // 重载*运算符，实现一个向量乘以一个标量，返回一个新的向量对象
        friend double operator*(const Vector& v1, const Vector& v2) {
            return dot(v1, v2);
        }
        // 重载*运算符，实现一个向量乘以一个标量，返回一个新的向量对象
        friend Vector operator*(const Vector& v, double k) {
            return Vector(v.x * k,
                v.y * k,
                v.z * k);
        }

        // 重载*运算符，实现一个标量乘以一个向量，返回一个新的向量对象
        friend Vector operator*(double k, const Vector& v) {
            return (v * k);
        }

        // 重载/运算符，实现一个向量除以一个标量，返回一个新的向量对象
        friend Vector operator/(const Vector& v, double k) {
            if (k == 0) {
                throw "division by zero";
            }
            else {
                return (v * (1 / k));
            }
        }


        // 向量点乘
        double dot(const Vector& v) const {
            return x * v.x + y * v.y + z * v.z;
        }
        // 重载^运算符，实现两个向量叉乘（外积），返回一个新的向量对象
        friend Vector operator^(const Vector& v1, const Vector& v2) {
            return cross(v1, v2);
        }

        // 定义一个静态函数，计算两个向量叉乘（外积），返回一个新的向量对象
        static Vector cross(const Vector& v1, const Vector& v2) {
            return Vector(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
        }
        // 定义一个函数，计算两个向量叉乘（外积），返回一个新的向量对象
        Vector Cross(const Vector& v1)const {
            return Vector(v1.y * z - v1.z * y,
                v1.z * x - v1.x * z,
                v1.x * y - v1.y * x);
        }
        // 重载!运算符，实现向量取反，返回一个新的向量对象
        friend Vector operator!(const Vector& v) {
            return Vector(-v.x, -v.y, -v.z);
        }

        // 重载~运算符，实现向量单位化，返回一个新的向量对象
        friend Vector operator~(const Vector& v) {
            double len = v.Length();
            if (len == 0) {
                throw "zero vector";
            }
            else {
                return (Vector(v / len));
            }
        }

        // 定义一个静态函数 返回表示方向的单位矢

        static Vector unit(const Vector& v) {
            return ~v;
        }

        // 重载==运算符，实现两个向量相等的判断，返回一个布尔值
        friend bool operator==(const Vector& v1, const Vector& v2) {
            return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
        }

        // 重载!=运算符，实现两个向量不等的判断，返回一个布尔值
        friend bool operator!=(const Vector& v1, const Vector& v2) {
            return !(v1 == v2);
        }

        // 重载<<运算符，实现向量的输出格式化，返回一个输出流对象
        friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
            os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
            return os;
        }
        // 宽字符标准输出函数
        friend std::wostream& operator<<(std::wostream& os, const Vector& v) {
            os << L"(" << std::setw(8) << std::fixed << std::setprecision(2) << v.x << L", "
                << std::setw(8) << std::fixed << std::setprecision(2) << v.y << L", "
                << std::setw(8) << std::fixed << std::setprecision(2) << v.z << L")";
            return os;
        }

        // 重载>>运算符，实现向量的输入格式化，返回一个输入流对象
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
    class Vector2 {
        // 私有成员变量
    private:
        double x; // x分量
        double y; // y分量
        // 公有成员函数
    public:
        // 构造函数，初始化x和y
        Vector2(double x = 0, double y = 0) {
            this->x = x;
            this->y = y;
        }
        // 获取x分量的函数
        double getX() const {
            return x;
        }
        // 获取y分量的函数
        double getY() const {
            return y;
        }
        // 设置x分量的函数
        void setX(double x) {
            this->x = x;
        }
        // 设置y分量的函数
        void setY(double y) {
            this->y = y;
        }

        // 计算向量的模（长度）的函数
        double magnitude() const {
            return sqrt(x * x + y * y);
        }
        // 计算向量的方向（角度）的函数，返回弧度值
        double angle() const {
            return atan2(y, x);
        }

        // 重载+运算符，实现向量相加的功能，返回一个新的Vector2D对象
        Vector2 operator+(const Vector2& v) const {
            return Vector2(x + v.x, y + v.y);
        }

        // 重载-运算符，实现向量相减的功能，返回一个新的Vector2D对象
        Vector2 operator-(const Vector2& v) const {
            return Vector2(x - v.x, y - v.y);
        }

        // 重载*运算符，实现向量与标量相乘的功能，返回一个新的Vector2D对象
        Vector2 operator*(double k) const {
            return Vector2(x * k, y * k);
        }

        // 重载/运算符，实现向量与标量相除的功能，返回一个新的Vector2D对象 
        Vector2 operator/(double k) const {
            if (k != 0) {
                return Vector2(x / k, y / k);
            }
            else {
                throw std::runtime_error("Division by zero");
            }
        }

        // 重载==运算符，判断两个向量是否相等（分别比较x和y是否相等）
        bool operator==(const Vector2& v) const {
            return (x == v.x && y == v.y);
        }

        // 重载!=运算符，判断两个向量是否不等（取==运算符结果的反值）
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

        // 构造函数
        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vector4(double a, double b, double c, double d) : x(a), y(b), z(c), w(d) {}

        // 赋值运算符
        Vector4& operator=(const Vector4& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        // 向量加法
        Vector4 operator+(const Vector4& other) const {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        // 向量减法
        Vector4 operator-(const Vector4& other) const {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        // 向量点乘
        double dot(const Vector4& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        // 向量叉乘（仅限于三维向量）
        Vector4 cross(const Vector4& other) const {
            return Vector4(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x,
                0.0f
            );
        }

        // 向量长度
        double length() const {
            return sqrt(dot(*this));
        }

        // 向量归一化
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
        // 构造函数
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



        // 设置元素值
        void Set(int row, int col, double value) {
            m_data[row][col] = value;
        }

        // 获取元素值
        double Get(int row, int col) const {
            return m_data[row][col];
        }

        // 非常量版本的下标运算符
        double* operator[](int row) {
            return m_data[row];
        }

        // 常量版本的下标运算符
        const double* operator[](int row) const {
            return m_data[row];
        }

        // 矩阵加法
        Matrix4 operator+(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result[i][j] = m_data[i][j] + other[i][j];
                }
            }
            return result;
        }

        // 矩阵乘法
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

        // 求逆矩阵
        Matrix4 Inverse() const {
            // TODO: 实现求逆矩阵的算法
            // 这里只是返回一个空的矩阵
            return Matrix4();
        }

        // 求转置矩阵
        Matrix4 Transpose() const {
            Matrix4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result[j][i] = m_data[i][j];
                }
            }
            return result;
        }
        // 移动矩阵的静态函数
        static Matrix4 translate(double x, double y, double z) {
            Matrix4 mat;
            mat.m_data[0][0] = mat.m_data[1][1] = mat.m_data[2][2] = mat.m_data[3][3] = 1.0;
            mat.m_data[0][3] = x;
            mat.m_data[1][3] = y;
            mat.m_data[2][3] = z;
            return mat;
        }

        // 缩放矩阵的静态函数
        static Matrix4 scale(double sx, double sy, double sz) {
            Matrix4 mat;
            mat.m_data[0][0] = sx;
            mat.m_data[1][1] = sy;
            mat.m_data[2][2] = sz;
            mat.m_data[3][3] = 1.0;
            return mat;
        }

        // 绕 X 轴旋转矩阵的静态函数
        static Matrix4 rotateX(double angle) {
            double rad = angle * PI / 180.0;
            double c = cos(rad);
            double s = sin(rad);
            Matrix4 mat;
            mat.m_data[0][0] = 1.0;
            mat.m_data[1][1] = c;
            mat.m_data[1][2] = -s;
            mat.m_data[2][1] = s;
            mat.m_data[2][2] = c;
            mat.m_data[3][3] = 1.0;
            return mat;
        }

        // 绕 Y 轴旋转矩阵的静态函数
        static Matrix4 rotateY(double angle) {
            double rad = angle * PI / 180.0;
            double c = cos(rad);
            double s = sin(rad);
            Matrix4 mat;
            mat.m_data[0][0] = c;
            mat.m_data[0][2] = s;
            mat.m_data[1][1] = 1.0;
            mat.m_data[2][0] = -s;
            mat.m_data[2][2] = c;
            mat.m_data[3][3] = 1.0;
            return mat;
        }

        // 绕 Z 轴旋转矩阵的静态函数
        static Matrix4 rotateZ(double angle) {
            double rad = angle * PI / 180.0;
            double c = cos(rad);
            double s = sin(rad);
            Matrix4 mat;
            mat.m_data[0][0] = c;
            mat.m_data[0][1] = -s;
            mat.m_data[1][0] = s;
            mat.m_data[1][1] = c;
            mat.m_data[2][2] = 1.0;
            mat.m_data[3][3] = 1.0;
            return mat;
        }
        // 移动矩阵的静态函数
        static Matrix4 translate(const vec::Vector& v) {
            Matrix4 mat;
            mat.m_data[0][0] = mat.m_data[1][1] = mat.m_data[2][2] = mat.m_data[3][3] = 1.0;
            mat.m_data[0][3] = v.x;
            mat.m_data[1][3] = v.y;
            mat.m_data[2][3] = v.z;
            return mat;
        }

        // 缩放矩阵的静态函数
        static Matrix4 scale(const vec::Vector& v) {
            Matrix4 mat;
            mat.m_data[0][0] = v.x;
            mat.m_data[1][1] = v.y;
            mat.m_data[2][2] = v.z;
            mat.m_data[3][3] = 1.0;
            return mat;
        }
        // 绕任意轴旋转矩阵的静态函数
        static Matrix4 rotate(const vec::Vector& axis, double angle) {
            // 计算旋转矩阵
            double rad = angle * PI / 180.0;
            double c = cos(rad);
            double s = sin(rad);
            double t = 1 - c;
            double x = axis.x, y = axis.y, z = axis.z;
            double m11 = t * x * x + c;
            double m12 = t * x * y - s * z;
            double m13 = t * x * z + s * y;
            double m21 = t * x * y + s * z;
            double m22 = t * y * y + c;
            double m23 = t * y * z - s * x;
            double m31 = t * x * z - s * y;
            double m32 = t * y * z + s * x;
            double m33 = t * z * z + c;

            // 将旋转矩阵存储到 4x4 矩阵中
            Matrix4 mat;
            mat.m_data[0][0] = m11;
            mat.m_data[0][1] = m12;
            mat.m_data[0][2] = m13;
            mat.m_data[0][3] = 0.0;
            mat.m_data[1][0] = m21;
            mat.m_data[1][1] = m22;
            mat.m_data[1][2] = m23;
            mat.m_data[1][3] = 0.0;
            mat.m_data[2][0] = m31;
            mat.m_data[2][1] = m32;
            mat.m_data[2][2] = m33;
            mat.m_data[2][3] = 0.0;
            mat.m_data[3][0] = 0.0;
            mat.m_data[3][1] = 0.0;
            mat.m_data[3][2] = 0.0;
            mat.m_data[3][3] = 1.0;

            return mat;
        }
        // 设置缩放矩阵
        void SetScale(const Vector3& scale);

        // 设置旋转矩阵
        void SetRotation(const Vector3& axis, double angle);

        // 获取平移向量
        Vector3 GetTranslation() const;

        // 设置平移矩阵
        void SetTranslation(const Vector3& pos);

        // 获取缩放向量
        Vector3 GetScale() const;

        // 获取逆矩阵
        Matrix4 GetInverse() const;

        // 设置透视投影矩阵
        void SetPerspective(double fov, double aspect, double nearClip, double farClip);
        // 输出到标准输出流
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
    // 四元数类
    class Quaternion {
    public:
        // 默认构造函数
        Quaternion();

        // 带参数的构造函数（按 r, i, j, k 的顺序）
        Quaternion(double r, double i, double j, double k);

        // 拷贝构造函数
        Quaternion(const Quaternion& other);

        // 析构函数
        ~Quaternion();

        // 重载运算符
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

        // 其他成员函数
        Quaternion Normalize() const;// 将四元数归一化
        double norm() const; // 计算四元数的模长
        double normSquared() const; // 计算四元数的模长的平方
        void conjugate(); // 求四元数的共轭
        Quaternion conjugated() const; // 返回四元数的共轭
        void invert(); // 求四元数的逆
        Quaternion inverted() const; // 返回四元数的逆
        double dot(const Quaternion& other) const; // 计算四元数的点积
        Quaternion lerp(const Quaternion& other, double t) const; // 在两个四元数之间进行线性插值
        Quaternion slerp(const Quaternion& other, double t) const; // 在两个四元数之间进行球面线性插值
        Vector imag()const;
        // 根据四元数分离旋转弧度和旋转轴
        //void QuaternionToAxisAngle(const Quaternion& q, Vector3& axis, float& angle);
        Vector GetAxis();
        void RotateAxis(const Vector3& newAxis);
        // 返回当前四元数表示的弧度
        double getAngle() const {
            // 计算四元数的模长
            double norm = std::sqrt(w * w + x * x + y * y + z * z);

            // 如果模长接近于零，则认为旋转角度非常小，直接返回0
            if (norm < 1e-6) {
                return 0.0;
            }

            // 对四元数进行单位化
            double w_prime = w / norm;
            double x_prime = x / norm;
            double y_prime = y / norm;
            double z_prime = z / norm;

            // 计算虚部的长度
            double s = std::sqrt(x_prime * x_prime + y_prime * y_prime + z_prime * z_prime);

            // 计算旋转角度
            double theta = 2.0 * std::atan2(s, w_prime);

            return theta;
        }
        // 根据四元数计算旋转矩阵
        static Matrix4 QuaternionToMatrix(const Quaternion& q);

        // 根据欧拉角构造四元数
        static Quaternion fromEulerAngles(double pitch, double yaw, double roll); 
        // 根据轴向和旋转角构造四元数
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
        double angle;       // 旋转角度（弧度）
        Vector3 axis;     // 旋转轴向量

        // 初始化旋转
        Rotation(double angle, Vector3 axis) : angle(angle), axis(axis.Normalize()) {}
        Rotation() : angle(0) ,axis(1,0,0) {}
        // 获取旋转矩阵
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
        // 旋转叠加
        Rotation compose(Rotation r) {
            // 将当前旋转和传入的旋转都转换为四元数形式
            Quaternion q1 = Quaternion::fromAxisAngle(axis, angle);
            Quaternion q2 = Quaternion::fromAxisAngle(r.axis, r.angle);

            // 对两个四元数求乘积，得到叠加后的四元数
            Quaternion q = q2 * q1;

            // 将叠加后的四元数转换为轴角表示
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



        // 旋转轴分离
        void decompose(double& angle, Vector3& axis) {
            angle = this->angle;
            axis = this->axis;
        }

        // 旋转弧度分离
        double radian() {
            return this->angle;
        }

        // 接收新的轴角旋转信息
        void setRotation(double angle, Vector3 axis) {
            this->angle = angle;
            this->axis = axis;
        }
    };
    //接受角度
    Quaternion FromAxisAngle(const Vector3& axis, float angle);
    Vector3 Transform(const Vector3& v, const Matrix4& worldToCamera);
    double DegToRad(double Deg);
    double RadToDeg(double Rad);
};

