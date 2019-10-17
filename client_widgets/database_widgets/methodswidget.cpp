#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsmodel.h"
#include "aanalyserapplication.h"

#include "channelsdefines.h"
#include "channelsutils.h"

#include <QApplication>
#include <QDebug>

MethodsWidget::MethodsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MethodsWidget)
  , m_model(new MetodicsModel(this))
{
    ui->setupUi(this);

    ui->tvMetods->viewport()->installEventFilter(this);
}

MethodsWidget::~MethodsWidget()
{
    delete ui;
}

void MethodsWidget::onDbConnect()
{
    if (m_model)
    {
        m_model->clear();
        m_model->load();
        ui->tvMetods->setModel(m_model);
        connect(ui->tvMetods->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MethodsWidget::on_selectionChanged);
    }
}

bool MethodsWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->tvMetods->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *me = static_cast <QMouseEvent *> (event);
        QModelIndex index = ui->tvMetods->indexAt(me->pos());

        if (!index.isValid())
        {
            ui->tvMetods->clearSelection();
            static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
        }

        return true;
    }

    return QWidget::eventFilter(watched, event);
}

void MethodsWidget::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    selectMetodic(ui->tvMetods->selectionModel()->currentIndex());
}

void MethodsWidget::selectMetodic(const QModelIndex index)
{
    if (index.isValid())
    {
        auto uid = m_model->index(index.row(), MetodicsModel::ColName, index.parent()).
                data(MetodicsModel::MetodicUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic(uid);
    }
}

void MethodsWidget::editMetodParams()
{
    QModelIndexList selIdxs = ui->tvMetods->selectionModel()->selectedIndexes();
    if (m_model && selIdxs.size() > 0)
        m_model->editMetodicParams(this, selIdxs.at(0).row());
}

void MethodsWidget::unselect()
{
    ui->tvMetods->clearSelection();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
}
