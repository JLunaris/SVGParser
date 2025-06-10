#include "SVGPen.h"

void SVGPen::parseStroke(const QString &stroke)
{
    if (stroke.isEmpty())
        return;
    else if (stroke == "none")
        setStyle(Qt::NoPen);
    else
        setColor(QColor::fromString(stroke));
}

void SVGPen::parseStrokeWidth(const QString &strokeWidth)
{
    if (strokeWidth.isEmpty())
        return;
    else
        setWidthF(strokeWidth.toDouble());
}

void SVGPen::parseStrokeLinecap(const QString &strokeLinecap)
{
    if (strokeLinecap.isEmpty())
        return;
    else if (strokeLinecap == "butt")
        setCapStyle(Qt::PenCapStyle::FlatCap);
    else if (strokeLinecap == "round")
        setCapStyle(Qt::PenCapStyle::RoundCap);
    else if (strokeLinecap == "square")
        setCapStyle(Qt::PenCapStyle::SquareCap);
}

void SVGPen::parseStrokeLinejoin(const QString &strokeLinejoin)
{
    if (strokeLinejoin.isEmpty())
        return;
    else if (strokeLinejoin == "miter")
        setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    else if (strokeLinejoin == "round")
        setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    else if (strokeLinejoin == "bevel")
        setJoinStyle(Qt::PenJoinStyle::BevelJoin);
}

void SVGPen::parseStrokeMiterlimit(const QString &strokeMiterlimit)
{
    if (strokeMiterlimit.isEmpty())
        return;
    else
        setMiterLimit(strokeMiterlimit.toDouble());
}

void SVGPen::parseStrokeDasharray(const QString &strokeDasharray)
{
    if (strokeDasharray.isEmpty())
        return;
    else if (strokeDasharray == "none")
        setStyle(Qt::SolidLine);
    else {
        QStringList stringList {strokeDasharray.split(',')};
        QList<qreal> pattern;

        for (const QString &string: stringList)
            pattern.append(string.toDouble());

        setDashPattern(pattern);
    }
}

void SVGPen::parseStrokeDashoffset(const QString &strokeDashoffset)
{
    if (strokeDashoffset.isEmpty())
        return;
    else
        setDashOffset(strokeDashoffset.toDouble());
}

void SVGPen::parseStrokeOpacity(const QString &strokeOpacity)
{
    if (strokeOpacity.isEmpty())
        return;
    else {
        QColor color {QPen::color()};
        color.setAlphaF(strokeOpacity.toFloat());
        setColor(color);
    }
}

void SVGPen::parseVectorEffect(const QString &vectorEffect)
{
    if (vectorEffect.isEmpty())
        return;
    else if (vectorEffect == "non-scaling-stroke")
        setCosmetic(true);
    else if (vectorEffect == "none")
        setCosmetic(false);
}

SVGPen::SVGPen()
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#StrokeProperty
    // 根据 SVG 1.2 官方规范定义画笔的默认值

    setStyle(Qt::NoPen); // stroke默认为none
    setWidth(1); // stroke_width默认为1
    setCapStyle(Qt::PenCapStyle::FlatCap); // stroke-linecap默认为butt
    setJoinStyle(Qt::PenJoinStyle::MiterJoin); // stroke-linejoin默认为miter
    setMiterLimit(4); // stroke-miterlimit默认为4
    // stroke-dasharray默认为none (与Qt默认行为一致，无需显式指定)
    // stroke-dashoffset默认为0 (与Qt默认行为一致，无需显式指定)
    // stroke-opacity默认为1 (与Qt默认行为一致，无需显式指定)
}

void SVGPen::syncWithAttributes(const QDomNamedNodeMap &attributes)
{
    // 获取需要的属性值。如果不存在该属性，则结果为空字符串。
    QString stroke {attributes.namedItem("stroke").nodeValue()};
    QString strokeWidth {attributes.namedItem("stroke-width").nodeValue()};
    QString strokeLinecap {attributes.namedItem("stroke-linecap").nodeValue()};
    QString strokeLinejoin {attributes.namedItem("stroke-linejoin").nodeValue()};
    QString strokeMiterlimit {attributes.namedItem("stroke-miterlimit").nodeValue()};
    QString strokeDasharray {attributes.namedItem("stroke-dasharray").nodeValue()};
    QString strokeDashoffset {attributes.namedItem("stroke-dashoffset").nodeValue()};
    QString strokeOpacity {attributes.namedItem("stroke-opacity").nodeValue()};
    QString vectorEffect(attributes.namedItem("vector-effect").nodeValue());

    // 解析属性
    parseStroke(stroke);
    parseStrokeWidth(strokeWidth);
    parseStrokeLinecap(strokeLinecap);
    parseStrokeLinejoin(strokeLinejoin);
    parseStrokeMiterlimit(strokeMiterlimit);
    parseStrokeDasharray(strokeDasharray);
    parseStrokeDashoffset(strokeDashoffset);
    parseStrokeOpacity(strokeOpacity);
    parseVectorEffect(vectorEffect);
}
