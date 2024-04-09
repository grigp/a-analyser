#ifndef METHODSWIDGET_H
#define METHODSWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QPushButton>
#include <QItemSelectionModel>
#include <QMenu>

namespace Ui {
class MethodsWidget;
}

class MetodicsModel;
class MetodicsKindModel;
class MethodicProxyModel;

class MethodsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MethodsWidget(QWidget *parent = nullptr);
    ~MethodsWidget();

    void onDbConnect(const bool isEvent = true);

    void onShow();

    /*!
     * \brief Возвращает uid выбранной методики
     */
    QString methodic() const;

    /*!
     * \brief Устанавливает видимость панели заголовка
     * \param isVisible - ключ видимости
     */
    void setTitleVisible(const bool isVisible);

//protected:
//    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void selectMetodic(const QModelIndex index);
    void editMetodParams();
    void unselectMetodic();

    void on_btnKindPressed();
    void on_doubleClicked(const QModelIndex &index);

    void on_popupMenuRequested(QPoint pos);

    void on_doubleClickedTable(QModelIndex index);

    void runTest();

signals:
    void selectMethod(const QString& uid, const bool isAccept = false);

private:
    Ui::MethodsWidget *ui;

    void setMethodicKindsButtons();

    MetodicsModel *m_mdlMethodics;
    MethodicProxyModel *m_pmdlMethodics;
    MetodicsKindModel *m_mdlKinds;

    QMap<QPushButton*, QString> m_btnToKindUid;

    bool m_isAppEvent {true};
    QMenu* m_menu {nullptr};
};

#endif // METHODSWIDGET_H
