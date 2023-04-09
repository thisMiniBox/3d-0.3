#pragma once
#include"LargeNumber.h"
class LgFloat
{
	unsigned long long point;
	bool symbol;
	std::vector<char> Int;
	BigInt num;
public:
	LgFloat() { symbol = true; point = 4; }
	LgFloat(std::string Number);
	LgFloat(long long Number);
	~LgFloat();
	LgFloat FixNumber(LgFloat);
	LgFloat FixNumber(std::string Number);
	LgFloat FixNumber(long long Number);
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
	LgFloat operator=(long long);
	LgFloat operator=(std::string);
	LgFloat operator=(LgFloat);

	bool operator==(LgFloat);
	bool operator==(std::string);
	bool operator==(long long);
	bool operator!=(LgFloat);
	bool operator!=(std::string);
	bool operator!=(long long);
	bool operator<(LgFloat);
	bool operator<=(LgFloat);
	bool operator>(LgFloat);
	bool operator>=(LgFloat);

	LgFloat operator+(LgFloat);
	LgFloat operator+(std::string);
	LgFloat operator+(long long);
	friend LgFloat operator+(std::string, LgFloat);
	friend LgFloat operator+(long long, LgFloat);

	LgFloat operator-();
	LgFloat operator-(LgFloat);
	LgFloat operator-(std::string);
	LgFloat operator-(long long);
	friend LgFloat operator-(std::string, LgFloat);
	friend LgFloat operator-(long long, LgFloat);

	LgFloat operator*(LgFloat);
	LgFloat operator*(std::string);
	LgFloat operator*(long long);
	friend LgFloat operator*(std::string, LgFloat);
	friend LgFloat operator*(long long, LgFloat);

	LgFloat operator/(LgFloat);
	LgFloat operator/(std::string);
	LgFloat operator/(long long);
	friend LgFloat operator/(std::string, LgFloat);
	friend LgFloat operator/(long long, LgFloat);

	LgFloat operator%(LgFloat);
	LgFloat operator%(std::string);
	LgFloat operator%(long long);
	friend LgFloat operator%(std::string, LgFloat);
	friend LgFloat operator%(long long, LgFloat);

	LgFloat operator^(LgFloat);
	LgFloat operator^(std::string);
	LgFloat operator^(long long);
	friend LgFloat operator^(std::string, LgFloat);
	friend LgFloat operator^(long long, LgFloat);

	LgFloat operator+=(LgFloat);
	LgFloat operator+=(std::string);
	LgFloat operator+=(long long);
	friend LgFloat operator+=(std::string, LgFloat);
	friend LgFloat operator+=(long long, LgFloat);

	LgFloat operator-=(LgFloat);
	LgFloat operator-=(std::string);
	LgFloat operator-=(long long);
	friend LgFloat operator-=(std::string, LgFloat);
	friend LgFloat operator-=(long long, LgFloat);

	LgFloat operator*=(LgFloat);
	LgFloat operator*=(std::string);
	LgFloat operator*=(long long);
	friend LgFloat operator*=(std::string, LgFloat);
	friend LgFloat operator*=(long long, LgFloat);

	LgFloat operator/=(LgFloat);
	LgFloat operator/=(std::string);
	LgFloat operator/=(long long);
	friend LgFloat operator/=(std::string, LgFloat);
	friend LgFloat operator/=(long long, LgFloat);

	LgFloat operator%=(LgFloat);
	LgFloat operator%=(std::string);
	LgFloat operator%=(long long);
	friend LgFloat operator%=(std::string, LgFloat);
	friend LgFloat operator%=(long long, LgFloat);

	LgFloat operator^=(LgFloat);
	LgFloat operator^=(std::string);
	LgFloat operator^=(long long);
	friend LgFloat operator^=(std::string, LgFloat);
	friend LgFloat operator^=(long long, LgFloat);

	void operator--(void);
	void operator++(void);
	void operator--(int);
	void operator++(int);
};
