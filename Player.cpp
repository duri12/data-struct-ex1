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
int add_team 1 10000
add_team 2 20000
add_team 3 30000
add_team 4 40000
add_player 1001 1 10 0 0 false
add_player 1002 1 10 0 0 false
add_player 1003 1 10 0 0 false
add_player 1004 1 10 0 2 false
add_player 1005 1 10 0 0 false
add_player 1006 1 10 4 3 false
add_player 1007 1 10 0 0 false
add_player 1008 1 10 0 0 true
add_player 1009 1 10 0 0 false
add_player 1010 1 10 0 0 false
add_player 1011 1 10 0 0 false
add_player 1012 1 10 0 0 false
add_player 2001 2 20 0 0 false
add_player 2002 2 20 0 0 false
add_player 2003 2 20 0 0 false
add_player 2004 2 20 0 0 false
add_player 2005 2 20 0 0 false
add_player 2006 2 20 0 0 false
add_player 2007 2 20 0 0 false
add_player 2008 2 20 6 4 true
add_player 2009 2 20 0 0 false
add_player 2010 2 0 0 0 false
add_player 2011 2 20 0 0 false
add_player 3001 3 30 0 0 false
add_player 3002 3 30 0 0 false
add_player 3003 3 30 0 0 false
add_player 3004 3 30 0 0 false
add_player 3005 3 30 2 0 false
add_player 3006 3 30 0 0 false
add_player 3007 3 30 0 0 false
add_player 3008 3 30 0 0 false
add_player 3009 3 30 0 2 false
add_player 3010 3 30 0 0 true
add_player 3011 3 30 0 0 true
add_player 4001 4 2 1 2 false
add_player 4002 4 2 2 2 false
update_player_stats 3011 2 1 1
play_match 2 3(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2) {
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


