#pragma once
#include <iostream>
#include <string>
#include <array>
#include <tuple>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Piece.h"

using namespace std;
class Square
{
private:
    int m_i, m_j;                               // row, col of the square in the board
    string *m_name;                             // a4 or b5 or ...
    Piece* m_piece;                             // pointer to a piece, nullptr if none
    unsigned int m_space_size;                  // square height on the board
    sf::RectangleShape* m_sprite;               // SFML sprite of the square
    sf::Text* m_text;                           // m_name to be displayed
    sf::Font* m_font;                           // font for m_text
    sf::Color* default_light_color;             // default colors
    sf::Color* default_dark_color;
    sf::Color* valid_color;                     // bright color for valid squares
    sf::Color* invalid_color;                   // red color for an invalid piece --  a piece that does not have a valid move
    sf::Sprite* empty_sprite;

    void create_sprite();
    void create_id();
    void create_piece(string a_piece_filename);

public:
    Square(int a_i, int a_j, unsigned int a_space_size, array<array<string, 8>, 8>& piece_filenames);
    ~Square();
    void set_new_piece(string a_piece_filename);
    void set_piece(Piece* a_piece) {m_piece = a_piece;}
    void set_valid_color() { m_sprite->setFillColor(*valid_color); }
    void set_invalid_color() { m_sprite->setFillColor(*invalid_color); }

    Piece* get_piece() { return m_piece; }
    sf::RectangleShape* get_sprite() { return m_sprite; }
    sf::Text* get_id() { return m_text; }
    sf::Sprite* get_piece_sprite();
    string get_name() { return *m_name; }

    void delete_piece();
    Piece* remove_piece();
    bool has_piece() { return (m_piece != nullptr); }
    void reset_color();
};

