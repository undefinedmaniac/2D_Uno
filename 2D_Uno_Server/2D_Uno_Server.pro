TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lws2_32

SOURCES += \
        main.cpp \
    game/card.cpp \
    game/deck.cpp \
    game/privateplayer.cpp \
    game/game.cpp \
    game/player.cpp \
    game/game_turn_manager.cpp \
    game/discard_pile.cpp \
    game/game_call_manager.cpp \
    network/liststreamsocket.cpp

HEADERS += \
    game/card.h \
    game/enums/card_type.h \
    game/enums/card_color.h \
    game/deck.h \
    game/privateplayer.h \
    game/game.h \
    game/interfaces/igameobserver.h \
    game/player.h \
    game/discard_pile.h \
    game/game_turn_manager.h \
    observer.h \
    utility/make_unique.h \
    game/game_call_manager.h \
    network/liststreamsocket.h

win32: LIBS += -L$$PWD/../../../libraries/boost_1_68_0/lib/ -llibboost_system-mgw51-mt-d-x64-1_68.dll

INCLUDEPATH += $$PWD/../../../libraries/boost_1_68_0/include
DEPENDPATH += $$PWD/../../../libraries/boost_1_68_0/include
