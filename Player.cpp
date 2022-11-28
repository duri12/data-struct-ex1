#include "Player.h"
Player::Player(int player_ID, int goals_scored, int cards, int games_played, int team_id, bool is_goalkeeper) :player_ID(player_ID),goals_scored(goals_scored),cards(cards),games_played(games_played),team_ID(team_id),is_goalkeeper(is_goalkeeper),global_left_closest(
        nullptr),global_right_closest(nullptr),team_pointer(nullptr)
{}
Player::~Player() =default;
int compare_players_by_ID(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
{
    if(player1->get_player_ID()>=player2->get_player_ID())
    {
        if(player1->get_player_ID()==player2->get_player_ID())
            return 0;

        return -1;
    }
    return 1;
}
int compare_players_by_Score(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
{
    if(player1->get_goals_scored()>player2->get_goals_scored())
        return -1;
    else if(player1->get_player_ID()<player2->get_player_ID())
        return 1;
    else {
        if (player1->get_cards() < player2->get_cards())
            return -1;
        else if (player1->get_cards() > player2->get_cards())
            return 1;
    }
    return compare_players_by_ID(player1, player2);


}



