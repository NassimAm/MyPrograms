#include <iostream>
#include <SFML/Graphics.hpp>
#include "Button.h"

Button::Button(sf::Vector2f position,sf::Vector2f buttonsize,std::string defaulttexturepath,std::string pressedtexturepath)
{
    if(!bdefaulttexture.loadFromFile(defaulttexturepath))
        std::cerr<<"ERROR: could not load the default button texture\n";
    if(!bpressedtexture.loadFromFile(pressedtexturepath))
        std::cerr<<"ERROR: could not load the pressed button texture\n";
    this->buttonsize = buttonsize;
    buttonsprite.setTexture(bdefaulttexture);
    buttonsprite.setPosition(position);
    buttonsprite.scale(sf::Vector2f(buttonsize.x/bdefaulttexture.getSize().x,buttonsize.y/bdefaulttexture.getSize().y));
    reset = true;
}

Button::Button(sf::Vector2f position,sf::Vector2f buttonsize,std::string defaulttexturepath,std::string pressedtexturepath,std::string fontpath,sf::Color textcolor)
{
    if(!bdefaulttexture.loadFromFile(defaulttexturepath))
        std::cerr<<"ERROR: could not load the default button texture\n";
    if(!bpressedtexture.loadFromFile(pressedtexturepath))
        std::cerr<<"ERROR: could not load the pressed button texture\n";
    if(!buttonfont.loadFromFile(fontpath))
        std::cerr<<"ERROR: could not load the button text font\n";
    this->buttonsize = buttonsize;
    buttonsprite.setTexture(bdefaulttexture);
    buttonsprite.setPosition(position);
    buttonsprite.scale(sf::Vector2f(buttonsize.x/bdefaulttexture.getSize().x,buttonsize.y/bdefaulttexture.getSize().y));
    buttontext.setFont(buttonfont);
    buttontext.setFillColor(textcolor);
    buttontext.setCharacterSize(buttonsize.y/2);
    buttontext.setPosition(position.x+position.x/30,position.y+position.y/20);
    textgap_x = position.x/30;
    textgap_y = position.y/20;
    reset = true;
}

void Button::setText(std::string fontpath,std::string text,sf::Color textcolor)
{
    if(!buttonfont.loadFromFile(fontpath))
        std::cerr<<"ERROR: could not load the text font\n";
    buttontext.setFont(buttonfont);
    buttontext.setFillColor(textcolor);
    buttontext.setString(text);
    unsigned int charactersize;
    charactersize = buttonsize.y/2;
    buttontext.setCharacterSize(charactersize);
    buttontext.setPosition(sf::Vector2f(buttonsprite.getPosition().x+(buttonsize.x-buttontext.getLocalBounds().width)/2,buttonsprite.getPosition().y+(buttonsize.y-charactersize)/2));
    textgap_x = (buttonsize.x-buttontext.getLocalBounds().width)/2;
    textgap_y = (buttonsize.y-charactersize)/2;
}

void Button::setText(std::string text)
{
    buttontext.setString(text);
}

void Button::CenterText(sf::Vector2f textposition)
{
    buttontext.setPosition(textposition);
    textgap_x = textposition.x-buttonsprite.getPosition().x;
    textgap_x = textposition.y-buttonsprite.getPosition().y;
}

void Button::setTextCharacterSize(int charactersize)
{
    buttontext.setCharacterSize(charactersize);
}

void Button::CheckMouse(sf::RenderWindow &window,sf::Vector2i &mouseposition,bool &var,bool &windowfocused)
{
    if(windowfocused)
    {
        sf::Cursor cursor;
        if(mouseposition.x>buttonsprite.getPosition().x&&mouseposition.x<buttonsprite.getPosition().x+buttonsprite.getGlobalBounds().width
           &&mouseposition.y>buttonsprite.getPosition().y&&mouseposition.y<buttonsprite.getPosition().y+buttonsprite.getGlobalBounds().height)
        {
            reset=true;
            buttonsprite.setTexture(bpressedtexture);
            cursor.loadFromSystem(sf::Cursor::Hand);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                var=true;
                clicked=true;
                cursor.loadFromSystem(sf::Cursor::Arrow);
            }

        }
        else
        {
            buttonsprite.setTexture(bdefaulttexture);
            if(reset)
            {
                cursor.loadFromSystem(sf::Cursor::Arrow);
                reset=false;
            }
        }

        if(mouseposition.x>800||mouseposition.x<0||mouseposition.y<0||mouseposition.y>600)
            reset=true;

        window.setMouseCursor(cursor);
    }
}


void Button::Draw(sf::RenderWindow &window)
{
    window.draw(buttonsprite);
    window.draw(buttontext);
}

bool Button::Clicked()
{
    return clicked;
}

void Button::ResetClick()
{
    clicked = false;
}

void Button::setPosition(sf::Vector2f position)
{
    buttonsprite.setPosition(position);
    buttontext.setPosition(position.x+textgap_x,position.y+textgap_y);
}

sf::Vector2f Button::getPosition()
{
    return buttonsprite.getPosition();
}

void Button::setGap(sf::Vector2f gap)
{
    textgap_x = gap.x;
    textgap_y = gap.y;
}
