#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <conio.h>
#include <map>

sf::UdpSocket socket;
sf::IpAddress Serverip;
std::string username;
char connectiontype;
char buffer[20];
std::size_t received;
std::map<unsigned short,sf::IpAddress>ComputerId;
std::string text="Connected to : ";

std::string sendmsg;
std::string prevsend;
char receivedmsg[2000];

bool message=false;
bool serverfinish=false;

sf::Mutex globalmutex;

void Server()
{
    sf::IpAddress ip=sf::IpAddress::getLocalAddress();
    std::cout<<"The server ip : "<<ip<<std::endl;
    char answer='b';
    do
    {
        sf::IpAddress rIp;
        unsigned short port;
        socket.receive(buffer,sizeof(buffer),received,rIp,port);
        if(received>0)
        {
            ComputerId[port]=rIp;
            std::cout<<rIp<<"----"<<port<<std::endl;
        }
        std::cin>>answer;
    }while(answer!='a');

    std::cout<<"Server created"<<std::endl;

    sf::Packet packet;
    serverfinish=true;
    packet<<serverfinish;
    std::map<unsigned short,sf::IpAddress>::iterator tempiterator;
    for(tempiterator=ComputerId.begin();tempiterator!=ComputerId.end();tempiterator++)
    {
        socket.send(packet,tempiterator->second,tempiterator->first);
    }
}

void Client()
{
    char sIp[20];
    std::string enteruser;
    std::cout<<"Welcome to the client service\nThis is a UDP chat server so if you want to chat follow the instructions below ;) "<<std::endl;
    std::cout<<"Please enter the server Ip : ";
    std::cin>>sIp;
    std::cout<<"Please enter an username(without space) : ";
    std::cin>>enteruser;
    username="("+enteruser+"): ";
    Serverip=sIp;
    socket.send(text.c_str(),text.length()+1,Serverip,2000);
    std::cout<<std::endl;
    sf::Packet packet;
    sf::IpAddress tempIp;
    unsigned short port;
    bool done;
    while(!message)
    {
        socket.receive(packet,tempIp,port);
        if(packet>>done)
        {
            message=done;
        }
    }
}


void DoServer()
{
    std::string msgresend;
    std::size_t receiveddata;
    std::map<unsigned short,sf::IpAddress>::iterator tempiterator;
    while(serverfinish)
    {
        char msg[2000];
        sf::IpAddress tempIp;
        unsigned short tempPort;
        socket.receive(msg,sizeof(msg),receiveddata,tempIp,tempPort);

        if(receiveddata>0)
        {
            globalmutex.lock();
            msgresend=msg;
            for(tempiterator=ComputerId.begin();tempiterator!=ComputerId.end();tempiterator++)
            {
                socket.send(msgresend.c_str(),msgresend.length()+1,tempiterator->second,tempiterator->first);
            }
            globalmutex.unlock();
        }
    }


}

void getData()
{
    sf::IpAddress tempIp;
    unsigned short tempPort;
    std::string Oldmsg;
    std::string Clearmsg="";
    std::size_t datareceived;
    while(message)
    {
        globalmutex.lock();
        socket.send(sendmsg.c_str(),sendmsg.length()+1,Serverip,2000);
        globalmutex.unlock();
        socket.receive(receivedmsg,sizeof(receivedmsg),datareceived,tempIp,tempPort);
        if(datareceived>0&&receivedmsg==sendmsg)
        {
            sendmsg="";
        }
        if(datareceived>0&&receivedmsg!=Oldmsg&&receivedmsg!=0&&receivedmsg!=Clearmsg&&receivedmsg!=prevsend)
        {
            std::cout<<receivedmsg<<std::endl;
            Oldmsg=receivedmsg;
        }
    }
}

void Input()
{
    while(message)
    {
        std::string msg;
        std::getline(std::cin,msg);
        globalmutex.lock();
        if(!msg.empty())
        {
            sendmsg=username+msg;
        }
        prevsend=sendmsg;
        globalmutex.unlock();
    }
}

int main()
{
    sf::Thread thread1(Input);
    sf::Thread thread2(getData);
    sf::Thread thread3(DoServer);

    std::cout<<"Enter (s) for server or (c) for client"<<std::endl;
    std::cin>>connectiontype;

    unsigned short port;
    std::cout<<"Set port number : ";
    std::cin>>port;

    socket.bind(port);

    if(connectiontype=='s')
    {
        Server();
    }
    else
    {
        Client();
    }


    if(connectiontype=='s')
    {
        socket.setBlocking(true);
        thread3.launch();
    }
    else
    {
        socket.setBlocking(false);
        thread2.launch();
        thread1.launch();
    }


    return 0;
}
