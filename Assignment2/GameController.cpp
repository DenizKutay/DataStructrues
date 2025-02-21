#include <fstream>
#include <iostream>
#include "GameController.h"
#include "BlockFall.h"

bool GameController::play(BlockFall& game, const string& commands_file){
    std::ifstream inputFile(commands_file);  //bunları değişcen
    std::string line;
    Leaderboard* leaderboard = new Leaderboard;
    LeaderboardEntry* player = new LeaderboardEntry(game.current_score, time(nullptr),game.player_name);
    leaderboard->read_from_file(game.leaderboard_file_name);
    place_block(game);
    int right_count = 0;
    while (std::getline(inputFile, line)) {
        if(line == "MOVE_RIGHT") {
            move(game, right_count, true);
        } else if (line == "MOVE_LEFT") {
            move(game, right_count, false);
        } else if (line == "DROP") {
            if(!drop(game,right_count,leaderboard)) {
                //No more blocks
                if(game.initial_block->next_block == NULL) {
                    std::cout << "\nYOU WIN!\n"
                                 "No more blocks.\n"
                                 "Final grid and score:\n\n";
                    print(game,-1,leaderboard);
                    player->score = game.current_score;
                    leaderboard->insert_new_entry(player);
                    leaderboard->print_leaderboard();
                    leaderboard->write_to_file(game.leaderboard_file_name);
                    delete leaderboard;
                    return true;
                }
                else {
                    std::cout << "\nGAME OVER!\nNext block that couldn't fit:\n";
                    for(int i = 0 ; i < game.initial_block->shape.size();i++) {
                        for (int j = 0 ; j < game.initial_block->shape[0].size();j++) {
                            if(game.initial_block->shape[i][j]) {
                                std::cout << occupiedCellChar;
                            } else {
                                std::cout << unoccupiedCellChar;
                            }
                        }
                        std::cout << "\n";
                    }
                    std::cout << "\nFinal grid and score:\n\n";
                    print(game,-1,leaderboard);
                    player->score = game.current_score;
                    leaderboard->insert_new_entry(player);
                    leaderboard->print_leaderboard();
                    leaderboard->write_to_file(game.leaderboard_file_name);
                    delete leaderboard;
                    return false;
                }

            }
            right_count = 0;
        } else if (line == "PRINT_GRID") {
            print(game,right_count,leaderboard);
        } else if (line == "ROTATE_LEFT") {
            rotate(game,right_count, false);
        } else if (line == "ROTATE_RIGHT") {
            rotate(game,right_count, true);
        } else if (line == "GRAVITY_SWITCH") {
            game.gravity_mode_on = !game.gravity_mode_on;
            gravity(game);
            clearFullRows(game,leaderboard);
        } else {
            std::cout << "Unknown command: " << line << "\n";
        }

    }
    std::cout << "\nGAME FINISHED!\n"
                  "No more commands.\n"
                  "Final grid and score:\n\n";
    print(game,-1,leaderboard);
    player->score = game.current_score;
    leaderboard->insert_new_entry(player);
    leaderboard->print_leaderboard();
    leaderboard->write_to_file(game.leaderboard_file_name);
    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    delete leaderboard;
    return true;
}

bool GameController::drop(BlockFall &game,const int &right_count,const Leaderboard* leaderboard) {

    int down_count = 0;
    bool canDown = true;
    std::vector<int> bottom;
    for (int j = 0; j < game.active_rotation->shape[0].size(); j++) {
        int count = 0;
        for (int i = game.active_rotation->shape.size() - 1 ; i >= 0; i--) {
            if (game.active_rotation->shape[i][j] == 1) {
                bottom.push_back(count);
                break;
            }
            count++;
        }
    }
    //find how much can a block go down
    while (canDown) {
        for (int j = 0; j < game.active_rotation->shape[0].size(); j++) {
            if (down_count + game.active_rotation->shape.size() - 1 == game.rows ||
                game.grid[down_count + game.active_rotation->shape.size() - bottom[j] - 1][j + right_count] == 1) {
                canDown = false;
                break;
            }
        }
        if (canDown) {
            down_count++;
        }
    }
    // Place the block in the grid
    down_count--;
        for (int i = 0; i < game.active_rotation->shape.size(); i++) {
            for (int j = 0; j < game.active_rotation->shape[0].size(); j++) {
                if (game.active_rotation->shape[i][j] == 1) {
                    game.grid[i + down_count][j + right_count] = game.active_rotation->shape[i][j];
                }
            }
        }
    calculate_point(game,down_count);
    gravity(game);
    power_up(game,leaderboard);
    clearFullRows(game,leaderboard);
    if(game.initial_block->next_block == NULL) {

        return false;
    }
    game.initial_block = game.initial_block->next_block;
    game.active_rotation = game.initial_block;
    return place_block(game);
}

