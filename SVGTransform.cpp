#include "SVGTransform.h"

void SVGTransform::parseTransform(QStringView transform)
{
    if (transform.isEmpty()) return;

    auto start {transform.indexOf('(')};
    auto end {transform.indexOf(')')};
    transform.slice(start + 1, end - start - 1);

    qreal m11, m12, m21, m22, dx, dy;

    QList<QStringView> list {transform.split(',')};
    assert(list.count() == 6);
    m11 = list[0].toDouble();
    m12 = list[1].toDouble();
    m21 = list[2].toDouble();
    m22 = list[3].toDouble();
    dx = list[4].toDouble();
    dy = list[5].toDouble();

    setMatrix(m11, m12, 0, m21, m22, 0, dx, dy, 1);
}

void SVGTransform::syncWithAttributes(const QDomNamedNodeMap &attributes)
{
    // 获取需要的属性值。如果不存在该属性，则结果为空字符串。
    QString transform {attributes.namedItem("transform").nodeValue()};

    // 解析属性
    parseTransform(transform);
}
