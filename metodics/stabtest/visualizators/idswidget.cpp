#include "idswidget.h"
#include "ui_idswidget.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "aanalyserapplication.h"
#include "settingsprovider.h"
#include "soundgenerator.h"
#include "testresultdata.h"
#include "channelsdefines.h"
#include "resultinfo.h"

#include "idscalculator.h"
#include "idsfactors.h"
#include "anysignal.h"

#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QByteArray>
#include <QBuffer>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QTimer>
#include <QDebug>

IDSWidget::IDSWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IDSWidget)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);
    restoreSplitterPosition();
    ui->tvFactors->viewport()->installEventFilter(this);
    ui->sldVolume->setValue(m_volume);
}

IDSWidget::~IDSWidget()
{
    if (m_fds)
        delete m_fds;
    if (m_trd)
        delete m_trd;
    if (m_tmStopSound > -1)
    {
        killTimer(m_tmStopSound);
        doneAudio();
    }
    delete ui;
}

void IDSWidget::calculate(IDSCalculator *calculator, const QString &testUid)
{
    m_calculator = calculator;
    m_testUid = testUid;

    QStringList headerLabels;
    headerLabels << tr("Показатель");

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
                headerLabels << pi.name;
        }

    addFactorsFromMultifactor(calculator);

//    for (int i = 0; i < calculator->factors(0)->fdsSize(); ++i)
//        qDebug() << calculator->factors(0)->fds(i);


    //! Подвяжем делегат проигрывания звука
    for (int i = 0; i < m_mdlTable.rowCount(); ++i)
        if (m_mdlTable.item(i)->data(FactorUidRole).toString() == IDSFactorsDefines::FreqUid)
        {
            auto dlgPS = new PlaySoundDelegate(ui->tvFactors);
            ui->tvFactors->setItemDelegateForRow(i, dlgPS);
            break;
        }
    ui->tvFactors->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::CurrentChanged);

    m_mdlTable.setHorizontalHeaderLabels(headerLabels);
    ui->tvFactors->setModel(&m_mdlTable);
    ui->tvFactors->resizeColumnToContents(0);
//    for (int i = 0; i < m_mdlTable.columnCount(); ++i)
//        ui->tvFactors->resizeColumnToContents(i);
    for (int i = 1; i < m_mdlTable.columnCount(); ++i)
        ui->tvFactors->header()->resizeSection(i, 100);

    ui->wgtSKG->setVisibleMarker(false);
}

void IDSWidget::timerEvent(QTimerEvent *event)
{
    QWidget::timerEvent(event);
    if (event->timerId() == m_tmStopSound)
    {
        doneAudio();
    }
}

bool IDSWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvFactors->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            auto curIdx = ui->tvFactors->selectionModel()->currentIndex();
            if (curIdx.row() != m_curRow || curIdx.column() != m_curCol)
            {
                m_curRow = curIdx.row();
                m_curCol = curIdx.column();
                if (m_curCol > 0)
                {
                    showSKG(m_curCol - 1);
                    showFDS(m_curCol - 1);
                }
            }
        }
    }
    return false;
}

void IDSWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void IDSWidget::tableClicked(const QModelIndex &index)
{
    if (index.column() > 0 && index.data(FactorUidRole).toString() == IDSFactorsDefines::FreqUid)
    {
        on_play(index.data(FrequencyRole).toDouble());
    }
}

void IDSWidget::on_play(const double frequency)
{
    if (m_tmStopSound == -1)
    {
        initAudio(frequency);

        m_soundGenerator->start();
        double linearVolume = QAudio::convertVolume(m_volume / double(100),
                                                    QAudio::LogarithmicVolumeScale,
                                                    QAudio::LinearVolumeScale);
        m_audioOutput->setVolume(linearVolume);
        m_audioOutput->start(m_soundGenerator);
        m_tmStopSound = startTimer(1000);
    }
}

void IDSWidget::setVolume(int volume)
{
    m_volume = volume;
}

void IDSWidget::addFactorsFromMultifactor(IDSCalculator *calculator)
{
    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < calculator->factors(0)->size(); ++i)
    {
        QList<QStandardItem*> items;
        auto fUid = calculator->factors(0)->factorUid(i);
        auto fi = app->getFactorInfo(fUid);
        auto fctName = fi.name();
        if (fi.measure() != "")
            fctName = fctName + ", " + fi.measure();
        auto *itemFactor = new QStandardItem(fctName);
        itemFactor->setEditable(false);
        itemFactor->setData(fUid, FactorUidRole);
        items << itemFactor;

        for (int j = 0; j < calculator->probesCount(); ++j)
        {
            auto value = calculator->factors(j)->factorValue(i);
            auto *item = new QStandardItem(QString::number(value, 'f', fi.format()));
            item->setData(fUid, FactorUidRole);
            if (fUid == IDSFactorsDefines::FreqUid)
                item->setData(value, FrequencyRole);
            item->setEditable(false);
            items << item;
        }

        m_mdlTable.appendRow(items);
    }
}

void IDSWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("IDSTestWidget", "SplitterDiagramPosition", ui->splDiagrams->saveState());
    SettingsProvider::setValueToRegAppCopy("IDSTestWidget", "SplitterTableDiagPosition", ui->splTblDiag->saveState());
}

void IDSWidget::restoreSplitterPosition()
{
    auto valDiag = SettingsProvider::valueFromRegAppCopy("IDSTestWidget", "SplitterDiagramPosition").toByteArray();
    ui->splDiagrams->restoreState(valDiag);
    auto valTblDiag = SettingsProvider::valueFromRegAppCopy("IDSTestWidget", "SplitterTableDiagPosition").toByteArray();
    ui->splTblDiag->restoreState(valTblDiag);
}

void IDSWidget::initAudio(const double frequency)
{
    m_audioFormat.setSampleRate(44100);
    m_audioFormat.setChannelCount(1);
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setCodec("audio/pcm");
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);

    if (!QAudioDeviceInfo::defaultOutputDevice().isFormatSupported(m_audioFormat))
        m_audioFormat = QAudioDeviceInfo::defaultOutputDevice().nearestFormat(m_audioFormat);

    m_soundGenerator = new SoundGenerator(m_audioFormat, 2 * 1000000, frequency);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), m_audioFormat);
}

void IDSWidget::doneAudio()
{
    if (m_soundGenerator)
    {
        m_soundGenerator->stop();
        m_soundGenerator->deleteLater();
    }
    if (m_audioOutput)
    {
        m_audioOutput->stop();
        m_audioOutput->deleteLater();
    }
    m_tmStopSound = -1;
}

void IDSWidget::showSKG(const int probeNum)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        m_trd->closeTest();
        m_trd->openTest(m_testUid);

        Q_ASSERT(ti.probes.size() == m_trd->probesCount() && probeNum < ti.probes.size());
        DataDefines::ProbeInfo pi;
        if (DataProvider::getProbeInfo(ti.probes.at(probeNum), pi))
        {
            auto* probe = m_trd->probe(probeNum);

            auto *sig = probe->signal(ChannelsDefines::chanStab);
            ui->wgtSKG->setSignal(sig);
            auto max = sig->absMaxValue();

            //! Установка диапазонов для СКГ
            QTimer::singleShot(0, [=]()
            {
                int diap = 1;
                while (diap < max)
                    diap = diap * 2;
                ui->wgtSKG->setDiap(diap);
            });
        }
    }
}

void IDSWidget::showFDS(const int probeNum)
{
    ui->wgtFDS->clear();
    auto* factors = m_calculator->factors(probeNum);

    if (!m_fds)
        m_fds = new AnySignal(factors->freqDiskr(), 1);
    else
        m_fds->clear();

    for (int i = 0; i < factors->fdsSize(); ++i)
    {
        QVector<double> rec;
        rec << factors->fds(i);
        m_fds->appendValue(rec);
    }

    ui->wgtFDS->appendSignal(m_fds, tr("ФДС"));
    ui->wgtFDS->area(0)->setDiapazone(m_fds->minValue(), m_fds->maxValue());
}


///<-----------------------------------------------------------------------------------------------
///< PlaySoundDelegate
PlaySoundDelegate::PlaySoundDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void PlaySoundDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() > 0)
    {
        if (option.state & QStyle::State_HasFocus)
        {
            painter->save();
            painter->setPen(QPen(QColor(0, 200, 200), 2, Qt::SolidLine));
            QRect rect = option.rect;
            rect.setTop(rect.top() + 1);
            rect.setLeft(rect.left() + 1);
            rect.setHeight(rect.height() - 1);
            rect.setWidth(rect.width() - 1);
            painter->drawRect(rect);
            painter->restore();
        }

        QStyleOptionButton opt;
        opt.rect = option.rect;
        opt.text = QString::number(trunc(index.data(IDSWidget::FrequencyRole).toDouble()));
        opt.icon = QIcon(":/images/Animate/Play.png");
        opt.iconSize = QSize(22, 22);
        opt.state |= QStyle::State_Enabled;
        opt.state |= QStyle::State_Active;
    //    if (index.model()->data(index, role()).toBool())
    //        opt.state |= QStyle::State_On;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QWidget *PlaySoundDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    auto *editor = new QPushButton(parent);
    editor->setText(QString::number(trunc(index.data(IDSWidget::FrequencyRole).toDouble())));
    editor->setIcon(QIcon(":/images/Animate/Play.png"));

    connect(editor, &QPushButton::pressed, [=]()
    {
        emit play(trunc(index.data(IDSWidget::FrequencyRole).toDouble()));
    });

    return editor;
}

void PlaySoundDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}


