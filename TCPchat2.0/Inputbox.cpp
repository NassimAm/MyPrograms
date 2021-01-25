#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Inputbox.h"

std::string alphabet = " !'#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[/]^_`abcdefghijklmnopqrstuvwxyz{|}-";

InputBox::InputBox(sf::Vector2f origin,sf::Vector2f boxsize,std::string texturepath,std::string fontpath,sf::Color textcolor)
{
    if(!boxtexture.loadFromFile(texturepath))
    {
        std::cerr<<"Error: could not load the inputbox texture\n";
    }
    if(!textfont.loadFromFile(fontpath))
    {
        std::cerr<<"Error: could not load the text font\n";
    }
    //Box sprite Configuration-------------------------------000000000000000000000000000000000
    boxsprite.setTexture(boxtexture);
    boxsprite.setPosition(origin.x-(boxsize.x/2),origin.y-(boxsize.y/2));
    boxsprite.setScale(boxsize.x/boxtexture.getSize().x,boxsize.y/boxtexture.getSize().y);
    this->boxsize = boxsize;
    this->textcolor = textcolor;
    //Cursor Configuration----------------------------------0000000000000000000000000000000000000
    cursor.setPosition(boxsprite.getPosition().x+boxsize.x/30,boxsprite.getPosition().y+boxsize.y/4);
    cursor.setSize(sf::Vector2f(1,boxsize.y/2));
    cursor.setFillColor(textcolor);
    cursor.setOutlineColor(textcolor);

    write = false;

    cursorclock.restart();
    pressclock.restart();
    slideclock.restart();
}

