#ifndef DATA_STRUCT_EX1_TEAM_H
#define DATA_STRUCT_EX1_TEAM_H
#include "Player.h"
#include "AvlTree.h"
class Team {
private:
    AvlTree<Player> players_in_team_by_score; // the players in the team by their score
    AvlTree<Player> players_in_team_by_id; // the players in the team by their identify number
    int sum_of_player_score;  // the sum of all the player's scores in the team (goals - cards)
    int teamID; // the team ID
    int player_count; // the number of players in the team
    int goalkeeper_count; // the number of goalkeepers in the team
    int additional_games_played; // the number of games the team has played so far
    int team_points; // the total number of points of the team
    Player *team_top_scorer; // a pointer to the team's top scorer
public:
Team(int teamID);
~Team();
};

#endif //DATA_STRUCT_EX1_TEAM_H
