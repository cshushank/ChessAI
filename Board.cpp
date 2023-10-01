#include "Board.h"


/*
* NAME
*      Board -- initializes certain aspects of the board
*
* SYNOPSYS
*
*      Board::Board();
*
* DESCRIPTION
*
*  This function initializes graphics independent aspects of the board
 *  Graphics dependent aspects are initialized in Graphics member function
*/
Board::Board() {
    // Create main board
    m_moves = 0;
    play_sequence[0] = "white";
    play_sequence[1] = "black";
    piece_filenames = { {
                                {"white_rook.png", "white_knight.png", "white_bishop.png", "white_queen.png",
                                 "white_king.png", "white_bishop.png", "white_knight.png", "white_rook.png"},
                                {"white_pawn.png", "white_pawn.png", "white_pawn.png", "white_pawn.png",
                                 "white_pawn.png", "white_pawn.png", "white_pawn.png", "white_pawn.png"},
                                {"", "", "", "", "", "", "", ""},
                                {"", "", "", "", "", "", "", ""},
                                {"", "", "", "", "", "", "", ""},
                                {"", "", "", "", "", "", "", ""},
                                {"black_pawn.png", "black_pawn.png", "black_pawn.png", "black_pawn.png",
                                 "black_pawn.png", "black_pawn.png", "black_pawn.png", "black_pawn.png"},
                                {"black_rook.png", "black_knight.png", "black_bishop.png", "black_queen.png",
                                 "black_king.png", "black_bishop.png", "black_knight.png", "black_rook.png"}
                        } };
}


/*
* NAME
*      ~Board -- destructs all squares in the board
*
* SYNOPSYS
*
*      Board::~Board();
* DESCRIPTION
*
*  This function will deallocate the dynamically allocated memory for the board
*/
Board::~Board()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            delete board[row][col];
        }
    }
}

/*
* NAME
*      GetValidMoves - Adds all valid moves for current piece
*
* SYNOPSYS
*
*      void Board::get_valid_moves(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos);
*      pos         ->  The position of the piece, i.e., tuple of row, col
*      v           ->  The valid moves set in which the valid moves are to be added
*
* DESCRIPTION
*
*  This function will add the valid moves of current piece, depending on which piece the pos has.
*  Calls other functions to get valid moves for pawn, rook, bishop, and queen.
 *  Finds the valid moves for king and knight in its own code.
 *  For king, checks all the squares in the vicinity
 *  For knight, using an array, checks the potentially valid positions
*/
void Board::get_valid_moves(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos) {
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);

    if (!board[row][col]->has_piece())
        return;

    Piece* piece = board[row][col]->get_piece();

    if (piece->get_name() == "white_pawn")
        deal_pawns(v, pos, true);

    else if (piece->get_name() == "black_pawn")
        deal_pawns(v, pos, false);

    else if (piece->get_name() == "white_rook" || piece->get_name() == "black_rook")
        deal_rook(v, pos);

    else if (piece->get_name() == "white_bishop" || piece->get_name() == "black_bishop")
        deal_bishop(v, pos);

    else if (piece->get_name() == "white_knight" || piece->get_name() == "black_knight")
    {
        string piece_color = board[row][col]->get_piece()->get_color();
        int possible_moves[8][2] = {
                {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
                {2, 1}, {1, 2}, {-1, 2}, {-2, 1}
        };

        for (auto &move: possible_moves)
        {
            int potential_row = row + move[0];
            int potential_col = col + move[1];

            std::tuple<int, int> potential_pos = std::make_tuple(potential_row, potential_col);

            if (is_within(potential_pos))
            {
                if (board[potential_row][potential_col]->has_piece())
                    get_valid_on_piece(piece_color, potential_pos, v);
                else
                    v.insert(potential_pos);
            }
        }
    }
    else if (piece->get_name() == "white_queen" || piece->get_name() == "black_queen")
    {
        // Queen's potential move is the combination of the rooks and bishop, if they were to exist at the same position
        deal_rook(v, pos);
        deal_bishop(v, pos);
    }

    else if (piece->get_name() == "white_king" || piece->get_name() == "black_king")
    {
        string piece_color = board[row][col]->get_piece()->get_color();
        int potential_row;
        int potential_col;
        std::tuple<int, int> potential_pos;
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                // skip the king's own square
                if (i == 0 && j == 0)
                    continue;
                potential_row = row + i;
                potential_col = col + j;
                potential_pos = std::make_tuple(potential_row, potential_col);
                if (!is_within(potential_pos))
                    continue;
                if (board[potential_row][potential_col]->has_piece())
                    get_valid_on_piece(piece_color, potential_pos, v);
                else
                    v.insert(potential_pos);
            }
        }
    }
}

