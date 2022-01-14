#include "triangleresultdata.h"

TriangleResultData::TriangleResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

TriangleResultData::TriangleResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int TriangleResultData::freq() const
{
    return data()["freq"].toInt();
}

void TriangleResultData::setFreq(const int freq)
{
    QJsonObject obj = data();
    obj["freq"] = freq;
    setData(obj);
}

int TriangleResultData::diap() const
{
    return data()["diap"].toInt();
}

void TriangleResultData::setDiap(const int diap)
{
    QJsonObject obj = data();
    obj["diap"] = diap;
    setData(obj);
}

int TriangleResultData::trainingLength() const
{
    return data()["training_length"].toInt();
}

void TriangleResultData::setTrainingLength(const int length)
{
    QJsonObject obj = data();
    obj["training_length"] = length;
    setData(obj);
}

int TriangleResultData::timeOffsetMarker() const
{
    return data()["time_offset_marker"].toInt();
}

void TriangleResultData::setTimeOffsetMarker(const int time)
{
    QJsonObject obj = data();
    obj["time_offset_marker"] = time;
    setData(obj);
}

QPointF TriangleResultData::topCorner() const
{
    auto corner = data()["top_corner"].toObject();
    double x = corner["x"].toDouble();
    double y = corner["y"].toDouble();
    return QPointF(x, y);
}

void TriangleResultData::setTopCorner(const QPointF point)
{
    QJsonObject obj = data();
    QJsonObject corner;
    corner["x"] = point.x();
    corner["y"] = point.y();
    obj["top_corner"] = corner;
    setData(obj);
}

QPointF TriangleResultData::leftDownCorner() const
{
    auto corner = data()["left_down_corner"].toObject();
    double x = corner["x"].toDouble();
    double y = corner["y"].toDouble();
    return QPointF(x, y);
}

void TriangleResultData::setLeftDownCorner(const QPointF point)
{
    QJsonObject obj = data();
    QJsonObject corner;
    corner["x"] = point.x();
    corner["y"] = point.y();
    obj["left_down_corner"] = corner;
    setData(obj);
}

QPointF TriangleResultData::rightDownCorner() const
{
    auto corner = data()["right_down_corner"].toObject();
    double x = corner["x"].toDouble();
    double y = corner["y"].toDouble();
    return QPointF(x, y);
}

void TriangleResultData::setRightDownCorner(const QPointF point)
{
    QJsonObject obj = data();
    QJsonObject corner;
    corner["x"] = point.x();
    corner["y"] = point.y();
    obj["right_down_corner"] = corner;
    setData(obj);
}
