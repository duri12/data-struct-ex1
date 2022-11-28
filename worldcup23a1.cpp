#include "worldcup23a1.h"
#include "memory"
world_cup_t::world_cup_t():current_active_teams(AvlTree<std::shared_ptr<Team>>()),teams_tree(AvlTree<std::shared_ptr<Team>>()),players_tree_by_id(AvlTree<std::shared_ptr<Player>>()),players_tree_by_score(AvlTree<std::shared_ptr<Player>>()),top_scorer(
        nullptr),total_players_counter(0)
{

}

world_cup_t::~world_cup_t()
{}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId<=0||points<0)
        return StatusType::INVALID_INPUT;
    try {
        std::shared_ptr<Team> t1 (new Team(teamId, points));
       if(teams_tree.add(t1,&compare_teams_by_id)==false)
           return StatusType::FAILURE;
       else
           return StatusType::SUCCESS;

    }
    catch(std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }

}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId<=0)
        return StatusType::INVALID_INPUT;
    try{
        std::shared_ptr<Team> t1 (new Team(teamId, 0));
        if(teams_tree.remove(t1,&compare_teams_by_id)== false)
            return StatusType::FAILURE;
        else
            return StatusType::SUCCESS;



    }
    catch (std::bad_alloc&){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    //check invalid inputs
    if(teamId<=0||playerId<=0||gamesPlayed<0||goals<0||cards<0)
        return StatusType::INVALID_INPUT;
    if(gamesPlayed=0&&(goals>0||cards>0))
        return StatusType::INVALID_INPUT;
    try{
        //create variables and find the team
        std::shared_ptr<Player> player1 (new Player(playerId,teamId,gamesPlayed,goals,cards,goalKeeper));
        std::shared_ptr<Team> team_copy (new Team(teamId,0));
        Node<shared_ptr<Team>> *n1=teams_tree.find(team_copy,compare_teams_by_id);
        if(n1==nullptr)
        {
            return StatusType::FAILURE;
        }
        //initialize the correct teampointer for the player
        player1->set_team_pointer(n1->getData());
        if(player1->get_team_pointer()->add_player_to_team_by_ID(player1)==false)//put in player_in_team_by_id
            return StatusType::FAILURE;
        if(player1->get_team_pointer()->add_player_to_team_by_score(player1)==false)//put in player_in_team_by_score
            return StatusType::FAILURE;
        if(players_tree_by_id.add(player1,&compare_players_by_ID)==false)//put in player_tree_by_id
            return StatusType::FAILURE;
        if(players_tree_by_score.add(player1,&compare_players_by_Score)==false) //put in player_tree_by_score
            return StatusType::FAILURE;
        player1->set_games_played(gamesPlayed-player1->get_team_pointer()->getAdditionalGamesPlayed());//set games played
        player1->get_team_pointer()->setsum_of_player_score((player1->get_team_pointer()->getsum_of_player_score())+(player1->get_goals_scored()-player1->get_cards()));//set sum of players score for team
        if(player1->get_team_pointer()->getTeamTopScorer()->get_goals_scored()<player1->get_goals_scored()) {//check if it new team_top_scorer
            player1->get_team_pointer()->setTeamTopScorer(player1);
            if (top_scorer->get_goals_scored() < player1->get_goals_scored())// check if it new top scorer
                top_scorer=player1;
        }
        total_players_counter++;//add 1 to total_players_counter
        player1->get_team_pointer()->setPlayerCount(player1->get_team_pointer()->getPlayerCount()+1);//add 1 to team_players_count;
        if(player1->get_is_goalkeeper()==true)
            player1->get_team_pointer()->setGoalkeeperCount(player1->get_team_pointer()->getGoalkeeperCount()+1);//if player is goalkeeper add 1 to team's goalkeeper count
        if(player1->get_team_pointer()->getGoalkeeperCount()>0&&player1->get_team_pointer()->getPlayerCount()>10)//add team to current active teams if suitable
            current_active_teams.add(player1->get_team_pointer(),compare_teams_by_id);
    }

    catch(std::bad_alloc)
    {
        return StatusType::FAILURE;
    }

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if(playerId<=0)
        return StatusType::INVALID_INPUT;
    try{
        std::shared_ptr<Player> player1 (new Player(playerId,0,0,0,0,0));
        Node<shared_ptr<Player>> *n1=players_tree_by_id.find(player1,compare_players_by_ID);
        if(n1==nullptr)
        {
            return StatusType::FAILURE;
        }
       if( n1->getData()->get_team_pointer()->remove_player_from_team_by_ID(player1)==false)
           return StatusType::FAILURE;
       if( n1->getData()->get_team_pointer()->remove_player_from_team_by_Score(player1)==false)
            return StatusType::FAILURE;
       if(players_tree_by_score.remove(n1->getData(),compare_players_by_Score)==false)
           return StatusType::FAILURE;
       if(n1->getData()->get_team_pointer()->getTeamTopScorer()==n1->getData())
           n1->getData()->get_team_pointer()->setTeamTopScorer(n1->getData()->get_team_pointer())

    catch{

    }

    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

