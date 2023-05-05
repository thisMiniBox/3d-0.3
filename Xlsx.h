#pragma once
#include<iostream>
#include<string>
#include<xlnt/xlnt.hpp>
class Xlsx
{
    xlnt::workbook wb;
    xlnt::worksheet ws;
public:
    Xlsx() = default;
    ~Xlsx() = default;
    //����xlsx�ļ�
    bool LoadXlsx(const std::string& filename);
    //����xlsx�ļ�
    bool SaveXlsx(const std::string& filename) const;
    //��ת��ҳ
    void JumpToSheet(const std::string& Sheetname);
    //��ȡҳ��
    std::vector<std::string> GetSheetNames() const;
    //���ҳ
    void AddSheet(const std::string& name);
    //ɾ��ҳ
    void RemoveSheet(const std::string& name);
    //���õ�Ԫ��
    void SetCell(const std::string& cell_address, const std::string& cell_value);
    void SetCell(unsigned int x, unsigned int y, const std::string& cell_value);
    void SetCell(const std::string& sheet_name, const std::string& cell_address, const std::string& cell_value);
    //��ȡ��Ԫ��
    std::string GetCell(const std::string& cell_address, const std::string& cell_value);
    std::string GetCell(unsigned int x, unsigned int y, const std::string& cell_value);
    std::string GetCell(const std::string& sheet_name, const std::string& cell_address) const;
    //���ù�ʽ
    void SetFormula(const std::string& sheet_name, const std::string& cell_address, const std::string& formula);
};

