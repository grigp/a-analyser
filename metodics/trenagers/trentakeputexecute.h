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

namespace Ui {
class TrenTakePutExecute;
}

class Driver;
class TrenTakePutPatientWindow;

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
 * \brief Класс виджета проведения сеанса тренинга TrenagerExecute class
 */
class TrenTakePutExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenTakePutExecute(QWidget *parent = 0);
    ~TrenTakePutExecute();

    void setParams(const QJsonObject &probeParams);

protected:
    void closeEvent(QCloseEvent *event);

    void resizeEvent(QResizeEvent* event);

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    void on_selectChannel(int chanIdx);
    void on_zeroing();
    void on_scaleChange(int scaleIdx);

private:
    Ui::TrenTakePutExecute *ui;

    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlZones = 1,
        zlvlElements = 2,
        zlvlTakeElements = 3,
        zlvlMarker = 4
    };

    void setSceneSize(QSize &size);
    void setZones(const QJsonArray &arrZones, QList<TrenTakePutDefines::GameZoneInfo> &zones);
    void setElements(const QJsonArray &arrElements,
                     QList<TrenTakePutDefines::GameElementInfo> &elements,
                     TrenTakePutDefines::GameStage stage);
    void setMarker(const QJsonObject &objMarker);

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    void elementsInteraction();

    void processStageWorking();

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

    /*!
     * \brief Генерация новой сцены
     */
    void generateNewScene(const bool isAddScore);

    void allocPairPictires();

    void allocSplitPictures();

    int getNextPictureNumber(const int filesCount);

    /*!
     * \brief Загрузка списка картинок для построения в режиме puzzle
     * \param folder - папка с картинками
     */
    void loadPicturesPuzzle(const QString &folder);

    /*!
     * \brief Загрузка списка картинок для построения в режиме pair
     * \param folder - папка с картинками
     */
    void loadPicturesPair(const QString &folder);

    void allocBySeparatePositions(TrenTakePutDefines::TakeOrder &takeOrder,
                                  QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                  QList<TrenTakePutDefines::GameElementInfo> &elements,
                                  const int zOrder);

    void allocElements(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                       QList<TrenTakePutDefines::GameElementInfo> &elements,
                       int enabled,         ///< -1 - все, 1 - только enabled == true, 0 - только enabled == false
                       const int zOrder);

    TrenTakePutDefines::GameElement* allocElement(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                                  TrenTakePutDefines::GameElementInfo *element,
                                                  const QPixmap *pixmap,
                                                  const int zOrder,
                                                  const int zoneIdx = -1);

    bool isEmptyZonesPresent(QList<TrenTakePutDefines::GameZoneInfo> &zones) const;

    int scaleMultiplier() const;

    /*!
     * \brief Проверяет, находится ли маркер на элементе
     * В режиме захвата m_gameStage == gmTake берет только элементы с movableWithMarker == true,
     * в режиме укладки m_gameStage == gmPut берет только элементы с movableWithMarker == false
     * \return указатель на элемент или nullptr, если не на элементе
     */
    TrenTakePutDefines::GameElement* markerOnGameElement();

    void showPatientWindow();
    void hidePatientWindow();

    void showFactors();

    QGraphicsScene* m_scene {nullptr};
    double m_prop = 1; ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    QGraphicsPixmapItem *m_marker {nullptr};
    QJsonObject m_markerObj;

    int m_frequency = 50;        ///< Частота дискретизации

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::DecartCoordControl* m_dcControl;  ///< Управление декартовыми данными в драйвере

    TrenTakePutDefines::GameStage m_gameStage {TrenTakePutDefines::gsTake};

    TrenTakePutDefines::TakeOrder m_takeTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    TrenTakePutDefines::TakeOrder m_putTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    int m_timeFixTake {0};
    int m_timeFixPut {0};
    TrenTakePutDefines::StageMode m_stageMode {TrenTakePutDefines::smTakePut};

    ///< Зоны игры и элементы
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesTake;
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesPut;
    QList<TrenTakePutDefines::GameElementInfo> m_elementsTake;
    QList<TrenTakePutDefines::GameElementInfo> m_elementsPut;

    TrenTakePutDefines::GameElement *m_elementTake {nullptr};
    TrenTakePutDefines::GameElement *m_elementPut {nullptr};
    int m_putElementCount {0}; ///< Счетчик элементов, уложенных в корзину
    int m_fixCount {0};        ///< Счетчик пакетов для фиксации захвата или укладки
    int m_score {0};           ///< Очки, набранные в пробе
    int m_errorsCount {0};     ///< Счетчик ошибок
    bool m_isError {false};    ///< Признак ошибки. Для исключения "лишних" ошибок
    QPointF m_pos;

    QMediaPlayer m_player;
    TrenTakePutPatientWindow* m_patientWindow {nullptr};   ///< Окно пациента
    QList<QString> m_filesPuzzle;      ///< Список файлов для построения картинок
    QList<FilesPair> m_filesPair;      ///< Список файлов для парных файлов
    QSet<int> m_filesUsed;   ///< Номера файлов, которые уже использовались

//    DataDefines::PatientKard m_kard;

};


#endif // TRENTAKEPUTEXECUTE_H
