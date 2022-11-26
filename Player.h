#ifndef DATA_STRUCT_EX1_PLAYER_H
#define DATA_STRUCT_EX1_PLAYER_H

#include "Team.h"
#include "AvlTree.h"
class Player {
private:
    int player_ID; // the player's ID
    int goals_scored; // the number of goals the player has scored
    int cards; // the number of cards the player has collected
    int games_played; // the number of games played for the team
    bool is_goalkeeper; // true if the player is goalkeeper false otherwise
    int team_ID; // the ID of the team the player belongs to
    Team *team_pointer; // the team the player belongs to
    Player *global_left_closest; // the player that is closest to this player from the left
    Player *global_right_closest; // the player that is closest to this player from the right
public:
    Player(int player_ID,int goals_scored,int cards,int games_played, int team_id,bool is_goalkeeper);
    ~Player();





};

#endif //DATA_STRUCT_EX1_PLAYER_H
