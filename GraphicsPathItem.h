#pragma once

#include <QDomNamedNodeMap>
#include <QGraphicsPathItem>

class GraphicsPathItem : public QGraphicsPathItem
{
public:
    explicit GraphicsPathItem(QGraphicsItem *parent = nullptr);
    explicit GraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent = nullptr);
};
