TEMPLATE = app

TARGET	= cybr
CONFIG	+= c++14

DESTDIR	= ../../build
OBJECTS_DIR = obj

LIBS += -llua -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-window

SOURCES += \
    main.cpp \
    Core.cpp \
    Input.cpp \
    Log.cpp \
    Math.cpp \
    Scene.cpp \
    AssetManager.cpp \
    ScriptManager.cpp \
    SoundManager.cpp \
    FontAsset.cpp \
    MusicAsset.cpp \
    ScriptAsset.cpp \
    ShaderAsset.cpp \
    TextureAsset.cpp \
    GUI.cpp \
    Widget.cpp \
    Label.cpp \
    Layout.cpp \
    VerticalLayout.cpp \
    HorizontalLayout.cpp \
    Intro.cpp \
    Game.cpp \
    MainMenu.cpp \

HEADERS	+= \
    Core.h \
    State.h \
    Input.h \
    Log.h \
    Math.h \
    Asset.h \
    Scene.h \
    AssetManager.h \
    ScriptManager.h \
    SoundManager.h \
    FontAsset.h \
    MusicAsset.h \
    ScriptAsset.h \
    ShaderAsset.h \
    TextureAsset.h \
    ConfigManager.h \
    GUI.h \
    Widget.h \
    Label.h \
    Layout.h \
    VerticalLayout.h \
    HorizontalLayout.h \
    Intro.h \
    Game.h \
    MainMenu.h \

