CONFIG += qt debug

QT += widgets

QMAKE_CXXFLAGS += -Wno-unused-parameter


HEADERS =              src/MainWindow.h   src/MenuScene.h   src/MouseEventPixmapItem.h   src/GameScene.h       \
                       src/GameLevelData.h   src/Plant.h   src/Zombie.h   src/Timeout.h   src/ImageManager.h   \
                       src/PlantCardItem.h   src/Coordinate.h
SOURCES = src/main.cpp src/MainWindow.cpp src/MenuScene.cpp src/MouseEventPixmapItem.cpp src/GameScene.cpp     \
                       src/GameLevelData.cpp src/Plant.cpp src/Zombie.cpp src/Timeout.cpp src/ImageManager.cpp \
                       src/PlantCardItem.cpp src/Coordinate.cpp

TARGET = main

OBJECTS_DIR = out/obj
MOC_DIR = out/moc