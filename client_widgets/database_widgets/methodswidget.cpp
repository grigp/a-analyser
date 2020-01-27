#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsmodel.h"
#include "aanalyserapplication.h"
#include "methodicproxymodel.h"

#include "channelsdefines.h"
#include "channelsutils.h"

#include <QApplication>
#include <QDebug>

MethodsWidget::MethodsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MethodsWidget)
  , m_mdlMethodics(new MetodicsModel(this))
  , m_pmdlMethodics(new MethodicProxyModel(this))
  , m_mdlKinds(new MetodicsKindModel(this))
{
    ui->setupUi(this);

//    ui->tvMetods->viewport()->installEventFilter(this);
}

MethodsWidget::~MethodsWidget()
{
    delete ui;
}

void MethodsWidget::onDbConnect()
{
    if (m_mdlMethodics && m_pmdlMethodics && m_mdlKinds)
    {
        m_mdlMethodics->clear();
        m_mdlMethodics->load();
        m_pmdlMethodics->setSourceModel(m_mdlMethodics);
        ui->tvMetods->setModel(m_pmdlMethodics);
        connect(ui->tvMetods->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MethodsWidget::on_selectMetodicChanged);

        m_mdlKinds->clear();
        m_mdlKinds->load();
        ui->tvKinds->setModel(m_mdlKinds);
        connect(ui->tvKinds->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MethodsWidget::on_selectKindChanged);
    }
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
        auto uid = index.data(MetodicsModel::MetodicUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic(uid);
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

void MethodsWidget::on_selectKindChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    auto kindUid = ui->tvKinds->selectionModel()->currentIndex().data(MetodicsKindModel::MetodicKindUidRole).toString();
    m_pmdlMethodics->selectMetodicKind(kindUid);
}
