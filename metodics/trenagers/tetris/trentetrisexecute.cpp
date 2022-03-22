#include "trentetrisexecute.h"
#include "ui_trentetrisexecute.h"

#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>

#include "aanalyserapplication.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "trenresultdata.h"
#include "takeputresultdata.h"
#include "trenresultfactors.h"
#include "tetrisglass.h"
#include "tetrisfigure.h"

namespace  {
///< Список фигур а-ля тетрис
QList<QVector<QVector<QColor>>> FiguresTetris = {
    {{Qt::white}},
    {{Qt::white, Qt::white}},
    {{Qt::white, Qt::white}, {Qt::white, Qt::white}},
    {{Qt::white, Qt::white, Qt::white}},
    {{Qt::white, Qt::white, Qt::white, Qt::white}},
    {{Qt::white, Qt::white, Qt::black}, {Qt::black, Qt::white, Qt::white}},
    {{Qt::black, Qt::white, Qt::white}, {Qt::white, Qt::white, Qt::black}},
    {{Qt::white, Qt::black, Qt::black}, {Qt::white, Qt::white, Qt::white}},
    {{Qt::black, Qt::black, Qt::white}, {Qt::white, Qt::white, Qt::white}},
};

///< Список фигур а-ля rectis (нет поворотов)
QList<QVector<QVector<QColor>>> FiguresRectis = {
    {{Qt::white}},
    {{Qt::white, Qt::white}},
    {{Qt::white}, {Qt::white}},
    {{Qt::white, Qt::white}, {Qt::white, Qt::white}},
    {{Qt::white, Qt::white, Qt::white}},
    {{Qt::white}, {Qt::white}, {Qt::white}},
    {{Qt::white, Qt::white, Qt::white}, {Qt::white, Qt::white, Qt::white}},
    {{Qt::white, Qt::white}, {Qt::white, Qt::white}, {Qt::white, Qt::white}},
    {{Qt::white, Qt::white, Qt::white}, {Qt::white, Qt::white, Qt::white}, {Qt::white, Qt::white, Qt::white}},
};

///< Список фигур а-ля rectis для цветного тетриса
QList<QVector<QVector<QColor>>> FiguresColored = {
    {{Qt::white}},
    {{Qt::white, Qt::white}},
    {{Qt::white}, {Qt::white}},
    {{Qt::white, Qt::white}, {Qt::white, Qt::white}},
    {{Qt::white, Qt::white, Qt::white}},
    {{Qt::white}, {Qt::white}, {Qt::white}},
};

///< Фигура для кубиков. Один кубик. Для единства обработки - список. Безобразно, но однообразно
QList<QVector<QVector<QColor>>> OneCube = {{{Qt::white}}};

}


TrenTetrisExecute::TrenTetrisExecute(QWidget *parent) :
    TrenStabExecute (parent),
    ui(new Ui::TrenTetrisExecute)
  , m_takePutResData(new TakePutResultData(ChannelsDefines::chanTakePutResult))
{
    ui->setupUi(this);

}

TrenTetrisExecute::~TrenTetrisExecute()
{
    delete ui;
}

void TrenTetrisExecute::setParams(const QJsonObject &params)
{
    TrenStabExecute::setParams(params);

    m_movingMode = TrenTetrisDefines::MovingModeValueIndex.value(params["moving_mode"].toString());
    m_complexityMode = TrenTetrisDefines::ComplexityModeValueIndex.value(params["complexity"].toString());
    m_deletingMode = TrenTetrisDefines::DeletingModeValueIndex.value(params["deleting"].toString());
    m_isShowGrid = params["is_show_grid"].toBool(false);

    m_glassHCount = params["width"].toInt();
    m_glassVCount = params["height"].toInt();

    m_cubeImageFileName = params["cube_image"].toString();

    auto objRdm = params["rows_deleting_mode"].toObject();
    m_cubeColor = BaseUtils::strRGBAToColor(objRdm["cube_color"].toString());
    m_glassColor = BaseUtils::strRGBAToColor(objRdm["glass_color"].toString());
    m_lastColor = BaseUtils::strRGBAToColor(objRdm["last_color"].toString());

    auto objCdm = params["colored_deleting_mode"].toObject();
    m_deletingCubeCount = objCdm["count"].toInt();
    m_colorModeColors.clear();
    auto arrCol = objCdm["colors"].toArray();
    for (int i = 0; i < arrCol.size(); ++i)
    {
        auto obj = arrCol.at(i).toObject();
        QColor color = BaseUtils::strRGBAToColor(obj["color"].toString());
        m_colorModeColors << color;
    }

    auto objAmm = params["auto_moving_mode"].toObject();
    m_autoMovingSpeed = objAmm["speed"].toInt();

    m_markerObj = params["marker"].toObject();
    m_glassObj = params["glass"].toObject();

    auto objSS = params["sounds"].toObject();
    m_soundSheme.take = objSS["take"].toString();
    m_soundSheme.put = objSS["put"].toString();
    m_soundSheme.deleteRow = objSS["delete"].toString();

    assignParams();
}

