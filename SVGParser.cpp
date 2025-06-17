#include "SVGParser.h"

#include "GraphicsPathItem.h"
#include "SVGBrush.h"
#include "SVGPainterPath.h"
#include "SVGPen.h"
#include "SVGTransform.h"

#include <QBuffer>
#include <QDebug>
#include <QPainter>
#include <QRegularExpression>
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
    SVGTransform transform;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);
    transform.syncWithAttributes(inheritedAttributes);

    // 获取元素的属性。如果属性值无效或不存在该属性，则结果为0。
    qreal x {e.attribute("x").toDouble()};
    qreal y {e.attribute("y").toDouble()};
    qreal width {e.attribute("width").toDouble()};
    qreal height {e.attribute("height").toDouble()};

    // parse attributes: for 'vector-effect'
    pen.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    path.addRect(x, y, width, height);
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);
    item->setTransform(transform);

    return item;
}

GraphicsPathItem *SVGParser::parseEllipse(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;
    SVGTransform transform;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);
    transform.syncWithAttributes(inheritedAttributes);

    // 获取元素的属性。如果属性值无效或不存在该属性，则结果为0。
    qreal cx {e.attribute("cx").toDouble()};
    qreal cy {e.attribute("cy").toDouble()};
    qreal rx {e.attribute("rx").toDouble()};
    qreal ry {e.attribute("ry").toDouble()};

    // parse attributes: for 'vector-effect'
    pen.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    path.addEllipse(cx, cy, rx, ry);
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);
    item->setTransform(transform);

    return item;
}

GraphicsPathItem *SVGParser::parseCircle(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;
    SVGTransform transform;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);
    transform.syncWithAttributes(inheritedAttributes);

    // 获取元素的属性。如果属性值无效或不存在该属性，则结果为0。
    qreal cx {e.attribute("cx").toDouble()};
    qreal cy {e.attribute("cy").toDouble()};
    qreal r {e.attribute("r").toDouble()};

    // parse attributes: for 'vector-effect'
    pen.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    path.addEllipse(cx, cy, r, r);
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);
    item->setTransform(transform);

    return item;
}

GraphicsPathItem *SVGParser::parsePolyline(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;
    SVGTransform transform;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);
    transform.syncWithAttributes(inheritedAttributes);

    // 获取并解析元素的属性。如果不存在该属性，则结果为空字符串。
    // reference: https://www.w3.org/TR/SVGTiny12/shapes.html#PolylineElement
    QString points {e.attribute("points")};
    QStringView pointsView {points};
    if (!pointsView.isEmpty()) {
        QList<QStringView> stringList {pointsView.split(QRegularExpression {R"([\s,]+)"}, Qt::SkipEmptyParts)};

        assert(stringList.size() % 2 == 0);

        double x0 {stringList[0].toDouble()};
        double y0 {stringList[1].toDouble()};
        path.moveTo(x0, y0);

        for (qsizetype i {2}; i < stringList.size(); i += 2) {
            double x {stringList[i].toDouble()};
            double y {stringList[i + 1].toDouble()};
            path.lineTo(x, y);
        }
    }

    // parse attributes: for 'vector-effect'
    pen.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);
    item->setTransform(transform);

    return item;
}

GraphicsPathItem *SVGParser::parsePath(const QDomElement &e, const QDomNamedNodeMap &inheritedAttributes) const
{
    GraphicsPathItem *item {};
    SVGPen pen;
    SVGBrush brush;
    SVGPainterPath path;
    SVGTransform transform;

    // parse inherited attributes
    pen.syncWithAttributes(inheritedAttributes);
    brush.syncWithAttributes(inheritedAttributes);
    path.syncWithAttributes(inheritedAttributes);
    transform.syncWithAttributes(inheritedAttributes);

    // 获取并解析元素的属性。如果不存在该属性，则结果为空字符串。
    // reference: https://www.w3.org/TR/SVGTiny12/paths.html
    QString d {e.attribute("d")};
    QStringView dView {d};
    if (!dView.isEmpty()) {
        QList<QStringView> stringList {dView.split(' ', Qt::SkipEmptyParts)};

        auto it {stringList.begin()};
        while (it != stringList.end()) {
            if (it->startsWith('M')) { // 例如"M10,15"
                it->slice(1);
                qsizetype commaIndex {it->indexOf(',')};
                double x {it->sliced(0, commaIndex).toDouble()};
                double y {it->sliced(commaIndex + 1).toDouble()};
                path.moveTo(x, y);
                ++it;
            } else if (it->startsWith('L')) { // 例如"L38,15"
                it->slice(1);
                qsizetype commaIndex {it->indexOf(',')};
                double x {it->sliced(0, commaIndex).toDouble()};
                double y {it->sliced(commaIndex + 1).toDouble()};
                path.lineTo(x, y);
                ++it;
            } else if (it->startsWith('C')) { // 例如"C15,23"
                QPointF ctrlPt1;
                QPointF ctrlPt2;
                QPointF endPt;

                qsizetype commaIndex;
                double x, y;

                it->slice(1);
                commaIndex = it->indexOf(',');
                x = it->sliced(0, commaIndex).toDouble();
                y = it->sliced(commaIndex + 1).toDouble();
                ctrlPt1 = QPointF {x, y};
                ++it;

                // 此时*it应形如"12,34"(即不带前缀字母)
                commaIndex = it->indexOf(',');
                x = it->sliced(0, commaIndex).toDouble();
                y = it->sliced(commaIndex + 1).toDouble();
                ctrlPt2 = QPointF {x, y};
                ++it;

                // 此时*it应形如"12,34"(即不带前缀字母)
                commaIndex = it->indexOf(',');
                x = it->sliced(0, commaIndex).toDouble();
                y = it->sliced(commaIndex + 1).toDouble();
                endPt = QPointF {x, y};
                ++it;

                path.cubicTo(ctrlPt1, ctrlPt2, endPt);
            }
        }
    }

    // parse attributes: for 'vector-effect' and 'fill-rule'
    pen.syncWithAttributes(e.attributes());
    path.syncWithAttributes(e.attributes());

    // apply all the parsed attributes to item
    item = new GraphicsPathItem {path};
    item->setPen(pen);
    item->setBrush(brush);
    item->setTransform(transform);

    return item;
}

