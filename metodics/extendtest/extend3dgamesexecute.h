#ifndef EXTEND3DGAMESEXECUTE_H
#define EXTEND3DGAMESEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QProcess>

namespace Ui {
class Extend3DGamesExecute;
}

class Driver;

/*!
 * \brief Структура параметров показателя в результатах игры GameResultsFactorInfo struct
 */
struct GameResultsFactorInfo
{
    QString uid;   //! uid показателя
    double value;  //! Значение показателя
};

class Extend3DGamesExecute : public QWidget
{
    Q_OBJECT

public:
    explicit Extend3DGamesExecute(QWidget *parent = nullptr);
    ~Extend3DGamesExecute();

    void run(const QJsonObject &params);

private:
    Ui::Extend3DGamesExecute *ui;

    /*!
     * \brief Выполнение внешней программы
     */
    void programExecute();

    void saveDriverParams() const;

    void savePatientData() const;

    void saveGameParams() const;

    void getGameResult() const;

    QList<GameResultsFactorInfo> getResultsFactors(const QString fn) const;
    QString getAAnalyserUIDByGamesUid(const QString fUID) const;
    double stringToDouble(const QString strVal) const;

    QString m_program = "";
    QString m_paramStr = "";
    QProcess* m_process {nullptr};

};

#endif // EXTEND3DGAMESEXECUTE_H
