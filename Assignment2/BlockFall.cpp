#include <fstream>
#include <iostream>
#include <sstream>
#include "BlockFall.h"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    std::ifstream inputFile(input_file);
    // Read each line from the file
    std::string line;
    std::vector<std::vector<std::vector<bool>>> shapes;
    while (std::getline(inputFile, line)) {
        if (line.front() == '[') {
            std::vector<std::vector<bool>> matrix;
            while (true) {
                std::istringstream iss(line);
                std::vector<bool> row;
                for (char ch:line) {
                    if (ch == 48)  {    //48 = 0 in ascii
                        row.push_back(false);
                    } else if (ch == 49) {  //49 = 1 in ascii
                        row.push_back(true);
                    }
                }
                matrix.push_back(row);

                //end of a block
                if(line.back() ==']') {
                    shapes.push_back(matrix);
                    break;
                }
                std::getline(inputFile, line);
            }
        }
    }

    power_up = shapes[shapes.size() - 1];   //power up block
    shapes.erase(shapes.end());    //removing power up block from shapes

    //Creating the blocks
    for(int shape = shapes.size() - 1;shape >= 0 ;shape--) {
        vector<Block> blocks;
        Block initialBlock;
        initialBlock.shape = shapes[shape];
        blocks.push_back(initialBlock);

        //rotating thr shapes
        for (int i = 0 ; i < 3; i++) {
            vector<vector<bool>> rotated = rotate_right(blocks[blocks.size() - 1].shape);
            Block rotatedBlock;
            rotatedBlock.shape = rotated;
            blocks.push_back(rotatedBlock);
        }

        //removing same blocks
        if(blocks[0] == blocks[2]) {
            blocks.erase(blocks.begin() + 2);
            blocks.erase(blocks.begin() + 3);
        } if(blocks[0] == blocks[1]) {
            blocks.erase(blocks.begin() + 1);
        }


        Block* start = nullptr;     //

        //Creating rotated blocks with pointers
        for(int i = 0; i < blocks.size();i++) {
            if(start == NULL) {
                Block* block = new Block;
                block->shape = blocks[i].shape;
                block->right_rotation = block;
                block->left_rotation = block;
                start = block;
                if(shape == shapes.size() - 1) {
                    block->next_block = nullptr;
                } else {
                    block->next_block = initial_block;
                }
                initial_block = block;
                active_rotation = initial_block;
                temp = block;
            } else {
                Block* new_block = new Block;   //creating new_block
                new_block->shape = blocks[i].shape; //define shape
                Block* last = start->left_rotation; //define last node
                new_block->left_rotation = last;    //define new blocks left rotation
                new_block->right_rotation = start;  //define new blocks right rotation
                last->right_rotation = new_block;   //
                start->left_rotation = new_block;   //changing first shapes left rotation
                if(shape == shapes.size() - 1) {
                    new_block->next_block = nullptr;
                } else {
                    new_block->next_block = initial_block;
                }
            }
        }


    }
    inputFile.close();





    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
}

void BlockFall::initialize_grid(const string &input_file) {
    std::ifstream inputFile(input_file);


    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        std::exit(-1);
    }

    std::string line;
    int rowCount = 0;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int value;
        int columnCount = 0;
        while (iss >> value) {
            columnCount++;
            row.push_back(value);
        }
        rowCount++;
        cols = columnCount;
        grid.push_back(row);
    }
    rows = rowCount;
    inputFile.close();
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
}


BlockFall::~BlockFall() {
    while(temp != NULL) {
        Block *kutay = temp->next_block;
        Block *right(temp->right_rotation);
        for (int i = 0; i < 3; i++) {
            if (right == temp) {
                break;
            }
            Block* cop = right->right_rotation;
            delete right;
            right = cop;
        }
        delete temp;
        temp = kutay;
    }
    initial_block = nullptr;
    active_rotation = nullptr;
    temp = nullptr;
    // TODO: Free dynamically allocated memory used for storing game blocks
}

vector<vector<bool>> BlockFall::rotate_right(const vector<vector<bool>> &matrix) {
    vector<vector<bool>> result;
    for(int a = 0 ; a < matrix[0].size(); a++ ) {
        vector<bool> kutay(matrix.size());
        result.push_back(kutay);
    }

    for(int i = 0; i < result.size(); i++) {
        for(int j = 0 ; j < result[0].size() ; j++) {
            result[i][j] = matrix[matrix.size() - 1 - j][i];
        }
    }
    return result;
}
