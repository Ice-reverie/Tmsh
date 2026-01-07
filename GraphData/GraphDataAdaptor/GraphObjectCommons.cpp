#include "GraphObjectCommons.h"

#include <QColor>

namespace Graph
{
    //名称
    QString GraphObjectCommons::_dataColorArrayName = "dataColorArrayName";
    QString GraphObjectCommons::_dataPointIDArrayName = "dataPointIDArrayName";
    QString GraphObjectCommons::_dataCellIDArrayName = "dataCellIDArrayName";

    //颜色
    QColor GraphObjectCommons::_geoPointDefaultColor = Qt::white;
    QColor GraphObjectCommons::_geoCurveDefaultColor = Qt::white;
    QColor GraphObjectCommons::_geoFaceDefaultColor = Qt::white;
    QColor GraphObjectCommons::_geoSolidDefaultColor = Qt::white;
    QColor GraphObjectCommons::_meshFaceDefaultColor = Qt::white;

    //大小
    double GraphObjectCommons::_geoPointDefaultSize = 5.0;
    double GraphObjectCommons::_geoLineDefaultSize = 2.0;

    //偏移参数
    double GraphObjectCommons::_geoGraphOffset = 1.0;
    // 网格线/面与几何面共面时需要更强的偏移，避免开启跨 renderer 深度缓冲后出现几何压住网格线的深度竞争
    double GraphObjectCommons::_mesh3DGraphOffset = -2.0;
    double GraphObjectCommons::_mesh2DGraphOffset = -6.0;
}
