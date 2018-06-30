#ifndef PACKETERROR_H
#define PACKETERROR_H

#include "packet.h"

class PacketError : public Packet
{
public:
    PacketError(QByteArray data);
    PacketError(QString errorMsg);
    PacketError();

    void writeError(QString error);
    bool validate();

    QString getError();
};

#endif // PACKETERROR_H
