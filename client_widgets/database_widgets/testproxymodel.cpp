#include "testproxymodel.h"

#include "testsmodel.h"
#include "aanalyserapplication.h"
#include "databasewigetdefines.h"

#include <QTimer>
#include <QDateTime>
#include <QDebug>

TestProxyModel::TestProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    QTimer::singleShot(100, [=]()
    {
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectPatient,
                this, &TestProxyModel::onSelectPatient);
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectMetodic,
                this, &TestProxyModel::onSelectMetodic);

        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::newTest, [=](const QString &testUid)
        {
            Q_UNUSED(testUid);
            invalidate();
        });
    });
}

bool TestProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idxPat = sourceModel()->index(source_row,
                                              DatabaseWidgetDefines::TestsModel::ColPatient,
                                              source_parent);
    auto patUid = idxPat.data(DatabaseWidgetDefines::TestsModel::PatientUidRole).toString();

    QModelIndex idxMet = sourceModel()->index(source_row,
                                              DatabaseWidgetDefines::TestsModel::ColMetodic,
                                              source_parent);
    auto metUid = idxMet.data(DatabaseWidgetDefines::TestsModel::MetodicUidRole).toString();

    bool patientOk = ((m_selectPatientUid != "") && (m_selectPatientUid == patUid)) || (m_selectPatientUid == "");
    bool metodicOk = ((m_selectMetodicUid != "") && (m_selectMetodicUid == metUid)) || (m_selectMetodicUid == "");
    return patientOk && metodicOk;
}

bool TestProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    if (source_column == DatabaseWidgetDefines::TestsModel::ColPatient)
        return m_selectPatientUid == "";
    else
    if (source_column == DatabaseWidgetDefines::TestsModel::ColMetodic)
        return m_selectMetodicUid == "";
    return true;
}

bool TestProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (source_left.column() == DatabaseWidgetDefines::TestsModel::ColDateTime &&
        source_right.column() == DatabaseWidgetDefines::TestsModel::ColDateTime)
    {
        auto sLeft = sourceModel()->data(source_left).toString();
        auto sRight = sourceModel()->data(source_right).toString();
        auto dtLeft = QDateTime::fromString(sLeft, "dd.MM.yyyy hh:mm");
        auto dtRight = QDateTime::fromString(sRight, "dd.MM.yyyy hh:mm");
        return dtLeft < dtRight;
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

void TestProxyModel::onSelectPatient(const QString &patientUid)
{
    m_selectPatientUid = patientUid;
    invalidate();
}

void TestProxyModel::onSelectMetodic(const QString &metodicUid)
{
    m_selectMetodicUid = metodicUid;
    invalidate();
}
