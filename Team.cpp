#include "Team.h"
Team::Team(int teamID,int team_points):teamID(teamID),team_points(team_points),sum_of_player_score(0),player_count(0),goalkeeper_count(0),additional_games_played(0),players_in_team_by_score(AvlTree<std::shared_ptr<Player>>()),players_in_team_by_id(AvlTree<std::shared_ptr<Player>>()),team_top_scorer(
        nullptr)
{}
Team::~Team()=default;