/*
* NAME
*      GetValidOnPiece - helper for GetValidMoves, adds the square containing opposing piece to the set of valid moves
*
* SYNOPSYS
*
*      void Board::get_valid_on_piece(string piece_color, std::tuple<int, int>& oppose_pos, set<std::tuple<int, int>>& v);
*      piece_color         ->  The piece's color whose valid moves is to be found
*      v                   ->  The valid moves set in which a potential valid move is to be added
*
* DESCRIPTION
*
*  This function will check if the last piece on the valid move is an opposing piece
*  If so, add to the valid moves, the square on which the opposing piece exists
 *  If it evaluates that the square being inspected is not opposing, it does not change the valids set.
*/
void Board::get_valid_on_piece(string piece_color, std::tuple<int, int>& oppose_pos, set<std::tuple<int, int>>& v) {

    int oppose_row = std::get<0>(oppose_pos);
    int oppose_col = std::get<1>(oppose_pos);
    bool same_color = (piece_color == board[oppose_row][oppose_col]->get_piece()->get_color());
    if (!same_color)
    {
        std::tuple<int, int> position = std::make_tuple(oppose_row, oppose_col);
        v.insert(oppose_pos);
    }
}


/*
* NAME
*      IsWithin - helper for GetValidMoves, checks if the position passed is within the bounds of the board
*
* SYNOPSYS
*
*      void Board::is_within(std::tuple<int, int>& pos);
*      pos                   ->  position on the board to be checked, a tuple of row, column
*
* DESCRIPTION
*
*  This function will check if the position passed is within the board
*/
bool Board::is_within(std::tuple<int, int>& pos) {
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);
    if (row < 0 || col < 0 || row > 7 || col > 7)
        return false;
    return true;
}

/*
* NAME
*      DealPawns - helper for GetValidMoves, finds and adds the valid moves for a pawn
*
* SYNOPSYS
*
*      void Board::deal_pawns(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos, bool goes_up);
*      v                   ->  set of valid moves
 *      pos                -> tuple of row, col -- current square on which the piece resides
 *     goes_up             -> bool that stores white or black piece
*
* DESCRIPTION
*
*  This function adds the valid moves for a pawn
 *  Depending on which pawn-- black or white, it adds the relevant positions (tuples of row, col) upward or downward
*/
void Board::deal_pawns(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos, bool goes_up) {
    int multiplier = (goes_up) ? 1 : -1;
    int row = std::get<0>(pos), col = get<1>(pos);

    string piece_color = board[row][col]->get_piece()->get_color();

    // move forward
    std::tuple<int, int> potential_pos = std::make_tuple(row + multiplier * 1, col);
    if (is_within(potential_pos) && !board[row + multiplier * 1][col]->has_piece())
        v.insert(potential_pos);

    // move foward-sideways if opposing key
    potential_pos = std::make_tuple(row + multiplier * 1, col + 1);
    if (is_within(potential_pos) && board[row + multiplier * 1][col + 1]->has_piece())
        get_valid_on_piece(piece_color, potential_pos, v);

    potential_pos = std::make_tuple(row + multiplier * 1, col - 1);
    if (is_within(potential_pos) && board[row + multiplier * 1][col - 1]->has_piece())
        get_valid_on_piece(piece_color, potential_pos, v);

    // first move
    int probable_row = (7 + multiplier * 1) % 7;
    potential_pos = std::make_tuple(probable_row + multiplier * 2, col);
    if (row == probable_row && !board[probable_row + multiplier * 2][col]->has_piece() && !board[probable_row + multiplier * 1][col]->has_piece())
        v.insert(potential_pos);
}


