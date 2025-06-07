#include "SVGPainterPath.h"

void SVGPainterPath::parseFillRule(const QString &fillRule)
{
    if (fillRule.isEmpty())
        return;
    else if (fillRule == "nonzero")
        setFillRule(Qt::FillRule::WindingFill);
    else if (fillRule == "evenodd")
        setFillRule(Qt::OddEvenFill);
}

SVGPainterPath::SVGPainterPath() noexcept
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#FillProperties
    // 根据 SVG 1.2 官方规范定义路径的默认值

    setFillRule(Qt::WindingFill); // fill-rule默认为nonzero
}

SVGPainterPath::SVGPainterPath(const QPointF &startPoint)
        : QPainterPath(startPoint)
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#FillProperties
    // 根据 SVG 1.2 官方规范定义路径的默认值

    setFillRule(Qt::WindingFill); // fill-rule默认为nonzero
}

SVGPainterPath::SVGPainterPath(const SVGPainterPath &other)
        : QPainterPath(other)
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#FillProperties
    // 根据 SVG 1.2 官方规范定义路径的默认值

    setFillRule(Qt::WindingFill); // fill-rule默认为nonzero
}

void SVGPainterPath::syncWithAttributes(const QDomNamedNodeMap &attributes)
{
    // 获取需要的属性值。如果不存在该属性，则结果为空字符串。
    QString fillRule {attributes.namedItem("fill-rule").nodeValue()};

    // 解析属性
    parseFillRule(fillRule);
}
