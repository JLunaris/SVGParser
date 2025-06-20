#pragma once

#include "SVGBrush.h"
#include "SVGPainterPath.h"
#include "SVGPen.h"
#include "SVGTransform.h"

#include <QDomDocument>
#include <QGradient>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QSvgRenderer>

class SVGParser : public QObject
{
    Q_OBJECT

    using GradientMap = std::unordered_map<QString, std::variant<QLinearGradient, QRadialGradient>>;

public:
    struct ParseResult {
        SVGPen pen;
        SVGBrush brush;
        SVGPainterPath painterPath;
        SVGTransform transform;
    };

private:
    QDomDocument m_doc;
    QSvgRenderer m_renderer;
    GradientMap m_globalGradients;

public Q_SLOTS:
    bool loadSVG(const QString &fileName);

private:
    QDomNamedNodeMap parseG(const QDomElement &e, QDomNamedNodeMap inheritedAttributes) const;
    GradientMap parseGradients(const QDomElement &e) const;

protected:
    // 获取<svg>结点
    QDomElement SVGNode() const { return m_doc.documentElement(); }

    // 解析各结点
    virtual ParseResult parseRect(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual ParseResult parseEllipse(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual ParseResult parseCircle(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual ParseResult parsePolyline(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    // <line>被QSvgGenerator视为<polyline>的一种
    // <polygon>被QSvgGenerator视为<path>的一种
    virtual ParseResult parsePath(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual QLinearGradient parseLinearGradient(const QDomElement &e) const;
    virtual QRadialGradient parseRadialGradient(const QDomElement &e) const;

public:
    SVGParser();

    // 获取Qt化后的svg文件的DOM树
    QDomDocument domDocument() const { return m_doc; }

    QRectF viewBoxF() const { return m_renderer.viewBoxF(); }

    QSize size() const { return m_renderer.defaultSize(); }

    [[nodiscard]] std::vector<ParseResult> parse();

    template<std::derived_from<QGraphicsItem> GraphicsItem>
    std::vector<GraphicsItem *> parse(QGraphicsScene *scene = nullptr);
};

template<std::derived_from<QGraphicsItem> GraphicsItem>
std::vector<GraphicsItem *> SVGParser::parse(QGraphicsScene *scene)
{
    std::vector<ParseResult> parseResults {parse()};
    std::vector<GraphicsItem *> items;

    for (const auto &parseResult: parseResults) {
        GraphicsItem *item {new GraphicsItem};

        item->setPen(parseResult.pen);
        item->setBrush(parseResult.brush);
        item->setPath(parseResult.painterPath);
        item->setTransform(parseResult.transform);

        items.push_back(item);

        if (scene)
            scene->addItem(item);
    }

    return items;
}