/*
* NAME
*      DealRook - helper for GetValidMoves, finds and adds the valid moves for a rook
*
* SYNOPSYS
*
*      void Board::deal_rook(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos);
*      v                   ->  set of valid moves
 *      pos                -> tuple of row, col -- current square on which the piece resides
*
* DESCRIPTION
*
*  This function adds the valid moves for a rook.
 * Using up, down, left, and right potential nearest moves, calls ContinueUntilPiece to get all the valid moves in each direction.
*/
void Board::deal_rook(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos)
{
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);

    string piece_color = board[row][col]->get_piece()->get_color();

    int move_down = row - 1, move_up = row + 1, move_left = col - 1, move_right = col + 1;

    // get all the valid squares moving down
    std::tuple<int, int> potential_pos = std::make_tuple(move_down, col);
    continue_until_piece(piece_color, v, pos, potential_pos);

    // get all the valid squares moving down
    potential_pos = std::make_tuple(move_up, col);
    continue_until_piece(piece_color, v, pos, potential_pos);

    // get all the valid squares moving left
    potential_pos = std::make_tuple(row, move_left);
    continue_until_piece(piece_color, v, pos, potential_pos);

    // get all the valid squares moving right
    potential_pos = std::make_tuple(row, move_right);
    continue_until_piece(piece_color, v, pos, potential_pos);
}


/*
* NAME
*      DealBishop - helper for GetValidMoves, finds and adds the valid moves for a bishop
*
* SYNOPSYS
*
*      void Board::deal_bishop(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos);
*      v                   ->  set of valid moves
 *      pos                -> tuple of row, col -- current square on which the piece resides
*
* DESCRIPTION
*
*  This function adds the valid moves for a rook.
 * Using potential nearest moves, calls ContinueUntilPiece to get all the valid moves in each direction.
*/
void Board::deal_bishop(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos)
{
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);

    string piece_color = board[row][col]->get_piece()->get_color();

    int row_up = row + 1;
    int col_right = col + 1;
    std::tuple<int, int> potential_pos = std::make_tuple(row_up, col_right);
    continue_until_piece(piece_color, v, pos, potential_pos);

    int row_down = row - 1;
    int col_left = col - 1;
    potential_pos = std::make_tuple(row_down, col_left);
    continue_until_piece(piece_color, v, pos, potential_pos);

    row_up = row + 1;
    col_left = col - 1;
    potential_pos = std::make_tuple(row_up, col_left);
    continue_until_piece(piece_color, v, pos, potential_pos);

    row_down = row - 1;
    col_right = col + 1;
    potential_pos = std::make_tuple(row_down, col_right);
    continue_until_piece(piece_color, v, pos, potential_pos);
}

/*
* NAME
*      ContinueUntilPiece - helper for deal_rook, deal_bishop, continues to add valid moves until there's a piece
*
* SYNOPSYS
*
*      void Board::continue_until_piece(string piece_color, set<std::tuple<int, int>>& v, std::tuple<int, int>& pos, std::tuple<int, int>& potential_pos)

*      piece_color          ->  the color of the current piece
 *      v                   -> the set of valid moves
 *      pos                 -> tuple of row, col -- square which we know is valid
 *     potential_pos        -> potential valid move
*
* DESCRIPTION
*
*  This function recursively adds the valid moves for any piece which has a recurring pattern of valid moves
 *  For example, rook, bishop, and by extension, queen
 *  Based on the change between pos and potential_pos,
 *  in the next recursion, it finds the next potential_pos.
 *  Continues until there is a piece, in which case, checks to see if the piece is opposing or not, adds to valid accordingly
*/
void Board::continue_until_piece(string piece_color, set<std::tuple<int, int>>& v, std::tuple<int, int>& pos, std::tuple<int, int>& potential_pos)
{
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);
    int potential_row = std::get<0>(potential_pos);
    int potential_col = std::get<1>(potential_pos);

    if (!is_within(potential_pos))
        return;

    if (board[potential_row][potential_col]->has_piece())
    {
        get_valid_on_piece(piece_color, potential_pos, v);
        return;
    }
    v.insert(potential_pos);
    std::tuple<int, int> next_potential_pos = make_tuple(potential_row + (potential_row - row), potential_col + (potential_col - col));
    continue_until_piece(piece_color, v, potential_pos, next_potential_pos);
}