void TrenTetrisExecute::on_recording()
{
    TrenStabExecute::on_recording();

    m_takePutResData->clear();
    m_glass->clear();
    m_tmStage = TrenTetrisDefines::tmsTake;
    m_rowsDeleted = 0;
    changeRowsDeleted(0);
}

void TrenTetrisExecute::setAdvancedChannels()
{
    TrenStabExecute::setAdvancedChannels();
    setAdvancedChannelEnable(1, m_complexityMode == TrenTetrisDefines::cmFigures);
}

void TrenTetrisExecute::elementsInteraction(DeviceProtocols::DeviceData *data)
{
    TrenStabExecute::elementsInteraction(data);

    auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
    if (multiData->subChanCount() > 0)
    {
        //! Управление маркером
        QPointF rec = qvariant_cast<QPointF>(multiData->value(0));
        if (m_marker)
        {
            //! Установка маркера
            double mx = rec.x() / (128 / BaseUtils::scaleMultiplier(scale())) * (scene()->sceneRect().width() / 2);
            double my = - rec.y() / (128 / BaseUtils::scaleMultiplier(scale())) * (scene()->sceneRect().height() / 2);

//            m_marker->setVisible((m_movingMode == TrenTetrisDefines::mmTake) && (m_tmStage == TrenTetrisDefines::tmsTake));

            if (m_movingMode == TrenTetrisDefines::mmTake)
                takeModeInteraction(mx, my);
            else
            if (m_movingMode == TrenTetrisDefines::mmAuto)
                autoDescentModeInteraction(mx, my);

            //! Поворот фигуры с помощью доп. канала
            if (isAdvancedChannelAboveBoundNow(1, false))
                m_glass->rotateFigure();

            if (isGlassFull())
            {
                if (!isRecording())
                    m_glass->clear();
                else
                    finishTest();
            }
        }
    }
}

void TrenTetrisExecute::boundingMarker(double &mx, double &my)
{
    if (mx < scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->borderLR())
        mx = scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->borderLR();
    if (mx > scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->boundingRect().width() - m_glass->borderLR())
        mx = scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->boundingRect().width() - m_glass->borderLR();
    if (my < scene()->sceneRect().y() + m_glass->boundingRect().top())
        my = scene()->sceneRect().y() + m_glass->boundingRect().top();
    if (my > scene()->sceneRect().y() + m_glass->boundingRect().top() + m_glass->boundingRect().height() - m_glass->borderB())
        my = scene()->sceneRect().y() + m_glass->boundingRect().top() + m_glass->boundingRect().height() - m_glass->borderB();
}

void TrenTetrisExecute::boundingFigure(double &mx, double &my)
{
    Q_UNUSED(mx);
    QRectF fig = m_glass->getFigureRect();

//    if (mx < scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->borderLR() + m_offsX + fig.width() / 2)
//        mx = scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->borderLR() + m_offsX + fig.width() / 2;
//    if (mx > scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->boundingRect().width() - m_glass->borderLR() + m_offsX - fig.width() / 2)
//        mx = scene()->sceneRect().x() + m_glass->boundingRect().left() + m_glass->boundingRect().width() - m_glass->borderLR() + m_offsX - fig.width() / 2;
    if (my < scene()->sceneRect().y() + m_glass->boundingRect().top() + m_offsY + fig.height() / 2)
        my = scene()->sceneRect().y() + m_glass->boundingRect().top() + m_offsY + fig.height() / 2;
    if (my > scene()->sceneRect().y() + m_glass->boundingRect().top() + m_glass->boundingRect().height() - m_glass->borderB() + m_offsY - fig.height() / 2)
        my = scene()->sceneRect().y() + m_glass->boundingRect().top() + m_glass->boundingRect().height() - m_glass->borderB() + m_offsY - fig.height() / 2;
}

