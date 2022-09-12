#ifndef SELECTTRANSFORMERDIALOG_H
#define SELECTTRANSFORMERDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QJsonObject>

namespace Ui {
class SelectTransformerDialog;
}

class SignalTransformerParamsWidget;

/*!
 * \brief Класс диалога выбора трансформера и его параметров The SelectTransformerDialog class
 */
class SelectTransformerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTransformerDialog(QWidget *parent = nullptr);
    ~SelectTransformerDialog();

    enum ListTransformModelRoles
    {
        UidRole = Qt::UserRole + 0
    };

    /*!
     * \brief Возвращает uid преобразователя сигнала
     * В случае ошибки возвращает QUuid().toString()
     */
    QString transformer() const;

    /*!
     * \brief Возвращает параметры преобразователя сигнала в виде json объекта
     * В случае ошибки возвращает QJsonObject()
     */
    QJsonObject params() const;

private slots:
    void on_selectItem(QModelIndex index);

private:
    Ui::SelectTransformerDialog *ui;

    SignalTransformerParamsWidget *m_wgtParams {nullptr};
    QString m_uid {""};

    void load();
};

#endif // SELECTTRANSFORMERDIALOG_H
