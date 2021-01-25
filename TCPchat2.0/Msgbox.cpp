#include <iostream>
#include <SFML/Graphics.hpp>
#include "Msgbox.h"

Msgbox::Msgbox(sf::Vector2f position,sf::Vector2f boxsize,std::string texturepath,std::string fontpath)
{
    if(!boxtexture.loadFromFile(texturepath))
        std::cerr<<"ERROR: could not load the msgbox texture\n";
    if(!Msgfont.loadFromFile(fontpath))
        std::cerr<<"ERROR: could not load the msg font\n";
    boxtexture.setSmooth(true);
    boxsprite.setTexture(boxtexture);
    boxsprite.setPosition(position);
    this->boxsize = boxsize;
}


void Msgbox::setMsg(std::string msg,sf::Color msgcolor)
{
    //Calculating size--------------------------------
    unsigned int identifier,idcounter,counter;
    identifier=0;
    idcounter=0;
    counter=0;
    while(identifier<msg.length())
    {
        std::string temptext;
        sf::Text testtext;
        testtext.setCharacterSize(boxsize.y/2);
        testtext.setFont(Msgfont);
        testtext.setColor(msgcolor);
        for(unsigned int i=identifier;i<msg.length();i++)
        {
            temptext+=msg[i];
            testtext.setString(temptext);
            idcounter++;
            if(testtext.getLocalBounds().width>(boxsize.x*27.5)/30)
                break;
        }
        identifier=idcounter;
        counter++;
        Msg.push_back(testtext);
    }
    boxsprite.scale(boxsize.x/boxsprite.getGlobalBounds().width,((boxsize.y*counter)-((Msg.size()-1)*boxsize.y/5.5))/boxsprite.getGlobalBounds().height);
}

void Msgbox::Draw(sf::RenderWindow &window)
{
    window.draw(boxsprite);
    for(unsigned int i=0;i<Msg.size();i++)
    {
        if(i==0)
            Msg[i].setPosition(boxsprite.getPosition().x+boxsize.x/30,boxsprite.getPosition().y+boxsize.y/5.5);
        else
            Msg[i].setPosition(boxsprite.getPosition().x+boxsize.x/30,Msg[i-1].getPosition().y+Msg[i-1].getLocalBounds().height+boxsize.y/5.5);

        window.draw(Msg[i]);
    }
}

void Msgbox::setId(int id)
{
    this->id = id;
}

void Msgbox::setPosition(sf::Vector2f position)
{
    boxsprite.setPosition(position);
}

unsigned int Msgbox::getId()
{
    return id;
}

void Msgbox::setTexture(sf::Texture texture)
{
    boxsprite.setTexture(texture);
}

const sf::Texture* Msgbox::getTexture()
{
    return boxsprite.getTexture();
}

sf::Vector2f Msgbox::getSize()
{
    sf::Vector2f sizeofbox;
    sizeofbox = sf::Vector2f(boxsprite.getGlobalBounds().width,boxsprite.getGlobalBounds().height);
    return sizeofbox;
}

void Msgbox::Reset()
{
    Msg.clear();
}