void InputBox::CheckInput(sf::Vector2i &mouseposition,sf::RenderWindow &window,sf::Event &event,bool &enteringtext,bool &send)
{
    //Check writing & Mouse cursor changing------------------------------------000000000000000000000000000000000000
    sf::Cursor mousecursor;
    if(mouseposition.x>boxsprite.getPosition().x&&mouseposition.x<boxsprite.getPosition().x+boxsize.x
       &&mouseposition.y>boxsprite.getPosition().y&&mouseposition.y<boxsprite.getPosition().y+boxsize.y)
    {
        mousecursor.loadFromSystem(sf::Cursor::Text);
        cursorchanged = true;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            write=true;
    }
    else if(cursorchanged)
    {
        mousecursor.loadFromSystem(sf::Cursor::Arrow);
        cursorchanged = false;
    }
    else
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            write=false;
    }

    if(mouseposition.x<0||mouseposition.x>window.getSize().x||mouseposition.y<0||mouseposition.y>window.getSize().y)
        cursorchanged=true;

    window.setMouseCursor(mousecursor);

    //Writing--------------------------------------------------000000000000000000000000000000000000000
    if(write)
    {
        cursor.setFillColor(textcolor);
        cursor.setOutlineColor(textcolor);
        //Entering text-------------------------0000000000000000000000000000000
        if(enteringtext)
        {
            window.draw(cursor);
            cursorclock.restart();
            sf::Text newtext;
            if(event.text.unicode==13&&!textshowed.empty())
            {
                send = true;
                write = false;
            }
            if(event.text.unicode==8&&!textshowed.empty()&&slide!=0)
            {
                textshowed.erase(textshowed.begin()+slide-1);
                resulttext.pop_back();
                slide--;
                canwrite = true;
            }
            if(event.text.unicode>=32&&event.text.unicode!=59&&canwrite)
            {
                textentered=alphabet[event.text.unicode-32];
                resulttext.insert(resulttext.begin()+slide,textentered);
                newtext.setString(textentered);
                textshowed.insert(textshowed.begin()+slide,newtext);
                slide++;
            }
        }

        sf::Time showtime=cursorclock.getElapsedTime();
        sf::Time presstime=pressclock.getElapsedTime();
        sf::Time slidetime=slideclock.getElapsedTime();
        if(!holdingleft&&!holdingright)
        {
            if(showtime.asSeconds()<0.5)
                window.draw(cursor);
            if(showtime.asSeconds()>1)
                cursorclock.restart();
        }
        else
        {
            window.draw(cursor);
            cursorclock.restart();
        }



        //Moving the cursor-------------------------------0000000000000000000000000000
        //Keyboard input------------------
        if(event.type==sf::Event::KeyPressed)
        {
            if(event.key.code==sf::Keyboard::Left)
            {
                if(releasedleft)
                {
                    slidingleft=true;
                    releasedleft=false;
                }
                if(presstime.asSeconds()>0.5)
                    holdingleft=true;
            }
            else if(event.key.code==sf::Keyboard::Right)
            {
                if(releasedright)
                {
                    slidingright=true;
                    releasedright=false;
                }
                if(presstime.asSeconds()>0.5)
                    holdingright=true;
            }
        }
        if(event.type==sf::Event::KeyReleased)
        {
            if(event.key.code==sf::Keyboard::Left)
            {
                holdingleft=false;
                releasedleft=true;
                slidingleft=false;
            }
            else if(event.key.code==sf::Keyboard::Right)
            {
                holdingright=false;
                releasedright=true;
                slidingright=false;
            }
        }

        //Cursor moves----------------------------
        //Clicking----
        if(slidingleft&&slide>0&&!holdingleft)
        {
            slide-=1;
            window.draw(cursor);
            cursorclock.restart();
            slidingleft=false;
        }
        if(slidingright&&slide<textshowed.size()&&!holdingright)
        {
            slide+=1;
            window.draw(cursor);
            cursorclock.restart();
            slidingright=false;
        }
        //Holding----
        if(holdingleft)
        {
           if(slidetime.asSeconds()>0.05)
            {
                slide-=1;
                slideclock.restart();
            }
        }
        if(holdingright)
        {
            if(slidetime.asSeconds()>0.05)
            {
                slide+=1;
                slideclock.restart();
            }
        }

        if(releasedleft&&releasedright)
        {
            pressclock.restart();
        }
        if(!holdingleft&&!holdingright)
        {
            slideclock.restart();
        }


        if(slide<0)
            slide=0;
        if(slide>textshowed.size())
            slide=textshowed.size();

        //Cursor position-------------------------
        if(textshowed.empty()||slide==0)
            cursor.setPosition(boxsprite.getPosition().x+boxsize.x/30,boxsprite.getPosition().y+boxsize.y/4);
        else
        {
            float lenght,reallenght;
            for(unsigned int i=0;i<slide;i++)
                lenght+=textshowed[i].getLocalBounds().width;
            for(unsigned int i=0;i<textshowed.size();i++)
                reallenght+=textshowed[i].getLocalBounds().width;
            if(reallenght+(textshowed.size()-1)>(boxsize.x*27.5)/30)
                canwrite=false;
            else
                canwrite=true;
            cursor.setPosition(boxsprite.getPosition().x+boxsize.x/30+lenght+(slide-1),cursor.getPosition().y);
        }
    }

}

void InputBox::Draw(sf::RenderWindow &window)
{
    window.draw(boxsprite);
    //Text Configuration-----------------------------------000000000000000000000000000000000000000
    for(unsigned int i=0;i<textshowed.size();i++)
    {
        textshowed[i].setFont(textfont);
        if(i>0)
            textshowed[i].setPosition(textshowed[i-1].getPosition().x+textshowed[i-1].getLocalBounds().width+1,boxsprite.getPosition().y+boxsize.y/5.5);
        else
            textshowed[i].setPosition(boxsprite.getPosition().x+boxsize.x/30,boxsprite.getPosition().y+boxsize.y/5.5);

        textshowed[i].setCharacterSize(boxsize.y/2);
        textshowed[i].setColor(textcolor);
        window.draw(textshowed[i]);
    }

}

void InputBox::setTextColor(sf::Color color)
{
    textcolor=color;
}

std::string InputBox::GetText()
{
    return resulttext;
}

void InputBox::setText(std::string text)
{
    resulttext = text;
    for(unsigned int i=0;text.length();i++)
    {
        sf::Text temptext;
        temptext.setString(text[i]);
        textshowed.push_back(temptext);
    }
}

void InputBox::Reset()
{
    textshowed.clear();
    resulttext.clear();
    canwrite = true;
}

bool InputBox::empty()
{
    if(textshowed.empty()&&resulttext.empty())
        return true;
    else
        return false;
}

bool InputBox::Writing()
{
    return write;
}
