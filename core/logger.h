#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QThread>

#include "command/commandmanager.h"

enum class MessageType
{
    INFO,
    WARNING,
    ERROR
};

class Logger : public QObject
{
    Q_OBJECT

    QThread thread; // The input thread
    bool hasInputTextHeader; // Is for indicating if we have the "SERVER >" prompt for input writen
    CommandManager commandManager;
private slots:
    void readCommands();
public:
    explicit Logger(QObject* parent = nullptr);

    void run(); // Moves this process to the thread and starts that tread
    void stop(); // Trys to stop the thread if that thread is running
    void output(QString str, MessageType type = MessageType::INFO, bool header = true); // Used for inserting text to the console and reseting the input prompt
};

#endif // LOGGER_H