/*
* NAME
*      Graphics - Main function, runs the game.
*
* SYNOPSYS
*
*      void Board::graphics();
*
* DESCRIPTION
*
*  This function calls ChooseSide, initializes the main board and the main window,
 *  and runs the complete graphical operation of the board including non-graphical aspects
 *  such as calling the minimax function.
*/
void Board::graphics()
{
    // get screen parameters and calculate square parameters accordingly
    screen_width = sf::VideoMode::getDesktopMode().width;
    screen_height = sf::VideoMode::getDesktopMode().height;
    square_width = screen_width / 8;
    square_height = screen_height / 9;

    // get side
    choose_side();

    // render main window
    window = new sf::RenderWindow(sf::VideoMode(8 * square_height, 8 * square_height), "Chess", sf::Style::Close | sf::Style::Resize);

    // create squares for the main board
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            board[row][col] = new Square(row, col, square_height, piece_filenames);
        }
    }
    // variables for the game
    set<std::tuple<int, int>> valids;
    int prev_row;
    int prev_col;
    bool move_piece = false;
    while (window->isOpen())
    {
        // if this is the first move and ai plays white
        if (user_side != play_sequence[(m_moves % 2)] && m_moves == 0)
        {
            smart_guy(square_height);
            if (is_terminal()){
                window->close();
                return;
            }
            m_moves++;
            continue;
        }
        // check for main window event
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == event.Closed) {
                window->close();
            }
            // Upon mouse click on a square

            // If this is the first click,
            // find the square that was clicked and if it has a piece, light up squares that are valid moves for it

            // if the board had a click before the current click,
            // see if the new click was on a valid square and move the piece to it

            // if it was not on a valid square, reset the color of squares that lit up before
            if (event.type == sf::Event::MouseButtonPressed) {
                // if there was a click before this
                if (move_piece)
                {
                    auto click_pos = on_click_get_square(event);
                    int row = std::get<0>(click_pos);
                    int col = std::get<1>(click_pos);

                    auto it = std::find(valids.begin(), valids.end(), click_pos);
                    if (it != valids.end())
                    {
                        // if the click is on a valid square
                        if (board[row][col]->has_piece())
                        {
                            // check for victory
                            string piece_name = board[row][col]->get_piece()->get_name();
                            if (piece_name == "white_king" || piece_name == "black_king")
                            {
                                window->close();
                                return;
                            }
                        }
                        Piece *temp = board[row][col]->get_piece();
                        board[row][col]->set_piece(board[prev_row][prev_col]->remove_piece());
                        delete temp;
                        if ((row == 0 || row == 7) && board[row][col]->get_piece()->get_type() == "pawn")
                            change_pawn_on_last(click_pos);
                        m_moves++;

                        // call ai after user makes a move
                        smart_guy(square_height);
                        if (is_terminal()){
                            window->close();
                            return;
                        }
                        m_moves++;
                    }
                    for (auto pos : valids)
                    {
                        int valid_row = get<0>(pos);
                        int valid_col = get<1>(pos);
                        board[valid_row][valid_col]->reset_color();
                    }
                    board[prev_row][prev_col]->reset_color();
                    valids.clear();
                    move_piece = false;
                    continue;
                }
                // if this is the first click, i.e., no previous clicks
                auto click_pos = on_click_get_square(event);
                int row = std::get<0>(click_pos);
                int col = std::get<1>(click_pos);

                if (board[row][col]->has_piece() && user_side == board[row][col]->get_piece()->get_color())
                {
                    get_valid_moves(valids, click_pos);
                    if (valids.empty())
                        board[row][col]->set_invalid_color();
                    else
                    {
                        for (auto pos : valids)
                        {
                            int valid_row = get<0>(pos);
                            int valid_col = get<1>(pos);
                            board[valid_row][valid_col]->set_valid_color();
                        }
                    }
                    prev_row = row;
                    prev_col = col;
                    move_piece = true;
                }
            }
        }

        // continuously draw the board until an event occurs
        window->clear();
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Square* current_square = board[i][j];
                window->draw(*(current_square->get_sprite()));
                sf::Text curr_square_name = *(current_square->get_id());
                window->draw(curr_square_name);
                if (current_square->has_piece())
                {
                    auto square_piece = current_square->get_piece();
                    auto piece_sprite = square_piece->get_sprite();
                    piece_sprite->setPosition(current_square->get_sprite()->getPosition().x + (square_height / 4), current_square->get_sprite()->getPosition().y + (square_height / 4));
                    window->draw(*piece_sprite);
                }
            }
        }
        window->display();
    }
}


