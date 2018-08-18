TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    game/card.cpp \
    game/deck.cpp \
    game/discardpile.cpp \
    game/privateplayer.cpp \
    game/game.cpp \
    game/player.cpp

HEADERS += \
    game/card.h \
    game/enums/card_type.h \
    game/enums/card_color.h \
    game/deck.h \
    game/privateplayer.h \
    game/game.h \
    game/interfaces/igameobserver.h \
    game/player.h \
    game/discard_pile.h
