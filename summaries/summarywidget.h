#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SummaryWidget;
}

class Summary;

/*!
 * \brief Класс виджета отображения сводки The SummaryWidget class
 */
class SummaryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryWidget(QWidget *parent = nullptr);
    ~SummaryWidget();

    void setModel(Summary *model);
    Summary* model() const;

    void setSpan(const int row, const int col, const int columnSpan);

    /*!
     * \brief Возвращает индекс в модели, выбранный пользователем
     */
    QModelIndex selectedIndex();

private slots:
    void on_ItemSelected(QModelIndex index);
    void on_modify(const bool isModify);

private:
    Ui::SummaryWidget *ui;
};

#endif // SUMMARYWIDGET_H
