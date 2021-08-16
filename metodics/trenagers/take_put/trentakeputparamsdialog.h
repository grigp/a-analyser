#ifndef TRENTAKEPUTPARAMSDIALOG_H
#define TRENTAKEPUTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class TrenTakePutParamsDialog;
}

class TrenTakePutParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrenTakePutParamsDialog(QWidget *parent = 0);
    ~TrenTakePutParamsDialog();

    /*!
     * \brief Получает параметры для редактирования
     * \param params - параметры
     */
    void setParams(const QJsonObject &params);

    /*!
     * \brief Возвращает отредактированные параметры
     */
    QJsonObject getParams();

private:
    Ui::TrenTakePutParamsDialog *ui;

    void initUi();
};

#endif // TRENTAKEPUTPARAMSDIALOG_H
