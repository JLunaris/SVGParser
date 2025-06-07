#include "GraphicsPathItem.h"

#include "SVGBrush.h"
#include "SVGPen.h"

GraphicsPathItem::GraphicsPathItem(QGraphicsItem *parent)
        : QGraphicsPathItem(parent)
{
}

GraphicsPathItem::GraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent)
        : QGraphicsPathItem(path, parent)
{
}
