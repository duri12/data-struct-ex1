#include "Team.h"
Team::Team(int teamID,int team_points):players_in_team_by_score(AvlTree<std::shared_ptr<Player>>()),players_in_team_by_id(AvlTree<std::shared_ptr<Player>>()),teamID(teamID),team_points(team_points),sum_of_player_score(0),player_count(0),goalkeeper_count(0),additional_games_played(0),team_top_scorer(nullptr)

{
    std::shared_ptr<Team> t(nullptr);
    global_left_closest_team=t;
    global_right_closest_team=t;
}
Team::~Team()=default;

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
bool Team::add_player_to_team_by_ID(std::shared_ptr<Player> player1)
{
    return players_in_team_by_id.add(player1,&compare_players_by_ID);
}
bool Team::add_player_to_team_by_score(std::shared_ptr<Player> player1)
{
    return players_in_team_by_score.add(player1,&compare_players_by_Score);

}
bool Team::remove_player_from_team_by_ID(std::shared_ptr<Player> player1)
{
    return players_in_team_by_id.remove(player1,&compare_players_by_ID);
}
bool Team::remove_player_from_team_by_Score(std::shared_ptr<Player> player1)
{
    return players_in_team_by_score.remove(player1,&compare_players_by_Score);
}
Node<std::shared_ptr<Player>>* Team::find_player_by_ID(std::shared_ptr<Player> player1){
    return players_in_team_by_id.find(player1,&compare_players_by_ID);
}
Node<std::shared_ptr<Player>>* Team::find_player_by_score(std::shared_ptr<Player> player1){
    return players_in_team_by_score.find(player1,&compare_players_by_Score);
}


const std::shared_ptr<Player> Team::find_max_by_Score() {
    if(players_in_team_by_score.getRoot()!=NULL)
    return players_in_team_by_score.findMax();
    return nullptr;
}
void Team::treeToArrayInOrder_for_team_byscore(std::shared_ptr<Player> * array,int size ){
     players_in_team_by_score.treeToArrayInOrder(array,size);
}

void Team::treeToArrayInOrder_for_team_byId(std::shared_ptr<Player> * array,int size ){
     players_in_team_by_id.treeToArrayInOrder(array,size);
}
bool Team::create_tree_from_array_by_Id(std::shared_ptr<Player> * array,int size ){
    return players_in_team_by_id.createTreeFromSortedArray(array,size);
}
bool Team::create_tree_from_array_by_Score(std::shared_ptr<Player> * array,int size ){
    return players_in_team_by_score.createTreeFromSortedArray(array,size);
}

ostream& operator<<(ostream& os, const Team& team)
{
    os<< "printing team :" <<team.teamID<<endl;
    os << "the team score:"<<team.sum_of_player_score<<endl;
    os << "the number of players"<<team.player_count<<endl;
    os << "the number of goalKeepers the team have" <<team.goalkeeper_count<<endl;
    return os;
}


