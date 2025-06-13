#pragma once

#include <QDomNamedNodeMap>
#include <QPainterPath>

class SVGPainterPath : public QPainterPath
{
    void parseFillRule(const QString &fillRule);

public:
    SVGPainterPath() noexcept;
    explicit SVGPainterPath(const QPointF &startPoint);
    SVGPainterPath(const SVGPainterPath &other);

    void syncWithAttributes(const QDomNamedNodeMap &attributes);
};
