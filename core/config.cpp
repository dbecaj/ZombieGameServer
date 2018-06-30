#include "config.h"

#include <QStringList>
#include <string>
#include <QCoreApplication>

Config::Config()
{
    isCreated = false;
    file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::in);
    if(!file.is_open())
    {
        file.close();
        file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::out);
        isCreated = true;
    }

    file.close();
}

void Config::write(QString key, QString value)
{
    bool found;
    QString tmp = read(key, &found);
    if(found) // if the key exists in the config then we just change it
    {
        file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::in);

        QList<std::string> configData;
        std::string line;
        while(std::getline(file, line))
        {
            QStringList keyAndValue = QString::fromStdString(line).split("=");
            if(keyAndValue[0] == key) // we found the value that needs to be changed
            {
                line = key.toStdString() + "=" + value.toStdString();
            }
            configData.push_back(line);
        }

        file.close();
        file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::out);

        for(std::string l : configData)
            file << l << "\n";

        file.close();

        return;
    }

    file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::app);

    file << key.toStdString() << "=" << value.toStdString() << std::endl;

    file.close();
}

QString Config::read(QString key, bool* found)
{
    file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::in);

    std::string line;
    while(std::getline(file, line))
    {
        QStringList keyAndValue = QString::fromStdString(line).split("=");
        if(keyAndValue[0] == key)
        {
            file.close();

            *found = true;
            return keyAndValue[1];
        }
    }

    file.close();

    *found = false;
    return "";
}
