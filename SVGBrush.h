#pragma once

#include <QBrush>
#include <QDomNamedNodeMap>

class SVGBrush : public QBrush
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#FillProperties
    // warning: 该类假定传给它的属性值要么是空字符串，要么是符合SVG标准要求的值。使用错误值被视为未定义行为。

    using GradientMap = std::unordered_map<QString, std::variant<QLinearGradient, QRadialGradient>>;

private:
    void parseFill(QStringView fill, const GradientMap &gradientMap);
    void parseFillOpacity(QStringView fillOpacity);

public:
    SVGBrush();

    void syncWithAttributes(const QDomNamedNodeMap &attributes, const GradientMap &gradientMap = GradientMap {});
};
