#include "SVGParser.h"

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
    m_renderer.render(&painter);
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
