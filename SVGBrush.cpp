#include "SVGBrush.h"

void SVGBrush::parseFill(QStringView fill, const GradientMap &gradientMap)
{
    if (fill.isEmpty())
        return;
    else if (fill == "none")
        setStyle(Qt::NoBrush);
    else if (fill.startsWith(QString {"url"})) {
        // 取得id
        qsizetype begin {fill.indexOf('#') + 1};
        qsizetype end {fill.indexOf(')')};
        fill.slice(begin, end - begin);

        // 根据id取得对应的渐变，然后应用到brush上
        const auto &gradient {gradientMap.at(fill.toString())};
        if (std::holds_alternative<QLinearGradient>(gradient))
            QBrush::operator=(std::get<QLinearGradient>(gradient));
        else if (std::holds_alternative<QRadialGradient>(gradient))
            QBrush::operator=(std::get<QRadialGradient>(gradient));

    } else {
        setStyle(Qt::SolidPattern);
        setColor(QColor::fromString(fill));
    }
}

void SVGBrush::parseFillOpacity(QStringView fillOpacity)
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

void SVGBrush::syncWithAttributes(const QDomNamedNodeMap &attributes, const GradientMap &gradientMap)
{
    // 获取需要的属性值。如果不存在该属性，则结果为空字符串。
    QString fill {attributes.namedItem("fill").nodeValue()};
    QString fillOpacity {attributes.namedItem("fill-opacity").nodeValue()};

    // 解析属性
    parseFill(fill, gradientMap);
    parseFillOpacity(fillOpacity);
}
