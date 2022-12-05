#include "Player.h"
Player::Player(int player_ID, int goals_scored, int cards, int games_played, int team_id, bool is_goalkeeper) :player_ID(player_ID),goals_scored(goals_scored),cards(cards),games_played(games_played),team_ID(team_id),is_goalkeeper(is_goalkeeper)
{
    std::shared_ptr<Team> t(nullptr);
    std::shared_ptr<Player> p(nullptr);
    team_pointer=t;
    global_right_closest=p;
    global_left_closest=p;

}
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
int compare_players_by_Score(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2) {
    if (player1->get_goals_scored() > player2->get_goals_scored())
        return -1;
    else if (player1->get_goals_scored() < player2->get_goals_scored())
        return 1;
    else {
        if (player1->get_cards() < player2->get_cards())
            return -1;
        else if (player1->get_cards() > player2->get_cards())
            return 1;
    }
    return compare_players_by_ID(player1, player2);
}




ostream& operator<<(ostream& os, const Player& player)
{
    os <<"printing player:"<<player.player_ID<<endl;
    os<< "teamId: "<<player.team_ID<<endl;
    os<< "goalsScored: "<<player.goals_scored<<endl;
    os<< "cards: "<<player.cards<<endl;
    os<< "gamesPlayed: "<<player.games_played<<endl;
    os<< "isGoalkeeper: "<<player.is_goalkeeper<<endl;
    return os;
}


