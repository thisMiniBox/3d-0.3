#include <iostream>
#include<string>
#include<cmath>
#include "large.h"
using namespace std;
large::large(string m_str1, char m_ch, string m_str2)//两数的运算
{

	int m_ilocation1;
	int m_ilocation2;
	string m_res;
	int m_istr;
	if (m_str1.find_first_of(".") == -1)
		m_ilocation1 = 0;
	else
		m_ilocation1 = m_str1.length() - m_str1.find_first_of(".") - 1;
	if (m_str2.find_first_of(".") == -1)
		m_ilocation2 = 0;
	else
		m_ilocation2 = m_str2.length() - m_str2.find_first_of(".") - 1;
	if (m_ilocation1 == 0 && m_ilocation2 == 0)
	{
		switch (m_ch)
		{
		case'+':
			m_res = ADD_INT(m_str1, m_str2); break;
		case'-':
			m_res = SUB_INT(m_str1, m_str2); break;
		case'*':
			m_res = MUL_INT(m_str1, m_str2); break;
		case'/':
			m_res = DIV_INT(m_str1, m_str2); break;
		case'%':
			m_res = MOD_INT(m_str1, m_str2); break;
		default:
			break;
		}
	}

	else
	{
		int m_ilocat = m_ilocation1 - m_ilocation2;
		int m_ilocation3;
		if (m_ilocation1 != 0)
			m_str1.erase(m_str1.find_first_of("."), m_str1.find_first_not_of(".") + 1);
		if (m_ilocation2 != 0)
			m_str2.erase(m_str2.find_first_of("."), m_str2.find_first_not_of(".") + 1);

		switch (m_ch)
		{
		case'+':
			if (m_ilocat >= 0)
			{
				for (int i = 0; i < m_ilocat; i++)
					m_str2 = m_str2 + '0';
				m_ilocation3 = m_ilocation1;
			}
			else
			{
				for (int i = 0; i < -m_ilocat; i++)
					m_str1 = m_str1 + '0';
				m_ilocation3 = m_ilocation2;
			}
			m_res = ADD_INT(m_str1, m_str2);
			m_istr = m_res.length();
			for (int i = 0; i < m_ilocation3; i++)
				m_istr = m_istr - 1;
			m_res.insert(m_istr, ".");
			break;
		case'-':
			if (m_ilocat >= 0)
			{
				for (int i = 0; i < m_ilocat; i++)
					m_str2 = m_str2 + '0';
				m_ilocation3 = m_ilocation1;
			}
			else
			{
				for (int i = 0; i < -m_ilocat; i++)
					m_str1 = m_str1 + '0';
				m_ilocation3 = m_ilocation2;
			}
			m_res = SUB_INT(m_str1, m_str2);
			m_istr = m_res.length();
			for (int i = 0; i < m_ilocation3; i++)
				m_istr = m_istr - 1;
			m_res.insert(m_istr, ".");
			break;
		case'*':
			m_ilocation3 = m_ilocation1 + m_ilocation2;
			m_res = MUL_INT(m_str1, m_str2);
			m_istr = m_res.length();
			for (int i = 0; i < m_ilocation3; i++)
				m_istr = m_istr - 1;
			m_res.insert(m_istr, "."); break;
		case'/':
			if (m_ilocat >= 0)
			{
				for (int i = 0; i < m_ilocat; i++)
					m_str2 = m_str2 + '0';
			}
			else
			{
				for (int i = 0; i < -m_ilocat; i++)
					m_str1 = m_str1 + '0';
			}
			m_res = DIV_INT(m_str1, m_str2); break;
		case'%':
			m_res = MOD_INT(m_str1, m_str2); break;
		default:
			break;
		}

	}
	cout << endl;
	cout << m_res << endl;
}
inline int large::compare(string str1, string str2) //相等返回0，大于返回1，小于返回-1
{
	if (str1.size() > str2.size())
		return 1;
	else if (str1.size() < str2.size())
		return -1;
	else return str1.compare(str2);     //若长度相等，则从头到尾按位比较
}
string large::ADD_INT(string str1, string str2)         //高精度加法
{
	int sign = 1;//sign为符号为
	string str;
	if (str1[0] == '-')
	{
		if (str2[0] == '-')       //负负
		{
			sign = -1;
			str = ADD_INT(str1.erase(0, 1), str2.erase(0, 1));//erase(first,last);删除从first到last之间的字符
		}
		else             //负正
		{
			str = SUB_INT(str2, str1.erase(0, 1));
		}
	}
	else
	{
		if (str2[0] == '-')        //正负
		{
			str = SUB_INT(str1, str2.erase(0, 1));
		}
		else                    //正正，把两个整数对齐，短整数前面加0补齐
		{
			string::size_type L1, L2;  //string::size_type抽象意义是尺寸单位类型
			int i;
			L1 = str1.size();
			L2 = str2.size();
			if (L1 < L2)
			{
				for (i = 0; i < L2 - L1; i++)
					str1 = "0" + str1;
			}
			else
			{
				for (i = 0; i < L1 - L2; i++)
					str2 = "0" + str2;
			}
			int int1 = 0, int2 = 0; //int2记录进位
			for (i = str1.size() - 1; i >= 0; i--)
			{
				int1 = (int(str1[i]) - '0' + int(str2[i]) - '0' + int2) % 10;
				int2 = (int(str1[i]) - '0' + int(str2[i]) - '0' + int2) / 10;
				str = char(int1 + '0') + str;
			}
			if (int2 != 0)str = char(int2 + '0') + str;
		}

	}
	//运算符处理符号
	if ((sign == -1) && (str[0] != '0'))str = "-" + str;
	return str;
}

