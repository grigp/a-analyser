#ifndef TRENTAKEPUTEXECUTE_H
#define TRENTAKEPUTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QMediaPlayer>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "trentakeputdefines.h"
#include "graphiccommon.h"
#include "trenstabexecute.h"

namespace Ui {
class TrenTakePutExecute;
}

/*!
 * \brief Структура записи о парных файлов FilesPair struct
 */
struct FilesPair
{
    QString take;
    QString put;
    FilesPair(const QString &ts, const QString &ps)
        : take(ts), put(ps){}
};

/*!
 * \brief Структура параметров звуковой схемы SoundSheme struct
 */
struct SoundSheme
{
    QString ok;
    QString error;
    QString scene;
    QString onTarget;
};

/*!
 * \brief Класс виджета проведения сеанса тренинга TrenagerExecute class
 */
class TrenTakePutExecute : public TrenStabExecute
{
    Q_OBJECT

public:
    explicit TrenTakePutExecute(QWidget *parent = nullptr);
    ~TrenTakePutExecute() override;

    void setParams(const QJsonObject &probeParams) override;

protected:
    /*!
     * \brief Взаимодействие элементов
     * Все управление сценой, маркеры и т.д.
     */
    void elementsInteraction(DeviceProtocols::DeviceData *data) override;


    /*!
     * \brief Генерация новой сцены
     */
    void generateNewScene() override;

    void addScoreNewScene();

    /*!
     * \brief Заполнить frame своими элементами, отображающими процесс игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameParams(QFrame *frame) override;

    /*!
     * \brief Заполнить frame своими элементами, отображающими подсказки в процессе игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameHints(QFrame *frame) override;

    QString getAutoSaveParamsSectionName() const override {return "TrenagerType1";}

protected slots:
    void on_recording() override;

private:
    Ui::TrenTakePutExecute *ui;

    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlZones = 2,
        zlvlElements = 3,
        zlvlTakeElements = 4,
        zlvlMarker = 5
    };

    ///< Типы элементов
    enum ElementKinds
    {
          ekTakePut = ekUser + 0   ///< захват / укладка
    };


    void setZones(const QJsonArray &arrZones, QList<TrenTakePutDefines::GameZoneInfo> &zones);
    void setElements(const QJsonArray &arrElements,
                     QList<GraphicCommon::GameElementInfo> &elements,
                     TrenTakePutDefines::GameStage stage);
    void setMarker(const QJsonObject &objMarker);

    void processStageWorking();

    /*!
     * \brief Обработка лимита времени для элементов
     */
    void elementsTimeLimitWorking();

    /*!
     * \brief Обработка подвижности для элементов
     */
    void elementsMobileWorking();

    void setRandomWorkMobilePosition(GraphicCommon::GameElement* ge);

    void setLinearMovingMobilePosition(GraphicCommon::GameElement* ge);

    void fixingTake();

    /*!
     * \brief Переключение этапов пробы
     */
    void fixingStage();

    void fixingError();

    /*!
     * \brief Задержка, чтобы зафиксировать собранную сцену перед генерацией новой
     */
    void delayScene();

    void allocPairPictires();

    void allocSplitPictures();

    int getNextPictureNumber(const int filesCount);

    /*!
     * \brief Загрузка списка одиночных картинок для построения, например в режиме puzzle
     * \param folder - папка с картинками
     */
    void loadPicturesSingle(const QString &folder, const QString &suffix);

    /*!
     * \brief Загрузка списка картинок для построения в режиме pair
     * \param folder - папка с картинками
     */
    void loadPicturesPair(const QString &folder);

    void allocByRandomPositions(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                QList<GraphicCommon::GameElementInfo> &elements);

    void allocBySeparatePositions(TrenTakePutDefines::TakeOrder &takeOrder,
                                  QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                  QList<GraphicCommon::GameElementInfo> &elements,
                                  const int zOrder);

    void allocElements(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                       QList<GraphicCommon::GameElementInfo> &elements,
                       int enabled,         ///< -1 - все, 1 - только enabled == true, 0 - только enabled == false
                       const int zOrder);

    GraphicCommon::GameElement* allocElement(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                  GraphicCommon::GameElementInfo *element,
                                                  const QPixmap *pixmap,
                                                  const int zOrder,
                                                  const int zoneIdx = -1);

    bool isEmptyZonesPresent(QList<TrenTakePutDefines::GameZoneInfo> &zones) const;

    /*!
     * \brief Проверяет, находится ли маркер на элементе
     * В режиме захвата m_gameStage == gmTake берет только элементы с movableWithMarker == true,
     * в режиме укладки m_gameStage == gmPut берет только элементы с movableWithMarker == false
     * \return указатель на элемент или nullptr, если не на элементе
     */
    GraphicCommon::GameElement* markerOnGameElement();

    void finishTest() override;

    /*!
     * \brief Изменяет значение кол-ва ошибок
     * \param value - значение, на которое меняем.
     */
    void changeErrors(const int value);


    GraphicCommon::MarkerElement *m_marker {nullptr};
    QJsonObject m_markerObj;
    TrenTakePutDefines::TargetAdvMode m_targetAdvMode {TrenTakePutDefines::tamNone};

    TrenTakePutDefines::GameStage m_gameStage {TrenTakePutDefines::gsTake};

    TrenTakePutDefines::TakeOrder m_takeTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    TrenTakePutDefines::TakeOrder m_putTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    int m_timeFixTake {0};
    int m_timeFixPut {0};
    TrenTakePutDefines::StageMode m_stageMode {TrenTakePutDefines::smTakePut};
    int m_delayAfterStage {0};

    ///< Зоны игры и элементы
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesTake;
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesPut;
    QList<GraphicCommon::GameElementInfo> m_elementsTake;
    QList<GraphicCommon::GameElementInfo> m_elementsPut;

    GraphicCommon::GameElement *m_elementTake {nullptr};
    GraphicCommon::GameElement *m_elementPut {nullptr};
    int m_putElementCount {0}; ///< Счетчик элементов, уложенных в корзину
    int m_fixCount {0};        ///< Счетчик пакетов для фиксации захвата или укладки
    int m_errorsCount {0};     ///< Счетчик ошибок
    bool m_isError {false};    ///< Признак ошибки. Для исключения "лишних" ошибок
    QPointF m_pos;
    QLabel* m_lblErrors {nullptr};

    QLabel* m_lblPicture {nullptr};    ///< Подсказка - собираемая картинка
    QLabel* m_lblPicturePW {nullptr};  ///< Подсказка - собираемая картинка для окна пациентов

    QMediaPlayer m_player;
    SoundSheme m_soundSheme;
    QList<QString> m_filesSingle;      ///< Список одиночных файлов для построения картинок или для охоты
    QList<FilesPair> m_filesPair;      ///< Список файлов для парных файлов
    QSet<int> m_filesUsed;   ///< Номера файлов, которые уже использовались
};


#endif // TRENTAKEPUTEXECUTE_H
