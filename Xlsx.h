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
    //加载xlsx文件
    bool LoadXlsx(const std::string& filename);
    //保存xlsx文件
    bool SaveXlsx(const std::string& filename) const;
    //跳转到页
    void JumpToSheet(const std::string& Sheetname);
    //获取页名
    std::vector<std::string> GetSheetNames() const;
    //添加页
    void AddSheet(const std::string& name);
    //删除页
    void RemoveSheet(const std::string& name);
    //设置单元格
    void SetCell(const std::string& cell_address, const std::string& cell_value);
    void SetCell(unsigned int x, unsigned int y, const std::string& cell_value);
    void SetCell(const std::string& sheet_name, const std::string& cell_address, const std::string& cell_value);
    //获取单元格
    std::string GetCell(const std::string& cell_address, const std::string& cell_value);
    std::string GetCell(unsigned int x, unsigned int y, const std::string& cell_value);
    std::string GetCell(const std::string& sheet_name, const std::string& cell_address) const;
    //设置公式
    void SetFormula(const std::string& sheet_name, const std::string& cell_address, const std::string& formula);
};

