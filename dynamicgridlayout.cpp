#include "dynamicgridlayout.h"

DynamicGridLayout::~DynamicGridLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void DynamicGridLayout::addWidget(QWidget *w)
{
    widgetList.push_back(w);
}

void DynamicGridLayout::init()
{
    QLayout::addWidget(widgetList.at(0));
}

void DynamicGridLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int DynamicGridLayout::count() const
{
    return itemList.size();
}

QLayoutItem* DynamicGridLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem* DynamicGridLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return nullptr;
}

void DynamicGridLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);

    int xStep = effectiveRect.width()  / widthCf ;
    int yStep = effectiveRect.height() / heightCf;

    QPoint topLeft = effectiveRect.topLeft();

    auto column = 1;
    auto row    = 1;

    std::for_each_n(itemList.begin(), currentNumber, [&, this](QLayoutItem * item)
    {
        item->setGeometry(QRect(topLeft, QSize(xStep, yStep)));

        if (column == widthCf)
        {
            column = 1;
            if (row != heightCf) row += 1;

        } else column += 1;

        topLeft.setX( effectiveRect.topLeft().x() + xStep * (column - 1) );
        topLeft.setY( effectiveRect.topLeft().y() + yStep * (row    - 1) );
    });
}

QSize DynamicGridLayout::sizeHint() const
{
    return minimumSize();
}

QSize DynamicGridLayout::minimumSize() const
{
    QSize size;
    for (QLayoutItem * item : itemList)
        size = size.expandedTo(item->minimumSize());

    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    size += QSize(left + right, top + bottom);
    return size;
}

void DynamicGridLayout::makeLayout(int index)
{
    oldNumber     = currentNumber;
    currentNumber = index + 1;

    auto sqrt       = std::sqrt(currentNumber);
    auto floor_sqrt = std::floor(sqrt);

    widthCf  = floor_sqrt == sqrt ? sqrt : floor_sqrt + 1;
    heightCf = std::round(sqrt);

    if (currentNumber > oldNumber) std::for_each_n(widgetList.begin() + oldNumber, currentNumber - oldNumber, [this](auto item)
                                   {
                                       QLayout::addWidget(item);
                                   });

    if (currentNumber < oldNumber) std::for_each_n(itemList.begin() + currentNumber, oldNumber - currentNumber, [this](auto item)
                                   {
                                       QLayout::removeItem(item);
                                       item->widget()->setParent(nullptr);
                                   });
}
