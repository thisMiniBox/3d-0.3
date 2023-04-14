#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<limits.h>
#include <sstream>
class BigInt
{
	bool symbol;
	std::vector<unsigned char>num;
public:
	BigInt() { symbol = true; };
	BigInt(std::string Number);
	BigInt(long long Number);
	~BigInt();
	BigInt FixNumber(BigInt);
	BigInt FixNumber(std::string Number);
	BigInt FixNumber(long long Number);
	void xprint();
	void print();
	std::string data();
	char dataChar();
	int dataInt();
	long long dataLL();
	unsigned int dataUInt();
	unsigned long long dataULL();
	bool compare(std::string Number1, std::string Number2);
	std::string AddNumber(std::string Number1, std::string Number2);
	std::string SubNumber(std::string Number1, std::string Number2);
	std::string MulNumber(std::string Number1, std::string Number2);
	std::string DivNumber(std::string Number1, std::string Number2);
	std::string ModNumber(std::string Number1, std::string Number2);
	double ToDouble() const;
	BigInt operator=(long long);
	BigInt operator=(int);
	BigInt operator=(std::string);
	BigInt operator=(BigInt);
	operator double() const;
	operator long long() const;
	operator int() const;
	bool operator==(BigInt);
	bool operator==(std::string);
	bool operator==(long long);
	bool operator==(int);
	bool operator!=(BigInt);
	bool operator!=(std::string);
	bool operator!=(long long);
	bool operator<(BigInt);
	bool operator<=(BigInt);
	bool operator>(int Number);
	bool operator<(int Number);
	bool operator>(BigInt);
	bool operator>=(BigInt);

	BigInt operator+(BigInt);
	BigInt operator+(std::string);
	BigInt operator+(long long);
	BigInt operator+(int);
	BigInt operator+(unsigned char);
	friend BigInt operator+(std::string, BigInt);
	friend BigInt operator+(long long, BigInt);

	BigInt operator-();
	BigInt operator-(BigInt);
	BigInt operator-(std::string);
	BigInt operator-(long long);
	friend BigInt operator-(std::string, BigInt);
	friend BigInt operator-(long long, BigInt);

	BigInt operator*(BigInt);
	BigInt operator*(std::string);
	BigInt operator*(long long);
	BigInt operator*(int);
	friend BigInt operator*(std::string, BigInt);
	friend BigInt operator*(long long, BigInt);

	BigInt operator/(BigInt);
	BigInt operator/(std::string);
	BigInt operator/(long long);
	friend BigInt operator/(std::string, BigInt);
	friend BigInt operator/(long long, BigInt);

	BigInt operator%(BigInt);
	BigInt operator%(std::string);
	BigInt operator%(long long);
	friend BigInt operator%(std::string, BigInt);
	friend BigInt operator%(long long, BigInt);

	BigInt operator^(BigInt);
	BigInt operator^(std::string);
	BigInt operator^(long long);
	friend BigInt operator^(std::string, BigInt);
	friend BigInt operator^(long long, BigInt);

	BigInt operator+=(BigInt);
	BigInt operator+=(std::string);
	BigInt operator+=(long long);
	friend BigInt operator+=(std::string, BigInt);
	friend BigInt operator+=(long long, BigInt);

	BigInt operator-=(BigInt);
	BigInt operator-=(std::string);
	BigInt operator-=(long long);
	friend BigInt operator-=(std::string, BigInt);
	friend BigInt operator-=(long long, BigInt);

	BigInt operator*=(BigInt);
	BigInt operator*=(std::string);
	BigInt operator*=(long long);
	friend BigInt operator*=(std::string, BigInt);
	friend BigInt operator*=(long long, BigInt);

	BigInt operator/=(BigInt);
	BigInt operator/=(std::string);
	BigInt operator/=(long long);
	friend BigInt operator/=(std::string, BigInt);
	friend BigInt operator/=(long long, BigInt);

	BigInt operator%=(BigInt);
	BigInt operator%=(std::string);
	BigInt operator%=(long long);
	friend BigInt operator%=(std::string, BigInt);
	friend BigInt operator%=(long long, BigInt);

	BigInt operator^=(BigInt);
	BigInt operator^=(std::string);
	BigInt operator^=(long long);
	friend BigInt operator^=(std::string, BigInt);
	friend BigInt operator^=(long long, BigInt);

	void operator--(void);
	void operator++(void);
	void operator--(int);
	void operator++(int);
};
int weishu(long long num);
std::string Div(std::string num, int a, int* mod);
class BigDecimal {
public:
	BigDecimal() : negative(false), exponent(0) {}

	// ���캯��
	BigDecimal(const std::string& str) : negative(false), exponent(0)
	{
		// ȥ��ǰ����ͽ�β�Ŀո�
		std::string s = trim(str);

		// ������
		if (!s.empty() && s[0] == '-') {
			negative = true;
			s.erase(0, 1);
		}

		// �����������ֺ�С������
		std::string intPart, fracPart;
		size_t pos = s.find('.');
		if (pos == std::string::npos) {
			intPart = s;
			fracPart = "";
		}
		else {
			intPart = s.substr(0, pos);
			fracPart = s.substr(pos + 1);
		}

		// ������������
		digits.reserve(intPart.size());
		for (size_t i = 0; i < intPart.size(); i++) {
			digits.push_back(intPart[i] - '0');
		}

		// ����С������
		if (!fracPart.empty()) {
			std::reverse(fracPart.begin(), fracPart.end());
			for (size_t i = 0; i < fracPart.size(); i++) {
				digits.push_back(fracPart[i] - '0');
				exponent--;
			}
		}

		// ȥ��ĩβ����
		while (!digits.empty() && digits.back() == 0) {
			digits.pop_back();
		}

		// ������Ϊ�㣬�����÷��ź�ָ��
		if (digits.empty()) {
			negative = false;
			exponent = 0;
		}
	}
	std::string to_string() const
	{
		std::ostringstream oss;
		if (negative) {
			oss << '-';
		}
		if (digits.empty()) {
			oss << "0";
			return oss.str();
		}
		size_t dotPos = digits.size() + exponent;
		for (size_t i = 0; i < digits.size(); i++) {
			if (i == dotPos) {
				oss << '.';
			}
			oss << static_cast<char>(digits[i] + '0');
		}
		while (dotPos < 0) {
			oss << "0";
			dotPos++;
		}
		if (dotPos == 0 && digits.empty()) {
			oss << "0";
		}
		return oss.str();
	}
	// �������
	friend std::ostream& operator<<(std::ostream& os, const BigDecimal& v)
	{
		if (v.negative) {
			os << '-';
		}
		if (v.digits.empty()) {
			os << "0";
			return os;
		}
		size_t dotPos = v.digits.size() + v.exponent;
		for (size_t i = 0; i < v.digits.size(); i++) {
			if (i == dotPos) {
				os << '.';
			}
			os << static_cast<char>(v.digits[i] + '0');
		}
		while (dotPos < 0) {
			os << "0";
			dotPos++;
		}
		if (dotPos == 0 && v.digits.empty()) {
			os << "0";
		}
		return os;
	}

private:
	bool negative;              // ����λ
	std::vector<int> digits;    // ��ֵ����
	int exponent;               // ָ��

	// ����������ȥ���ַ���ǰ��Ŀո�
	static std::string trim(const std::string& s)
	{
		size_t first = s.find_first_not_of(' ');
		if (first == std::string::npos) {
			return "";
		}
		size_t last = s.find_last_not_of(' ');
		return s.substr(first, last - first + 1);
	}
};
