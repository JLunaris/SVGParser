#pragma once

#include <QBrush>
#include <QDomNamedNodeMap>

class SVGBrush : public QBrush
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#FillProperties
    // warning: 该类假定传给它的属性值要么是空字符串，要么是符合SVG标准要求的值。使用错误值被视为未定义行为。

private:
    void parseFill(const QString &fill);
    void parseFillOpacity(const QString &fillOpacity);

public:
    SVGBrush();
    void syncWithAttributes(const QDomNamedNodeMap &attributes);
};
