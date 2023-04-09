#pragma once
#include<string>
class large
{
public:
	large() {}
	large(std::string m_str1, char m_ch, std::string m_str2);            //两数的运算
	inline int compare(std::string str1, std::string str2); //相等返回0，大于返回1，小于返回-1
	std::string SUB_INT(std::string str1, std::string str2);   //高精度减法
	std::string ADD_INT(std::string str1, std::string str2);         //高精度加法
	std::string MUL_INT(std::string str1, std::string str2);     //高精度乘法 
	std::string DIVIDE_INT(std::string str1, std::string str2, int flag); //高精度除法，flag==1,返回商;flag==0时，返回余数
	std::string DIV_INT(std::string str1, std::string str2); //高精度除法，返回商
	std::string MOD_INT(std::string str1, std::string str2);  //高精度除法，返回余数
	large(large& e);  //拷贝构造
	~large() {};   //析构函数
};