void TrenTetrisExecute::takeModeInteraction(double &mx, double &my)
{
    //! На этапе поиска захвата
    if (m_tmStage == TrenTetrisDefines::tmsTake)
    {
        boundingMarker(mx, my);

        m_marker->setPos(mx - m_marker->boundingRect().width() / 2,
                         my - m_marker->boundingRect().height() / 2);

        //! Координаты маркера в координатах сцены
        qreal mxs = mx + scene()->sceneRect().width() / 2;
        qreal mys = my + scene()->sceneRect().height() / 2;

        auto fig = m_glass->getFigureRect();
        //! Маркер на фигуре
        if (fig.contains(mxs, mys) && isAdvancedChannelAboveBound(0))
        {
            m_offsX = mxs - fig.center().x();
            m_offsY = mys - fig.center().y();
            m_tmStage = TrenTetrisDefines::tmsPut;
            m_marker->setVisible(false);
            m_takePutResData->addEvent(recCount(), TakePutResultData::tpkTake);

            if (m_soundSheme.take != "")
            {
                m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.take));
                m_player.play();
            }
        }
    }
    else
    //! На этапе поиска укладки
    if (m_tmStage == TrenTetrisDefines::tmsPut)
    {
        boundingFigure(mx, my);

        //! Устанавливаем фигуру на позицию согласно координатам маркера и
        //! проверяем, положена ли фигура
        if (m_glass->setFigureCoordinates(mx + scene()->sceneRect().width() / 2 - m_offsX,
                                          my + scene()->sceneRect().height() / 2 - m_offsY) &&
                isAdvancedChannelAboveBound(0))
        {
            //! Если положена, то...
            putFigure();
            m_tmStage = TrenTetrisDefines::tmsTake;
            //! Добавление новой фигуры
            m_glass->setNewFigure(m_nextFigure);
            m_nextFigure = newFigure();
            m_marker->setVisible(true);

            m_takePutResData->addEvent(recCount(), TakePutResultData::tpkPut);
            if (m_soundSheme.put != "")
            {
                m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.put));
                m_player.play();
            }
        }
    }
}

void TrenTetrisExecute::autoDescentModeInteraction(double &mx, double &my)
{
    Q_UNUSED(my);
    auto y = m_glass->figureY();
    y += m_autoMovingSpeed;

    //! Сброс фигуры
    if (isAdvancedChannelAboveBoundNow(0))
    {
        y = 1000;
    }

    //! Устанавливаем фигуру на позицию согласно координатам маркера и
    //! проверяем, положена ли фигура
    if (m_glass->setFigureCoordinates(mx + scene()->sceneRect().width() / 2 - m_offsX, y))
    {
        //! Если положена, то...
        putFigure();
        m_tmStage = TrenTetrisDefines::tmsTake;
        //! Добавление новой фигуры
        m_glass->setNewFigure(m_nextFigure);
        m_nextFigure = newFigure();
    }
}

void TrenTetrisExecute::putFigure()
{
    //! Снятие пометки последней фигуры в режиме удаления строк
    if (m_deletingMode == TrenTetrisDefines::dmRows)
        clearLastColor();

    //! Установка новой фигуры
    QList<QPoint> lastFigCubes;
    lastFigCubes.clear();
    auto pos = m_glass->getFigurePosition();
    for (int i = 0; i < pos.width(); ++i)
    {
        for (int j = 0; j < pos.height(); ++j)
        {
            if (m_deletingMode == TrenTetrisDefines::dmColored)
            {
                auto color = m_glass->getFigureColor(i, j);
                if (color != Qt::black)
                {
                    m_glass->setValue(pos.x() + i, pos.y() + pos.height() - 1 - j, color);
                    lastFigCubes << QPoint(pos.x() + i, pos.y() + pos.height() - 1 - j);
                }
            }
            else
            if (m_deletingMode == TrenTetrisDefines::dmRows)
            {
                if (m_glass->getFigureColor(i, j) != Qt::black)
                    m_glass->setValue(pos.x() + i, pos.y() + j, m_lastColor);
            }
        }
    }
    changeGameScore(1);

    //! Анализ и удаление кубиков
    if (m_deletingMode == TrenTetrisDefines::dmColored)
        deleteOneColorCubes(lastFigCubes);
    else
    if (m_deletingMode == TrenTetrisDefines::dmRows)
        deleteRows();
}


