#include "symbol.h"

#include <qvector.h>

Symbol::Symbol(QString value, std::vector<int> position, int siteId, int counter): value(value), siteId(siteId), counter(counter)
{
    this->position = std::move(position);
}
Symbol::Symbol(){

}
QString Symbol::getValue() const{
    return value;
}
void Symbol::setValue(QString value){
    this->value = value;
}

int Symbol::getSiteId() const
{
    return siteId;
}

void Symbol::setSiteId(int value)
{
    siteId = value;
}

QChar Symbol::getAlign()
{
    return align;
}

void Symbol::setAlign(QChar a)
{
    align=a;
}

int Symbol::getCounter() const
{
    return counter;
}

void Symbol::setCounter(int value)
{
    counter = value;
}

std::vector<int> Symbol::getPosizione() const
{
    return position;
}

void Symbol::setPosizione(const std::vector<int> &value)
{
    position = value;
}

QString Symbol::getFamily()
{
    return family;
}

bool Symbol::getBold()
{
    return bold;
}

bool Symbol::getItalic()
{
    return italic;
}

bool Symbol::getUnderln()
{
    return underln;
}

qreal Symbol::getSize()
{
    return size;
}

void Symbol::setFamily(QString f)
{
    family=f;
}

void Symbol::setBold(bool b)
{
    bold=b;
}

void Symbol::setItalic(bool i)
{
    italic=i;
}

void Symbol::setUnderln(bool u)
{
    underln=u;
}

void Symbol::setSize(qreal s)
{
    size=s;
}

Symbol Symbol::fromJson(const QJsonDocument &charJD){
    QJsonObject charJ = charJD.object();
    auto value = charJ["value"].toString();
    int siteId = charJ["siteId"].toInt();
    QJsonArray fractionalPosJ = charJ["position"].toArray();
    int counter = charJ["counter"].toInt();

    std::vector<int> fractionalPos;
    for(auto i: fractionalPosJ){
        fractionalPos.push_back(i.toInt());
    }

    QString family = charJ["family"].toString();
    bool bold = charJ["bold"].toBool();
    bool italic = charJ["italic"].toBool();
    bool underln = charJ["underln"].toBool();
    qreal size = charJ["size"].toDouble();
    QChar align = charJ["align"].toInt();

    Symbol result(value, fractionalPos, siteId, counter);
    result.setFamily(family);
    result.setBold(bold);
    result.setItalic(italic);
    result.setUnderln(underln);
    result.setSize(size);
    result.setAlign(align);
    return result;
}

QJsonDocument Symbol::toJson()
{
    QJsonObject json_obj;
    json_obj["value"] = this->value;
    json_obj["siteId"] = this->siteId;
    json_obj["counter"] = this->counter;
    json_obj["family"] = this->family;
    json_obj["bold"] = this->bold;
    json_obj["italic"] = this->italic;
    json_obj["underln"] = this->underln;
    json_obj["size"] = this->size;
    json_obj["align"] = this->align.toLatin1();

    QJsonArray positionj;
    for(int pos : this->position) {
        positionj.append(pos);
    }
    json_obj["position"] = positionj;
    return QJsonDocument(json_obj);





}
