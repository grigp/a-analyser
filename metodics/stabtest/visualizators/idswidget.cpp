#include "idswidget.h"
#include "ui_idswidget.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "aanalyserapplication.h"
#include "settingsprovider.h"
#include "soundgenerator.h"

#include "idscalculator.h"
#include "idsfactors.h"

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
{
    ui->setupUi(this);
    restoreSplitterPosition();
    initAudio();
}

IDSWidget::~IDSWidget()
{
    delete ui;
}

void IDSWidget::calculate(IDSCalculator *calculator, const QString &testUid)
{
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
            connect(dlgPS, &PlaySoundDelegate::play, this, &IDSWidget::on_play);
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
}

void IDSWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void IDSWidget::on_play(const double frequency)
{
    m_soundGenerator->reset(m_audioFormat, 2 * 1000000, frequency);
    m_soundGenerator->start();
    m_audioOutput->start(m_soundGenerator);
    QTimer::singleShot(1000, [=]()
    {
        m_soundGenerator->stop();
        m_audioOutput->stop();
    });
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
            else
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

void IDSWidget::initAudio()
{
    m_audioFormat.setSampleRate(44100);
    m_audioFormat.setChannelCount(1);
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setCodec("audio/pcm");
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);

    if (!QAudioDeviceInfo::defaultOutputDevice().isFormatSupported(m_audioFormat))
        m_audioFormat = QAudioDeviceInfo::defaultOutputDevice().nearestFormat(m_audioFormat);

    m_soundGenerator = new SoundGenerator(m_audioFormat, 2 * 1000000, 500);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), m_audioFormat);
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


