<<<<<<< HEAD
#include "databaseManager.h"
#include <iostream>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{

}

void DatabaseManager::insertInDB(Message* mes) {

}

void DatabaseManager::deleteFromDB(Message* mes)
{

}

void DatabaseManager::updateDB(Message* m)
{

}

void DatabaseManager::createFile(QString nome, int user)
{

}

void DatabaseManager::retrieveFile(QString nome)
{

}


=======
#include "databaseManager.h"
#include <iostream>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{

}

void DatabaseManager::insertInDB(Message* mes) {
    std::cout<< "inserito";
    std::cout<< mes->getAction()<< " e " << mes->getSimbolo()->getCounter();
    qDebug()<<"diocane";
}

void DatabaseManager::deleteFromDB(Message* mes)
{

}

void DatabaseManager::updateDB(Message* m)
{

}

void DatabaseManager::createFile(std::string nome, int user)
{

}

void DatabaseManager::retrieveFile(std::string nome, int user)
{

}

>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
