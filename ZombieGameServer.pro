QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = ZombieGameServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

MOC_DIR     = release/build/moc
OBJECTS_DIR = release/build/obj
RCC_DIR     = release/

SOURCES += main.cpp \
    packets/packet.cpp \
    game/level.cpp \
    game/gamemp.cpp \
    game/tile.cpp \
    game/entities/mobs/scientist.cpp \
    game/entities/mobs/zombie.cpp \
    game/entities/bullet.cpp \
    game/entities/mobs/mob.cpp \
    game/entities/entity.cpp \
    game/entities/mobs/gun.cpp \
    core/client.cpp \
    core/config.cpp \
    core/logger.cpp \
    core/server.cpp \
    utils/vector2d.cpp \
    game/entities/mobs/player.cpp \
    packets/packetentitydata.cpp \
    packets/packetleveldata.cpp \
    packets/packetplayerpos.cpp \
    packets/packetzombiechangetarget.cpp \
    packets/packetscientistchangedir.cpp \
    packets/packetfirebullet.cpp \
    packets/packetzombiehit.cpp \
    packets/packetbullethit.cpp \
    packets/packetplayerchat.cpp \
    packets/packetinfo.cpp \
    command/commandmanager.cpp \
    command/command.cpp \
    command/commandsend.cpp \
    command/commandkick.cpp \
    command/commandplayers.cpp \
    command/commandstop.cpp \
    packets/packetdisconnect.cpp \
    packets/packeterror.cpp \
    packets/packetinfected.cpp \
    packets/packetjoin.cpp \
    packets/packetkick.cpp \
    packets/packetping.cpp \
    packets/packetshutdown.cpp \
    packets/packetbasic.cpp \
    command/commandreset.cpp \
    packets/packetentityupdate.cpp

HEADERS += \
    packets/packet.h \
    game/level.h \
    game/gamemp.h \
    game/tile.h \
    game/entities/mobs/scientist.h \
    game/entities/mobs/zombie.h \
    game/entities/bullet.h \
    game/entities/mobs/mob.h \
    game/entities/entity.h \
    game/entities/mobs/gun.h \
    core/client.h \
    core/config.h \
    core/logger.h \
    core/server.h \
    utils/vector2d.h \
    game/entities/mobs/player.h \
    packets/packetentitydata.h \
    packets/packetleveldata.h \
    packets/packetplayerpos.h \
    packets/packetzombiechangetarget.h \
    packets/packetscientistchangedir.h \
    packets/packetfirebullet.h \
    packets/packetzombiehit.h \
    packets/packetbullethit.h \
    packets/packetplayerchat.h \
    packets/packetinfo.h \
    command/commandmanager.h \
    command/command.h \
    command/commandsend.h \
    command/commandkick.h \
    command/commandplayers.h \
    command/commandstop.h \
    packets/packetdisconnect.h \
    packets/packeterror.h \
    packets/packetinfected.h \
    packets/packetjoin.h \
    packets/packetkick.h \
    packets/packetping.h \
    packets/packetshutdown.h \
    packets/packetbasic.h \
    command/commandreset.h \
    packets/packetentityupdate.h

RC_FILE = ZombieGameServer.rc
