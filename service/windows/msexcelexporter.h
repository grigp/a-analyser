#ifndef MSEXCELEXPORTER_H
#define MSEXCELEXPORTER_H

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>
#include <QString>


#ifdef Q_OS_WIN32
class MSExcelExporter
{
public:
    MSExcelExporter(const MSExcelExporter& other) = delete;
    MSExcelExporter& operator=(const MSExcelExporter& other) = delete;

    MSExcelExporter(bool closeExcelOnExit = false);
    ~MSExcelExporter();

    void setCellValue(int lineIndex, int columnIndex, const QString& value);
    void saveAs(const QString& fileName);


private:
    QAxObject* m_excelApplication;
    QAxObject* m_workbooks;
    QAxObject* m_workbook;
    QAxObject* m_sheets;
    QAxObject* m_sheet;
    bool m_closeExcelOnExit;
};
#endif

#endif // MSEXCELEXPORTER_H
