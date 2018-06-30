#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <QString>

#define CONFIG_FILE_NAME "config.txt"

class Config
{
    bool isCreated;
    std::fstream file;
public:
    Config();

    void write(QString key, QString value); // Create or overwrite the value with the given key
    QString read(QString key, bool* found = nullptr); // Return value which maps to the key if we find it
    bool isNew() { return isCreated; }
};

#endif // CONFIG_H
