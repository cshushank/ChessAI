/*
Piece class
-- Stores the links to name, type, color, SFML sprite, and filename to the image of the piece.
-- Two constructors: constructor for the piece in display board (with sprite)
					and constructor for the minimax board (without sprite).
-- Relevent getters and setters
*/


#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Piece
{
private:
    string *m_type;							// rook or knight or ...
    string *m_color;						// white or black piece
    string *m_name;							// white_rook or black_knight or ...
    string *m_filename;						// white_rook.png or black_knight.png or ...
    sf::Sprite* m_sprite;					// sfml sprite
public:

    // Constructors and Destructor
    Piece(string a_filename, unsigned int a_space_size);
    Piece(string a_filename);
    ~Piece();


    string get_name() { return *m_name; }
    string get_type() { return *m_type; }
    string get_color() { return *m_color; }
    void set_sprite_position(unsigned int x, unsigned int y) { m_sprite->setPosition(x, y); }
    sf::Sprite* get_sprite() { return m_sprite; }
    string get_filename();
};

