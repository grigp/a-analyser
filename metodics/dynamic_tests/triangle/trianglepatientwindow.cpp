#include "trianglepatientwindow.h"

TrianglePatientWindow::TrianglePatientWindow(QWidget *parent)
    : StabDynamicTestPatientWindow (parent)
{
    setWindowTitle("Тест \"Треугольник\"");
}

void TrianglePatientWindow::run()
{

}

void TrianglePatientWindow::stop()
{

}

void TrianglePatientWindow::setMarker(const double x, const double y)
{
    StabDynamicTestPatientWindow::setMarker(x, y);
}