/*
* NAME
*      OnClickGetSquare - Helper function for Graphics, returns the square that was clicked.
*
* SYNOPSYS
*
*      std::tuple<int, int> Board::on_click_get_square(sf::Event& event);
 *
 *
*      event -> the SFML event on the board
*
* DESCRIPTION
*
*  This function get event when there's an mouse click event on the main board
 *  and returns the row, col of the clicked square
*/
std::tuple<int, int> Board::on_click_get_square(sf::Event& event)
{
    int i, j;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (board[i][j]->get_sprite()->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
            {
                std::tuple<int, int> click_square = std::make_tuple(i, j);
                return click_square;
            }
        }
    }
}


/*
* NAME
*      ChangePawnOnLast - Upon reaching last square, gives the option to a user to change pieces graphically
*
* SYNOPSYS
*
*      void Board::change_pawn_on_last(std::tuple<int, int>& pos);
 *      pos                -> tuple of row, col -- current square on which the piece resides
*
* DESCRIPTION
*
*  This function creates a new board and a new SFML window to let the user choose a new piece for their pawn
 *  at the end of its journey.
 *  It displays all the piece in the new window
 *  When mouse is clicked on a square with a valid piece, it moves the piece to the main board
 *  in the position where  the pawn resided -- the pawn that reached the last square.
*/
void Board::change_pawn_on_last(std::tuple<int, int>& pos)
{
    int row = std::get<0>(pos);
    int col = std::get<1>(pos);

    // get left-top-most position of the main window on the user screen
    auto position = window->getPosition();
    option_window = new sf::RenderWindow(sf::VideoMode(8 * square_height, square_height), "Choose the switch!", sf::Style::Close | sf::Style::Resize);
    if (board[row][col]->get_piece()->get_name() == "black_pawn")
    {
        position.y += 7 * square_height;
        piece_filenames = { {
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"black_rook.png", "black_knight.png", "black_bishop.png", "black_queen.png",
                                     "", "black_bishop.png", "black_knight.png", "black_rook.png"}
                            } };
    }
    else if (board[row][col]->get_piece()->get_name() == "white_pawn")
    {
        piece_filenames = { {
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"", "", "", "", "", "", "", ""},
                                    {"white_rook.png", "white_knight.png", "white_bishop.png", "white_queen.png",
                                     "", "white_bishop.png", "white_knight.png", "white_rook.png"}
                            } };
    }
    // set the position of the option windows exactly at the position of the main window
    option_window->setPosition(position);

    // create squares for the option window
    for (int column = 0; column < 8; column++)
        option_board[column] = new Square(7, column, square_height, piece_filenames);

    // while the window is open
    while (option_window->isOpen())
    {
        sf::Event event;
        // wait for an event
        while (option_window->pollEvent(event))
        {
            // if the user closes the window, close the window
            if (event.type == sf::Event::Closed)
                option_window->close();

            // if there is a mouse click, find on which square it occured
            if (event.type == sf::Event::MouseButtonPressed)
            {
                for (int column = 0; column < 8; column++)
                {
                    // if mouse click in the current column, move the piece to the main board in the position where the old pawn resided.
                    if (option_board[column]->get_sprite()->getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
                    {
                        if (option_board[column]->has_piece()) {
                            board[row][col]->set_new_piece(option_board[column]->get_piece()->get_filename());
                            option_window->close();
                        }
                    }
                }
            }
        }
        // Continuously draw and refresh the board until an event occurs
        option_window->clear();
        for (int col = 0; col < 8; col++)
        {
            option_window->draw(*(option_board[col]->get_sprite()));
            option_window->draw(*(option_board[col]->get_id()));
            option_window->draw(*(option_board[col]->get_piece_sprite()));
        }
        option_window->display();
    }
    for (int column = 0; column < 8; column++)
        delete option_board[column];
    delete option_window;
}

