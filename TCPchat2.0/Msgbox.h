#ifndef MSGBOX_H_INCLUDED
#define MSGBOX_H_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class Msgbox
{
public:
    Msgbox(sf::Vector2f position,sf::Vector2f boxsize,std::string texturepath,std::string fontpath);
    void setMsg(std::string msg,sf::Color msgcolor=sf::Color::Black);
    void Draw(sf::RenderWindow &window);
    void setId(int id);
    void setPosition(sf::Vector2f position);
    unsigned int getId();
    void setTexture(sf::Texture texture);
    const sf::Texture* getTexture();
    sf::Vector2f getSize();
    void Reset();
private:
    sf::Sprite boxsprite;
    sf::Vector2f boxsize;
    sf::Texture boxtexture;
    std::vector<sf::Text> Msg;
    sf::Font Msgfont;
    unsigned int id=0;
};

#endif // MSGBOX_H_INCLUDED
