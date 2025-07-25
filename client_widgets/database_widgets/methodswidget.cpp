#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsmodel.h"
#include "aanalyserapplication.h"
#include "methodicproxymodel.h"
#include "settingsprovider.h"

#include "channelsdefines.h"
#include "channelsutils.h"
#include "databasewigetdefines.h"
#include "aanalysersettings.h"

#include <QApplication>
#include <QSpacerItem>
#include <QDebug>

MethodsWidget::MethodsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MethodsWidget)
  , m_mdlMethodics(new MetodicsModel(this))
  , m_pmdlMethodics(new MethodicProxyModel(this))
  , m_mdlKinds(new MetodicsKindModel(this))
{
    ui->setupUi(this);

    ui->btnUnselect->setVisible(false);
//    ui->tvMetods->viewport()->installEventFilter(this);

    ui->tvMetods->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvMetods, &QListView::customContextMenuRequested, this, &MethodsWidget::on_popupMenuRequested);
    connect(ui->tvMetods, &QListView::doubleClicked, this, &MethodsWidget::on_doubleClickedTable);

}

MethodsWidget::~MethodsWidget()
{
    delete ui;
}

void MethodsWidget::onDbConnect(const bool isAppEvent)
{
    m_isAppEvent = isAppEvent;

    if (m_mdlMethodics && m_pmdlMethodics && m_mdlKinds)
    {
        m_mdlMethodics->clear();
        m_mdlMethodics->load();
        m_pmdlMethodics->setSourceModel(m_mdlMethodics);
        ui->tvMetods->setModel(m_pmdlMethodics);
//        if (isAppEvent)
            connect(ui->tvMetods->selectionModel(), &QItemSelectionModel::selectionChanged,
                    this, &MethodsWidget::on_selectMetodicChanged);
            connect(ui->tvMetods, &QListView::doubleClicked, this, &MethodsWidget::on_doubleClicked);

        //! Если методика одна, то уберем виджет ее выбора и выберем ее для использования
        static_cast<AAnalyserApplication*>(QApplication::instance())->getMethodicCount(m_mdlMethodics->rowCount());
        if (m_mdlMethodics->rowCount() == 1)
        {
            selectMetodic(m_mdlMethodics->index(0, 0));
            setVisible(false);
        }

        m_mdlKinds->clear();
        m_mdlKinds->load();
        setMethodicKindsButtons();
    }
}

void MethodsWidget::onShow()
{
    //! uid выбранной в приложении методики
    auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
    if (mi.uid == "")
        mi.uid = QUuid().toString();

    //! uid выбранной в таблице методики
    auto metUid = methodic();

    if (mi.uid != metUid)
    {
        ui->tvMetods->selectionModel()->clearSelection();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
    }
}

QString MethodsWidget::methodic() const
{
    auto selMdl = ui->tvMetods->selectionModel();
    if (selMdl)
    {
        auto index = m_pmdlMethodics->mapToSource(selMdl->currentIndex());
        if (index.isValid())
            return index.data(DatabaseWidgetDefines::MetodicsModel::MetodicUidRole).toString();
    }
    return QUuid().toString();
}

void MethodsWidget::setTitleVisible(const bool isVisible)
{
    ui->frTitle->setVisible(isVisible);
}

//bool MethodsWidget::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == ui->tvMetods->viewport() && event->type() == QEvent::MouseButtonRelease)
//    {
//        QMouseEvent *me = static_cast <QMouseEvent *> (event);
//        QModelIndex index = ui->tvMetods->indexAt(me->pos());

//        if (!index.isValid())
//        {
//            ui->tvMetods->clearSelection();
//            static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
//        }

//        return true;
//    }

//    return QWidget::eventFilter(watched, event);
//}

void MethodsWidget::on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    auto index = m_pmdlMethodics->mapToSource(ui->tvMetods->selectionModel()->currentIndex());
    selectMetodic(index);
}

void MethodsWidget::selectMetodic(const QModelIndex index)
{
    if (index.isValid())
    {
        auto uid = index.data(DatabaseWidgetDefines::MetodicsModel::MetodicUidRole).toString();
        emit selectMethod(uid);
        if (m_isAppEvent)
            static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic(uid);
    }
    else
    {
        emit selectMethod("");
        if (m_isAppEvent)
            static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
    }
}

