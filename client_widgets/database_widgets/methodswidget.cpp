#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsmodel.h"
#include "aanalyserapplication.h"
#include "methodicproxymodel.h"
#include "settingsprovider.h"

#include "channelsdefines.h"
#include "channelsutils.h"

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
        setMethodicKindsButtons();
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
    else
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
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
    ui->lblTitle->setText(title);

    selectMetodic(QModelIndex());
}

void MethodsWidget::setMethodicKindsButtons()
{
    m_btnToKindUid.clear();
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
            btn->setToolTip("<Все методики>");
            btn->setChecked(true);
        }
        connect(btn, &QPushButton::clicked, this, &MethodsWidget::on_btnKindPressed);
        ui->frTestKindButtons->layout()->addWidget(btn);
    }

    auto *spacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->frTestKindButtons->layout()->addItem(spacer);
}

