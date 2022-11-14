#include "init.h"
#include "constants.h"

int main()
{
    Init init(chatapp::amqpAddress);

    auto &connection = init.getConnection();

    auto &channel = init.getChannel();

    channel.declareExchange(chatapp::exchange1);

    // create a temporary queue
    channel.declareQueue(chatapp::user1Inbox).onSuccess([&connection,&channel](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
        
        // report the name of the temporary queue
        std::cout << "declared queue " << name << std::endl;

        channel.publish("",chatapp::user2Inbox,"Hi There");
    });

    channel.bindQueue(chatapp::exchange1,chatapp::user1Inbox,chatapp::routingKey);

    // callback function that is called when the consume operation starts
    auto startCb = [](const std::string &consumertag) 
    {
        std::cout << "consume operation started" << std::endl;
    };

    // callback function that is called when the consume operation failed
    auto errorCb = [](const char *message) 
    {
        std::cout << "consume operation failed" << std::endl;
    };

    // callback operation when a message was received
    auto messageCb = [&channel,&connection](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) 
    {
        std::string msg(message.body(),message.bodySize());

        std::cout << "message received: " << msg << std::endl;
        // acknowledge the message
        channel.ack(deliveryTag);

        if (msg.compare("exit")!= 0)
        {
            std::string reply;

            std::cout << "Do you want to continue (Yes/No)" << std::endl;
        
            std::getline(std::cin,reply);

            if (reply.compare("Yes")== 0)
            {
                std::cout << "Reply:\n";

                std::getline(std::cin,reply);

                channel.publish("",chatapp::user2Inbox,reply);
            }
            else
            {   
                channel.publish("",chatapp::user2Inbox,"exit");

                channel.removeQueue(chatapp::user1Inbox).onSuccess([](uint32_t messageCount)
                {
                    std::cout << "Queue deleted, along with " << messageCount << " messages" << std::endl;
                });
                
                connection.close();
            }
        }
        else
        {
            channel.removeQueue(chatapp::user1Inbox);
            connection.close();
        }
    };

    // start consuming from the queue, and install the callbacks
    channel.consume(chatapp::user1Inbox)
        .onReceived(messageCb)
        .onSuccess(startCb)
        .onError(errorCb);

    init.startloop();

    return 0;
}