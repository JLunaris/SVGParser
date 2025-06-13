#include "SVGBrush.h"

void SVGBrush::parseFill(const QString &fill)
{
    if (fill.isEmpty())
        return;
    else if (fill == "none")
        setStyle(Qt::NoBrush);
    else {
        setStyle(Qt::SolidPattern);
        setColor(QColor::fromString(fill));
    }
}

void SVGBrush::parseFillOpacity(const QString &fillOpacity)
{
    if (fillOpacity.isEmpty())
        return;
    else {
        QColor color {QBrush::color()};
        color.setAlphaF(fillOpacity.toFloat());
        setColor(color);
    }
}

SVGBrush::SVGBrush()
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#FillProperties
    // 根据 SVG 1.2 官方规范定义画刷的默认值

    setColor(Qt::black); // fill默认为black
    // fill-opacity默认为1 (与Qt默认行为一致，无需显式指定)
}

void SVGBrush::syncWithAttributes(const QDomNamedNodeMap &attributes)
{
    // 获取需要的属性值。如果不存在该属性，则结果为空字符串。
    QString fill {attributes.namedItem("fill").nodeValue()};
    QString fillOpacity {attributes.namedItem("fill-opacity").nodeValue()};

    // 解析属性
    parseFill(fill);
    parseFillOpacity(fillOpacity);
}
