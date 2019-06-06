#ifndef DATABASERESULTWIDGET_H
#define DATABASERESULTWIDGET_H

#include <QWidget>
#include "clientwidget.h"

namespace Ui {
class DataBaseResultWidget;
}

namespace ClientWidgets
{
    static const QString uidDatabaseResultWidgetUid = "CW_DatabaseResult";
}

/*!
 * \brief Класс масштабируемого pixmap ScaledPixmap
 */
class ScaledPixmap : public QWidget
{
public:
      ScaledPixmap(QWidget *parent = 0);

      void setScaledPixmap(const QPixmap &pixmap);

protected:
      void paintEvent(QPaintEvent *event);

private:
      QPixmap m_pixmap;
};



class DataBaseResultWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit DataBaseResultWidget(QWidget *parent = 0);
    ~DataBaseResultWidget();

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

    void onDBConnect() override;

    void onDBDisconnect() override;

private slots:
    void onSelectTest(const QString &testUid);

private:
    Ui::DataBaseResultWidget *ui;

    ScaledPixmap *m_pmp;
    QWidget *m_wgtResult {nullptr};  ///< Текущий виджет для показа результатов теста
};

#endif // DATABASERESULTWIDGET_H
