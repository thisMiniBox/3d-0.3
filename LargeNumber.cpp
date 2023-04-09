#include "LargeNumber.h"
//返回数字位数
int weishu(long long num) { return std::to_string(num).size(); }
//小数字对大数字求商和余数
std::string Div(std::string num, int a, int* mod)
{
	if (num.size() < weishu(a))
	{
		if (num != "")
			*mod = std::stol(num.c_str());
		return "0";
	}
	int minNum = 0;
	std::string quotient;
	for (int i = 0; i < num.size(); i++)
	{
		minNum *= 10;
		minNum += (num[i] - '0');
		quotient += (minNum / a + '0');
		*mod = minNum % a;
		minNum = *mod;
	}
	while (1)
	{
		if (quotient[0] == '0')
			quotient.erase(quotient.begin());
		else return quotient;
	}
}
//初始化数字
BigInt::BigInt(std::string Number)
{
	if (Number[0] == '-') {
		symbol = false;
		Number.erase(Number.begin());
	}
	else symbol = true;
	int mod = 0;
	do
	{
		Number = Div(Number, 256, &mod);
		num.push_back(mod);
		if (Number == "")break;
	} while (Number != "0");
}
BigInt::BigInt(long long Number)
{
	if (Number < 0)
	{
		symbol = false;
		Number = -Number;
	}
	else symbol = true;
	int mod = 0;
	do
	{
		num.push_back(Number % 256);
		Number = Number / 256;
	} while (Number != 0);
}
BigInt::~BigInt()
{
	num.~vector();
}
//查看数字（16进制）
void BigInt::xprint()
{
	if (symbol == false)printf_s("-");
	printf_s("0x:");
	for (int i = num.size() - 1; i >= 0; i--)
		printf_s("%0x ", num[i]);
	printf_s("\n");
}
//查看数字（10进制）
void BigInt::print()
{
	std::string out;

	std::string str("0");

	for (int i = 0; i < num.size(); i++)
	{
		out = "1";
		for (int u = 0; u < i; u++)
		{
			out = MulNumber("256", out);
		}
		str = AddNumber(MulNumber(std::to_string(num[i]), out), str);
	}
	if (symbol == false)str.insert(str.begin(), '-');
	printf_s("%s\n", str.c_str());

	//LargeNumber Lnm = *this;
	//do
	//{
	//	out.insert(out.begin(),(Lnm % 10).dataChar());
	//	Lnm /= 10;
	//} while (Lnm != 0);
	//if (!symbol)out.insert(out.begin(), '-');
	//printf_s("%s", out.c_str());
}
//返回数据
std::string BigInt::data()
{
	std::string out;
	std::string str("0");

	for (int i = 0; i < num.size(); i++)
	{
		out = "1";
		for (int u = 0; u < i; u++)
		{
			out = MulNumber("256", out);
		}
		str = AddNumber(MulNumber(std::to_string(num[i]), out), str);
	}
	if (symbol == false)str.insert(str.begin(), '-');
	return str;
}
char BigInt::dataChar()
{
	if (num.size() > 1 || num[0] > 9)
	{
		printf_s("数组过长或超出范围\n");
		return 0;
	}
	return (num[0] + '0');
}
int BigInt::dataInt()
{
	if (num.size() >= sizeof(int))
	{
		if (num.size() > sizeof(int))
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
		if (num[sizeof(int) - 1] > 128)
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
	}
	int out = 0;
	memcpy(num.data(), &out, sizeof(int));
	return out;
}
unsigned int BigInt::dataUInt()
{
	if (num.size() > sizeof(int))
	{
		printf_s("LNumber的大小已经大于Int的容量\n");
		return 0;
	}
	unsigned int out = 0;
	memcpy(num.data(), &out, sizeof(int));
	return out;
}
long long BigInt::dataLL()
{
	if (num.size() >= sizeof(long long))
	{
		if (num.size() > sizeof(long long))
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
		if (num[sizeof(long long) - 1] > 128)
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
	}
	long long out = 0;
	memcpy(num.data(), &out, sizeof(long long));
	return out;
}
unsigned long long BigInt::dataULL()
{
	if (num.size() > sizeof(long long))
	{
		printf_s("LNumber的大小已经大于Int的容量\n");
		return 0;
	}
	unsigned long long out = 0;
	memcpy(num.data(), &out, sizeof(long long));
	return out;
}
//修改数字
BigInt BigInt::FixNumber(BigInt Number) { return *this = Number; }
BigInt BigInt::FixNumber(std::string Number)
{
	if (Number[0] == '-') {
		symbol = false;
		Number.erase(Number.begin());
	}
	num.clear();
	int mod = 0;
	do
	{
		Number = Div(Number, 256, &mod);
		num.push_back(mod);
		if (Number == "")break;
	} while (Number != "0");
	return *this;
}
BigInt BigInt::FixNumber(long long Number) { this->operator=(Number); return *this; }
//字符串转256大数字
BigInt BigInt::operator=(std::string Number) { return this->FixNumber(Number); }
BigInt BigInt::operator=(BigInt Number) { num = Number.num; symbol = Number.symbol; return Number; }
BigInt BigInt::operator=(long long Number) { return this->FixNumber(std::to_string(Number)); }
BigInt BigInt::operator=(int Number) { return this->FixNumber(std::to_string(Number)); }
bool BigInt::operator!=(BigInt Number)
{
	if (symbol != Number.symbol)
	{
		if (num.size() == 1 && num[0] == 0)return false;
		return true;
	}
	if (this->num.size() != Number.num.size())return true;
	bool identical = false;
	for (int i = 0; i < this->num.size(); i++)
	{
		if (this->num[i] != Number.num[i])
		{
			identical = true;
			break;
		}
	}
	return identical;
}
bool BigInt::operator!=(std::string Number)
{
	BigInt num2(Number);
	return *this != num2;
}
bool BigInt::operator!=(long long Number)
{
	return *this != BigInt(Number);
}
//比较
bool BigInt::compare(std::string Number1, std::string Number2)
{
	if (Number1.size() > Number2.size())
		return 1;
	else if (Number1.size() < Number2.size())
		return -1;
	else return Number1.compare(Number2);     //若长度相等，则从头到尾按位比较
}
bool BigInt::operator==(BigInt Number)
{
	if (symbol != Number.symbol)
	{
		if (num.size() == 1 && num[0] == 0)return true;
		return false;
	}
	if (this->num.size() != Number.num.size())return false;
	bool identical = true;
	for (int i = 0; i < this->num.size(); i++)
	{
		if (this->num[i] != Number.num[i])
		{
			identical = false;
			break;
		}
	}
	return identical;
}
bool BigInt::operator==(std::string Number)
{
	BigInt num2(Number);
	return *this == num2;
}
bool BigInt::operator==(long long Number)
{
	return *this == BigInt(Number);
}
bool BigInt::operator==(int Number)
{
	return *this == BigInt(Number);
}
bool BigInt::operator<(BigInt Number)
{
	if (symbol && Number.symbol)
	{
		if (num.size() < Number.num.size())return true;
		else if (num.size() > Number.num.size())return false;
		else
		{
			for (int i = num.size() - 1; i >= 0; i--)
			{
				if (num[i] > Number.num[i])return false;
				else if (num[i] < Number.num[i])return true;
			}
			return false;
		}
	}
	else if (symbol && !Number.symbol)return false;
	else if (!symbol && Number.symbol)return true;
	else
	{
		bool a = true;
		Number.symbol = true;
		symbol = true;
		a = *this < Number;
		symbol = false;
		return a ? false : true;
	}
}
bool BigInt::operator<=(BigInt Number)
{
	if (symbol && Number.symbol)
	{
		if (num.size() < Number.num.size())return true;
		else if (num.size() > Number.num.size())return false;
		else
		{
			for (int i = num.size() - 1; i >= 0; i--)
			{
				if (num[i] > Number.num[i])return false;
				else if (num[i] < Number.num[i])return true;
			}
			return true;
		}
	}
	else if (symbol && !Number.symbol)return false;
	else if (!symbol && Number.symbol)return true;
	else
	{
		bool a = true;
		Number.symbol = true;
		symbol = true;
		a = *this <= Number;
		symbol = false;
		return a ? false : true;
	}
}
bool BigInt::operator>(BigInt Number)
{
	if (symbol && Number.symbol)
	{
		if (num.size() < Number.num.size())return false;
		else if (num.size() > Number.num.size())return true;
		else
		{
			for (int i = num.size() - 1; i >= 0; i--)
			{
				if (num[i] > Number.num[i])return true;
				else if (num[i] < Number.num[i])return false;
			}
			return false;
		}
	}
	else if (symbol && !Number.symbol)return true;
	else if (!symbol && Number.symbol)return false;
	else
	{
		bool a = true;
		Number.symbol = true;
		symbol = true;
		a = *this > Number;
		symbol = false;
		return a ? false : true;
	}
}
bool BigInt::operator>=(BigInt Number)
{
	if (symbol && Number.symbol)
	{
		if (num.size() < Number.num.size())return false;
		else if (num.size() > Number.num.size())return true;
		else
		{
			for (int i = num.size() - 1; i >= 0; i--)
			{
				if (num[i] > Number.num[i])return true;
				else if (num[i] < Number.num[i])return false;
			}
			return true;
		}
	}
	else if (symbol && !Number.symbol)return true;
	else if (!symbol && Number.symbol)return false;
	else
	{
		bool a = true;
		Number.symbol = true;
		symbol = true;
		a = *this > Number;
		symbol = false;
		return a ? false : true;
	}
}
//加
std::string BigInt::AddNumber(std::string Number1, std::string Number2)
{
	int sign = 1;//sign为符号为
	std::string str;
	if (Number1[0] == '-')
	{
		if (Number2[0] == '-')       //负负
		{
			sign = -1;
			str = AddNumber(Number1.erase(0, 1), Number2.erase(0, 1));//erase(first,last);删除从first到last之间的字符
		}
		else             //负正
		{
			str = SubNumber(Number2, Number1.erase(0, 1));
		}
	}
	else
	{
		if (Number2[0] == '-')        //正负
		{
			str = SubNumber(Number1, Number2.erase(0, 1));
		}
		else                    //正正，把两个整数对齐，短整数前面加0补齐
		{
			std::string::size_type L1, L2;  //string::size_type抽象意义是尺寸单位类型
			int i;
			L1 = Number1.size();
			L2 = Number2.size();
			if (L1 < L2)
			{
				for (i = 0; i < L2 - L1; i++)
					Number1 = "0" + Number1;
			}
			else
			{
				for (i = 0; i < L1 - L2; i++)
					Number2 = "0" + Number2;
			}
			int int1 = 0, int2 = 0; //int2记录进位
			for (i = Number1.size() - 1; i >= 0; i--)
			{
				int1 = (int(Number1[i]) - '0' + int(Number2[i]) - '0' + int2) % 10;
				int2 = (int(Number1[i]) - '0' + int(Number2[i]) - '0' + int2) / 10;
				str = char(int1 + '0') + str;
			}
			if (int2 != 0)str = char(int2 + '0') + str;
		}

	}
	//运算符处理符号
	if ((sign == -1) && (str[0] != '0'))str = "-" + str;
	return str;
}
BigInt BigInt::operator+(BigInt Number)
{
	if (!symbol && !Number.symbol)
	{
		symbol = true;
		Number.symbol = true;
		return *this + Number;
	}
	else if (!symbol && Number.symbol)
	{
		symbol = true;
		return Number - *this;
	}
	else if (symbol && !Number.symbol)
	{
		Number.symbol = true;
		return *this - Number;
	}
	BigInt out;
	int minNum = 0;
	int modNum = 0;
	if (num.size() < Number.num.size())
	{
		for (int i = 0; i < num.size(); i++)
		{
			modNum = num[i] + Number.num[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.num.push_back(modNum);
		}
		for (int i = num.size(); i < Number.num.size(); i++)
		{
			modNum = Number.num[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.num.push_back(modNum);
		}
		if (minNum != 0)out.num.push_back(1);
	}
	else
	{
		for (int i = 0; i < Number.num.size(); i++)
		{
			modNum = num[i] + Number.num[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.num.push_back(modNum);
		}
		for (int i = Number.num.size(); i < num.size(); i++)
		{
			modNum = num[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.num.push_back(modNum);
		}
		if (minNum != 0)out.num.push_back(1);
	}
	return out;
}
BigInt BigInt::operator+(std::string str)
{
	return *this + BigInt(str);
}
BigInt BigInt::operator+(long long Num)
{
	return *this + BigInt(Num);
}
BigInt BigInt::operator+(int Num)
{
	return *this + BigInt(Num);
}
BigInt BigInt::operator+(unsigned char Num)
{
	return *this + BigInt(Num);
}
BigInt operator+(std::string str, BigInt Num1)
{
	return Num1 + str;
}
BigInt operator+(long long INT1, BigInt Num2)
{
	return Num2 + INT1;
}
//减
std::string BigInt::SubNumber(std::string Number1, std::string Number2)
{
	int sign = 1; //sign为符号位
	std::string str;
	int i, j;
	if (Number2[0] == '-')
	{
		str = AddNumber(Number1, Number2.erase(0, 1));
	}
	else
	{
		int res = compare(Number1, Number2);
		if (res == 0)return "0";
		if (res < 0)
		{
			sign = -1;
			std::string temp = Number1;
			Number1 = Number2;
			Number2 = temp;
		}
		std::string::size_type tempint;
		tempint = Number1.size() - Number2.size();
		for (i = Number2.size() - 1; i >= 0; i--)
		{
			if (Number1[i + tempint] < Number2[i])          //借位
			{
				j = 1;
				while (1)
				{
					if (Number1[tempint - j + i] == '0')
					{
						Number1[i + tempint - j] = '9';
						j++;
					}
					else
					{
						Number1[i + tempint - j] = char(int(Number1[i + tempint - j]) - 1);
						break;
					}
				}
				str = char(Number1[i + tempint] - Number2[i] + ':') + str;
			}
			else
			{
				str = char(Number1[i + tempint] - Number2[i] + '0') + str;
			}
		}
		for (i = tempint - 1; i >= 0; i--)
			str = Number1[i] + str;
	}
	//去出结果中多余的前导0
	str.erase(0, str.find_first_not_of('0'));
	if (str.empty())str = "0";
	if ((sign == -1) && (str[0] != '0'))str = "-" + str;
	return str;
}
BigInt BigInt::operator-()
{
	BigInt out = *this;
	if (symbol = true)out.symbol = false;
	else out.symbol = true;
	return out;
}
BigInt BigInt::operator-(BigInt Number)
{
	BigInt out;
	if (!Number.symbol)
	{
		Number.symbol = true;
		out = *this + Number;
		return out;
	}
	if (!symbol)
	{
		symbol = true;
		out = *this + Number;
		symbol = false;
		return out;
	}
	int minNum = 0;
	int modNum = 0;
	if (*this < Number)return -(Number - *this);
	else
	{
		for (int i = 0; i < Number.num.size(); i++)
		{
			modNum = num[i] - Number.num[i] - minNum;
			minNum = 0;
			if (modNum < 0)
			{
				minNum = 1;
				modNum += 256;
			}
			out.num.push_back(modNum);
		}
		for (int i = Number.num.size(); i < num.size(); i++)
		{
			modNum = num[i] - minNum;
			minNum = 0;
			if (modNum < 0)
			{
				minNum = 1;
				modNum += 256;
			}
			out.num.push_back(modNum);
		}
		if (minNum != 0)out.num.push_back(1);
	}
	return out;
}
BigInt BigInt::operator-(std::string str)
{
	return *this - BigInt(str);
}
BigInt BigInt::operator-(long long Num)
{
	return *this - BigInt(Num);
}
BigInt operator-(std::string str, BigInt Num1)
{
	return -Num1 + str;
}
BigInt operator-(long long INT1, BigInt Num2)
{
	return -Num2 + INT1;
}
//乘
std::string BigInt::MulNumber(std::string Number1, std::string Number2)
{
	int sign = 1;
	std::string str = "0";        //记录当前值
	if (Number1[0] == '-')
	{
		sign *= -1;
		Number1 = Number1.erase(0, 1);
	}
	if (Number2[0] == '-')
	{
		sign *= -1;
		Number2 = Number2.erase(0, 1);
	}
	int i, j;
	std::string::size_type L1, L2;
	L1 = Number1.size();
	L2 = Number2.size();
	for (i = L2 - 1; i >= 0; i--)              //模拟手工乘法竖式
	{
		std::string tempstr;
		int int1 = 0, int2 = 0, int3 = int(Number2[i]) - '0';
		if (int3 != 0)
		{
			for (j = 1; j <= (int)(L2 - 1 - i); j++)
				tempstr = "0" + tempstr;
			for (j = L1 - 1; j >= 0; j--)
			{
				int1 = (int3 * (int(Number1[j]) - '0') + int2) % 10;
				int2 = (int3 * (int(Number1[j]) - '0') + int2) / 10;
				tempstr = char(int1 + '0') + tempstr;
			}
			if (int2 != 0)tempstr = char(int2 + '0') + tempstr;
		}
		str = AddNumber(str, tempstr);
	}
	//去除结果中的前导0
	str.erase(0, str.find_first_not_of("0"));
	if (str.empty())str = "0";
	if ((sign == -1) && (str[0] != '0'))str = "-" + str;
	return str;
}
BigInt BigInt::operator*(BigInt Number)
{
	long long campare = 0;
	long long a = 0;
	BigInt z;
	BigInt out;
	for (int i = 0; i < num.size(); i++)
	{
		campare = 0;
		z.num.clear();
		for (int u = 0; u < Number.num.size(); u++)
		{
			a = num[i];
			campare = a * Number.num[u] + campare;
			z.num.push_back(campare % 256);
			campare = campare / 256;
		}
		if (campare != 0)z.num.push_back(campare % 256);
		for (int u = 0; u < i; u++)
			z.num.emplace(z.num.begin(), 0);
		out = out + z;
	}
	for (int i = out.num.size() - 1; i > 0; i--)
	{
		if (out.num[i] == 0)out.num.erase(out.num.end() - 1);
		else break;
	}
	return out;
	//return MulNumber(this->data(), Number.data());
}
BigInt BigInt::operator*(long long Num)
{
	return *this * (BigInt::BigInt(std::to_string(Num)));
}
BigInt BigInt::operator*(int Num)
{
	return *this * (BigInt::BigInt(std::to_string(Num)));
}
BigInt BigInt::operator*(std::string str) { return *this * BigInt(str); }
BigInt operator*(std::string str, BigInt num) { return num * BigInt(str); }
BigInt operator*(long long num1, BigInt num2) { return num2 * BigInt(num1); }
//除
std::string BigInt::DivNumber(std::string Number1, std::string Number2)
{
	std::string quotient, residue;  //定义商和余数
	int sign1 = 1, sign2 = 1;
	if (Number2 == "0")   //判断除数是否为0
	{
		quotient = "ERROR!";
		residue = "ERROR!";
		return quotient;
	}
	if (Number1 == "0")     //判断被除数是否为0
	{
		quotient = "0";
		residue = "0";
	}
	if (Number1[0] == '-')
	{
		Number1 = Number1.erase(0, 1);
		sign1 *= -1;
		sign2 = -1;
	}
	if (Number2[0] == '-')
	{
		Number2 = Number2.erase(0, 1);
		sign1 *= -1;
	}
	int res = compare(Number1, Number2);
	if (res < 0)
	{
		quotient = "0";
		residue = Number1;
	}
	else if (res == 0)
	{
		quotient = "1";
		residue = "0";
	}
	else
	{
		std::string::size_type L1, L2;
		L1 = Number1.size();
		L2 = Number2.size();
		std::string tempstr;
		tempstr.append(Number1, 0, L2 - 1); //将str1中为值0到L2-1的字符串追加到tempstr
		for (int i = L2 - 1; i < L1; i++)  //模拟手工除法竖式
		{
			tempstr = tempstr + Number1[i];
			tempstr.erase(0, tempstr.find_first_not_of('0')); //在字符串中查找第一个与'0'不匹配的字符，返回它的位置
			if (tempstr.empty())tempstr = "0";  //q.empty()，当队列空时，返回true
			for (char ch = '9'; ch >= '0'; ch--) //试商
			{
				std::string str;
				str = str + ch;
				if (compare(MulNumber(Number2, str), tempstr) <= 0)
				{
					quotient = quotient + ch;
					tempstr = SubNumber(tempstr, MulNumber(Number2, str));
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
	return quotient;
}
BigInt BigInt::operator/(BigInt obe)
{
	if (obe == 0)return 0;
	if (*this < obe)return 0;
	BigInt quotient, residue, a;
	int bei = 0;
	for (int i = this->num.size() - 1; i >= 0; i--)
	{
		quotient = quotient * 256;
		residue = residue * 256;
		residue = residue + this->num[i];
		if (residue < obe)continue;
		bei = 255;
		do {
			a = obe * bei;
			if (a <= residue)
			{
				residue = residue - a;
				quotient = quotient + bei;
				break;
			}
			bei--;
		} while (bei > 0);
	}
	for (int i = quotient.num.size() - 1; i > 0; i--)
	{
		if (quotient.num[i] == 0)quotient.num.erase(quotient.num.end() - 1);
		else break;
	}
	return quotient;
}
BigInt BigInt::operator/(long long Num)
{
	return *this / (BigInt(Num));
}
BigInt BigInt::operator/(std::string str) { return *this / BigInt(str); }
BigInt operator/(std::string str, BigInt num) { return num / BigInt(str); }
BigInt operator/(long long num1, BigInt num2) { return num2 / BigInt(num1); }

//余
std::string BigInt::ModNumber(std::string Number1, std::string Number2)
{
	std::string quotient, residue;  //定义商和余数
	int sign1 = 1, sign2 = 1;
	if (Number2 == "0")   //判断除数是否为0
	{
		quotient = "ERROR!";
		residue = "ERROR!";
		return residue;
	}
	if (Number1 == "0")     //判断被除数是否为0
	{
		quotient = "0";
		residue = "0";
	}
	if (Number1[0] == '-')
	{
		Number1 = Number1.erase(0, 1);
		sign1 *= -1;
		sign2 = -1;
	}
	if (Number2[0] == '-')
	{
		Number2 = Number2.erase(0, 1);
		sign1 *= -1;
	}
	int res = compare(Number1, Number2);
	if (res < 0)
	{
		quotient = "0";
		residue = Number1;
	}
	else if (res == 0)
	{
		quotient = "1";
		residue = "0";
	}
	else
	{
		std::string::size_type L1, L2;
		L1 = Number1.size();
		L2 = Number2.size();
		std::string tempstr;
		tempstr.append(Number1, 0, L2 - 1); //将str1中为值0到L2-1的字符串追加到tempstr
		for (int i = L2 - 1; i < L1; i++)  //模拟手工除法竖式
		{
			tempstr = tempstr + Number1[i];
			tempstr.erase(0, tempstr.find_first_not_of('0')); //在字符串中查找第一个与'0'不匹配的字符，返回它的位置
			if (tempstr.empty())tempstr = "0";  //q.empty()，当队列空时，返回true
			for (char ch = '9'; ch >= '0'; ch--) //试商
			{
				std::string str;
				str = str + ch;
				if (compare(MulNumber(Number2, str), tempstr) <= 0)
				{
					quotient = quotient + ch;
					tempstr = SubNumber(tempstr, MulNumber(Number2, str));
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
	return residue;
}
BigInt BigInt::operator%(BigInt obe)
{
	if (obe == 0)return 0;
	if (*this < obe)return *this;
	BigInt quotient, residue, a;
	int bei = 0;
	for (int i = this->num.size() - 1; i >= 0; i--)
	{
		quotient = quotient * 256;
		residue = residue * 256;
		residue = residue + this->num[i];
		if (residue < obe)continue;
		bei = 255;
		do {
			a = obe * bei;
			if (a <= residue)
			{
				residue = residue - a;
				quotient = quotient + bei;
				break;
			}
			bei--;
		} while (bei > 0);
	}
	for (int i = residue.num.size() - 1; i > 0; i--)
	{
		if (residue.num[i] == 0)residue.num.erase(residue.num.end() - 1);
		else break;
	}
	return residue;
}
BigInt BigInt::operator%(long long Num) { return *this % (BigInt::BigInt(std::to_string(Num))); }
BigInt BigInt::operator%(std::string str) { return *this % BigInt(str); }
BigInt operator%(std::string str, BigInt num) { return num % BigInt(str); }
BigInt operator%(long long num1, BigInt num2) { return num2 % BigInt(num1); }
//次方
BigInt BigInt::operator^(BigInt Number)
{
	BigInt out(1);
	if (Number > 0)
	{
		do {
			out = out * *this;
			--Number;
		} while (Number > 0);
	}
	else return 1;
	return out;
}
double BigInt::ToDouble() const
{
	double result = 0.0;
	bool negative = false;
	for (std::vector<uint8_t>::const_reverse_iterator it = num.rbegin(); it != num.rend(); it++) {
		result *= 256.0;
		result += *it;
	}
	if (result < 0) {
		negative = true;
		result = -result;
	}
	int exp = static_cast<int>(log10(result));
	if (negative) {
		result *= -1;
	}
	if (exp >= DBL_MAX_10_EXP) {
		return negative ? -DBL_MAX : DBL_MAX;
	}
	else if (exp <= DBL_MIN_10_EXP) {
		return 0.0;
	}
	else {
		result *= pow(10, -exp);
		return negative ? -result : result;
	}
}


BigInt BigInt::operator^(long long Num) { return *this ^ (BigInt::BigInt(std::to_string(Num))); }
BigInt BigInt::operator^(std::string str) { return *this ^ BigInt(str); }
BigInt operator^(std::string str, BigInt num) { return num ^ BigInt(str); }
BigInt operator^(long long num1, BigInt num2) { return num2 ^ BigInt(num1); }

BigInt BigInt::operator+=(BigInt Num) { return *this = *this + Num; }
BigInt BigInt::operator+=(std::string str) { return *this += BigInt(str); }
BigInt BigInt::operator+=(long long num1) { return *this += BigInt(num1); }
BigInt operator+=(std::string str, BigInt Num) { return Num += BigInt(str); }
BigInt operator+=(long long num1, BigInt Num) { return Num += BigInt(num1); }
BigInt BigInt::operator-=(BigInt Num) { return *this = *this - Num; }
BigInt BigInt::operator-=(std::string str) { return *this -= BigInt(str); }
BigInt BigInt::operator-=(long long num1) { return *this -= BigInt(num1); }
BigInt operator-=(std::string str, BigInt Num) { return Num -= BigInt(str); }
BigInt operator-=(long long num1, BigInt Num) { return Num -= BigInt(num1); }
BigInt BigInt::operator*=(BigInt Num) { return *this = *this * Num; }
BigInt BigInt::operator*=(std::string str) { return *this *= BigInt(str); }
BigInt BigInt::operator*=(long long num1) { return *this *= BigInt(num1); }
BigInt operator*=(std::string str, BigInt Num) { return Num *= BigInt(str); }
BigInt operator*=(long long num1, BigInt Num) { return Num *= BigInt(num1); }
BigInt BigInt::operator/=(BigInt Num) { return *this = *this / Num; }
BigInt BigInt::operator/=(std::string str) { return *this /= BigInt(str); }
BigInt BigInt::operator/=(long long num1) { return *this /= BigInt(num1); }
BigInt operator/=(std::string str, BigInt Num) { return Num /= BigInt(str); }
BigInt operator/=(long long num1, BigInt Num) { return Num /= BigInt(num1); }
BigInt BigInt::operator%=(BigInt Num) { return *this = *this % Num; }
BigInt BigInt::operator%=(std::string str) { return *this %= BigInt(str); }
BigInt BigInt::operator%=(long long num1) { return *this %= BigInt(num1); }
BigInt operator%=(std::string str, BigInt Num) { return Num %= BigInt(str); }
BigInt operator%=(long long num1, BigInt Num) { return Num %= BigInt(num1); }
BigInt BigInt::operator^=(BigInt Num) { return *this = *this ^ Num; }
BigInt BigInt::operator^=(std::string str) { return *this ^= BigInt(str); }
BigInt BigInt::operator^=(long long num1) { return *this ^= BigInt(num1); }
BigInt operator^=(std::string str, BigInt Num) { return Num ^= BigInt(str); }
BigInt operator^=(long long num1, BigInt Num) { return Num ^= BigInt(num1); }
BigInt::operator double() const
{
	double result = 0.0;
	bool negative = false;
	for (std::vector<uint8_t>::const_reverse_iterator it = num.rbegin(); it != num.rend(); it++) {
		result *= 256.0;
		result += *it;
	}
	if (result < 0) {
		negative = true;
		result = -result;
	}
	int exp = static_cast<int>(log10(result));
	if (negative) {
		result *= -1;
	}
	if (exp >= DBL_MAX_10_EXP) {
		return negative ? -DBL_MAX : DBL_MAX;
	}
	else if (exp <= DBL_MIN_10_EXP) {
		return 0.0;
	}
	else {
		result *= pow(10, -exp);
		return negative ? -result : result;
	}
}
BigInt::operator long long() const
{
	long long result = 0;
	for (std::vector<uint8_t>::const_reverse_iterator it = num.rbegin(); it != num.rend(); it++) {
		result *= 256;
		result += *it;
	}
	return result;
}
BigInt::operator int() const
{
	int result = 0;
	for (std::vector<uint8_t>::const_reverse_iterator it = num.rbegin(); it != num.rend(); it++) {
		result *= 256;
		result += *it;
	}
	return result;
}
bool BigInt::operator>(int Number)
{
	return *this > BigInt(Number);
}
bool BigInt::operator<(int Number)
{
	return *this < BigInt(Number);
}
void BigInt::operator--() { *this = *this - "1"; }
void BigInt::operator++() { *this = *this + 1; }
void BigInt::operator--(int) { *this = *this - "1"; }
void BigInt::operator++(int) { *this = *this + 1; }
