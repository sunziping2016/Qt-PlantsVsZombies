//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_IMAGEMANAGER_H
#define PLANTS_VS_ZOMBIES_IMAGEMANAGER_H


#include <QtGui>

class ImageManager
{
public:
    QPixmap loadPixmap(const QString &path);

private:
    QMap<QString, QPixmap> pixmaps;
};

extern ImageManager *gImageManager;

void InitImageManager();
void DestoryImageManager();

#endif //PLANTS_VS_ZOMBIES_IMAGEMANAGER_H
