#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>

class Command
{
protected:
    bool valid;
    QStringList commandList;
    QString structure;
    QString description;
    QString errorMsg;
public:
    Command(QString command);

    virtual bool validate() = 0;
    virtual bool execute() = 0;

    bool isValid() { return valid; }
    QString getStructure() { return structure; }
    QString getDescription() { return description; }
    QString getErrorMsg() { return errorMsg; }
};

#endif // COMMAND_H
