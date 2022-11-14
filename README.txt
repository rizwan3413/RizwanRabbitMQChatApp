ChatApp Using RabbitMQ 
=======================

- ChatApp is an application written with the use of RabbitMQ which allows two users who are located at different location/hosts to connect and communicate with each other 

Use case & Procedure to Test the Chat app
=========================================
- Clone the git repo on Ubuntu14 0r 16 or 18
- Install cmake
- create build folder inside riz_example folder using mkdir build
- Execute command cmake .. and after that make
- They will be two binaries user1 and user2 generated 
- Now start user2 in a different terminal and than start user1 in same terminal
- Type a message like How are you from terminal 2 , you shall see a message in terminal 1
- If you wish to discontinue messaging any time please enter no 
- Correcponding Queues will be deleted 