/*
* NAME
*      ChooseSide - Give the user the choice of which side to play
*
* SYNOPSYS
*
*      void Board::choose_side();
*
* DESCRIPTION
*
*  This function creates a new board and a new SFML window to let the user choose a side.
 *  It displays two pieces in the new window--both kings.
 *  Depending on which square the user clicks, it changes the user_side member function.
*/
void Board::choose_side()
{
    side_window = new sf::RenderWindow(sf::VideoMode(2 * square_height, square_height), "Choose a side!", sf::Style::Close | sf::Style::Resize);

    for (int i = 0; i < 2; i++)
    {
        side_board[i] = new Square(7, i, square_height, piece_filenames);
    }
    side_board[0]->set_new_piece("white_king.png");
    side_board[1]->set_new_piece("black_king.png");
    while (side_window->isOpen())
    {
        sf::Event event;
        while (side_window->pollEvent(event))
        {
            if (event.type == event.Closed)
                side_window->close();
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 2; i++) {
                    if (side_board[i]->get_sprite()->getGlobalBounds().contains(
                            sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        user_side = side_board[i]->get_piece()->get_color();
                        side_window->close();
                    }
                }
            }
        }
        side_window->clear();
        for (int i = 0; i < 2; i++)
        {
            auto current_square = side_board[i];
            side_window->draw(*(current_square->get_sprite()));
            if (side_board[i]->has_piece())
            {
                auto square_piece = current_square->get_piece();
                auto piece_sprite = square_piece->get_sprite();
                piece_sprite->setPosition(current_square->get_sprite()->getPosition().x + (square_height / 4), current_square->get_sprite()->getPosition().y + (square_height / 4));
                side_window->draw(*piece_sprite);
            }
        }
        side_window->display();
    }
}


/*
* NAME
*      GetAllValids - helper for minimax, finds all valids for a side
*
* SYNOPSYS
*
*      void Board::get_all_valids(set< std::tuple<std::tuple<int, int>, std::tuple<int, int>>> &all_valids, string side);
*      all_valids                   ->  set of valid moves for all pieces of the side
 *      side                        -> string containing the color of the side AI wants to find the valids for
*
* DESCRIPTION
*
*  This function iterates through the board, calling GetValidMoves on each piece
 *  that is of the same color as the side
 *  Adds all of them to the all_valids set
*/
void Board::get_all_valids(set< std::tuple<std::tuple<int, int>, std::tuple<int, int>>> &all_valids, string side)
{
    std::tuple<std::tuple<int, int>, std::tuple<int, int>> position_valid_pair;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j]->has_piece() && board[i][j]->get_piece()->get_color() == side)
            {
                set<std::tuple<int, int>> valids;
                std::tuple<int, int> pos = std::make_tuple(i, j);
                get_valid_moves(valids, pos);
                for (const auto& valid : valids)
                {
                    position_valid_pair = std::make_tuple(pos, valid);
                    all_valids.insert(position_valid_pair);
                }
            }
        }
    }
    return;
}

/*
* NAME
*      IsTerminal - checks to see if the board has reached a terminal state
*
* SYNOPSYS
*
*      bool Board::is_terminal();
*
* DESCRIPTION
*
*  This function returns true if one of the king has fallen,
 *  effectively helping in terminating the function.
*/
bool Board::is_terminal()
{
    int num_kings = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j]->has_piece() && board[i][j]->get_piece()->get_type() == "king")
                num_kings++;
        }
    }
    if (num_kings < 2)
        return true;
    return false;
}


/*
* NAME
*      SmartGuy - this function starts the minimax.
*
* SYNOPSYS
*
*      void Board::smart_guy(unsigned int a_space_size);
 *
 *      a_space_size -> the square size
*
* DESCRIPTION
*
*  This function starts the minimax code, passing appropriate values for alpha-beta pruning and number of steps so far.
 *  Depending on the new best_action variable
 *  (tuple of current square of the piece which is calculated to be moved by minimax for the best outcome
 *             and the best valid square for the piece)
 *  that minimax changes, it moves the appropriate piece to the best position
*/
void Board::smart_guy(unsigned int a_space_size)
{
    int steps = 0;
    if (user_side == "black")
    {
        int alpha_comp_util_max = std::numeric_limits<int>::max();
        maximize(steps, alpha_comp_util_max);
    }
    else
    {
        int alpha_comp_util_min = std::numeric_limits<int>::min();
        minimize(steps, alpha_comp_util_min);
    }
    auto pos = std::get<0>(best_action);
    int p = std::get<0>(pos);
    int q = std::get<1>(pos);
    auto valid = std::get<1>(best_action);
    int v = std::get<0>(valid);
    int w = std::get<1>(valid);

    Piece *temp = board[v][w]->get_piece();
    board[v][w]->set_piece(board[p][q]->remove_piece());
    if ((v == 0 || v == 7) && board[v][w]->get_piece()->get_type() == "pawn")
    {
        // create a new queen and add it in place of pawn
        string piece_filename = "";
        if (board[v][w]->get_piece()->get_color() == "white")
            piece_filename = "white_queen.png";
        else
            piece_filename = "black_queen.png";
        Piece *piece_change = new Piece(piece_filename, a_space_size);
        Piece *temp2 = board[v][w]->get_piece();
        // add the queen
        board[v][w]->set_piece(piece_change);
        delete temp2;
    }
    delete temp;
}