void GameController::print(BlockFall &game,const int &right_count,const Leaderboard* leaderboard) {

    bool showActiveRotation = true;
    if(right_count != -2) {
        std::cout << "Score: " << game.current_score <<"\n";
        if(leaderboard->head_leaderboard_entry != NULL) {
            std::cout << "High Score: " << leaderboard->head_leaderboard_entry->score << "\n";
        } else {
            std::cout << "High Score: " << "0" << "\n";
        }
    }
    if(right_count < 0) {
        showActiveRotation = false;
    }else if(game.active_rotation == NULL) {
        showActiveRotation = false;
    }else {
        for(int i = 0;i < game.active_rotation->shape.size();i++) {
            for(int j = 0;j < game.active_rotation->shape[0].size();j++) {
                if(game.active_rotation->shape[i][j]) {
                    game.grid[i][j + right_count] = game.active_rotation->shape[i][j];
                }
            }
        }
    }

    for(int i = 0 ; i < game.rows;i++) {
        for (int j = 0 ; j < game.cols;j++) {
            if(game.grid[i][j] == 0) {
                std::cout << unoccupiedCellChar;    //değişti
            } else {
                std::cout << occupiedCellChar;   //değişti
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    if(right_count != -2) {
        std::cout << "\n";
    }
    if (showActiveRotation) {
        for(int i = 0;i < game.active_rotation->shape.size();i++) {
            for(int j = 0;j < game.active_rotation->shape[0].size();j++) {
                if(game.active_rotation->shape[i][j] == 1) {
                    game.grid[i][j + right_count] = 0;
                }
            }
        }
    }
}

void GameController::rotate(BlockFall &game,const int &right_count,bool rotate_right) {
    //changing rotation
    if(rotate_right) {
        game.active_rotation = game.active_rotation->right_rotation;
    }else {
        game.active_rotation = game.active_rotation->left_rotation;
    }
    bool canRotate = true;
    //check if it can rotate
    for(int i = 0; i < game.active_rotation->shape.size();i++) {
        for (int j = 0 ; j < game.active_rotation->shape[0].size();j++) {
            if(game.active_rotation->shape[i][j] == 1) {
                if(game.grid[i][j + right_count] == 1) {
                    canRotate = false;
                }
            }
        }
    }
    if(!canRotate) {
        if(rotate_right) {
            game.active_rotation = game.active_rotation->left_rotation;
        } else {
            game.active_rotation = game.active_rotation->right_rotation;
        }
    }

}

bool GameController::place_block(BlockFall &game) {
    for(int i = 0 ; i < game.active_rotation->shape.size();i++) {
        for (int j = 0 ; j < game.active_rotation->shape[0].size();j++) {
            //GAME OVER
            if(game.active_rotation->shape[i][j] == 1 && game.grid[i][j] == 1) {
                    return false;
            }
        }
    }
    return true;
}

void GameController::move(BlockFall &game, int &right_count, bool isRight) {
    bool canTranslate = true;
    if(isRight) {
        right_count++;
    } else {
        right_count--;
    }
    //checks if block is out of grid
    if(right_count < 0 || right_count + game.active_rotation->shape[0].size() > game.cols) {
        canTranslate = false;
    }
    else {
        //check if the replacement is full
        for (int i = 0; i < game.active_rotation->shape.size(); i++) {
            if (game.active_rotation->shape[0].size() + right_count + 1 <= game.grid[0].size() - 1) {
                if (game.grid[i][game.active_rotation->shape[0].size() + right_count + 1] == 1) {
                    canTranslate = false;
                }
            }
        }
    }
    if(canTranslate) {
    }
    else {
        if(isRight) {
            right_count--;
        } else {
            right_count++;
        }
    }

}

void GameController::gravity(BlockFall &game) {
    if (game.gravity_mode_on) {
        for (int j = 0; j < game.cols; j++) {
            int boxCount = 0;
            for (int i = 0; i < game.rows; i++) {
                if (game.grid[i][j] == 1) {
                    boxCount++;
                    game.grid[i][j] = 0;
                }
            }
            for (int a = 0; a < boxCount; a++) {
                game.grid[game.grid.size() - 1 - a][j] = 1;
            }
        }
    }

}

void GameController::clearFullRows(BlockFall &game,const Leaderboard* leaderboard) {
    std::vector<int> fullRows;
    for(int i = 0;i<game.grid.size();i++) {
        bool isFull = true;
        for(int j = 0 ; j < game.grid[0].size();j++) {
            if(game.grid[i][j] == 0) {
                isFull = false;
                break;
            }
        }
        if(isFull) {
            fullRows.push_back(i);
        }
    }
    game.current_score += fullRows.size() * game.cols;
    if(!fullRows.empty()) {
        std::cout << "Before clearing:\n";
        print(game,-2,leaderboard);
        for (int row: fullRows) {
            for (int i = row; i > 0; i--) {
                for (int j = 0; j < game.grid[0].size(); j++) {
                    game.grid[0][j] = 0;
                    game.grid[i][j] = game.grid[i - 1][j];
                }
            }
        }

    }

}



void GameController::power_up(BlockFall &game,const Leaderboard* leaderboard) {
    bool power = false;
    for(int i = 0 ; i < game.grid.size() - game.power_up.size() + 1;i++) {
        for(int j = 0; j < game.grid[0].size() - game.power_up.size() + 1;j++) {
            //inside power up
            power = true;
            for(int k = 0;k < game.power_up.size();k++) {
                for(int l = 0; l < game.power_up[0].size();l++) {
                    if(game.power_up[k][l] != game.grid[i + k][j + l]) {
                        power = false;
                        break;
                    }
                }
                if(!power){ break;}
            }
            if(power) { break;}
        }
        if(power){ break;}
    }
    if(power) {
        std::cout<< "Before clearing:\n";
        print(game,-2,leaderboard);
        clear_grid(game);
    }

}

void GameController::clear_grid(BlockFall &game) {
    int boxCount = 0;
    for(int i = 0;i < game.rows;i++) {
        for(int j = 0; j < game.cols;j++) {
            if(game.grid[i][j] == 1 ) {
                boxCount++;
                game.grid[i][j] = 0;
            }
        }
    }
    game.current_score += 1000 + boxCount;
}

void GameController::calculate_point(BlockFall &game, const int down_count) {
    int squareCount = 0;
    for(int i = 0;i < game.active_rotation->shape.size();i++) {
        for(int j = 0;j < game.active_rotation->shape[0].size();j++) {
            if(game.active_rotation->shape[i][j] == 1) {
                squareCount++;
            }
        }
    }
    game.current_score += squareCount * down_count;
}





