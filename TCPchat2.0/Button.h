#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(sf::Vector2f position,sf::Vector2f buttonsize,std::string defaulttexturepath,std::string pressedtexturepath);
    Button(sf::Vector2f position,sf::Vector2f buttonsize,std::string defaulttexturepath,std::string pressedtexturepath,std::string fontpath,sf::Color textcolor = sf::Color::Black);
    void CheckMouse(sf::RenderWindow &window,sf::Vector2i &mouseposition,bool &var,bool &windowfocused);
    void setText(std::string fontpath,std::string text,sf::Color textcolor=sf::Color::Black);
    void setText(std::string text);
    void setTextCharacterSize(int charactersize);
    void CenterText(sf::Vector2f textposition);
    void Draw(sf::RenderWindow &window);
    bool Clicked();
    void ResetClick();
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition();
    void setGap(sf::Vector2f gap);
protected:
    sf::Sprite buttonsprite;
    sf::Texture bdefaulttexture,bpressedtexture;
    sf::Vector2f buttonsize;
    bool reset=true;
    bool clicked=false;
    sf::Text buttontext;
    sf::Font buttonfont;
    float textgap_x,textgap_y=0;

};

#endif // BUTTON_H_INCLUDED