/*
* NAME
*      Minimize - this function is recursive and attempts to minimize the utility of the board for the black side
*
* SYNOPSYS
*
*      int Board::minimize(int &steps, int alpha_comp_util);
 *
 *     steps            -> the depth of minimax tree
 *     alpha_comp_util  -> alpha_beta value
*
* DESCRIPTION
*
*  Checks if the board is terminal, if so, calls the evaluation function SmartGuysHelper.
 * If the board is not terminal, finds all the valid move for black side,
 * For each valid move
        * makes the move on the same board, and passes to maximize
        * to see what the utility maximize returns for current valid move or action.
        * Changes the board back to its original state.
* Finds the action with lowest utility saving it to the Board member function best_action.
*/
int Board::minimize(int &steps, int alpha_comp_util)
{
    steps++;
    if (is_terminal() || steps > max_steps)
    {
        steps--;
        return smart_guys_helper();
    }
    set< std::tuple<std::tuple<int, int>, std::tuple<int, int>>> all_valids;
    std::tuple<std::tuple<int, int>, std::tuple<int, int>> lowest_action;
    get_all_valids(all_valids, "black");

    int utility = std::numeric_limits<int>::max();
    int temp_utility;
    Piece *hold_pos_piece = nullptr;
    Piece *hold_val_piece = nullptr;
    Piece *piece_change = nullptr;

    for (auto& pos_valid : all_valids)
    {
        auto pos = std::get<0>(pos_valid);
        int p = std::get<0>(pos);
        int q = std::get<1>(pos);
        auto valid = std::get<1>(pos_valid);
        int v = std::get<0>(valid);
        int w = std::get<1>(valid);

        hold_pos_piece = board[p][q]->get_piece();
        hold_val_piece = board[v][w]->get_piece();
        board[v][w]->set_piece(board[p][q]->remove_piece());
        if ((v == 0) && board[v][w]->get_piece()->get_type() == "pawn")
        {
            piece_change = new Piece("black_queen.png");
            board[v][w]->set_piece(piece_change);
        }
        temp_utility = std::min(utility, maximize(steps, utility));
        if (temp_utility < utility)
        {
            utility = temp_utility;
            lowest_action = pos_valid;
        }
        board[p][q]->set_piece(hold_pos_piece);
        board[v][w]->set_piece(hold_val_piece);
        delete piece_change;
        piece_change = nullptr;
        hold_pos_piece = nullptr;
        hold_val_piece = nullptr;
        if (utility <= alpha_comp_util)
            break;
    }
    if (steps == 1)
    {
        best_action = lowest_action;
    }
    steps--;
    return utility;
}