void MethodsWidget::editMetodParams()
{
    QModelIndexList selIdxs = ui->tvMetods->selectionModel()->selectedIndexes();
    if (m_mdlMethodics && selIdxs.size() > 0)
    {
        auto index = m_pmdlMethodics->mapToSource(selIdxs.at(0));
        m_mdlMethodics->editMetodicParams(this, index.row());
    }
}

void MethodsWidget::unselectMetodic()
{
    ui->tvMetods->clearSelection();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
}

void MethodsWidget::on_btnKindPressed()
{
    QObjectList children = ui->frTestKindButtons->children();
    foreach(QObject * child, children)
    {
        auto* btn = dynamic_cast<QPushButton*>(child);
        if (btn)
            btn->setChecked(false);
    }

    QPushButton* btn = static_cast<QPushButton*>(sender());
    btn->setChecked(true);
    m_pmdlMethodics->selectMetodicKind(m_btnToKindUid.value(btn));

    QString title = tr("Методики");
    if (m_btnToKindUid.value(btn) != QUuid().toString())
        title = title + " (" + btn->toolTip() + ")";
 //   else
        unselectMetodic();

    ui->lblTitle->setText(title);

    selectMetodic(QModelIndex());
}

void MethodsWidget::on_doubleClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        auto uid = index.data(DatabaseWidgetDefines::MetodicsModel::MetodicUidRole).toString();
        emit selectMethod(uid, true);
    }
}

void MethodsWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);

        QAction *editParams = new QAction(QIcon(":/images/MethodsEdit.png"), tr("Редактировать параметры методики..."), this);
        connect(editParams, &QAction::triggered, this, &MethodsWidget::editMetodParams);
        m_menu->addAction(editParams);

        QAction *runTest = new QAction(QIcon(":/images/ResultsRun.png"), tr("Провести тест по методике"), this);
        connect(runTest, &QAction::triggered, this, &MethodsWidget::runTest);
        m_menu->addAction(runTest);

    }
    m_menu->popup(ui->tvMetods->mapToGlobal(pos));
}

void MethodsWidget::on_doubleClickedTable(QModelIndex index)
{
    Q_UNUSED(index);
    auto tdcm = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_testDoubleClickMode, BaseDefines::tdcmEditParams).toInt();
    if (BaseDefines::tdcmEditParams == tdcm)
    {
        editMetodParams();
    }
    else
    if (BaseDefines::tdcmRunTest == tdcm)
    {
        runTest();
    }

}

void MethodsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

void MethodsWidget::setMethodicKindsButtons()
{
    m_btnToKindUid.clear();

    //! Очистка панели кнопок
    while (QLayoutItem* item = ui->frTestKindButtons->layout()->takeAt(0))
    {
        delete item->widget();
        delete item;
    }

    for (int i = 0; i < m_mdlKinds->rowCount(); ++i)
    {
        auto btn = new QPushButton(ui->frTestKindButtons);
        btn->setIconSize(QSize(32, 32));
        btn->setObjectName("MethodicKind");
        btn->setCheckable(true);

        m_btnToKindUid.insert(btn, m_mdlKinds->index(i, 0).data(MetodicsKindModel::MetodicKindUidRole).toString());

        if (m_mdlKinds->index(i, 0).data(MetodicsKindModel::MetodicKindUidRole).toString() != QUuid().toString())
        {
            btn->setToolTip(m_mdlKinds->index(i, 0).data().toString());
            btn->setIcon(m_mdlKinds->item(i)->icon());
        }
        else
        {
            btn->setIcon(QIcon(":/images/Methodics/All.png"));
            btn->setToolTip(tr("<Все методики>"));
            btn->setChecked(true);
        }
        connect(btn, &QPushButton::clicked, this, &MethodsWidget::on_btnKindPressed);
        ui->frTestKindButtons->layout()->addWidget(btn);
    }

    auto *spacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->frTestKindButtons->layout()->addItem(spacer);

    if (m_mdlKinds->rowCount() <= 2)
        ui->frTestKindButtons->setVisible(false);
}