string large::SUB_INT(string str1, string str2)  //高精度减法
{
	int sign = 1; //sign为符号位
	string str;
	int i, j;
	if (str2[0] == '-')
	{
		str = ADD_INT(str1, str2.erase(0, 1));
	}
	else
	{
		int res = compare(str1, str2);
		if (res == 0)return "0";
		if (res < 0)
		{
			sign = -1;
			string temp = str1;
			str1 = str2;
			str2 = temp;
		}
		string::size_type tempint;
		tempint = str1.size() - str2.size();
		for (i = str2.size() - 1; i >= 0; i--)
		{
			if (str1[i + tempint] < str2[i])          //借位
			{
				j = 1;
				while (1)
				{
					if (str1[tempint - j + i] == '0')
					{
						str1[i + tempint - j] = '9';
						j++;
					}
					else
					{
						str1[i + tempint - j] = char(int(str1[i + tempint - j]) - 1);
						break;
					}
				}
				str = char(str1[i + tempint] - str2[i] + ':') + str;
			}
			else
			{
				str = char(str1[i + tempint] - str2[i] + '0') + str;
			}
		}
		for (i = tempint - 1; i >= 0; i--)
			str = str1[i] + str;
	}
	//去出结果中多余的前导0
	str.erase(0, str.find_first_not_of('0'));
	if (str.empty())str = "0";
	if ((sign == -1) && (str[0] != '0'))str = "-" + str;
	return str;
}

string large::MUL_INT(string str1, string str2)     //高精度乘法 
{
	int sign = 1;
	string str = "0";        //记录当前值
	if (str1[0] == '-')
	{
		sign *= -1;
		str1 = str1.erase(0, 1);
	}
	if (str2[0] == '-')
	{
		sign *= -1;
		str2 = str2.erase(0, 1);
	}
	int i, j;
	string::size_type L1, L2;
	L1 = str1.size();
	L2 = str2.size();
	for (i = L2 - 1; i >= 0; i--)              //模拟手工乘法竖式
	{
		string tempstr;
		int int1 = 0, int2 = 0, int3 = int(str2[i]) - '0';
		if (int3 != 0)
		{
			for (j = 1; j <= (int)(L2 - 1 - i); j++)
				tempstr = "0" + tempstr;
			for (j = L1 - 1; j >= 0; j--)
			{
				int1 = (int3 * (int(str1[j]) - '0') + int2) % 10;
				int2 = (int3 * (int(str1[j]) - '0') + int2) / 10;
				tempstr = char(int1 + '0') + tempstr;
			}
			if (int2 != 0)tempstr = char(int2 + '0') + tempstr;
		}
		str = ADD_INT(str, tempstr);
	}
	//去除结果中的前导0
	str.erase(0, str.find_first_not_of("0"));
	if (str.empty())str = "0";
	if ((sign == -1) && (str[0] != '0'))str = "-" + str;
	return str;
}

string large::DIVIDE_INT(string str1, string str2, int flag) //高精度除法，flag==1,返回商;flag==0时，返回余数
{
	string quotient, residue;  //定义商和余数
	int sign1 = 1, sign2 = 1;
	if (str2 == "0")   //判断除数是否为0
	{
		quotient = "ERROR!";
		residue = "ERROR!";
		if (flag == 1)return quotient;
		else return residue;
	}
	if (str1 == "0")     //判断被除数是否为0
	{
		quotient = "0";
		residue = "0";
	}
	if (str1[0] == '-')
	{
		str1 = str1.erase(0, 1);
		sign1 *= -1;
		sign2 = -1;
	}
	if (str2[0] == '-')
	{
		str2 = str2.erase(0, 1);
		sign1 *= -1;
	}
	int res = compare(str1, str2);
	if (res < 0)
	{
		quotient = "0";
		residue = str1;
	}
	else if (res == 0)
	{
		quotient = "1";
		residue = "0";
	}
	else
	{
		string::size_type L1, L2;
		L1 = str1.size();
		L2 = str2.size();
		string tempstr;
		tempstr.append(str1, 0, L2 - 1); //将str1中为值0到L2-1的字符串追加到tempstr
		for (int i = L2 - 1; i < L1; i++)  //模拟手工除法竖式
		{
			tempstr = tempstr + str1[i];
			tempstr.erase(0, tempstr.find_first_not_of('0')); //在字符串中查找第一个与'0'不匹配的字符，返回它的位置
			if (tempstr.empty())tempstr = "0";  //q.empty()，当队列空时，返回true
			for (char ch = '9'; ch >= '0'; ch--) //试商
			{
				string str;
				str = str + ch;
				if (compare(MUL_INT(str2, str), tempstr) <= 0)
				{
					quotient = quotient + ch;
					tempstr = SUB_INT(tempstr, MUL_INT(str2, str));
					break;
				}
			}
		}
		residue = tempstr;
	}
	//去除结果中的前导0
	quotient.erase(0, quotient.find_first_not_of("0"));
	if (quotient.empty())quotient = "0";
	if ((sign1 == -1) && (quotient[0] != '0'))quotient = "-" + quotient;
	if ((sign2 == -1) && (residue[0] != '0'))residue = "-" + residue;
	if (flag == 1)return quotient;
	else
		return residue;
}
string large::DIV_INT(string str1, string str2) //高精度除法，返回商
{
	return DIVIDE_INT(str1, str2, 1);
}
string large::MOD_INT(string str1, string str2)  //高精度除法，返回余数
{
	return DIVIDE_INT(str1, str2, 0);
}