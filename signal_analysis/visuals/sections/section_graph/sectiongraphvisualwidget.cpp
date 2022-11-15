#include "sectiongraphvisualwidget.h"
#include "ui_sectiongraphvisualwidget.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "anysignal.h"
#include "baseutils.h"
#include "selecttransformerdialog.h"
#include "computefft.h"
#include "spectrparamsdialog.h"
#include "settingsprovider.h"
#include "diagspectr.h"

namespace
{
static const QString TransformSuffix = "sigtfm";
static const QString emfnTransform = QCoreApplication::tr("Файлы преобразований сигнала") + " *." + TransformSuffix + " (*." + TransformSuffix + ")";


}

SectionGraphVisualWidget::SectionGraphVisualWidget(VisualDescriptor* visual,
                                                   const QString& testUid,
                                                   const QString& probeUid,
                                                   const QString& channelId,
                                                   const QString& sectionNumber,
                                                   QWidget *parent) :
    SectionVisual(visual, testUid, probeUid, channelId, sectionNumber, parent),
    ui(new Ui::SectionGraphVisualWidget)
{
    ui->setupUi(this);
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");

    ui->splHorizontal->setSizes(QList<int>() << 500 << 0);

    connect(ui->wgtSpectrSrc, &DiagSpectr::press, this, &SectionGraphVisualWidget::on_press);
    connect(ui->wgtSpectrSrc, &DiagSpectr::release, this, &SectionGraphVisualWidget::on_release);
    connect(ui->wgtSpectrSrc, &DiagSpectr::move, this, &SectionGraphVisualWidget::on_move);
    connect(ui->wgtSpectrRes, &DiagSpectr::press, this, &SectionGraphVisualWidget::on_press);
    connect(ui->wgtSpectrRes, &DiagSpectr::release, this, &SectionGraphVisualWidget::on_release);
    connect(ui->wgtSpectrRes, &DiagSpectr::move, this, &SectionGraphVisualWidget::on_move);

    createBtnActions();
}

SectionGraphVisualWidget::~SectionGraphVisualWidget()
{
    delete ui;
}

bool SectionGraphVisualWidget::isValid()
{
    return true;
}

void SectionGraphVisualWidget::calculate()
{
    QByteArray data;
    auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
    if (DataProvider::getSectionData(channelUid, sectionNumber(), data))
    {
        if (!m_signal)
        {
            m_signal = new AnySignal(data);
            ui->wgtGraph->setLine1Color(Qt::blue);
            ui->wgtGraph->setLine2Color(Qt::red);
            ui->wgtGraph->appendSignal(m_signal, tr(""));

            m_diap = qMax(m_signal->midValue() - m_signal->minValue(), m_signal->maxValue() - m_signal->midValue());

            ui->wgtGraph->setDiapazone(-m_diap, m_diap);
            ui->wgtGraph->setIsZeroing(true);
            ui->cbScale->setCurrentIndex(0);
        }
    }
    m_isCalculate = true;
}

void SectionGraphVisualWidget::scaleChange(int idx)
{
    if (m_isCalculate)
    {
        auto diap = BaseUtils::scaleMultiplier(idx);
        ui->wgtGraph->setDiapazone(-m_diap/diap, m_diap/diap);
    }
}

void SectionGraphVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
    setDiapazones();
}

void SectionGraphVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
    setDiapazones();
}

void SectionGraphVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 0.0625) //1)
        ui->wgtGraph->setHScale(hScale / 2);
    setDiapazones();
}

void SectionGraphVisualWidget::signalScroll(int pos)
{
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
    setDiapazones();
}

void SectionGraphVisualWidget::on_popupMenuRequested(QPoint pos)
{
    Q_UNUSED(pos);

}

void SectionGraphVisualWidget::on_createSection()
{

}

void SectionGraphVisualWidget::on_press(const int x, const int y)
{
    m_selectionProcess = true;
    m_selFrom = QPoint(x, y);

    m_selectAreaWidget = static_cast<DiagSpectr*>(sender());
    m_selAreaX = x;
    m_selAreaY = y;
}

void SectionGraphVisualWidget::on_release(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    if (m_selectionProcess)
    {
        if (m_selTo.x() > m_selFrom.x() && m_selTo.y() > m_selFrom.y())
        {
            auto from = static_cast<DiagSpectr*>(sender())->getValues(m_selFrom);
            auto to = static_cast<DiagSpectr*>(sender())->getValues(m_selTo);
            static_cast<DiagSpectr*>(sender())->setVisualArea(from.x(), to.x(), to.y(), from.y());
        }
        else
        if (m_selTo.x() < m_selFrom.x() && m_selTo.y() < m_selFrom.y())
            static_cast<DiagSpectr*>(sender())->resetVisualArea();

        selectionReset();
    }
}