void TrenTetrisExecute::generateNewScene()
{
    TrenStabExecute::generateNewScene();

    setGlass(m_glassObj);
    m_glass->setGeometry(scene()->width(), scene()->height(), m_glassHCount, m_glassVCount);
    scene()->addItem(m_glass);
    m_glass->setZValue(zlvlGlass);
    m_glass->setPos(-scene()->width() / 2, -scene()->height()/ 2);

    //! Добавление цветов в список допустимых для стакана
    if (m_deletingMode == TrenTetrisDefines::dmRows)
    {
        m_glass->addColor(m_glassColor);
        m_glass->addColor(m_lastColor);
        m_glass->addColor(m_cubeColor);
    }
    else
    if (m_deletingMode == TrenTetrisDefines::dmColored)
    {
        foreach (auto color, m_colorModeColors)
            m_glass->addColor(color);
    }
    m_glass->addColor(m_deletingColor);
    m_glass->setDeletingCubesColor(m_deletingColor);

    //! Добавление новой фигуры
    m_glass->setNewFigure(newFigure());
    m_nextFigure = newFigure();

    setMarker(m_markerObj);
    scene()->addItem(m_marker);
    m_marker->setZValue(zlvlMarker);
    m_marker->setPos(0 - m_marker->boundingRect().width() / 2,
                     0 - m_marker->boundingRect().height() / 2);
    m_marker->setVisible(m_movingMode == TrenTetrisDefines::mmTake);
}

void TrenTetrisExecute::fillGameParams(QFrame *frame)
{
    TrenStabExecute::fillGameParams(frame);

    QString style = "font-size: 16pt; color: rgb(9,105,156);";
    QString name = tr("Строки");
    m_lblRowsDel = new QLabel(frame);
    m_lblRowsDel->setText(name);
    m_lblRowsDel->setStyleSheet(style);
    frame->layout()->addWidget(m_lblRowsDel);
    pwSetGameParamLabel(name, style);

    changeRowsDeleted(0);
}

void TrenTetrisExecute::fillGameHints(QFrame *frame)
{
    TrenStabExecute::fillGameHints(frame);

    auto lblCaption = new QLabel(frame);
    lblCaption->setText(tr("Следующая фигура"));
    lblCaption->setAlignment(Qt::AlignHCenter);
    frame->layout()->addWidget(lblCaption);

    m_wgtNextFigure = new TetrisFigure(frame);
    frame->layout()->addWidget(m_wgtNextFigure);

    lblCaption = new QLabel(frame);
    lblCaption->setText(tr("Следующая фигура"));
    lblCaption->setAlignment(Qt::AlignHCenter);
    pwAddHintWidget(lblCaption);

    m_wgtNextFigurePW = new TetrisFigure();
    pwAddHintWidget(m_wgtNextFigurePW);
}

void TrenTetrisExecute::finishTest()
{
    m_takePutResData->setIsEnabledErrors(false);
    m_takePutResData->setIsEnabledTake(m_movingMode == TrenTetrisDefines::mmTake);
    m_takePutResData->setIsEnabledPut(m_movingMode == TrenTetrisDefines::mmTake);
    addChannel(m_takePutResData);

    //! Добавляем значение специфического показателя для подкласса TrenTakePutExecute: время игры и количество удаленных строк
    addFactorValue(TrenResultFactorsDefines::TimeUid, recCount() / frequency());
    addFactorValue(TrenResultFactorsDefines::RowsDeletedUid, m_rowsDeleted);

    TrenStabExecute::finishTest();
}

void TrenTetrisExecute::assignParams()
{
    if (m_movingMode == TrenTetrisDefines::mmTake)
        setAdvancedFunctionTitle(0, tr("для захвата и укладки"));
    else
    if (m_movingMode == TrenTetrisDefines::mmAuto)
        setAdvancedFunctionTitle(0, tr("для сброса фигуры"));
    setAdvancedFunctionTitle(1, tr("для поворота фигуры"));
}

void TrenTetrisExecute::setMarker(const QJsonObject &objMarker)
{
    auto style = objMarker["style"].toString();
    if (style == "picture")
    {
        QPixmap mpmp(":/images/Games/" + objMarker["image"].toString());
        if (objMarker["repaint"].toBool())
            BaseUtils::setColoredPicture(mpmp, BaseUtils::strRGBAToColor(objMarker["color"].toString()));
        m_marker = new GraphicCommon::MarkerElement(mpmp);
    }
}