QDomNamedNodeMap SVGParser::parseG(const QDomElement &e, QDomNamedNodeMap inheritedAttributes) const
{
    QDomNamedNodeMap localAttributes {e.attributes()};
    for (int i {0}; i < localAttributes.count(); ++i) {
        inheritedAttributes.setNamedItem(localAttributes.item(i));
    }
    return inheritedAttributes;
}

QLinearGradient SVGParser::parseLinearGradient(const QDomElement &e) const
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#LinearGradientElement
    QLinearGradient linearGradient;

    // 获取属性
    QString gradientUnits {e.attribute("gradientUnits")};
    qreal x1 {e.attribute("x1").toDouble()};
    qreal y1 {e.attribute("y1").toDouble()};
    qreal x2 {e.attribute("x2").toDouble()};
    qreal y2 {e.attribute("y2").toDouble()};

    // 解析属性
    if (gradientUnits == "objectBoundingBox")
        linearGradient.setCoordinateMode(QGradient::ObjectMode);
    else {
        assert(gradientUnits == "userSpaceOnUse");
        linearGradient.setCoordinateMode(QGradient::LogicalMode);
    }

    linearGradient.setStart(x1, y1);
    linearGradient.setFinalStop(x2, y2);

    // 获取并解析子结点<stop>的属性
    QDomNodeList childNodes {e.childNodes()};
    for (auto childNode: childNodes) {
        auto childElement {childNode.toElement()};

        qreal offset {childElement.attribute("offset").toDouble()};
        QColor stopColor {childElement.attribute("stop-color")};
        float stopOpacity {childElement.attribute("stop-opacity").toFloat()};

        stopColor.setAlphaF(stopOpacity);
        linearGradient.setColorAt(offset, stopColor);
    }

    return linearGradient;
}

QRadialGradient SVGParser::parseRadialGradient(const QDomElement &e) const
{
    // reference: https://www.w3.org/TR/SVGTiny12/painting.html#RadialGradientElement
    QRadialGradient radialGradient;

    // 获取属性
    QString gradientUnits {e.attribute("gradientUnits")};
    qreal cx {e.attribute("cx").toDouble()};
    qreal cy {e.attribute("cy").toDouble()};
    qreal r {e.attribute("r").toDouble()};
    qreal fx {e.attribute("fx").toDouble()};
    qreal fy {e.attribute("fy").toDouble()};

    // 解析属性
    if (gradientUnits == "objectBoundingBox")
        radialGradient.setCoordinateMode(QGradient::ObjectMode);
    else {
        assert(gradientUnits == "userSpaceOnUse");
        radialGradient.setCoordinateMode(QGradient::LogicalMode);
    }

    radialGradient.setCenter(cx, cy);
    radialGradient.setRadius(r);
    radialGradient.setFocalPoint(fx, fy);

    // 获取并解析子结点<stop>的属性
    QDomNodeList childNodes {e.childNodes()};
    for (auto childNode: childNodes) {
        auto childElement {childNode.toElement()};

        qreal offset {childElement.attribute("offset").toDouble()};
        QColor stopColor {childElement.attribute("stop-color")};
        float stopOpacity {childElement.attribute("stop-opacity").toFloat()};

        stopColor.setAlphaF(stopOpacity);
        radialGradient.setColorAt(offset, stopColor);
    }

    return radialGradient;
}

SVGParser::SVGParser()
{
    m_renderer.setOptions(QtSvg::Tiny12FeaturesOnly); // 仅解析SVG 1.2 Tiny规范的标签，不属于该规范的标签一律不解析
    // 这样做是为了避免Qt将复杂的标签强行解析为<image>标签，这样的图元缩放会失真
}

std::vector<QGraphicsPathItem *> SVGParser::parse() const
{
    std::vector<QGraphicsPathItem *> items;
    QDomElement SVGNode {this->SVGNode()};

    // 开始解析图形元素
    QDomElement outerGNode {SVGNode.firstChildElement("g")};
    QDomNamedNodeMap outerAttributes {outerGNode.attributes()};

    QDomNodeList innerGNodes {outerGNode.childNodes()};

    for (const QDomNode &innerGNode: innerGNodes) {
        QDomElement itemNode {innerGNode.firstChild().toElement()};
        if (itemNode.isNull()) continue;

        QDomNamedNodeMap attributes {parseG(innerGNode.toElement(), outerAttributes)};

        QString itemType {itemNode.tagName()};
        if (itemType == "rect")
            items.push_back(parseRect(itemNode, attributes));
        else if (itemType == "ellipse")
            items.push_back(parseEllipse(itemNode, attributes));
        else if (itemType == "circle")
            items.push_back(parseCircle(itemNode, attributes));
        else if (itemType == "polyline")
            items.push_back(parsePolyline(itemNode, attributes));
        else if (itemType == "path")
            items.push_back(parsePath(itemNode, attributes));
    }

    return items;
}
