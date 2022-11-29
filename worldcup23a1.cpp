#include "worldcup23a1.h"
#include "memory"
using namespace std;
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


    }
    catch(std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;

}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId<=0)
        return StatusType::INVALID_INPUT;
    try{
        std::shared_ptr<Team> t1 (new Team(teamId, 0));
        if(teams_tree.remove(t1,&compare_teams_by_id)== false)
            return StatusType::FAILURE;



        return StatusType::SUCCESS;



    }
    catch (std::bad_alloc&){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
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
        Node<shared_ptr<Team>> *n1=teams_tree.find(team_copy,&compare_teams_by_id);
        if(n1==nullptr)
        {
            return StatusType::FAILURE;
        }
        //initialize the correct teampointer for the player
        player1->set_team_pointer(n1->getData());
        if(player1->get_team_pointer().lock()->add_player_to_team_by_ID(player1)==false)//put in player_in_team_by_id
            return StatusType::FAILURE;
        if(player1->get_team_pointer().lock()->add_player_to_team_by_score(player1)==false)//put in player_in_team_by_score
            return StatusType::FAILURE;
        if(players_tree_by_id.add(player1,&compare_players_by_ID)==false)//put in player_tree_by_id
            return StatusType::FAILURE;
        if(players_tree_by_score.add(player1,&compare_players_by_Score,player1->get_global_left_closest().lock(),player1->get_global_right_closest().lock())==false) //put in player_tree_by_score
            return StatusType::FAILURE;
        player1->set_games_played(gamesPlayed-player1->get_team_pointer().lock()->getAdditionalGamesPlayed());//set games played
        player1->get_team_pointer().lock()->setsum_of_player_score((player1->get_team_pointer().lock()->getsum_of_player_score())+(player1->get_goals_scored()-player1->get_cards()));//set sum of players score for team
        if(player1->get_team_pointer().lock()->getTeamTopScorer()->get_goals_scored()<player1->get_goals_scored()) {//check if it new team_top_scorer
            player1->get_team_pointer().lock()->setTeamTopScorer(player1);
            if (top_scorer->get_goals_scored() < player1->get_goals_scored())// check if it new top scorer
                top_scorer=player1;
        }
        total_players_counter++;//add 1 to total_players_counter
        player1->get_team_pointer().lock()->setPlayerCount(player1->get_team_pointer().lock()->getPlayerCount()+1);//add 1 to team_players_count;
        if(player1->get_is_goalkeeper()==true)
            player1->get_team_pointer().lock()->setGoalkeeperCount(player1->get_team_pointer().lock()->getGoalkeeperCount()+1);//if player is goalkeeper add 1 to team's goalkeeper count
        if(player1->get_team_pointer().lock()->getGoalkeeperCount()>0&&player1->get_team_pointer().lock()->getPlayerCount()>10)//add team to current active teams if suitable
            current_active_teams.add(player1->get_team_pointer().lock(),&compare_teams_by_id,player1->get_team_pointer().lock()->getglobal_left_closest_team().lock(),player1->get_team_pointer().lock()->getglobal_right_closest_team().lock());
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

        std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, 0, 0));
        Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, &compare_players_by_ID);
        if (n1 == nullptr) {
            return StatusType::FAILURE;
        }
        std::weak_ptr<Team> tempteam1=n1->getData()->get_team_pointer();

        //remove player from trees
        if (n1->getData()->get_team_pointer().lock()->remove_player_from_team_by_ID(player1) == false)
            return StatusType::FAILURE;
        if (n1->getData()->get_team_pointer().lock()->remove_player_from_team_by_Score(player1) == false)
            return StatusType::FAILURE;
        if (players_tree_by_score.remove(n1->getData(), &compare_players_by_Score) == false)
            return StatusType::FAILURE;
      //check if top scorer for team or all players
        if (n1->getData()->get_team_pointer().lock()->getTeamTopScorer() == n1->getData()) {
            n1->getData()->get_team_pointer().lock()->setTeamTopScorer(n1->getData()->get_team_pointer().lock()->find_max_by_Score());
            if (top_scorer = n1->getData())
                top_scorer = players_tree_by_score.findMax();
        }
        //remove player from last tree and set nre team values
        std::shared_ptr<Player> tempplayer(new Player(0,0,0,0,0,0));
        tempplayer=n1->getData();
        if(players_tree_by_id.remove(n1->getData(), &compare_players_by_ID) == false)
            return StatusType::FAILURE;
        tempteam1.lock()->setPlayerCount(tempteam1.lock()->getPlayerCount()-1);
        tempteam1.lock()->setsum_of_player_score(tempteam1.lock()->getsum_of_player_score()-(tempplayer->get_goals_scored()-tempplayer->get_cards()));
        if(tempplayer->get_is_goalkeeper()==true)
            tempteam1.lock()->setGoalkeeperCount(tempteam1.lock()->getGoalkeeperCount()-1);
        total_players_counter--;
        if((tempplayer->get_is_goalkeeper()==true&&tempteam1.lock()->getGoalkeeperCount()==0)||tempteam1.lock()->getPlayerCount()==10){
            if(current_active_teams.remove(tempteam1.lock(),&compare_teams_by_id))
                return StatusType::FAILURE;
            tempteam1.lock()->setglobal_left_closest_team(tempteam1.lock()->getglobal_right_closest_team());
            tempteam1.lock()->setglobal_right_closest_team(tempteam1.lock()->getglobal_left_closest_team());

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
    Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, &compare_players_by_ID);
    if (n1 == nullptr)
    {
        return StatusType::FAILURE;
    }
    if(players_tree_by_score.remove(n1->getData(),&compare_players_by_Score)== false)
        return StatusType::FAILURE;
    n1->getData()->set_global_left_closest(n1->getData()->get_global_right_closest());
    n1->getData()->set_global_right_closest(n1->getData()->get_global_left_closest());
    if(n1->getData()->get_team_pointer().lock()->remove_player_from_team_by_Score(n1->getData())==false)
        return StatusType::FAILURE;
    n1->getData()->get_team_pointer().lock()->setsum_of_player_score(n1->getData()->get_team_pointer().lock()->getsum_of_player_score()+-cardsReceived+scoredGoals);
    n1->getData()->set_games_played(n1->getData()->get_games_played()+gamesPlayed);
    n1->getData()->set_goals_scored(n1->getData()->get_goals_scored()+scoredGoals);
    n1->getData()->set_cards(n1->getData()->get_cards()+cardsReceived);
    if(players_tree_by_score.add(n1->getData(),&compare_players_by_Score,n1->getData()->get_global_right_closest().lock()->get_global_left_closest().lock(),n1->getData()->get_global_left_closest().lock()->get_global_right_closest().lock())==false)
        return StatusType::FAILURE;
    if(n1->getData()->get_team_pointer().lock()->add_player_to_team_by_score(n1->getData()))
        return StatusType::FAILURE;
    if(n1->getData()->get_goals_scored()>n1->getData()->get_team_pointer().lock()->getTeamTopScorer()->get_goals_scored()) {
        n1->getData()->get_team_pointer().lock()->setTeamTopScorer(n1->getData());
    if(n1->getData()->get_goals_scored()>top_scorer->get_goals_scored())
        top_scorer=n1->getData();
    }



    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0||teamId2<=0||teamId1==teamId2)
        return StatusType::INVALID_INPUT;
    std::shared_ptr<Team> team1(new Team(teamId1, 0));
    Node<shared_ptr<Team>> *n1 = current_active_teams.find(team1, &compare_teams_by_id);
    std::shared_ptr<Team> team2(new Team(teamId2, 0));
    Node<shared_ptr<Team>> *n2 = current_active_teams.find(team2, &compare_teams_by_id);
    if (n1 == nullptr || n2 == nullptr)
        return StatusType::FAILURE;
    n1->getData()->setAdditionalGamesPlayed(n1->getData()->getAdditionalGamesPlayed()+1);
    n2->getData()->setAdditionalGamesPlayed(n2->getData()->getAdditionalGamesPlayed()+1);
    if(n1->getData()->getTeamPoints()+n1->getData()->getsum_of_player_score()>n2->getData()->getTeamPoints()+n2->getData()->getsum_of_player_score()){
    n1->getData()->setTeamPoints(n1->getData()->getTeamPoints()+3);
    }
    else if(n1->getData()->getTeamPoints()+n1->getData()->getsum_of_player_score()<n2->getData()->getTeamPoints()+n2->getData()->getsum_of_player_score()) {
        n2->getData()->setTeamPoints(n2->getData()->getTeamPoints()+3);
    }
    else {
        n1->getData()->setTeamPoints(n1->getData()->getTeamPoints() + 1);
        n2->getData()->setTeamPoints(n2->getData()->getTeamPoints() + 1);
    }

    return StatusType::SUCCESS;





    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId<=0)
        return output_t<int>(StatusType::INVALID_INPUT);
    std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, 0, 0));
    Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, &compare_players_by_ID);
    if (n1 == nullptr)
        return output_t<int>(StatusType::FAILURE);
	int value=n1->getData()->get_games_played()+n1->getData()->get_team_pointer().lock()->getAdditionalGamesPlayed();
    return output_t<int>(value);
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId<=0)
        return output_t<int>(StatusType::INVALID_INPUT);
    std::shared_ptr<Team> team1(new Team(teamId,0));
    Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
    if (n1 == nullptr)
        return output_t<int>(StatusType::FAILURE);
    int value=n1->getData()->getTeamPoints();
    return output_t<int>(value);
}


StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if(teamId=0)
        return output_t<int>(StatusType::INVALID_INPUT);
    if(teamId<0)
        return output_t<int>(top_scorer->get_player_ID());
    if(teamId>0)
    {
        std::shared_ptr<Team> team1(new Team(teamId,0));
        Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
        if (n1 == nullptr)
            return output_t<int>(StatusType::FAILURE);
        return output_t<int>(n1->getData()->getTeamTopScorer()->get_player_ID());

    }
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId==0)
        return output_t<int>(StatusType::INVALID_INPUT);
    if(teamId<0)
        return output_t<int>(total_players_counter);
    if(teamId>0)
    {
        std::shared_ptr<Team> team1(new Team(teamId,0));
        Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
        if (n1 == nullptr)
            return output_t<int>(StatusType::FAILURE);
        return output_t<int>(n1->getData()->getPlayerCount());
}
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{

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

