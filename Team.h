#ifndef DATA_STRUCT_EX1_TEAM_H
#define DATA_STRUCT_EX1_TEAM_H
#include "Player.h"
#include "AvlTree.h"
#include "memory"
using namespace std;

class Team {
private:
    AvlTree<std::shared_ptr<Player>> players_in_team_by_score; // the players in the team by their score
    AvlTree<std::shared_ptr<Player>> players_in_team_by_id; // the players in the team by their identify number
    int sum_of_player_score;  // the sum of all the player's scores in the team (goals - cards)
    int teamID; // the team ID
    int player_count; // the number of players in the team
    int goalkeeper_count; // the number of goalkeepers in the team
    int additional_games_played; // the number of games the team has played so far
    int team_points; // the total number of points of the team
    std::shared_ptr<Player> team_top_scorer; // a pointer to the team's top scorer
    std::weak_ptr<Team> global_left_closest_team;
    std::weak_ptr<Team> global_right_closest_team;

public:
Team(int teamID,int team_points);
~Team();
int getteamID() const { return teamID; }
int getsum_of_player_score() const { return sum_of_player_score;}
int getPlayerCount() const { return player_count; }
int getGoalkeeperCount() const { return goalkeeper_count; }
int getAdditionalGamesPlayed() const { return additional_games_played; }
int getTeamPoints() const { return team_points; }
std::shared_ptr<Player> getTeamTopScorer() const { return team_top_scorer;}
std::weak_ptr<Team> getglobal_left_closest_team() const { return global_left_closest_team;}
std::weak_ptr<Team> getglobal_right_closest_team() const { return global_right_closest_team;}
void setsum_of_player_score(int new_sum_of_player_score){this->sum_of_player_score = new_sum_of_player_score;}
void setTeamPoints(int new_team_points) { this->team_points = new_team_points;}
void setTeamTopScorer(std::shared_ptr<Player> new_team_top_scorer){ this->team_top_scorer = new_team_top_scorer;}
void setAdditionalGamesPlayed(int new_additional_games_played) { this->additional_games_played=new_additional_games_played;}
void setTeamID(int new_team_id) { this->teamID = new_team_id;}
void setPlayerCount(int new_player_count) { this->player_count = new_player_count;}
void setGoalkeeperCount(int new_goalkeeper_count) { this->goalkeeper_count =new_goalkeeper_count;}
void setglobal_left_closest_team(std::weak_ptr<Team> new_global_left_closest_team) { this->global_left_closest_team = new_global_left_closest_team;}
void setglobal_right_closest_team(std::weak_ptr<Team> new_global_right_closest_team) { this->global_right_closest_team = new_global_right_closest_team;}
bool add_player_to_team_by_ID(std::shared_ptr<Player> player1);
bool add_player_to_team_by_score(std::shared_ptr<Player> player1);
bool remove_player_from_team_by_ID(std::shared_ptr<Player> player1);
bool remove_player_from_team_by_Score(std::shared_ptr<Player> player1);
bool find_player_by_ID(std::shared_ptr<Player> player1);
bool find_player_by_score(std::shared_ptr<Player> player1);
const std::shared_ptr<Player> find_max_by_ID();
const std::shared_ptr<Player> find_max_by_Score();

};
int compare_teams_by_id(std::shared_ptr<Team> team1, std::shared_ptr<Team>team2);


#endif //DATA_STRUCT_EX1_TEAM_H
