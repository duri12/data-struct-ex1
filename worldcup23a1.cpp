#include "worldcup23a1.h"
#include "memory"
world_cup_t::world_cup_t():current_active_teams(AvlTree<std::shared_ptr<Team>>()),teams_tree(AvlTree<std::shared_ptr<Team>>()),players_tree_by_id(AvlTree<std::shared_ptr<Player>>()),players_tree_by_score(AvlTree<std::shared_ptr<Player>>()),top_scorer(
        nullptr),total_players_counter(0)
{

}

world_cup_t::~world_cup_t()=default;



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
    try {
        std::shared_ptr<Team> tempteam (new Team(0,0));
        std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, 0, 0));
        Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, compare_players_by_ID);
        if (n1 == nullptr) {
            return StatusType::FAILURE;
        }
        tempteam=n1->getData()->get_team_pointer();
        //remove player from trees
        if (n1->getData()->get_team_pointer()->remove_player_from_team_by_ID(player1) == false)
            return StatusType::FAILURE;
        if (n1->getData()->get_team_pointer()->remove_player_from_team_by_Score(player1) == false)
            return StatusType::FAILURE;
        if (players_tree_by_score.remove(n1->getData(), compare_players_by_Score) == false)
            return StatusType::FAILURE;
      //check if top scorer for team or all players
        if (n1->getData()->get_team_pointer()->getTeamTopScorer() == n1->getData()) {
            n1->getData()->get_team_pointer()->setTeamTopScorer(n1->getData()->get_team_pointer()->find_max_by_Score());
            if (top_scorer = n1->getData())
                top_scorer = players_tree_by_score.findMax();
        }
        //remove player from last tree and set nre team values
        std::shared_ptr<Player> tempplayer(new Player(0,0,0,0,0,0));
        tempplayer=n1->getData();
        if(players_tree_by_id.remove(n1->getData(), compare_players_by_ID) == false)
            return StatusType::FAILURE;
        tempteam->setPlayerCount(tempteam->getPlayerCount()-1);
        tempteam->setsum_of_player_score(tempteam->getsum_of_player_score()-(tempplayer->get_goals_scored()-tempplayer->get_cards()));
        if(tempplayer->get_is_goalkeeper()==true)
            tempteam->setGoalkeeperCount(tempteam->getGoalkeeperCount()-1);
        total_players_counter--;
        if((tempplayer->get_is_goalkeeper()==true&&tempteam->getGoalkeeperCount()==0)||tempteam->getPlayerCount()==10){
            if(current_active_teams.remove(tempteam,compare_teams_by_id))
                return StatusType::FAILURE;
        }
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc)
    {
    return StatusType::ALLOCATION_ERROR  ;
    }


	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
    if(playerId<=0||gamesPlayed<0||scoredGoals<0||cardsReceived<0)//check input
        return StatusType::INVALID_INPUT;
    std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, 0, 0));
    Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, compare_players_by_ID);
    if (n1 == nullptr) {
        return StatusType::FAILURE;
    }
    if(players_tree_by_score.remove(n1->getData(),compare_players_by_Score)== false)
        return StatusType::FAILURE;
    if(n1->getData()->get_team_pointer()->remove_player_from_team_by_Score(n1->getData())==false)
        return StatusType::FAILURE;
    n1->getData()->get_team_pointer()->setsum_of_player_score(n1->getData()->get_team_pointer()->getsum_of_player_score()+-cardsReceived+scoredGoals);
    n1->getData()->set_games_played(n1->getData()->get_games_played()+gamesPlayed);
    n1->getData()->set_goals_scored(n1->getData()->get_goals_scored()+scoredGoals);
    n1->getData()->set_cards(n1->getData()->get_cards()+cardsReceived);
    if(players_tree_by_score.add(n1->getData(),compare_players_by_Score))
        return StatusType::FAILURE;
    if(n1->getData()->get_team_pointer()->add_player_to_team_by_score(n1->getData()))
        return StatusType::FAILURE;
    if(n1->getData()->get_goals_scored()>n1->getData()->get_team_pointer()->getTeamTopScorer()->get_goals_scored()) {
        n1->getData()->get_team_pointer()->setTeamTopScorer(n1->getData());
    if(n1->getData()->get_goals_scored()>top_scorer->get_goals_scored())
        top_scorer=n1->getData();
    }



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

