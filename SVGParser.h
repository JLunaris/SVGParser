#pragma once

#include <QDomDocument>
#include <QGradient>
#include <QGraphicsItem>
#include <QObject>
#include <QSvgRenderer>

class GraphicsPathItem;

class SVGParser : public QObject
{
    Q_OBJECT

    QDomDocument m_doc;
    QSvgRenderer m_renderer;

public Q_SLOTS:
    bool loadSVG(const QString &fileName);

protected:
    // 获取<svg>结点
    QDomElement SVGNode() const { return m_doc.documentElement(); }

    // 解析各结点
    virtual GraphicsPathItem *parseRect(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual GraphicsPathItem *parseEllipse(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual GraphicsPathItem *parseCircle(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual GraphicsPathItem *parsePolyline(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    // <line>被QSvgGenerator视为<polyline>的一种
    // <polygon>被QSvgGenerator视为<path>的一种

    virtual GraphicsPathItem *parsePath(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const;
    virtual QDomNamedNodeMap parseG(const QDomElement &e, QDomNamedNodeMap inheritedAttributes) const;

    virtual QLinearGradient parseLinearGradient(const QDomElement &e) const;

public:
    SVGParser();

    // 获取Qt化后的svg文件的DOM树
    QDomDocument domDocument() const { return m_doc; }

    QRectF viewBoxF() const { return m_renderer.viewBoxF(); }

    QSize size() const { return m_renderer.defaultSize(); }

    [[nodiscard]] std::vector<QGraphicsPathItem *> parse() const;
};