/*
* NAME
*      Maximize - this function is recursive and attempts to maximize the utility of the board for the white side
*
* SYNOPSYS
*
*      int Board::maximize(int &steps, int alpha_comp_util);
 *
 *     steps            -> the depth of minimax tree
 *     alpha_comp_util  -> alpha_beta value
*
* DESCRIPTION
*
*  Checks if the board is terminal, if so, calls the evaluation function SmartGuysHelper.
 * If the board is not terminal, finds all the valid move for white side,
 * For each valid move
        * makes the move on the same board, and passes to minimize
        * to see what the utility minimize returns for current valid move or action.
        * Changes the board back to its original state.
* Finds the action with highest utility saving it to the Board member function best_action.
*/
int Board::maximize(int &steps, int alpha_comp_util)
{
    steps++;
    if (is_terminal() || steps > max_steps)
    {
        steps--;
        return smart_guys_helper();
    }
    set< std::tuple<std::tuple<int, int>, std::tuple<int, int>>> all_valids;
    std::tuple<std::tuple<int, int>, std::tuple<int, int>> highest_action;
    get_all_valids(all_valids, "white");

    int utility = std::numeric_limits<int>::min();
    int temp_utility;
    Piece *hold_pos_piece = nullptr;
    Piece *hold_val_piece = nullptr;
    Piece *piece_change = nullptr;
    for (auto& pos_valid : all_valids)
    {
        auto pos = std::get<0>(pos_valid);
        int p = std::get<0>(pos);
        int q = std::get<1>(pos);
        auto valid = std::get<1>(pos_valid);
        int v = std::get<0>(valid);
        int w = std::get<1>(valid);

        hold_pos_piece = board[p][q]->get_piece();
        hold_val_piece = board[v][w]->get_piece();
        board[v][w]->set_piece(board[p][q]->remove_piece());
        if ((v == 7) && board[v][w]->get_piece()->get_type() == "pawn")
        {
            piece_change = new Piece("white_queen.png");
            board[v][w]->set_piece(piece_change);
        }

        temp_utility = std::max(utility, minimize(steps, utility));
        if (temp_utility > utility)
        {
            utility = temp_utility;
            highest_action = pos_valid;
        }
        board[p][q]->set_piece(hold_pos_piece);
        board[v][w]->set_piece(hold_val_piece);
        delete piece_change;
        piece_change = nullptr;
        hold_pos_piece = nullptr;
        hold_val_piece = nullptr;
        if (utility >= alpha_comp_util)
            break;
    }
    if (steps == 1)
        best_action = highest_action;
    steps--;
    return utility;
}


/*
* NAME
*      SmartGuysHelper - this function is the evaluation function that returns the utility of the board
*
* SYNOPSYS
*
*      int Board::smart_guys_helper();
 *
*
* DESCRIPTION
*   // Returns utility of the board
    // Calculates utility by assigning values to each pieces (material value)
    // For white pieces, positive values are assigned
    // For black pieces, equal negative values are assigned
    // Hence, for white side, it would be optimal to have least important black pieces and most important white pieces
    // vice-versa for black pieces.


    // In addition, takes into account the mobility of the pieces.
    // Mobilities' weight increases as the game progresses
*/
int Board::smart_guys_helper()
{


    // assign values to each pieces
    std::unordered_map<std::string, int> key_values;
    key_values["pawn"] = 1;
    key_values["knight"] = 3;
    key_values["bishop"] = 3;
    key_values["rook"] = 5;
    key_values["queen"] = 9;
    key_values["king"] = 100;

    // count pieces to determine game start, mid game, or end game
    int num_pieces = count_pieces();
    float mobility = 0.0;
    if (num_pieces <= 11)
        mobility = 0.6;
    else if (num_pieces <= 22)
        mobility = 0.3;
    else
        mobility = 0.1;

    int utility = 0;
    int current_value;
    string current_color;
    string current_type;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (board[row][col]->has_piece())
            {
                // material value
                current_color = board[row][col]->get_piece()->get_color();
                current_type = board[row][col]->get_piece()->get_type();
                current_value = (current_color == "white") ? key_values[current_type] : (-1) * key_values[current_type];
                utility += current_value;

                // mobility except for king
                if (current_type != "king"){
                    set<std::tuple<int, int>> valids;
                    std::tuple<int, int> pos = std::make_tuple(row, col);
                    get_valid_moves(valids, pos);
                    current_value = (current_color == "white") ? valids.size() : (-1) * valids.size();
                    utility += (mobility * current_value);
                }
            }
        }
    }
    return utility;
}


/*
* NAME
*      CountPieces - this function counts the remaining pieces on the board
*
* SYNOPSYS
*
*      int Board::count_pieces();
 *
*
* DESCRIPTION
*   Loops through the board and counts the number of pieces
*/
int Board::count_pieces() {
    int num_pieces = 0;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++) {
            if (board[row][col]->has_piece())
                num_pieces++;
        }
    }
    return num_pieces;
}

