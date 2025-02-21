#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
private:
    void move(BlockFall& game, int &right_count, bool isRight);
    bool drop(BlockFall& game,const int &right_count,const Leaderboard* leaderboard);
    void print(BlockFall& game,const int &right_count,const Leaderboard* leaderboard);
    void rotate(BlockFall& game,const int &right_count,bool rotate_right);
    bool place_block(BlockFall& game);
    void gravity(BlockFall& game);
    void clearFullRows(BlockFall& game,const Leaderboard* leaderboard);
    void power_up(BlockFall& game,const Leaderboard* leaderboard);
    void clear_grid(BlockFall& game);
    void calculate_point(BlockFall& game,int down_count);
};


#endif //PA2_GAMECONTROLLER_H
