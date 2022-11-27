#ifndef DATA_STRUCT_EX1_TEAM_H
#define DATA_STRUCT_EX1_TEAM_H
#include "Player.h"
#include "AvlTree.h"
#include "memory"
class Team {
private:
    AvlTree<Player*> players_in_team_by_score; // the players in the team by their score
    AvlTree<Player*> players_in_team_by_id; // the players in the team by their identify number
    int sum_of_player_score;  // the sum of all the player's scores in the team (goals - cards)
    int teamID; // the team ID
    int player_count; // the number of players in the team
    int goalkeeper_count; // the number of goalkeepers in the team
    int additional_games_played; // the number of games the team has played so far
    int team_points; // the total number of points of the team
    Player *team_top_scorer; // a pointer to the team's top scorer
public:
Team(int teamID,int team_points);
~Team();
int getteamID() const { return teamID; }
int getPlayerCount() const { return player_count; }
int getGoalkeeperCount() const { return goalkeeper_count; }
int getAdditionalGamesPlayed() const { return additional_games_played; }
int getTeamPoints() const { return team_points; }
Player *getTeamTopScorer() const { return team_top_scorer; }
void setTeamPoints(int team_points) { this->team_points = team_points;}
void setTeamTopScorer(Player *new_team_top_scorer) { this->team_top_scorer=new_team_top_scorer;}
void setAdditionalGamesPlayed(int new_additional_games_played) { this->additional_games_played=new_additional_games_played;}
void setTeamID(int new_team_id) { this->teamID = new_team_id;}
void setPlayerCount(int new_player_count) { this->player_count = new_player_count;}
void setGoalkeeperCount(int new_goalkeeper_count) { this->goalkeeper_count =new_goalkeeper_count;}





};
int compare_teams_by_id(std::shared_ptr<Team> team1, std::shared_ptr<Team>team2)
{
    if(team1->getteamID()>=team2->getteamID())
    {
        if(team1->getteamID()==team2->getteamID())
            return 0;

        return -1;
    }
    return 1;

}

#endif //DATA_STRUCT_EX1_TEAM_H