void TrenTetrisExecute::setGlass(const QJsonObject &objGlass)
{
    m_glass = new TetrisGlass();

    auto style = objGlass["style"].toString();
    if (style == "plate")
        m_glass->setStyle(TetrisGlass::stlPlate);
    else
    if (style == "color")
        m_glass->setStyle(TetrisGlass::stlColor);

    m_glass->setPliteImage(objGlass["image"].toString());
    m_glass->setColor(BaseUtils::strRGBAToColor(objGlass["color"].toString()));

    auto objFrames = objGlass["frames"].toObject();
    m_glass->setFrameColor(BaseUtils::strRGBAToColor(objFrames["color"].toString()));
    m_glass->setFrameLeftImage(objFrames["left"].toString());
    m_glass->setFrameRightImage(objFrames["right"].toString());
    m_glass->setFrameBottomImage(objFrames["bottom"].toString());
    m_glass->setFrameCornerLeftImage(objFrames["corner_left"].toString());
    m_glass->setFrameCornerRightImage(objFrames["corner_right"].toString());
    m_glass->setCubeImage(":/images/Games/" + m_cubeImageFileName);

    static_cast<TetrisFigure*>(m_wgtNextFigure)->setCubeFileName(":/images/Games/" + m_cubeImageFileName);
    static_cast<TetrisFigure*>(m_wgtNextFigurePW)->setCubeFileName(":/images/Games/" + m_cubeImageFileName);

    m_glass->setIsShowGrid(m_isShowGrid);
//    m_glass->setIsShowFigurePos(true);
}

void TrenTetrisExecute::changeRowsDeleted(const int value)
{
    m_rowsDeleted += value;
    QString text = tr("Cтроки") + " - " + QString::number(m_rowsDeleted);
    m_lblRowsDel->setText(text);
    pwSetGameParamLabelValue(1, text);
}

void TrenTetrisExecute::clearLastColor()
{
    for (int i = 0; i < m_glassHCount; ++i)
        for (int j = 0; j < m_glassVCount; ++j)
            if (m_glass->value(i, j) != Qt::black)
                m_glass->setValue(i, j, m_glassColor);
}

void TrenTetrisExecute::deleteRows()
{
    //! Анализ и составление списка строк
    QList<int> fullRows;
    fullRows.clear();
    for (int j = 0; j < m_glassVCount - 1; ++j)
    {
        bool full = true;
        for (int i = 0; i < m_glassHCount; ++i)
        {
            if (m_glass->value(i, j) == Qt::black)
            {
                full = false;
                break;
            }
        }
        if (full)
            fullRows << j;
    }

    //! Удаление строк
    if (fullRows.size() > 0)
    {
        //! Пометить кубики, как удаляемые
        foreach (auto row, fullRows)
            for (int i = 0; i < m_glassHCount; ++i)
                m_glass->setDeletingCube(i, row);

        //! Отложенное удаление, чтоб видеть, что удаляем
        QTimer::singleShot(1000, [=]
        {
            m_glass->clearDeletingCubes();

            if (fullRows.size() > 0 && m_soundSheme.deleteRow != "")
            {
                m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.deleteRow));
                m_player.play();
            }

            //! Удаляем, начиная с верхней строчки, ибо, если наоборот, то номера последующих удаляемых строчек меняются
            for (int i = fullRows.size() - 1; i >= 0; --i)
            {
                deleteRow(fullRows.at(i));
                changeRowsDeleted(1);
                changeGameScore(m_glassHCount * 3);
            }
        });
    }
}

void TrenTetrisExecute::deleteRow(const int row)
{
    //! Смещение строк
    for (int j = row; j < m_glassVCount - 1; ++j)
        for (int i = 0; i < m_glassHCount; ++i)
            m_glass->setValue(i, j, m_glass->value(i, j + 1));

    //! Заполнение верхней строки пустым
    for (int i = 0; i < m_glassHCount; ++i)
        m_glass->setValue(i, m_glassVCount - 1, Qt::black);
}

