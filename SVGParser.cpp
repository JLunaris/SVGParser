#include "SVGParser.h"

#include "GraphicsPathItem.h"
#include "SVGBrush.h"
#include "SVGPainterPath.h"
#include "SVGPen.h"

#include <QBuffer>
#include <QDebug>
#include <QPainter>
#include <QSvgGenerator>

bool SVGParser::loadSVG(const QString &fileName)
{
    // Load on QSvgRenderer.
    if (!m_renderer.load(fileName)) {
        qWarning() << "Failed to load file on m_renderer";
        return false;
    }

    // Create QBuffer as I/O device.
    QBuffer svgBuffer;
    if (!svgBuffer.open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open buffer for SVG output";
        return false;
    }

    // Set svg generator.
    QSvgGenerator generator;
    generator.setOutputDevice(&svgBuffer);
    generator.setViewBox(m_renderer.viewBoxF());

    // Render to generator.
    QPainter painter;
    painter.begin(&generator);
    m_renderer.render(&painter, m_renderer.viewBoxF()); // 显式指定渲染范围，解决了因“同时指定了viewBox和size属性”时引起的svg缩放而导致只渲染部分区域的问题
    painter.end();

    // Set current position to 0.
    svgBuffer.seek(0);

    // Read from svgBuffer to construct DOM tree.
    if (!m_doc.setContent(&svgBuffer)) {
        qWarning() << "Failed to parse SVG XML data";
        return false;
    }

    // Close svgBuffer.
    svgBuffer.close();

    return true;
}

GraphicsPathItem *SVGParser::parseRect(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);

    // 获取元素的属性。如果属性值无效或不存在该属性，则结果为0。
    qreal x {e.attribute("x").toDouble()};
    qreal y {e.attribute("y").toDouble()};
    qreal width {e.attribute("width").toDouble()};
    qreal height {e.attribute("height").toDouble()};

    // parse attributes
    pen.syncWithAttributes(e.attributes());
    brush.syncWithAttributes(e.attributes());
    path.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    path.addRect(x, y, width, height);
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);

    return item;
}

GraphicsPathItem *SVGParser::parseEllipse(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);

    // 获取元素的属性。如果属性值无效或不存在该属性，则结果为0。
    qreal cx {e.attribute("cx").toDouble()};
    qreal cy {e.attribute("cy").toDouble()};
    qreal rx {e.attribute("rx").toDouble()};
    qreal ry {e.attribute("ry").toDouble()};

    // parse attributes
    pen.syncWithAttributes(e.attributes());
    brush.syncWithAttributes(e.attributes());
    path.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    path.addEllipse(cx, cy, rx, ry);
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);

    return item;
}

GraphicsPathItem *SVGParser::parseCircle(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);

    // 获取元素的属性。如果属性值无效或不存在该属性，则结果为0。
    qreal cx {e.attribute("cx").toDouble()};
    qreal cy {e.attribute("cy").toDouble()};
    qreal r {e.attribute("r").toDouble()};

    // parse attributes
    pen.syncWithAttributes(e.attributes());
    brush.syncWithAttributes(e.attributes());
    path.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    path.addEllipse(cx, cy, r, r);
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);

    return item;
}
