#include <string>
#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>

class Init
{
    // access to the event loop
    struct ev_loop *loop;

    // handler for libev (so we don't have to implement AMQP::TcpHandler!)
    AMQP::LibEvHandler handler;

    // make a connection
    AMQP::TcpConnection connection;

    // we need a channel too
    AMQP::TcpChannel channel;

    public:

    Init(std::string);
    ~Init();

    AMQP::TcpConnection& getConnection();
    AMQP::TcpChannel& getChannel();

    void startloop();
    
};