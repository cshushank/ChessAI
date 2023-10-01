#pragma once

#include <iostream>
#include <string>
#include <array>
#include <tuple>
#include <vector>
#include "Piece.h"
#include "Square.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <set>
#include <unordered_map>
#include <thread>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

class Board
{
private:
    unsigned int m_moves;                                       // Number of moves made
    string play_sequence[2];                                    // Play sequence array to ensure white plays first
    array<array<string, 8>, 8> piece_filenames;                 // Array of array  of strings containing piece filenames
    Square* board[8][8];                                        // Main board
    Square* option_board[8];                                    // Option board used to display options when pawn reaches last square
    Square* side_board[2];                                      // Side board to choose which side to play
    string user_side;                                           // Holds the user selection of the side, white or black

    // variables to store size of screen and square
    unsigned int screen_width;
    unsigned int screen_height;
    unsigned int square_height;
    unsigned int square_width;

    // SFML windows for main game,
    //              for choosing sides, and
    //              for choosing option once the pawn reaches last square
    sf::RenderWindow* window;
    sf::RenderWindow* side_window;
    sf::RenderWindow* option_window;

    const int max_steps = 4;                                            // Max number of depth for minimax
    std::tuple<std::tuple<int, int>, std::tuple<int, int>> best_action; // Best action returned by minimax
                                                                        // tuple of one piece and the best valid move for it
                                                                        // essentially, the best move that the ai can make

    // Return the main board
    Square* (*get_board())[8][8] { return &board; }

    // Check if the last piece on the valid move is an opposing piece
    // If so, add to the valid moves, the square on which the opposing piece exists
    void get_valid_on_piece(string piece_color, std::tuple<int, int>& oppose_pos, set<std::tuple<int, int>>& v);

    // Run graphics when the piece reaches last square
    // Give option to change pieces
    void change_pawn_on_last(std::tuple<int, int>& pos);

    // Run graphics to give the user an option to choose side
    void choose_side();

    // Get valid move for a piece
    void get_valid_moves(set<tuple<int, int>>& v, tuple<int, int>& pos);

    /*
     * Helper functions for get_valid_moves start
     */

    // Ensure the potential valid move is within the board
    bool is_within(tuple<int, int>& pos);

    // Help find the valid moves for a Pawn
    void deal_pawns(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos, bool goes_up);

    // Help find the valid move for pieces with continuous valid moves
    void continue_until_piece(string piece_color, set<std::tuple<int, int>>& v, std::tuple<int, int>& pos, std::tuple<int, int>& potential_pos);

    // Help find the valid moves for a rook
    void deal_rook(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos);

    // Help find the valid moves for a Bishop
    void deal_bishop(set<std::tuple<int, int>>& v, std::tuple<int, int>& pos);

    /*
     * Helper functions for get_valid_moves end
     */

    // Return the square on which user clicked
    std::tuple<int, int> on_click_get_square(sf::Event& event);

    // Minimax originator
    void smart_guy(unsigned int a_space_size);

    // Evaluation function: returns the utility of the board
    int smart_guys_helper();

    // Return all valid moves for a side
    void get_all_valids(set< std::tuple<std::tuple<int, int>, std::tuple<int, int>>>& all_valids, string side);

    // Check if the board has reached a terminal state
    bool is_terminal();

    // Utility minimizer
    int minimize(int &steps, int alpha_comp_util);

    // Utility maximizer
    int maximize(int &steps, int alpha_comp_util);

    // Count pieces: helper for evaluation function
    int count_pieces();

public:
    Board();
    void graphics();
    ~Board();
};

