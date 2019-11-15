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

