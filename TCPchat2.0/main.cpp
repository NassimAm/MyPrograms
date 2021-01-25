#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Inputbox.h"
#include "Button.h"
#include "Msgbox.h"

using namespace std;


bool enteringtext=false;
bool menu = true;
bool returning = false;
bool username_entered=false;
bool servercreated=false;
bool serverconnecting = false;
bool clientconnecting = false;
bool send = false;
bool refreshing = false;
bool otherwriting;
std::string onlinestatus;
char otheronlinestatus;

bool initiatingserver = false;
bool initiatingclient = false;
bool creatingserver = false;
bool creatingclient = false;

bool windowfocused = true;

//TCP networking----------
sf::TcpSocket socket;
std::string username;
std::string getusername;
int msgid=0;

//UDP networking-----------
sf::UdpSocket udpsocket;
std::string receivedusername;
unsigned short receivedport;
std::size_t received;
std::size_t sent;

std::vector<sf::IpAddress> ComputerIP;
std::vector<unsigned short> ComputerPort;
std::vector<Button> ServersButtons;
bool ServerChosen = false;

sf::IpAddress OtherIP;
unsigned short OtherPort;

//Initiating Server----------------
unsigned short listningport;
sf::IpAddress localip;
std::string iproot;
int hostend;
//Initiating Client----------------
sf::IpAddress Serverip;
unsigned short Serverport;
unsigned int counter;
//Testing Status-----------
std::string receivedstatus;
std::string mystatus = "Connected";

void CreateServer()
{
    sf::IpAddress tempIP;
    unsigned short tempport;
    sf::Packet sendpacket;
    sendpacket<<username;

    sf::Packet receivepacket;
    if(receivedport==0)
    {
        udpsocket.receive(receivepacket,tempIP,tempport);
        receivepacket>>receivedport;
        for(unsigned int i=2;i<256;i++)
        {
            hostend = i;
            sf::IpAddress newip = iproot + std::to_string(hostend);
            udpsocket.send(sendpacket,newip,2001);
        }
    }
    else if(receivedport>0)
    {
        OtherIP = tempIP;
        OtherPort = tempport;
        creatingserver = true;
        serverconnecting = false;
    }
}
void Server()
{
    sf::TcpListener listener;
    listener.listen(receivedport);
    listener.accept(socket);
    std::cout<<"Connected to Client\n";
    //Send username--------
    sf::Packet sendpacket;
    sendpacket<<username;
    socket.send(sendpacket);
    //Receive username--------
    sf::Packet receivepacket;
    socket.receive(receivepacket);
    receivepacket>>getusername;
    servercreated = true;
}

void SearchForServers(Button &buttonexample,sf::RenderWindow &window,sf::Vector2i &mouseposition,sf::Text noservertext)
{
    sf::Packet receivepacket;
    udpsocket.receive(receivepacket,Serverip,Serverport);
    receivepacket>>receivedusername;
    if(!receivedusername.empty())
    {
        counter = 0;
        for(unsigned int i=0;i<ComputerIP.size();i++)
        {
            if(Serverip==ComputerIP[i])
            {
                counter = 0;
                for(unsigned int j=0;j<ComputerPort.size();j++)
                {
                    if(Serverport==ComputerPort[j])
                    {
                        counter = 0;
                        break;
                    }
                    else
                        counter++;

                }
                break;
            }
            else
                counter++;
        }
        if(counter==ComputerIP.size()||counter==ComputerPort.size())
        {
            ComputerIP.push_back(Serverip);
            ComputerPort.push_back(Serverport);
            buttonexample.setText(receivedusername+"   ------     1/2     ------     TCPchat server");
            ServersButtons.push_back(buttonexample);
        }
        receivedusername = "";
    }

    if(ServersButtons.empty())
    {
        window.draw(noservertext);
    }
    else
    {
        for(unsigned int i=0;i<ServersButtons.size();i++)
        {
            ServersButtons[i].setPosition(sf::Vector2f(120,95+40*i));
            ServersButtons[i].CheckMouse(window,mouseposition,ServerChosen,windowfocused);
            ServersButtons[i].Draw(window);
            if(ServersButtons[i].Clicked())
            {
                sf::Packet sendpacket;
                OtherIP = ComputerIP[i];
                OtherPort = ComputerPort[i];
                sendpacket<<OtherPort;
                udpsocket.send(sendpacket,OtherIP,OtherPort);
                creatingclient = true;
                clientconnecting = false;
            }
        }
    }
}
void Client()
{
    if(socket.connect(OtherIP,OtherPort)==sf::Socket::Done)
    {
        std::cout<<"Connected to Server\n";
        servercreated = true;
        //Send username--------
        sf::Packet sendpacket;
        sendpacket<<username;
        socket.send(sendpacket);
        //Receive username--------
        sf::Packet receivepacket;
        socket.receive(receivepacket);
        receivepacket>>getusername;
    }
}



