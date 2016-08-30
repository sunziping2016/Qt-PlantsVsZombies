//
// Created by sun on 8/26/16.
//

#include "ImageManager.h"

ImageManager *gImageManager;

QPixmap ImageManager::loadPixmap(const QString &path)
{
    if (pixmaps.find(path) == pixmaps.end())
        pixmaps.insert(path, QPixmap("images/" + path));
    return pixmaps[path];
}

void InitImageManager()
{
    gImageManager = new ImageManager;
}

void DestoryImageManager()
{
    delete gImageManager;
}