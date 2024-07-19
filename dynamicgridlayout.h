#pragma once

#include <QWidget>
#include <QLayout>

/*
 * DynamicGridLayout - layout для отображения виджетов в виде "правильной" сетки
 * Под правильной сеткой подразумевается сетка вида 1x1 ; 2x2 ; 3x3 ; 4x4 ; 5x5 ; 6x6 и т.д.
 *
 * Для использования DynamicGridLayout необходимо создать экземпляр этого класса
 * ( new DynamicGridLayout )
 *
 * Следующим шагом будет заполнение его виджетами - с помощью фукнции addWidget
 *
 * Можно избавиться от фукнции addWidget, и заменить её на фукцнию, которая сразу устанавливает ссылку на объект,
 * который хранит коллекцию виджетов. Этот объект должен заменить widgetList
 *
 * Предполагается, что добавляемые виджеты имеют ненулевой margin. В противном случае на DynamicGridLayout
 * они будут располагаться без отступа
 *
 * После создания необходимо вызвать функцию init, с её помощью будет создан первый элемент типа QLayoutItem
 * После этого автоматически будет вызвана функция setGeometry
 *
 * Для дальнейшей работы требуется использовать слот makeLayout
 *
 * Параметром makeLayout является index - число элементов, которое будет отображено на DynamicGridLayout,
 * уменьшенное на единицу
 *
 * Удачный пример использования QComboBox, где элементы - последовательность натуральных числе без 0,
 * а индексы, соответвенно последовательность натуральных чисел с 0
 */

class DynamicGridLayout final : public QLayout
{
    Q_OBJECT

public:
    ~DynamicGridLayout();

    // можно заменить на метод, для установления
    // ссылки на коллекию с виджетами
    void addWidget(QWidget *w);

    // init может быть private
    // и вызываться в конце функции
    // в которой устанавливается ссылка на коллекцию с виджетами
    void init();
    void addItem(QLayoutItem *item)       override;
    int  count()                    const override;
    QLayoutItem *itemAt(int index)  const override;
    QLayoutItem *takeAt(int index)        override;

    void  setGeometry(const QRect &rect)  override;
    QSize sizeHint()                const override;
    QSize minimumSize()             const override;

public slots:
    void makeLayout(int index);

private:
    QList<QLayoutItem *> itemList;

    // можно заменить на ссылку на коллекцию
    // с виджетами
    QList<QWidget *>     widgetList;

    int currentNumber = 1;
    int oldNumber     = 1;

    int widthCf = 1, heightCf = 1;
};
