#include "testproxymodel.h"

#include "testsmodel.h"
#include "aanalyserapplication.h"

#include <QDebug>

TestProxyModel::TestProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectPatient,
            this, &TestProxyModel::onSelectPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectMetodic,
            this, &TestProxyModel::onSelectMetodic);
}

bool TestProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idxPat = sourceModel()->index(source_row,
                                              TestsModel::ColPatient,
                                              source_parent);
    auto patUid = idxPat.data(TestsModel::PatientUidRole).toString();

    QModelIndex idxMet = sourceModel()->index(source_row,
                                              TestsModel::ColMetodic,
                                              source_parent);
    auto metUid = idxMet.data(TestsModel::MetodicUidRole).toString();

    bool patientOk = ((m_selectPatientUid != "") && (m_selectPatientUid == patUid)) || (m_selectPatientUid == "");
    bool metodicOk = ((m_selectMetodicUid != "") && (m_selectMetodicUid == metUid)) || (m_selectMetodicUid == "");
    return patientOk && metodicOk;
}

bool TestProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    if (source_column == TestsModel::ColPatient)
        return m_selectPatientUid == "";
    else
    if (source_column == TestsModel::ColMetodic)
        return m_selectMetodicUid == "";
    return true;
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
