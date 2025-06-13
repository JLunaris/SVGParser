#pragma once

#include <QDomNamedNodeMap>
#include <QTransform>

class SVGTransform : public QTransform
{
    void parseTransform(QStringView transform);

public:
    void syncWithAttributes(const QDomNamedNodeMap &attributes);
};
