#pragma once
#include<string>
class large
{
public:
	large() {}
	large(std::string m_str1, char m_ch, std::string m_str2);            //����������
	inline int compare(std::string str1, std::string str2); //��ȷ���0�����ڷ���1��С�ڷ���-1
	std::string SUB_INT(std::string str1, std::string str2);   //�߾��ȼ���
	std::string ADD_INT(std::string str1, std::string str2);         //�߾��ȼӷ�
	std::string MUL_INT(std::string str1, std::string str2);     //�߾��ȳ˷� 
	std::string DIVIDE_INT(std::string str1, std::string str2, int flag); //�߾��ȳ�����flag==1,������;flag==0ʱ����������
	std::string DIV_INT(std::string str1, std::string str2); //�߾��ȳ�����������
	std::string MOD_INT(std::string str1, std::string str2);  //�߾��ȳ�������������
	large(large& e);  //��������
	~large() {};   //��������
};

