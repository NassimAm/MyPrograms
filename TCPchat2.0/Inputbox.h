#ifndef INPUTBOX_H_INCLUDED
#define INPUTBOX_H_INCLUDED

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class InputBox
{
public:
    InputBox(sf::Vector2f origin,sf::Vector2f boxsize,std::string texturepath,std::string fontpath,sf::Color textcolor=sf::Color::White);
    void CheckInput(sf::Vector2i &mouseposition,sf::RenderWindow &window,sf::Event &event,bool &enteringtext,bool &send);
    void Draw(sf::RenderWindow &window);
    void setTextColor(sf::Color color);
    void setText(std::string text);
    std::string GetText();
    void Reset();
    bool empty();
    bool Writing();
private:
    sf::Sprite boxsprite;
    sf::Vector2f boxsize;
    bool cursorchanged,releasedleft,releasedright,canwrite = true;
    bool write,slidingleft,slidingright,holdingleft,holdingright = false;
    sf::Texture boxtexture;

    sf::RectangleShape cursor;
    int slide = 1;
    sf::Clock cursorclock,pressclock,slideclock;


    std::vector<sf::Text>textshowed;
    sf::Font textfont;
    sf::Color textcolor;
    char textentered;
    std::string resulttext;
};

#endif // INPUTBOX_H_INCLUDED