void SectionGraphVisualWidget::on_move(const int x, const int y)
{
    if (m_selectionProcess)
    {
        m_selTo = QPoint(x, y);
        if (m_selTo.x() > m_selFrom.x() && m_selTo.y() > m_selFrom.y())
            m_selectAreaWidget->selectArea(QRect(m_selFrom, m_selTo));
    }
}

void SectionGraphVisualWidget::on_transformNew()
{
    SelectTransformerDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted)
    {
        QVector<double> buffer;
        for (int i = 0; i < m_signal->size(); ++i)
            buffer << m_signal->value(1, i);
        int freq = m_signal->frequency();

        auto params = dlg.params();
        params["freq_sample"] = freq;

        static_cast<AAnalyserApplication*>(QApplication::instance())->transformSignal(dlg.transformer(), buffer, params);

        auto *signal = new AnySignal(freq, 2);
        for (int i = 0; i < m_signal->size(); ++i)
        {
            QVector<double> rec;
            rec << m_signal->value(0, i) << buffer.at(i);
            signal->appendValue(rec);
        }

        ui->wgtGraph->clear();
        m_signal = signal;
        updateSectionData();

        //! Запомнить действие
        auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
        DataProvider::addTransformActionToSection(channelUid, sectionNumber(), dlg.transformer(), params);
    }
}

void SectionGraphVisualWidget::on_transformFile()
{
    //! Запрос файла схемы преобразования
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getOpenFileName(this, tr("Файл схемы преобразований сигнала"), path, emfnTransform);
    if (fileName != "")
    {
        //! Чтение файла схемы преобазования
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QByteArray ba = file.readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
            auto tfmArray = loadDoc.array();
            file.close();

            transform(tfmArray);
        }
    }
}

void SectionGraphVisualWidget::on_transformMemory()
{
    auto tfmArray = static_cast<AAnalyserApplication*>(QApplication::instance())->getTransformerScheme();
    transform(tfmArray);
}

void SectionGraphVisualWidget::on_revert()
{
    auto id = QMessageBox::question(nullptr, tr("Запрос"), tr("Отменить все преобразования?"));
    if (id == QMessageBox::Yes)
    {
        int freq = m_signal->frequency();
        auto *signal = new AnySignal(freq, 2);
        for (int i = 0; i < m_signal->size(); ++i)
        {
            QVector<double> rec;
            rec << m_signal->value(0, i) << m_signal->value(0, i);
            signal->appendValue(rec);
        }

        ui->wgtGraph->clear();
        m_signal = signal;
        updateSectionData();

        //! Запомнить действие
        auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
        DataProvider::clearTransformActionToSection(channelUid, sectionNumber());
    }

}

void SectionGraphVisualWidget::on_calculateSpectr()
{
    //! Диалог параметров спектра
    SpectrParamsDialog dlg(m_signal->size(), m_signal->frequency());

    auto sp = SettingsProvider::valueFromRegAppCopy("CalculateSpectr", "Settings").toByteArray();
    if (sp.size() != 0)
    {
        QJsonDocument doc(QJsonDocument::fromJson(sp));
        QJsonObject params = doc.object();
        dlg.setParams(params);
    }

    if (dlg.exec() == QDialog::Accepted)
    {
        auto params = dlg.params();
        QJsonDocument doc(params);
        QByteArray ba = doc.toJson();
        SettingsProvider::setValueToRegAppCopy("CalculateSpectr", "Settings", ba);

        //! Буфера данных для спектра
        QVector<double> dataSrc;
        QVector<double> dataRes;

        //! Расчет спектра
        int frequency = m_signal->frequency();
        if (params["is_averaging"].toBool())
            computeSpectrAveraging(dataSrc, dataRes, params["points"].toInt(), params["averaging_offset"].toInt());
        else
        {
            computeSpectrDecimation(dataSrc, dataRes, params["points"].toInt());
            frequency = static_cast<int>(static_cast<double>(frequency) / (static_cast<double>(m_signal->size()) / static_cast<double>(params["points"].toInt())));
        }

        //! Прорисовка спектра в одном диапазоне амплитуд
        double maxVS = 0;
        double maxVR = 0;
        showSpectr(ui->wgtSpectrSrc, dataSrc, maxVS, frequency, params["max_frequency"].toDouble());
        showSpectr(ui->wgtSpectrRes, dataRes, maxVR, frequency, params["max_frequency"].toDouble());
        double max = qMax(maxVS, maxVR);
        ui->wgtSpectrSrc->setVisualArea(0, params["max_frequency"].toDouble(), 0, max);
        ui->wgtSpectrRes->setVisualArea(0, params["max_frequency"].toDouble(), 0, max);

        //! Автоматически показать панель спектров
        ui->splHorizontal->setSizes(QList<int>() << 500 << 500);
    }
}

