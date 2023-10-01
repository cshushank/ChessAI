#include "Piece.h"

/*
* NAME
*      Piece -- creates a piece along with graphics
*
* SYNOPSYS
*
*      Piece::Piece(string a_filename, unsigned int a_space_size);
 *      a_filename      ->  the piece's filename
 *      a_space_size    ->  the height of square on which the piece is to exist
*
* DESCRIPTION
*
*  This function creates SFML texture and sprite for the piece
 *  and initializes color, name, and type by dissecting the filename.
*/
Piece::Piece(string a_filename, unsigned int a_space_size)
{
    m_filename = new string(a_filename);
    auto texture = new sf::Texture();
    if (!texture->loadFromFile(a_filename))
        cout << "Error loading " << a_filename << endl;
    m_sprite = new sf::Sprite(*texture);
    m_sprite->setScale(a_space_size * 0.45f / m_sprite->getLocalBounds().width, a_space_size * 0.45f / m_sprite->getLocalBounds().height);
    m_color = new string(a_filename.substr(0, 5));
    m_name = new string (a_filename.substr(0, a_filename.size() - 4));
    m_type = new string (a_filename.substr(6, a_filename.size() - 10));
}


/*
* NAME
*      Piece -- creates a piece without graphical components
*
* SYNOPSYS
*
*      Piece::Piece(string a_filename)
 *      a_filename      ->  the piece's filename
*
* DESCRIPTION
*
*  This function initializes color, name, and type by dissecting the filename.
 *  Useful for minimax algorithm where a graphical piece is not required.
*/
Piece::Piece(string a_filename)
{
    m_filename = new string(a_filename);
    m_color = new string(a_filename.substr(0, 5));
    m_name = new string(a_filename.substr(0, a_filename.size() - 4));
    m_type = new string(a_filename.substr(6, a_filename.size() - 10));
    m_sprite = nullptr;
}


/*
* NAME
*      ~Piece - destroys the piece
*
* SYNOPSYS
*
*      Piece::~Piece();
*
* DESCRIPTION
*
*  This function deallocates all the memory on the heap
*/
Piece::~Piece()
{
    if (m_sprite)
    {
        delete m_sprite->getTexture();
        delete m_sprite;
    }
    delete m_filename;
    delete m_color;
    delete m_name;
    delete m_type;
}


/*
* NAME
*      GetFilename - returns the filename of the piece
*
* SYNOPSYS
*
*      string Piece::get_filename();
*
* DESCRIPTION
*
*  This function returns the piece filename, the pointer to which is stored in the member function m_filename
*/
string Piece::get_filename()
{
    if (m_filename != nullptr)
        return *m_filename;
    else
        return "";
}
