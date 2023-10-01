
#include "Square.h"


/*
* NAME
*      Square -- creates a square with graphical components
*
* SYNOPSYS
*
*      Square::Square(int a_i, int a_j, unsigned int a_space_size, array<array<string, 8>, 8>& piece_filenames);
 *      a_space_size    ->  the height of square on which the piece is to exist
*       piece_filenames -> array of all files of pieces
* DESCRIPTION
*
*  This function initializes the main components of the square
 *  and calls create sprite,
 *      create id (for the name of square on the board)
 *      create piece
*/
Square::Square(int a_i, int a_j, unsigned int a_space_size, array<array<string, 8>, 8>& piece_filenames)
{
    m_i = a_i;
    m_j = a_j;
    m_name = new string((char)(m_j + 97) + to_string(m_i + 1));
    m_piece = nullptr;
    m_space_size = a_space_size;
    empty_sprite = new sf::Sprite();

    // Colors
    default_light_color = new sf::Color(205, 133, 63);
    default_dark_color = new sf::Color(101, 67, 33);
    valid_color = new sf::Color(173, 216, 230);
    invalid_color = new sf::Color(255, 0, 0);

    create_sprite();
    create_id();
    create_piece(piece_filenames[m_i][m_j]);
}


/*
* NAME
*      ~Square -- destroys the square when the board is about to be destroyed
*
* SYNOPSYS
*
*      Square::~Square();
 *
* DESCRIPTION
*
*  This function deallocates all the allocated memory on the heap when a square is created.
*/
Square::~Square()
{
    if (m_piece != nullptr)
        delete m_piece;
    if (m_sprite)
        delete m_sprite;
    if (m_font)
        delete m_font;
    if (m_text)
        delete m_text;
    if (default_light_color)
        delete default_light_color;
    if (default_dark_color)
        delete default_dark_color;
    if (valid_color)
        delete valid_color;
    if (invalid_color)
        delete invalid_color;
    if (empty_sprite)
        delete empty_sprite;
    if (m_name)
        delete m_name;
}


/*
* NAME
*      CreateSprite -- creates sprite for the square and fills it with appropriate default color
*
* SYNOPSYS
*
*      void Square::create_sprite();
 *
* DESCRIPTION
*
 *   This function creates sprite for the square and fills it with appropriate default color.
*
*/
void Square::create_sprite()
{
    m_sprite = new sf::RectangleShape(sf::Vector2f((float)m_space_size, (float)m_space_size));
    m_sprite->setOutlineThickness(2.f);
    m_sprite->setOutlineColor(sf::Color::Black);
    m_sprite->setPosition(m_j * m_space_size, (7 - m_i) * m_space_size);
    if ((m_i + m_j) % 2 == 0) {
        m_sprite->setFillColor(*default_dark_color);
    }
    else {
        m_sprite->setFillColor(*default_light_color);
    }
}


/*
* NAME
*      CreateId - creates sfml text for the name of the board
*
* SYNOPSYS
*
*     void Square::create_id();
*
* DESCRIPTION
*
*  This function creates sfml text for the name of the board
 *  First, it loads the font file, creates a sfml font object
 *  and finds the appropriate position for the text to be displayed
*/
void Square::create_id()
{
    // create square ID
    m_font = new sf::Font();
    if (!m_font->loadFromFile("Arial Unicode.ttf"))
    {
        cout << "Could not load font." << endl;
    }
    m_text = new sf::Text();
    m_text->setFont(*m_font);
    m_text->setString(*m_name);
    m_text->setCharacterSize(m_space_size / 8.0f);
    m_text->setFillColor(sf::Color::Black);
    m_text->setStyle(sf::Text::Bold);
    m_text->setPosition(m_sprite->getPosition().x + 1, m_sprite->getPosition().y + 1);
}


/*
* NAME
*      CreatePiece - creates a piece to be attached to the square
*
* SYNOPSYS
*
*     void Square::create_piece(string a_piece_filename);
*
 *    a_piece_filename      -> filename of the piece to be created
* DESCRIPTION
*
*  This function creates a piece based on the piece filename passed
 *  and sets its position according to the position of the square sprite
*/
void Square::create_piece(string a_piece_filename)
{
    // create Piece
    if (a_piece_filename == "")
        m_piece = nullptr;
    else
    {
        m_piece = new Piece(a_piece_filename, m_space_size);
        m_piece->set_sprite_position(m_sprite->getPosition().x + (m_space_size / 4), m_sprite->getPosition().y + (m_space_size / 4));
    }
}


/*
* NAME
*      SetNewPiece - removes the old piece and creates a new piece
*
* SYNOPSYS
*
*     void Square::set_new_piece(string a_piece_filename);
*
 *    a_piece_filename      -> filename of the piece to be created
* DESCRIPTION
*
*  This function removes the old piece and creates a new piece
*/
void Square::set_new_piece(string a_piece_filename)
{
    delete_piece();
    create_piece(a_piece_filename);
}


/*
* NAME
*      RemovePiece - returns the piece that currently exists on the square
*
* SYNOPSYS
*
*     Piece* Square::remove_piece();
 *
* DESCRIPTION
*
*  This function returns the piece that currently exists on the square
*/
Piece* Square::remove_piece() {
    // Returns the piece on the square
    if (m_piece != nullptr)
    {
        Piece* temp = m_piece;
        m_piece = nullptr;
        return temp;
    }
    return nullptr;
}



/*
* NAME
*      DeletePiece - removes the piece that currently exists on the square
*
* SYNOPSYS
*
*     void Square::delete_piece();
 *
* DESCRIPTION
*
*  This function removes the piece that currently exists on the square
*/
void Square::delete_piece() {
    if (m_piece)
    {
        auto temp = m_piece;
        m_piece = nullptr;
        delete temp;
    }
}


/*
* NAME
*      GetPieceSprite - returns SFML sprite of the piece that currently exists on the square
*
* SYNOPSYS
*
*     sf::Sprite* Square::get_piece_sprite();
 *
* DESCRIPTION
*
*  This function returns the SFML sprite of the piece that currently exists on the square
*/
sf::Sprite* Square::get_piece_sprite() {
    if (m_piece != nullptr)
    {
        return m_piece->get_sprite();
    }
    return empty_sprite;
}

/*
* NAME
*      ResetColor - resets the colors of the square
*
* SYNOPSYS
*
*     void Square::reset_color();
 *
* DESCRIPTION
*
*  This function resets the color of the current square back to its original color
 *  Useful after the user makes a move, the squares that lit up to show valid moves,
 *  need to be reset back to their original color.
*/
void Square::reset_color() {
    if ((m_i + m_j) % 2 == 0) {
        m_sprite->setFillColor(*default_dark_color);
    }
    else {
        m_sprite->setFillColor(*default_light_color);
    }
}
