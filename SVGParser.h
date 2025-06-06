#pragma once

#include <QDomDocument>
#include <QObject>
#include <QSvgGenerator>
#include <QSvgRenderer>

class SVGParser : public QObject
{
    Q_OBJECT

    QDomDocument m_doc;
    QSvgRenderer m_renderer;

public Q_SLOTS:
    bool loadSVG(const QString &fileName);

public:
    SVGParser() = default;

    // 获取Qt化后的svg文件的DOM树
    QDomDocument domDocument() const { return m_doc; }

    QRectF viewBoxF() const { return m_renderer.viewBoxF(); }

    QSize size() const { return m_renderer.defaultSize(); }
};
