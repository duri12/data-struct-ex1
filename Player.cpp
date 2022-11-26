#include "Player.h"
Player::Player(int player_ID, int goals_scored, int cards, int games_played, int team_id, bool is_goalkeeper) :player_ID(player_ID),goals_scored(goals_scored),cards(cards),games_played(games_played),team_ID(team_id),is_goalkeeper(is_goalkeeper),global_left_closest(
        nullptr),global_right_closest(nullptr),team_pointer(nullptr)
{}
Player::~Player() =default;

