#include "init.h"
#include "constants.h"

int main()
{
    Init init(chatapp::amqpAddress);

    auto &connection = init.getConnection();

    auto &channel = init.getChannel();

    channel.declareExchange(chatapp::exchange2);

    // create a temporary queue
    channel.declareQueue(chatapp::user2Inbox);

    channel.bindQueue(chatapp::exchange2,chatapp::user2Inbox,chatapp::routingKey);
    
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

                channel.publish("",chatapp::user1Inbox,reply);
            }
            else
            {   
                channel.publish("",chatapp::user1Inbox,"exit");

                channel.removeQueue(chatapp::user2Inbox).onSuccess([](uint32_t messageCount)
                {
                    std::cout << "Queue deleted, along with " << messageCount << " messages" << std::endl;
                });

                connection.close();
            }
        }
        else
        {
            channel.removeQueue(chatapp::user2Inbox);
            connection.close();
        }
    };

    // start consuming from the queue, and install the callbacks
    channel.consume(chatapp::user2Inbox)
        .onReceived(messageCb)
        .onSuccess(startCb)
        .onError(errorCb);

    init.startloop();
    
    

    return 0;
}