#ifndef ACTIVEPERSONALPROGRAMEDITOR_H
#define ACTIVEPERSONALPROGRAMEDITOR_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class ActivePersonalProgramEditor;
}

class ActivePersonalProgramEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ActivePersonalProgramEditor(QWidget *parent = nullptr);
    ~ActivePersonalProgramEditor();

    /*!
     * \brief Отображает данные индивидуальной программы
     * \param objPPAll - индивидуальная программа
     */
    void setPersonalProgram(const QJsonObject& objPPAll);

    /*!
     * \brief Возвращает отредактированную персональную программу
     */
    QJsonObject personalProgram() const;

private:
    Ui::ActivePersonalProgramEditor *ui;

    void prepareParams();
};

#endif // ACTIVEPERSONALPROGRAMEDITOR_H
