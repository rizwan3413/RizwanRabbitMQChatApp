#include "init.h"

Init :: Init(std::string address):loop(EV_DEFAULT),
                handler(loop),connection(&handler, AMQP::Address(address)),
                channel(&connection){}

Init::~Init(){}

AMQP::TcpConnection& Init::getConnection()
{
    return connection;
}

AMQP::TcpChannel& Init::getChannel()
{
    return channel;
}

void Init::startloop()
{
    // run the loop
    ev_run(loop, 0);
}
