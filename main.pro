QT += widgets multimedia

QMAKE_CXXFLAGS += -Wno-unused-parameter


HEADERS +=              src/MainView.h   src/SelectorScene.h   src/MouseEventPixmapItem.h   src/GameScene.h   \
                        src/GameLevelData.h   src/Plant.h   src/Zombie.h   src/Timer.h   src/ImageManager.h   \
                        src/PlantCardItem.h   src/Coordinate.h   src/AspectRatioLayout.h   src/Animate.h
SOURCES += src/main.cpp src/MainView.cpp src/SelectorScene.cpp src/MouseEventPixmapItem.cpp src/GameScene.cpp \
                        src/GameLevelData.cpp src/Plant.cpp src/Zombie.cpp src/Timer.cpp src/ImageManager.cpp \
                        src/PlantCardItem.cpp src/Coordinate.cpp src/AspectRatioLayout.cpp src/Animate.cpp
RESOURCES += main.qrc

TRANSLATIONS = translations/main.zh_CN.ts

TARGET = main

OBJECTS_DIR = out/obj
MOC_DIR = out/moc