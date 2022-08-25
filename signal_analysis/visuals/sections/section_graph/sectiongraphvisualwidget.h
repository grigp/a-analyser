#ifndef SECTIONGRAPHVISUALWIDGET_H
#define SECTIONGRAPHVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class SectionGraphVisualWidget;
}

/*!
 * \brief Виджет визуализатора графика секции The SectionGraphVisualWidget class
 */
class SectionGraphVisualWidget : public SectionVisual
{
    Q_OBJECT

public:
    explicit SectionGraphVisualWidget(VisualDescriptor* visual,
                                      const QString& testUid, const QString& probeUid, const QString& channelId, const QString& sectionNumber,
                                      QWidget *parent = nullptr);
    ~SectionGraphVisualWidget() override;

    bool isValid() override;
    void calculate() override;


private:
    Ui::SectionGraphVisualWidget *ui;
};

#endif // SECTIONGRAPHVISUALWIDGET_H
