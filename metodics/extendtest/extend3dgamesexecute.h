#ifndef EXTEND3DGAMESEXECUTE_H
#define EXTEND3DGAMESEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QProcess>

namespace Ui {
class Extend3DGamesExecute;
}

class Driver;

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

    QString m_program = "";
    QString m_paramStr = "";
    QProcess* m_process {nullptr};

};

#endif // EXTEND3DGAMESEXECUTE_H
