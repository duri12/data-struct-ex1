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
    if(teamId<=0||playerId<=0||gamesPlayed<0||goals<0||cards<0)
        return StatusType::INVALID_INPUT;
    if(gamesPlayed=0&&(goals>0||cards>0))
        return StatusType::INVALID_INPUT;
    try{
        std::shared_ptr<Player> player1 (new Player(playerId,teamId,gamesPlayed,goals,cards,goalKeeper));
        std::shared_ptr<Team> team_copy (new Team(teamId,0));
        Node<shared_ptr<Team>> *n1=teams_tree.find(team_copy,compare_teams_by_id);
        if(n1==nullptr)
        {
            return StatusType::FAILURE;
        }
        player1->set_team_pointer(n1->getData());
        player1->pla






    }
    catch(std::bad_alloc)
    {

    }

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
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

