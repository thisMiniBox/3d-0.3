#include "LgFloat.h"
//初始化数字
LgFloat::LgFloat(std::string Number)
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
		Int.push_back(mod);
		if (Number == "")break;
	} while (Number != "0");
}
LgFloat::LgFloat(long long Number)
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
		Int.push_back(Number % 256);
		Number = Number / 256;
	} while (Number != 0);
}
LgFloat::~LgFloat()
{
	Int.~vector();
}
//查看数字（16进制）
void LgFloat::xprint()
{
	if (symbol == false)printf_s("-");
	printf_s("0x:");
	for (int i = Int.size() - 1; i >= 0; i--)
		printf_s("%0x ", Int[i]);
	printf_s("\n");
}
//查看数字（10进制）
void LgFloat::print()
{
	std::string out;

	std::string str("0");

	for (int i = 0; i < Int.size(); i++)
	{
		out = "1";
		for (int u = 0; u < i; u++)
		{
			out = MulNumber("256", out);
		}
		str = AddNumber(MulNumber(std::to_string(Int[i]), out), str);
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
std::string LgFloat::data()
{
	std::string out;
	std::string str("0");

	for (int i = 0; i < Int.size(); i++)
	{
		out = "1";
		for (int u = 0; u < i; u++)
		{
			out = MulNumber("256", out);
		}
		str = AddNumber(MulNumber(std::to_string(Int[i]), out), str);
	}
	if (symbol == false)str.insert(str.begin(), '-');
	return str;
}
char LgFloat::dataChar()
{
	if (Int.size() > 1 || Int[0] > 9)
	{
		printf_s("数组过长或超出范围\n");
		return 0;
	}
	return (Int[0] + '0');
}
int LgFloat::dataInt()
{
	if (Int.size() >= sizeof(int))
	{
		if (Int.size() > sizeof(int))
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
		if (Int[sizeof(int) - 1] > 128)
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
	}
	int out = 0;
	memcpy(Int.data(), &out, sizeof(int));
	return out;
}
unsigned int LgFloat::dataUInt()
{
	if (Int.size() > sizeof(int))
	{
		printf_s("LNumber的大小已经大于Int的容量\n");
		return 0;
	}
	unsigned int out = 0;
	memcpy(Int.data(), &out, sizeof(int));
	return out;
}
long long LgFloat::dataLL()
{
	if (Int.size() >= sizeof(long long))
	{
		if (Int.size() > sizeof(long long))
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
		if (Int[sizeof(long long) - 1] > 128)
		{
			printf_s("LNumber的大小已经大于Int的容量\n");
			return 0;
		}
	}
	long long out = 0;
	memcpy(Int.data(), &out, sizeof(long long));
	return out;
}
unsigned long long LgFloat::dataULL()
{
	if (Int.size() > sizeof(long long))
	{
		printf_s("LNumber的大小已经大于Int的容量\n");
		return 0;
	}
	unsigned long long out = 0;
	memcpy(Int.data(), &out, sizeof(long long));
	return out;
}
//修改数字
LgFloat LgFloat::FixNumber(LgFloat Number) { return *this = Number; }
LgFloat LgFloat::FixNumber(std::string Number)
{
	if (Number[0] == '-') {
		symbol = false;
		Number.erase(Number.begin());
	}
	Int.clear();
	int mod = 0;
	do
	{
		Number = Div(Number, 256, &mod);
		Int.push_back(mod);
		if (Number == "")break;
	} while (Number != "0");
	return *this;
}
LgFloat LgFloat::FixNumber(long long Number) { this->operator=(Number); return *this; }
//字符串转256大数字
LgFloat LgFloat::operator=(std::string Number) { return this->FixNumber(Number); }
LgFloat LgFloat::operator=(LgFloat Number) { Int = Number.Int; symbol = Number.symbol; return Number; }
LgFloat LgFloat::operator=(long long Number) { return this->FixNumber(std::to_string(Number)); }
bool LgFloat::operator!=(LgFloat Number)
{
	if (symbol != Number.symbol)
	{
		if (Int.size() == 1 && Int[0] == 0)return false;
		return true;
	}
	if (this->Int.size() != Number.Int.size())return true;
	bool identical = false;
	for (int i = 0; i < this->Int.size(); i++)
	{
		if (this->Int[i] != Number.Int[i])
		{
			identical = true;
			break;
		}
	}
	return identical;
}
bool LgFloat::operator!=(std::string Number)
{
	LgFloat num2(Number);
	return *this != num2;
}
bool LgFloat::operator!=(long long Number)
{
	return *this != LgFloat(Number);
}
//比较
bool LgFloat::compare(std::string Number1, std::string Number2)
{
	if (Number1.size() > Number2.size())
		return 1;
	else if (Number1.size() < Number2.size())
		return -1;
	else return Number1.compare(Number2);     //若长度相等，则从头到尾按位比较
}
bool LgFloat::operator==(LgFloat Number)
{
	if (symbol != Number.symbol)
	{
		if (Int.size() == 1 && Int[0] == 0)return true;
		return false;
	}
	if (this->Int.size() != Number.Int.size())return false;
	bool identical = true;
	for (int i = 0; i < this->Int.size(); i++)
	{
		if (this->Int[i] != Number.Int[i])
		{
			identical = false;
			break;
		}
	}
	return identical;
}
bool LgFloat::operator==(std::string Number)
{
	LgFloat num2(Number);
	return *this == num2;
}
bool LgFloat::operator==(long long Number)
{
	return *this == LgFloat(Number);
}
bool LgFloat::operator<(LgFloat Number)
{
	if (symbol && Number.symbol)
	{
		if (Int.size() < Number.Int.size())return true;
		else if (Int.size() > Number.Int.size())return false;
		else
		{
			for (int i = Int.size() - 1; i >= 0; i--)
			{
				if (Int[i] > Number.Int[i])return false;
				else if (Int[i] < Number.Int[i])return true;
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
bool LgFloat::operator<=(LgFloat Number)
{
	if (symbol && Number.symbol)
	{
		if (Int.size() < Number.Int.size())return true;
		else if (Int.size() > Number.Int.size())return false;
		else
		{
			for (int i = Int.size() - 1; i >= 0; i--)
			{
				if (Int[i] > Number.Int[i])return false;
				else if (Int[i] < Number.Int[i])return true;
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
bool LgFloat::operator>(LgFloat Number)
{
	if (symbol && Number.symbol)
	{
		if (Int.size() < Number.Int.size())return false;
		else if (Int.size() > Number.Int.size())return true;
		else
		{
			for (int i = Int.size() - 1; i >= 0; i--)
			{
				if (Int[i] > Number.Int[i])return true;
				else if (Int[i] < Number.Int[i])return false;
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
bool LgFloat::operator>=(LgFloat Number)
{
	if (symbol && Number.symbol)
	{
		if (Int.size() < Number.Int.size())return false;
		else if (Int.size() > Number.Int.size())return true;
		else
		{
			for (int i = Int.size() - 1; i >= 0; i--)
			{
				if (Int[i] > Number.Int[i])return true;
				else if (Int[i] < Number.Int[i])return false;
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
std::string LgFloat::AddNumber(std::string Number1, std::string Number2)
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
LgFloat LgFloat::operator+(LgFloat Number)
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
	LgFloat out;
	int minNum = 0;
	int modNum = 0;
	if (Int.size() < Number.Int.size())
	{
		for (int i = 0; i < Int.size(); i++)
		{
			modNum = Int[i] + Number.Int[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.Int.push_back(modNum);
		}
		for (int i = Int.size(); i < Number.Int.size(); i++)
		{
			modNum = Number.Int[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.Int.push_back(modNum);
		}
		if (minNum != 0)out.Int.push_back(1);
	}
	else
	{
		for (int i = 0; i < Number.Int.size(); i++)
		{
			modNum = Int[i] + Number.Int[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.Int.push_back(modNum);
		}
		for (int i = Number.Int.size(); i < Int.size(); i++)
		{
			modNum = Int[i] + minNum;
			minNum = 0;
			if (modNum > 255)
			{
				minNum = 1;
				modNum -= 256;
			}
			out.Int.push_back(modNum);
		}
		if (minNum != 0)out.Int.push_back(1);
	}
	return out;
}
LgFloat LgFloat::operator+(std::string str)
{
	return *this + LgFloat(str);
}
LgFloat LgFloat::operator+(long long Num)
{
	return *this + LgFloat(Num);
}
LgFloat operator+(std::string str, LgFloat Num1)
{
	return Num1 + str;
}
LgFloat operator+(long long INT1, LgFloat Num2)
{
	return Num2 + INT1;
}
//减
std::string LgFloat::SubNumber(std::string Number1, std::string Number2)
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
LgFloat LgFloat::operator-()
{
	LgFloat out = *this;
	if (symbol = true)out.symbol = false;
	else out.symbol = true;
	return out;
}
LgFloat LgFloat::operator-(LgFloat Number)
{
	LgFloat out;
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
		for (int i = 0; i < Number.Int.size(); i++)
		{
			modNum = Int[i] - Number.Int[i] - minNum;
			minNum = 0;
			if (modNum < 0)
			{
				minNum = 1;
				modNum += 256;
			}
			out.Int.push_back(modNum);
		}
		for (int i = Number.Int.size(); i < Int.size(); i++)
		{
			modNum = Int[i] - minNum;
			minNum = 0;
			if (modNum < 0)
			{
				minNum = 1;
				modNum += 256;
			}
			out.Int.push_back(modNum);
		}
		if (minNum != 0)out.Int.push_back(1);
	}
	return out;
}
LgFloat LgFloat::operator-(std::string str)
{
	return *this - LgFloat(str);
}
LgFloat LgFloat::operator-(long long Num)
{
	return *this - LgFloat(Num);
}
LgFloat operator-(std::string str, LgFloat Num1)
{
	return -Num1 + str;
}
LgFloat operator-(long long INT1, LgFloat Num2)
{
	return -Num2 + INT1;
}
//乘
std::string LgFloat::MulNumber(std::string Number1, std::string Number2)
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
LgFloat LgFloat::operator*(LgFloat Number)
{
	long long campare = 0;
	long long a = 0;
	LgFloat z;
	LgFloat out;
	for (int i = 0; i < Int.size(); i++)
	{
		campare = 0;
		z.Int.clear();
		for (int u = 0; u < Number.Int.size(); u++)
		{
			a = Int[i];
			campare = a * Number.Int[u] + campare;
			z.Int.push_back(campare % 256);
			campare = campare / 256;
		}
		if (campare != 0)z.Int.push_back(campare % 256);
		for (int u = 0; u < i; u++)
			z.Int.emplace(z.Int.begin(), 0);
		out = out + z;
	}
	for (int i = out.Int.size() - 1; i > 0; i--)
	{
		if (out.Int[i] == 0)out.Int.erase(out.Int.end() - 1);
		else break;
	}
	return out;
	//return MulNumber(this->data(), Number.data());
}
LgFloat LgFloat::operator*(long long Num)
{
	return *this * (LgFloat::LgFloat(std::to_string(Num)));
}
LgFloat LgFloat::operator*(std::string str) { return *this * LgFloat(str); }
LgFloat operator*(std::string str, LgFloat num) { return num * LgFloat(str); }
LgFloat operator*(long long num1, LgFloat num2) { return num2 * LgFloat(num1); }
//除
std::string LgFloat::DivNumber(std::string Number1, std::string Number2)
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
LgFloat LgFloat::operator/(LgFloat obe)
{
	if (obe == 0)return 0;
	if (*this < obe)return 0;
	LgFloat quotient, residue, a;
	int bei = 0;
	for (int i = this->Int.size() - 1; i >= 0; i--)
	{
		quotient = quotient * 256;
		residue = residue * 256;
		residue = residue + this->Int[i];
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
	for (int i = quotient.Int.size() - 1; i > 0; i--)
	{
		if (quotient.Int[i] == 0)quotient.Int.erase(quotient.Int.end() - 1);
		else break;
	}
	return quotient;
}
LgFloat LgFloat::operator/(long long Num)
{
	return *this / (LgFloat(Num));
}
LgFloat LgFloat::operator/(std::string str) { return *this / LgFloat(str); }
LgFloat operator/(std::string str, LgFloat num) { return num / LgFloat(str); }
LgFloat operator/(long long num1, LgFloat num2) { return num2 / LgFloat(num1); }

//余
std::string LgFloat::ModNumber(std::string Number1, std::string Number2)
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
LgFloat LgFloat::operator%(LgFloat obe)
{
	if (obe == 0)return 0;
	if (*this < obe)return *this;
	LgFloat quotient, residue, a;
	int bei = 0;
	for (int i = this->Int.size() - 1; i >= 0; i--)
	{
		quotient = quotient * 256;
		residue = residue * 256;
		residue = residue + this->Int[i];
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
	for (int i = residue.Int.size() - 1; i > 0; i--)
	{
		if (residue.Int[i] == 0)residue.Int.erase(residue.Int.end() - 1);
		else break;
	}
	return residue;
}
LgFloat LgFloat::operator%(long long Num) { return *this % (LgFloat::LgFloat(std::to_string(Num))); }
LgFloat LgFloat::operator%(std::string str) { return *this % LgFloat(str); }
LgFloat operator%(std::string str, LgFloat num) { return num % LgFloat(str); }
LgFloat operator%(long long num1, LgFloat num2) { return num2 % LgFloat(num1); }
//次方
LgFloat LgFloat::operator^(LgFloat Number)
{
	LgFloat out(1);
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
LgFloat LgFloat::operator^(long long Num) { return *this ^ (LgFloat::LgFloat(std::to_string(Num))); }
LgFloat LgFloat::operator^(std::string str) { return *this ^ LgFloat(str); }
LgFloat operator^(std::string str, LgFloat num) { return num ^ LgFloat(str); }
LgFloat operator^(long long num1, LgFloat num2) { return num2 ^ LgFloat(num1); }

LgFloat LgFloat::operator+=(LgFloat Num) { return *this = *this + Num; }
LgFloat LgFloat::operator+=(std::string str) { return *this += LgFloat(str); }
LgFloat LgFloat::operator+=(long long num1) { return *this += LgFloat(num1); }
LgFloat operator+=(std::string str, LgFloat Num) { return Num += LgFloat(str); }
LgFloat operator+=(long long num1, LgFloat Num) { return Num += LgFloat(num1); }
LgFloat LgFloat::operator-=(LgFloat Num) { return *this = *this - Num; }
LgFloat LgFloat::operator-=(std::string str) { return *this -= LgFloat(str); }
LgFloat LgFloat::operator-=(long long num1) { return *this -= LgFloat(num1); }
LgFloat operator-=(std::string str, LgFloat Num) { return Num -= LgFloat(str); }
LgFloat operator-=(long long num1, LgFloat Num) { return Num -= LgFloat(num1); }
LgFloat LgFloat::operator*=(LgFloat Num) { return *this = *this * Num; }
LgFloat LgFloat::operator*=(std::string str) { return *this *= LgFloat(str); }
LgFloat LgFloat::operator*=(long long num1) { return *this *= LgFloat(num1); }
LgFloat operator*=(std::string str, LgFloat Num) { return Num *= LgFloat(str); }
LgFloat operator*=(long long num1, LgFloat Num) { return Num *= LgFloat(num1); }
LgFloat LgFloat::operator/=(LgFloat Num) { return *this = *this / Num; }
LgFloat LgFloat::operator/=(std::string str) { return *this /= LgFloat(str); }
LgFloat LgFloat::operator/=(long long num1) { return *this /= LgFloat(num1); }
LgFloat operator/=(std::string str, LgFloat Num) { return Num /= LgFloat(str); }
LgFloat operator/=(long long num1, LgFloat Num) { return Num /= LgFloat(num1); }
LgFloat LgFloat::operator%=(LgFloat Num) { return *this = *this % Num; }
LgFloat LgFloat::operator%=(std::string str) { return *this %= LgFloat(str); }
LgFloat LgFloat::operator%=(long long num1) { return *this %= LgFloat(num1); }
LgFloat operator%=(std::string str, LgFloat Num) { return Num %= LgFloat(str); }
LgFloat operator%=(long long num1, LgFloat Num) { return Num %= LgFloat(num1); }
LgFloat LgFloat::operator^=(LgFloat Num) { return *this = *this ^ Num; }
LgFloat LgFloat::operator^=(std::string str) { return *this ^= LgFloat(str); }
LgFloat LgFloat::operator^=(long long num1) { return *this ^= LgFloat(num1); }
LgFloat operator^=(std::string str, LgFloat Num) { return Num ^= LgFloat(str); }
LgFloat operator^=(long long num1, LgFloat Num) { return Num ^= LgFloat(num1); }

void LgFloat::operator--() { *this = *this - "1"; }
void LgFloat::operator++() { *this = *this + "1"; }
void LgFloat::operator--(int) { *this = *this - "1"; }
void LgFloat::operator++(int) { *this = *this + "1"; }