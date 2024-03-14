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
    explicit TrenTakePutParamsDialog(QWidget *parent = nullptr);
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

private slots:
    void onPhisioChanEnabledChange(bool enabled);

    void on_ok();
    void on_cancel();
    void on_default();

private:
    Ui::TrenTakePutParamsDialog *ui;

    QJsonObject m_params;

    void initUi();
};

#endif // TRENTAKEPUTPARAMSDIALOG_H
