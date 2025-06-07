#pragma once

#include <QDomNamedNodeMap>
#include <QPen>

class SVGPen : public QPen
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#StrokeProperty
    // warning: 该类假定传给它的属性值要么是空字符串，要么是符合SVG标准要求的值。使用错误值被视为未定义行为。

private:
    void parseStroke(const QString &stroke);
    void parseStrokeWidth(const QString &strokeWidth);
    void parseStrokeLinecap(const QString &strokeLinecap);
    void parseStrokeLinejoin(const QString &strokeLinejoin);
    void parseStrokeMiterlimit(const QString &strokeMiterlimit);
    void parseStrokeDasharray(const QString &strokeDasharray);
    void parseStrokeDashoffset(const QString &strokeDashoffset);
    void parseStrokeOpacity(const QString &strokeOpacity);
    void parseVectorEffect(const QString &vectorEffect);

public:
    SVGPen();
    void syncWithAttributes(const QDomNamedNodeMap &attributes);
};
