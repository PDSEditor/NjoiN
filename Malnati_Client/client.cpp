#include "client.h"

Client::Client()
{
    sockm=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
    crdt= new Crdt();


}