void SectionGraphVisualWidget::on_saveTransformToFile()
{
    DataDefines::SectionInfo si;
    auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
    if (DataProvider::getSectionData(channelUid, sectionNumber(), si))
    {
        if (si.actions != QJsonArray())
        {
            QString path = DataDefines::aanalyserDocumentsPath();
            auto fileName = QFileDialog::getSaveFileName(this, tr("Файл схемы преобразований сигнала"), path, emfnTransform);
            if (fileName != "")
            {
                QFile fileRec(fileName);
                fileRec.remove();
                if (fileRec.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QJsonDocument doc(si.actions);
                    QByteArray ba = doc.toJson();
                    fileRec.write(ba);

                    fileRec.close();
                }
            }
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Преобразований не было"));
    }
}

void SectionGraphVisualWidget::on_saveTransformToMemory()
{
    DataDefines::SectionInfo si;
    auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
    if (DataProvider::getSectionData(channelUid, sectionNumber(), si))
    {
        if (si.actions != QJsonArray())
            static_cast<AAnalyserApplication*>(QApplication::instance())->rememberTransformerScheme(si.actions);
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Преобразований не было"));
    }
}

void SectionGraphVisualWidget::transform(QJsonArray &sheme)
{
    if (sheme != QJsonArray())
    {
        //! Создания буфера для преобразования
        QVector<double> buffer;
        for (int i = 0; i < m_signal->size(); ++i)
            buffer << m_signal->value(1, i);
        int freq = m_signal->frequency();

        //! Последовательное применение преобразований
        auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
        for (int i = 0; i < sheme.size(); ++i)
        {
            auto tfmObj = sheme.at(i).toObject();
            auto uid = tfmObj["uid"].toString();
            auto params = tfmObj["params"].toObject();

            static_cast<AAnalyserApplication*>(QApplication::instance())->transformSignal(uid, buffer, params);

            //! Запомнить действие
            DataProvider::addTransformActionToSection(channelUid, sectionNumber(), uid, params);
        }

        //! Замена исходного сигнала преобразованным
        auto *signal = new AnySignal(freq, 2);
        for (int i = 0; i < m_signal->size(); ++i)
        {
            QVector<double> rec;
            rec << m_signal->value(0, i) << buffer.at(i);
            signal->appendValue(rec);
        }

        ui->wgtGraph->clear();
        m_signal = signal;
        updateSectionData();
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Отсутствует схема преобразований"));
}

void SectionGraphVisualWidget::computeSpectrAveraging(QVector<double> &dataSrc, QVector<double> &dataRes,
                                                      const int points, const int offset)
{
    initData(dataSrc, points);
    initData(dataRes, points);

    int start = 0;
    int n = 0;
    do
    {
        QVector<double> ds;
        QVector<double> dr;

        for (int i = 0; i < points; ++i)
        {
            ds << m_signal->value(0, start + i);
            dr << m_signal->value(1, start + i);
        }

        ComputeFFT::baseFFT(ds);
        ComputeFFT::baseFFT(dr);

        for (int i = 0; i < points; ++i)
        {
            dataSrc[i] += ds[i];
            dataRes[i] += dr[i];
        }

        start += offset;
        ++n;
    }
    while(start + points < m_signal->size());

    if (n > 0)
        for (int i = 0; i < points; ++i)
        {
            dataSrc[i] /= n;
            dataRes[i] /= n;
        }
}

void SectionGraphVisualWidget::computeSpectrDecimation(QVector<double> &dataSrc, QVector<double> &dataRes, const int points)
{
    double prop = static_cast<double>(m_signal->size()) / static_cast<double>(points);
    double ir = 0.0;
    while (ir < m_signal->size())
    {
        int i = static_cast<int>(ir);
        dataSrc << m_signal->value(0, i);
        dataRes << m_signal->value(1, i);
        ir += prop;
    }

    ComputeFFT::baseFFT(dataSrc);
    ComputeFFT::baseFFT(dataRes);
}

void SectionGraphVisualWidget::initData(QVector<double> &data, const int size)
{
    data.clear();
    for (int i = 0; i < size; ++i)
        data << 0;
}

void SectionGraphVisualWidget::showSpectr(DiagSpectr *area, QVector<double> &data, double &maxV,
                                          const int freqSample, const double maxFreq)
{
    area->clear();
//    QFile file("d://1//spectr.txt");
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
//    QTextStream fs(&file);
    for (int i = 0; i < data.size() / 2; ++i)
    {
        area->addValue(data[i]);
//        fs << QString::number(data[i]) << "\n";
        if (data[i] > maxV)
            maxV = data[i];
    }
//    file.close();
    area->setFormatData(freqSample, maxFreq);
}

void SectionGraphVisualWidget::updateSectionData()
{
    ui->wgtGraph->appendSignal(m_signal, tr(""));
    ui->wgtGraph->setLine1Color(Qt::blue);
    ui->wgtGraph->setLine2Color(Qt::red);
    m_diap = qMax(m_signal->midValue() - m_signal->minValue(), m_signal->maxValue() - m_signal->midValue());
    ui->wgtGraph->setDiapazone(-m_diap, m_diap);
    ui->cbScale->setCurrentIndex(0);

    QByteArray data;
    m_signal->toByteArray(data);
    auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
    DataProvider::updateSection(channelUid, sectionNumber(), data);
}

void SectionGraphVisualWidget::setDiapazones()
{
//    if (m_isCalculate)
//    {
//            double mid;

//            int n = 0;
//            for (int i = ui->wgtGraph->startPoint(); i < ui->wgtGraph->startPoint() + ui->wgtGraph->areaWidth() / ui->wgtGraph->hScale(); ++i)
//            {
//                if (i < m_signal->size())
//                {
//                    for (int j = 0; j < m_signal->subChansCount(); ++j)
//                    {
//                        auto v = m_signal->value(j, i);
//                        mid += v;
//                    }
//                    ++n;
//                }
//            }
//            if (n > 0)
//                for (int i = 0; i < m_signal->subChansCount(); ++i)
//                {
//                    auto mid = mids.at(i);
//                    mid /= n;
//                    mids.replace(i, mid);
//                }

//            int v = scaleMultiplier(ui->cbScale->currentIndex());
//            if (ui->wgtGraph->areasesCount() == m_signal->subChansCount())
//            {
//                for (int i = 0; i < m_signal->subChansCount(); ++i)
//                {
//                    auto min = m_signal->minValueChan(i);
//                    auto max = m_signal->maxValueChan(i);
//                    double diap = (max - min) / v * 1.2;
//                    ui->wgtGraph->setDiapazone(i, mids.at(i) - diap / 2, mids.at(i) + diap / 2);
//                }
//            }
//            else
//            {
//                if (m_selectedChan >= 0 && m_selectedChan < m_signal->subChansCount())
//                {
//                    auto min = m_signal->minValueChan(m_selectedChan);
//                    auto max = m_signal->maxValueChan(m_selectedChan);
//                    double diap = (max - min) / v * 1.2;
//                    ui->wgtGraph->setDiapazone(0,
//                                               mids.at(m_selectedChan) - diap / 2,
//                                               mids.at(m_selectedChan) + diap / 2);
//                }
//            }
    //    }
}

void SectionGraphVisualWidget::selectionReset()
{
    m_selectionProcess = false;
    m_selFrom = QPoint(-1, -1);
    m_selTo = QPoint(-1, -1);
    m_selectAreaWidget->selectArea(QRect());
}

void SectionGraphVisualWidget::createBtnActions()
{
    auto acSaveTransformToFile = new QAction(tr("В файл..."));
    connect(acSaveTransformToFile, &QAction::triggered, this, &SectionGraphVisualWidget::on_saveTransformToFile);
    ui->btnSaveTransform->addAction(acSaveTransformToFile);
    auto acSaveTransformToMemory = new QAction(tr("В память"));
    connect(acSaveTransformToMemory, &QAction::triggered, this, &SectionGraphVisualWidget::on_saveTransformToMemory);
    ui->btnSaveTransform->addAction(acSaveTransformToMemory);


    auto acTransformNew = new QAction(tr("Новое преобразование..."));
    connect(acTransformNew, &QAction::triggered, this, &SectionGraphVisualWidget::on_transformNew);
    ui->btnTransform->addAction(acTransformNew);
    auto acTransformFile = new QAction(tr("Схема из файла..."));
    connect(acTransformFile, &QAction::triggered, this, &SectionGraphVisualWidget::on_transformFile);
    ui->btnTransform->addAction(acTransformFile);
    auto acTransformMemory = new QAction(tr("Схема из памяти"));
    connect(acTransformMemory, &QAction::triggered, this, &SectionGraphVisualWidget::on_transformMemory);
    ui->btnTransform->addAction(acTransformMemory);
}