void TrenTetrisExecute::deleteOneColorCubes(const QList<QPoint> lastFigCubes)
{
    foreach (auto pos, lastFigCubes)
    {
        if (m_glass->value(pos.x(), pos.y()) != Qt::black)
        {
            QList<QPoint> oneColorCubes;
            oneColorCubes.clear();
            fillOneColorCubesList(oneColorCubes, pos, m_glass->value(pos.x(), pos.y()));

            if (oneColorCubes.size() >= m_deletingCubeCount)
            {
//                qDebug() << oneColorCubes;
                //! Пометить кубики, как удаляемые
                foreach (auto cube, oneColorCubes)
                    m_glass->setDeletingCube(cube.x(), cube.y());

                //! Отложенное удаление, чтоб видеть, что удаляем
                QTimer::singleShot(1000, [=]
                {
                    m_glass->clearDeletingCubes();

                    if (oneColorCubes.size() > 0 && m_soundSheme.deleteRow != "")
                    {
                        m_player.setMedia(QUrl("qrc:/sound/" + m_soundSheme.deleteRow));
                        m_player.play();
                    }

                    foreach (auto cube, oneColorCubes)
                        m_glass->setValue(cube.x(), cube.y(), Qt::black);
                    foreach (auto cube, oneColorCubes)
                        shiftCol(cube.x(), cube.y());
                    changeRowsDeleted(1);
                    changeGameScore(oneColorCubes.size() * 3);
                });
            };
        }
    }
}

void TrenTetrisExecute::fillOneColorCubesList(QList<QPoint> &oneColorCubes, const QPoint pos, const QColor color) const
{
    if (!oneColorCubes.contains(QPoint(pos.x(), pos.y())) &&
        m_glass->value(pos.x(), pos.y()) == color)
    {
        oneColorCubes << pos;

        if (pos.x() > 0)
            fillOneColorCubesList(oneColorCubes, QPoint(pos.x() - 1, pos.y()), color);
        if (pos.x() < m_glassHCount - 1)
            fillOneColorCubesList(oneColorCubes, QPoint(pos.x() + 1, pos.y()), color);
        if (pos.y() > 0)
            fillOneColorCubesList(oneColorCubes, QPoint(pos.x(), pos.y() - 1), color);
        if (pos.y() < m_glassVCount - 1)
            fillOneColorCubesList(oneColorCubes, QPoint(pos.x(), pos.y() + 1), color);
    }
}

void TrenTetrisExecute::shiftCol(const int x, const int y)
{
//    qDebug() << "------------" << x << y;
    //! Поиск нижней точки, в которую сдвигать кубики
    int down = -1;
    for (int i = y; i >= 0; --i)
    {
        if (m_glass->value(x, i) != Qt::black)
        {
            down = i;
            break;
        }
    }
    ++down;

    if (y >= down)
    {
        //! Все кубики, что сверху, опустить к нижней точке
        int n = 0;
        for (int i = y; i < m_glassVCount; ++i)
        {
            if (m_glass->value(x, i) != Qt::black)
            {
                m_glass->setValue(x, down + n, m_glass->value(x, i));
                m_glass->setValue(x, i, Qt::black);
                ++n;
            }
        }
    }
}

bool TrenTetrisExecute::isGlassFull()
{
    for (int i = 0; i < m_glassHCount; ++i)
        if (m_glass->value(i, m_glassVCount - 1) != Qt::black)
            return true;
    return false;
}

QVector<QVector<QColor>> TrenTetrisExecute::newFigure()
{
    //! Выбор списка фигур в зависимости от режима
    QList<QVector<QVector<QColor>>> listFigures;
//    if (m_complexityMode == TrenTetrisDefines::cmCubes)
//        listFigures = OneCube;
//    else
//    if (m_movingMode == TrenTetrisDefines::mmTake && m_complexityMode == TrenTetrisDefines::cmFigures)
//        listFigures = FiguresRectis;
//    else
//        listFigures = FiguresTetris;
    if (m_complexityMode == TrenTetrisDefines::cmCubes)
        listFigures = OneCube;
    else
    if (m_deletingMode == TrenTetrisDefines::dmRows && m_complexityMode == TrenTetrisDefines::cmFigures)
        listFigures = FiguresRectis;
    else
        listFigures = FiguresColored;

    //! Выбор фигуры из списка фигур
    int r = qrand() % listFigures.size();
    auto retval = listFigures.at(r);

    //! Раскрашивание фигуры в зависимости от режима
    for (int i = 0; i < retval.size(); ++i)
        for (int j = 0; j < retval[i].size(); ++j)
            if (retval[i][j] != Qt::black)
            {
                if (m_deletingMode == TrenTetrisDefines::dmRows)
                    retval[i][j] = m_cubeColor;
                else
                {
                    int colIdx = qrand() % m_colorModeColors.size();
                    retval[i][j] = m_colorModeColors.at(colIdx);
                }
            }

    static_cast<TetrisFigure*>(m_wgtNextFigure)->setFigure(retval);
    static_cast<TetrisFigure*>(m_wgtNextFigurePW)->setFigure(retval);

    return retval;
}

