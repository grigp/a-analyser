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
#include "trenresultfactors.h"
#include "tetrisglass.h"

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
    {{Qt::white, Qt::white, Qt::white}, {Qt::white, Qt::white, Qt::white}, {Qt::white, Qt::white, Qt::white}},
};

///< Фигура для кубиков. Один кубик. Для единства обработки - список. Безобразно, но однообразно
QList<QVector<QVector<QColor>>> OneCube = {{{Qt::white}}};

}


TrenTetrisExecute::TrenTetrisExecute(QWidget *parent) :
    TrenStabExecute (parent),
    ui(new Ui::TrenTetrisExecute)
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
}

void TrenTetrisExecute::on_recording()
{
    TrenStabExecute::on_recording();

    m_rowsDeleted = 0;
    changeRowsDeleted(0);
}

//int n = 0; Для вращения

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

//            ++n; Вращение
//            if (n % 250 == 0)
//                m_glass->rotateFigure();
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
        if (fig.contains(mxs, mys))
        {
            m_offsX = mxs - fig.center().x();
            m_offsY = mys - fig.center().y();
            m_tmStage = TrenTetrisDefines::tmsPut;
            m_marker->setVisible(false);
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
                                       my + scene()->sceneRect().height() / 2 - m_offsY))
        {
            //! Если положена, то...
            putFigure();
            m_tmStage = TrenTetrisDefines::tmsTake;
            //! Добавление новой фигуры
            m_glass->setNewFigure(newFigure());
            m_marker->setVisible(true);
        }
    }
}

void TrenTetrisExecute::putFigure()
{
    //! Снятие пометки последней фигуры в режиме удаления строк
    if (m_deletingMode == TrenTetrisDefines::dmRows)
        clearLastColor();

    //! Установка новой фигуры
    auto pos = m_glass->getFigurePosition();
    for (int i = 0; i < pos.width(); ++i)
    {
        for (int j = 0; j < pos.height(); ++j)
        {
            if (m_deletingMode == TrenTetrisDefines::dmColored)
            {
                auto color = m_glass->getFigureColor(i, j);
                if (color != Qt::black)
                    m_glass->setValue(pos.x() + i, pos.y() + j, color);
            }
            else
            if (m_deletingMode == TrenTetrisDefines::dmRows)
            {
                if (m_glass->getFigureColor(i, j) != Qt::black)
                    m_glass->setValue(pos.x() + i, pos.y() + j, m_lastColor);
            }
        }
    }

    //! Анализ и удаление кубиков
    if (m_deletingMode == TrenTetrisDefines::dmColored)
        deleteOneColorCubes();
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
    //! Добавление новой фигуры
    m_glass->setNewFigure(newFigure());

    setMarker(m_markerObj);
    scene()->addItem(m_marker);
    m_marker->setZValue(zlvlMarker);
    m_marker->setPos(0 - m_marker->boundingRect().width() / 2,
                     0 - m_marker->boundingRect().height() / 2);
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
    m_glass->setCubeImage(m_cubeImageFileName);
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
    foreach (auto row, fullRows)
        deleteRow(row);
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

void TrenTetrisExecute::deleteOneColorCubes()
{

}

QVector<QVector<QColor>> TrenTetrisExecute::newFigure()
{
    //! Выбор списка фигур в зависимости от режима
    QList<QVector<QVector<QColor>>> listFigures;
    if (m_complexityMode == TrenTetrisDefines::cmCubes)
        listFigures = OneCube;
    else
    if (m_movingMode == TrenTetrisDefines::mmTake && m_complexityMode == TrenTetrisDefines::cmFigures)
        listFigures = FiguresRectis;
    else
        listFigures = FiguresTetris;

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

    return retval;
}

