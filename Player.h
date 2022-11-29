#ifndef DATA_STRUCT_EX1_PLAYER_H
#define DATA_STRUCT_EX1_PLAYER_H
#include "memory"
#include "AvlTree.h"
using namespace std;

class Team;
class Player {
private:
    int player_ID; // the player's ID
    int goals_scored; // the number of goals the player has scored
    int cards; // the number of cards the player has collected
    int games_played; // the number of games played for the team
    bool is_goalkeeper; // true if the player is goalkeeper false otherwise
    int team_ID; // the ID of the team the player belongs to
    std::weak_ptr<Team> team_pointer; // the team the player belongs to
    std::weak_ptr<Player> global_left_closest; // the player that is closest to this player from the left
    std::weak_ptr<Player> global_right_closest; // the player that is closest to this player from the right
public:
    Player(int player_ID,int goals_scored,int cards,int games_played, int team_id,bool is_goalkeeper);
    ~Player();
    int get_player_ID() const { return player_ID; }
    int get_goals_scored() const { return goals_scored; }
    int get_cards() const { return cards; }
    int get_games_played() const { return games_played; }
    bool get_is_goalkeeper() const { return is_goalkeeper; }
    int get_team_ID() const { return team_ID; }
    std::weak_ptr<Team> get_team_pointer() const { return team_pointer;}
    std::weak_ptr<Player> get_global_left_closest() const { return global_left_closest;}
    std::weak_ptr<Player> get_global_right_closest() const { return global_right_closest;}
    void set_global_left_closest(std::weak_ptr<Player> new_global_left_closest){ this->global_left_closest = new_global_left_closest;}
    void set_global_right_closest(std::weak_ptr<Player> new_global_right_closest){ this->global_right_closest = new_global_right_closest;}
    void set_team_pointer(std::weak_ptr<Team> new_team_pointer){ this->team_pointer = new_team_pointer;}
    void set_is_goalkeeper(bool new_is_goalkeeper) { this->is_goalkeeper=new_is_goalkeeper;}
    void set_team_ID(int new_team_ID) { this->team_ID=new_team_ID;}
    void set_player_ID(int new_player_ID) { this->player_ID= new_player_ID;}
    void set_goals_scored(int new_goals_scored) { this->goals_scored=new_goals_scored;}
    void set_cards(int new_cards) { this->cards=new_cards;}
    void set_games_played(int new_games_played) { this->games_played=new_games_played;}








};
int compare_players_by_ID(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2);
int compare_players_by_Score(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2);

#endif //DATA_STRUCT_EX1_PLAYER_H