int main()
{
    sf::RenderWindow Mainwindow(sf::VideoMode(800,600),"TCPchat");
    Mainwindow.setFramerateLimit(60);
    //Menu interface------------------------------000000000000000000000000000000000000000000
    //Background--------
    sf::Sprite menubackground;
    sf::Texture mbgtexture;
    if(!mbgtexture.loadFromFile("ressources/textures/menubackground.jpg"))
        std::cerr<<"ERROR: could not load the background texture\n";
    menubackground.setTexture(mbgtexture);
    menubackground.setPosition(-130,-210);
    //Credits----------
    sf::Text credits;
    sf::Font creditfont;
    if(!creditfont.loadFromFile("ressources/fonts/creditsfont.ttf"))
        std::cerr<<"ERROR: could not load the credits font\n";
    credits.setFont(creditfont);
    credits.setFillColor(sf::Color(0,0,0));
    credits.setCharacterSize(20);
    credits.setString("Programmed and designed by:\n"
                      "     The great Nassim Am");
    credits.setPosition(555,450);
    //Buttons----------
    Button Menuserverbutton(sf::Vector2f(250,250),sf::Vector2f(300,80),"ressources/textures/menubutton.png","ressources/textures/menubuttonpressed.png");
    Button Clientbutton(sf::Vector2f(250,350),sf::Vector2f(300,80),"ressources/textures/menubutton.png","ressources/textures/menubuttonpressed.png");
    Menuserverbutton.setText("ressources/fonts/Arial.ttf","Create Server",sf::Color::White);
    Clientbutton.setText("ressources/fonts/Arial.ttf","Find Servers",sf::Color::White);
    Menuserverbutton.CenterText(sf::Vector2f(272.5,262.5));
    Clientbutton.CenterText(sf::Vector2f(285.5,362.5));
    //Return Button
    Button ReturnButton(sf::Vector2f(10,15),sf::Vector2f(40,40),"ressources/textures/returnbutton.png","ressources/textures/returnbuttonpressed.png");
    //Username inputbox----------------
    InputBox usernameinput(sf::Vector2f(580,180),sf::Vector2f(200,40),"ressources/textures/whitebutton.png","ressources/fonts/Arial.ttf",sf::Color::Black);
    sf::Text usernameinstruction;
    sf::Font arialfont;
    if(!arialfont.loadFromFile("ressources/fonts/Arial.ttf"))
        std::cerr<<"ERROR: could not load the arial font\n";
    usernameinstruction.setFont(arialfont);
    usernameinstruction.setString("USERNAME:");
    usernameinstruction.setFillColor(sf::Color(0,0,0));
    usernameinstruction.setCharacterSize(25);
    usernameinstruction.setPosition(503.5,122.5);
    //Waiting Client Interface--------------------------------------------000000000000000000000000000
    sf::Text waitingtext;
    waitingtext.setFont(creditfont);
    waitingtext.setCharacterSize(65);
    waitingtext.setPosition(250,260);
    waitingtext.setString("Waiting Client...");

    sf::Text usernameshowtext;
    sf::Font italicarial;
    if(!italicarial.loadFromFile("ressources/fonts/Arialitalic.ttf"))
        std::cerr<<"ERROR: could not load the ItalicArial font\n";
    usernameshowtext.setFont(italicarial);
    usernameshowtext.setCharacterSize(25);
    usernameshowtext.setPosition(5,565);

    sf::Cursor defaultcursor;
    defaultcursor.loadFromSystem(sf::Cursor::Arrow);
    //Searching Servers Interface-------------------------------------0000000000000000000000000000000
    Button ServerButton(sf::Vector2f(0,0),sf::Vector2f(540,40),"ressources/textures/graybutton.png","ressources/textures/graybuttonpressed.png","ressources/fonts/Arial.ttf");
    ServerButton.setGap(sf::Vector2f(20,8));
    Button RefreshButton(sf::Vector2f(0,0),sf::Vector2f(100,30),"ressources/textures/graybutton.png","ressources/textures/graybuttonpressed.png","ressources/fonts/creditsfont.ttf",sf::Color::White);
    RefreshButton.setText("Refresh");
    RefreshButton.setGap(sf::Vector2f(25,2));
    RefreshButton.setPosition(sf::Vector2f(570,60));
    RefreshButton.setTextCharacterSize(20);

    sf::Text ServerListtext;
    ServerListtext.setFont(creditfont);
    ServerListtext.setCharacterSize(30);
    ServerListtext.setPosition(120,55);
    ServerListtext.setString("Servers list:");
    sf::Text Noservertext;
    Noservertext.setFont(italicarial);
    Noservertext.setCharacterSize(15);
    Noservertext.setPosition(260,120);
    Noservertext.setString("No server found          Please wait...");
    Noservertext.setFillColor(sf::Color(150,150,150,240));

    sf::Sprite searchingbackground;
    sf::Texture searchingtexture;
    if(!searchingtexture.loadFromFile("ressources/textures/communicationbg.jpg"))
        std::cerr<<"ERROR : could not load the searching background texture\n";
    searchingbackground.setTexture(searchingtexture);
    searchingbackground.scale(800.0f/searchingtexture.getSize().x,600.0f/searchingtexture.getSize().y);

    sf::RectangleShape Serverlistbox;
    Serverlistbox.setSize(sf::Vector2f(560,420));
    Serverlistbox.setPosition(110,90);
    Serverlistbox.setFillColor(sf::Color(30,30,30,200));
    //Chat interface------------------------------000000000000000000000000000000000000000000
    InputBox chatinput(sf::Vector2f(350,585),sf::Vector2f(700,30),"ressources/textures/chatbox.png","ressources/fonts/Arial.ttf",sf::Color::Black);
    Button sendbutton(sf::Vector2f(700,570),sf::Vector2f(100,30),"ressources/textures/arrow.png","ressources/textures/arrowpressed.png");
    std::vector<Msgbox> MyMsg;
    std::vector<Msgbox> MsgReceived;
    Msgbox newMsgbox(sf::Vector2f(0,0),sf::Vector2f(400,40),"ressources/textures/msgboxyours.png","ressources/fonts/Arial.ttf");
    Msgbox newMsgboxreceived(sf::Vector2f(0,0),sf::Vector2f(400,40),"ressources/textures/msgboxothers.png","ressources/fonts/Arial.ttf");
    Msgbox otherwritingbox(sf::Vector2f(0,0),sf::Vector2f(200,40),"ressources/textures/msgboxothers.png","ressources/fonts/Arialitalic.ttf");
    sf::Text Otherusername;
    Otherusername.setFont(creditfont);
    Otherusername.setCharacterSize(25);
    Otherusername.setPosition(60,10);
    Otherusername.setFillColor(sf::Color(0,0,0));
    sf::Text statustext;
    statustext.setFont(arialfont);
    statustext.setCharacterSize(20);
    statustext.setPosition(60,40);
    statustext.setFillColor(sf::Color(70,70,70,240));

    sf::RectangleShape chattemplate;
    chattemplate.setSize(sf::Vector2f(810,70));
    chattemplate.setPosition(-5,-5);
    chattemplate.setFillColor(sf::Color::White);
    chattemplate.setOutlineColor(sf::Color(150,150,150,240));
    chattemplate.setOutlineThickness(2);

    while(Mainwindow.isOpen())
    {
        sf::Vector2i mouseposition = sf::Mouse::getPosition(Mainwindow);

        sf::Event event;
        while(Mainwindow.pollEvent(event))
        {
            if(event.type==sf::Event::Closed)
                {
                    Mainwindow.close();
                }

            if(event.type==sf::Event::GainedFocus)
                windowfocused = true;
            else if(event.type==sf::Event::LostFocus)
                windowfocused = false;

            if(event.type==sf::Event::TextEntered)
                enteringtext=true;
        }
        //Menu-------------------------------------000000000000000000000000000000000
        if(menu)
        {
            if(username_entered)
            {
                username=usernameinput.GetText();
            }
            Mainwindow.clear(sf::Color::White);
            Mainwindow.draw(menubackground);
            Mainwindow.draw(credits);
            Menuserverbutton.Draw(Mainwindow);
            Clientbutton.Draw(Mainwindow);
            Menuserverbutton.CheckMouse(Mainwindow,mouseposition,initiatingserver,windowfocused);
            Clientbutton.CheckMouse(Mainwindow,mouseposition,initiatingclient,windowfocused);
            usernameinput.Draw(Mainwindow);
            usernameinput.CheckInput(mouseposition,Mainwindow,event,enteringtext,username_entered);
            Mainwindow.draw(usernameinstruction);
            Mainwindow.display();
        }
        //Begin Chat----------------------------0000000000000000000000000000000
        if(servercreated)
        {
            menu = false;
            //Chatting-------------------------------------------------
            //Receiving-----------------------------------000000000000000000000000
            sf::Packet packetreceived;
            std::string msgreceived;

            socket.receive(packetreceived);
            packetreceived>>msgreceived>>otherwriting>>receivedstatus;
            if(!msgreceived.empty())
            {
                newMsgboxreceived.setMsg(msgreceived);
                MsgReceived.push_back(newMsgboxreceived);
                MsgReceived[MsgReceived.size()-1].setId(msgid);
                msgid++;
                newMsgboxreceived.Reset();
            }

            if(receivedstatus=="Connected")
            {
                statustext.setString("Online");
            }
            else
                statustext.setString("Disconnected");
            //Sending--------------------------------------000000000000000000000000
            sf::Packet sendpacket;
            std::string sendtext;
            if(send)
            {
                if(!chatinput.empty())
                {
                    sendtext=chatinput.GetText();
                    newMsgbox.setMsg(chatinput.GetText());
                    MyMsg.push_back(newMsgbox);
                    MyMsg[MyMsg.size()-1].setId(msgid);
                    msgid++;
                    chatinput.Reset();
                    newMsgbox.Reset();
                }
                send=false;
            }
            else
                sendtext.clear();

            sendpacket<<sendtext<<chatinput.Writing()<<mystatus;
            socket.send(sendpacket);

            Mainwindow.clear(sf::Color::White);
            chatinput.Draw(Mainwindow);
            chatinput.CheckInput(mouseposition,Mainwindow,event,enteringtext,send);
            sendbutton.Draw(Mainwindow);
            sendbutton.CheckMouse(Mainwindow,mouseposition,send,windowfocused);
            //Drawing Messages-------------------------------------000000000000000000000000000000000000000
            for(unsigned int i=0;i<MyMsg.size()+MsgReceived.size();i++)
            {
                for(unsigned int j=0;j<MyMsg.size();j++)
                {
                    if(!MyMsg.empty())
                    {
                        if(MyMsg[j].getId()==i)
                        {
                            float sizedelay;
                            sizedelay = 0;
                            for(unsigned int h=j;h<MyMsg.size();h++)
                                sizedelay += MyMsg[h].getSize().y+10;
                            for(unsigned int h=0;h<MsgReceived.size();h++)
                            {
                                if(MsgReceived[h].getId()>MyMsg[j].getId())
                                {
                                    sizedelay += MsgReceived[h].getSize().y+10;
                                }
                            }
                            if(otherwriting)
                                MyMsg[j].setPosition(sf::Vector2f(380,500-sizedelay));
                            else
                                MyMsg[j].setPosition(sf::Vector2f(380,560-sizedelay));

                            MyMsg[j].Draw(Mainwindow);
                        }
                    }
                }
                for(unsigned int j=0;j<MsgReceived.size();j++)
                {
                    if(!MsgReceived.empty())
                    {
                        if(MsgReceived[j].getId()==i)
                        {
                            float sizedelay;
                            sizedelay = 0;
                            for(unsigned int h=j;h<MsgReceived.size();h++)
                                sizedelay += MsgReceived[h].getSize().y+10;
                            for(unsigned int h=0;h<MyMsg.size();h++)
                            {
                                if(MyMsg[h].getId()>MsgReceived[j].getId())
                                {
                                    sizedelay += MyMsg[h].getSize().y+10;
                                }
                            }
                            if(otherwriting)
                                MsgReceived[j].setPosition(sf::Vector2f(20,500-sizedelay));
                            else
                                MsgReceived[j].setPosition(sf::Vector2f(20,560-sizedelay));

                            MsgReceived[j].Draw(Mainwindow);
                        }
                    }
                }

            }
            if(otherwriting)
            {
                otherwritingbox.setPosition(sf::Vector2f(20,500));
                otherwritingbox.Draw(Mainwindow);
            }
            Otherusername.setString(getusername);

            Mainwindow.draw(chattemplate);
            ReturnButton.Draw(Mainwindow);
            ReturnButton.CheckMouse(Mainwindow,mouseposition,returning,windowfocused);
            Mainwindow.draw(Otherusername);
            Mainwindow.draw(statustext);

            Mainwindow.display();

            if(returning)
            {
                menu = true;
                socket.disconnect();
                username.clear();
                ServersButtons.clear();
                ComputerIP.clear();
                ComputerPort.clear();
                servercreated = false;
                returning = false;
            }
        }
        //Creation of the Server-----------------------------------
        if(initiatingserver)
        {
            udpsocket.bind(2000);
            udpsocket.setBlocking(false);
            localip=sf::IpAddress::getLocalAddress();
            iproot = localip.toString();
            hostend = 0;
            iproot.erase(iproot.end()-2,iproot.end());
            if(username.empty()&&usernameinput.empty())
            {
                int id=rand() % 100;
                username = "Server"+std::to_string(id);
            }
            else
            {
                username = usernameinput.GetText();
            }
            usernameshowtext.setString("UN: "+username);
            serverconnecting = true;
            initiatingserver=false;
            menu = false;
        }
        if(serverconnecting)
        {
            CreateServer();
            Mainwindow.clear();
            ReturnButton.CheckMouse(Mainwindow,mouseposition,returning,windowfocused);
            ReturnButton.Draw(Mainwindow);
            Mainwindow.draw(waitingtext);
            Mainwindow.draw(usernameshowtext);
            Mainwindow.display();
            if(returning)
            {
                username.clear();
                ServersButtons.clear();
                ComputerIP.clear();
                ComputerPort.clear();
                serverconnecting = false;
                menu = true;
                returning = false;
            }
        }
        if(creatingserver)
        {
            Server();
            otherwritingbox.setMsg(getusername+" is writing...");
            socket.setBlocking(false);
            creatingserver=false;
        }
        //Searching Servers------------------------------------------------------------------
            //Creation of the Client--------------------------------------
        if(initiatingclient)
        {
            clientconnecting = true;
            udpsocket.bind(2001);
            udpsocket.setBlocking(false);
            if(username.empty()&&usernameinput.empty())
            {
                int id=rand() % 100;
                username = "Client"+std::to_string(id);
            }
            else
            {
                username = usernameinput.GetText();
            }
            usernameshowtext.setString("UN: "+username);
            initiatingclient = false;
            menu = false;
        }
        if(clientconnecting)
        {
            if(refreshing)
            {
                ServersButtons.clear();
                ComputerIP.clear();
                ComputerPort.clear();
                refreshing = false;
            }
            Mainwindow.clear();
            Mainwindow.draw(searchingbackground);
            ReturnButton.CheckMouse(Mainwindow,mouseposition,returning,windowfocused);
            ReturnButton.Draw(Mainwindow);
            RefreshButton.CheckMouse(Mainwindow,mouseposition,refreshing,windowfocused);
            RefreshButton.Draw(Mainwindow);
            Mainwindow.draw(Serverlistbox);
            SearchForServers(ServerButton,Mainwindow,mouseposition,Noservertext);
            Mainwindow.draw(usernameshowtext);
            Mainwindow.draw(ServerListtext);
            Mainwindow.display();
            if(returning)
            {
                username.clear();
                ServersButtons.clear();
                ComputerIP.clear();
                ComputerPort.clear();
                clientconnecting = false;
                menu = true;
                returning = false;
            }
        }
        if(creatingclient)
        {
            Client();
            otherwritingbox.setMsg(getusername+" is writing...");
            socket.setBlocking(false);
            creatingclient=false;
        }
        enteringtext = false;

    }
    return 0;
}
