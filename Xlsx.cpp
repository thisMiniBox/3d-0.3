#include "Xlsx.h"
bool Xlsx::LoadXlsx(const std::string& filename)
{
    try {
        wb.load(filename);
        ws = wb.active_sheet();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading xlsx file: " << e.what() << std::endl;
        return false;
    }
}

bool Xlsx::SaveXlsx(const std::string& filename) const
{
    try {
        wb.save(filename);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving xlsx file: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> Xlsx::GetSheetNames() const
{
    std::vector<std::string> sheet_names;
    for (const auto& sheet : wb) {
        sheet_names.push_back(sheet.title());
    }
    return sheet_names;
}
void Xlsx::JumpToSheet(const std::string& Sheetname)
{
    ws = wb.sheet_by_title(Sheetname);
}
void Xlsx::AddSheet(const std::string& name)
{
    ws = wb.active_sheet();
    ws.title(name);
}

void Xlsx::RemoveSheet(const std::string& name)
{
    ws = wb.sheet_by_title(name);
    wb.remove_sheet(ws);
    ws = wb.active_sheet();
}

void Xlsx::SetCell(const std::string& sheet_name, const std::string& cell_address, const std::string& cell_value)
{
    ws = wb.sheet_by_title(sheet_name);
    ws.cell(cell_address).value(cell_value);
}

std::string Xlsx::GetCell(const std::string& sheet_name, const std::string& cell_address) const
{
    return  wb.sheet_by_title(sheet_name).cell(cell_address).to_string();
}

void Xlsx::SetFormula(const std::string& sheet_name, const std::string& cell_address, const std::string& formula)
{
    auto sheet = wb[sheet_name];
    auto cell = sheet.cell(xlnt::cell_reference(cell_address));
    cell.formula(formula);
}
void Xlsx::SetCell(const std::string& cell_address, const std::string& cell_value)
{
    ws.cell(cell_address).value(cell_value);
}
void Xlsx::SetCell(unsigned int x, unsigned int y, const std::string& cell_value)
{
    ws.cell(x, y).value(cell_value);
}
std::string Xlsx::GetCell(const std::string& cell_address, const std::string& cell_value)
{
    return ws.cell(cell_address).to_string();
}
std::string Xlsx::GetCell(unsigned int x, unsigned int y, const std::string& cell_value)
{
    return ws.cell(x, y).to_string();
}