#ifndef IMAGEPARAMS_H
#define IMAGEPARAMS_H

#include <QMap>
#include <QColor>
#include <QDataStream>

using namespace std;

// Параметры визуализации моделей
class ImageParams
{
public:
    bool isMesh;       // Отображение сетки
    bool isVertex;     // ... узлов
    bool isFace;       // ... поверхности
    bool isCoord;      // ... осей координат
    bool isLight;      // ... освещения
    bool isQuality;    // Качественная прорисовка линий сетки на поверхности объекта (иногда сильно тормозит отображение)
    float alpha;       // Прозрачность материала
    QColor bkgColor;   // Цвет фона изображения
    ImageParams(void)
    {
        init();
    }
    ~ImageParams(void) {}
    ImageParams& operator = (const ImageParams& right)
    {
        alpha = right.alpha;
        isCoord = right.isCoord;
        isLight = right.isLight;
        isQuality = right.isQuality;
        isMesh = right.isMesh;
        isFace = right.isFace;
        isVertex = right.isVertex;
        bkgColor = right.bkgColor;
        return *this;
    }
    void write(QDataStream& file)
    {
        file << alpha;
        file << isCoord;
        file << isLight;
        file << isQuality;
        file << isMesh;
        file << isFace;
        file << isVertex;
        file << bkgColor;
    }
    void read(QDataStream& file)
    {
        file >> alpha;
        file >> isCoord;
        file >> isLight;
        file >> isQuality;
        file >> isMesh;
        file >> isFace;
        file >> isVertex;
        file >> bkgColor;
    }
    void init(void)
    {
        alpha = 1.0;
        isCoord = isLight = isQuality = isFace = true;
        isMesh = isVertex =false;
        bkgColor = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0).dark();
    }
};

#endif // IMAGEPARAMS_H
