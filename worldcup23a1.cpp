#include "worldcup23a1.h"
#include "memory"
using namespace std;
world_cup_t::world_cup_t():current_active_teams(AvlTree<std::shared_ptr<Team>>()),teams_tree(AvlTree<std::shared_ptr<Team>>()),players_tree_by_id(AvlTree<std::shared_ptr<Player>>()),players_tree_by_score(AvlTree<std::shared_ptr<Player>>()),top_scorer(
        nullptr),total_players_counter(0){}

world_cup_t::~world_cup_t()=default;



StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId<=0||points<0)
        return StatusType::INVALID_INPUT;
    try {
        std::shared_ptr<Team> t1 (new Team(teamId, points));
       if(!teams_tree.add(t1,&compare_teams_by_id))
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
        Node<shared_ptr<Team>> *n1 = teams_tree.find(t1, &compare_teams_by_id);
        if(n1==nullptr||n1->getData()->getPlayerCount()>0)
            return StatusType::FAILURE;
        if(!teams_tree.remove(t1,&compare_teams_by_id))
            return StatusType::FAILURE;



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
    if(gamesPlayed==0&&(goals>0||cards>0))
        return StatusType::INVALID_INPUT;
    try {
        //create variables and find the team
        std::shared_ptr<Player> player1(new Player(playerId, goals, cards, gamesPlayed, teamId, goalKeeper));
        std::shared_ptr<Team> team_copy(new Team(teamId, 0));
        Node<shared_ptr<Team>> *n1 = teams_tree.find(team_copy, &compare_teams_by_id);
        if (n1 == nullptr) {
            return StatusType::FAILURE;
        }
        //initialize the correct teampointer for the player
        player1->set_team_pointer(n1->getData());
        if (!player1->get_team_pointer().lock()->add_player_to_team_by_ID(player1))//put in player_in_team_by_id
            return StatusType::FAILURE;
        if (!player1->get_team_pointer().lock()->add_player_to_team_by_score(player1))//put in player_in_team_by_score
            return StatusType::FAILURE;
        if (!players_tree_by_id.add(player1, &compare_players_by_ID))//put in player_tree_by_id
            return StatusType::FAILURE;
        shared_ptr<Player> left(new Player(0,0,0,0,0,0));
        shared_ptr<Player> right(new Player(0,0,0,0,0,0));
        if (!players_tree_by_score.add(player1, &compare_players_by_Score, left, right)) //put in player_tree_by_score
            return StatusType::FAILURE;
        if(left->get_player_ID()!= player1->get_player_ID()) {
            player1->set_global_left_closest(left);
        }
        if(right->get_player_ID() !=player1->get_player_ID()){
            player1->set_global_right_closest(right);
        }
        if(player1->get_global_right_closest().lock()!= nullptr)
        player1->get_global_right_closest().lock()->set_global_left_closest(player1);
        if(player1->get_global_left_closest().lock()!= nullptr)
        player1->get_global_left_closest().lock()->set_global_right_closest(player1);
        player1->set_games_played(gamesPlayed - player1->get_team_pointer().lock()->getAdditionalGamesPlayed());//set games playedplayer1->get_team_pointer().lock()->setsum_of_player_score((player1->get_team_pointer().lock()->getsum_of_player_score()) +(player1->get_goals_scored() - player1->get_cards()));//set sum of players score for team
         if(player1->get_team_pointer().lock()->getTeamTopScorer()== nullptr)
             player1->get_team_pointer().lock()->setTeamTopScorer(player1);
         else{
             if(compare_players_by_Score(player1->get_team_pointer().lock()->getTeamTopScorer(),player1)==1){
                player1->get_team_pointer().lock()->setTeamTopScorer(player1);
            if(top_scorer==nullptr)
                top_scorer=player1;
            else {
                if (compare_players_by_Score(top_scorer, player1) == 1)// check if it new top scorer
                    top_scorer = player1;
            }
        }}
        total_players_counter++;//add 1 to total_players_counter
        player1->get_team_pointer().lock()->setPlayerCount(
                player1->get_team_pointer().lock()->getPlayerCount() + 1);//add 1 to team_players_count;
        if (player1->get_is_goalkeeper())
            player1->get_team_pointer().lock()->setGoalkeeperCount(player1->get_team_pointer().lock()->getGoalkeeperCount() +1);//if player is goalkeeper add 1 to team's goalkeeper count
        if (player1->get_team_pointer().lock()->getGoalkeeperCount() > 0 && player1->get_team_pointer().lock()->getPlayerCount() > 10) {//add team to current active teams if suitable
            shared_ptr<Team> left1(new Team(0, 0));
            shared_ptr<Team> right1(new Team(0, 0));


            current_active_teams.add(player1->get_team_pointer().lock(), &compare_teams_by_id,left1,right1);
            if(left1->getteamID() != player1->get_team_pointer().lock()->getteamID()) {
                player1->get_team_pointer().lock()->setglobal_left_closest_team(left1);
            }
            if(right1->getteamID() !=player1->get_team_pointer().lock()->getteamID()){
                player1->get_team_pointer().lock()->setglobal_right_closest_team(right1);
            }
            player1->get_team_pointer().lock()->getglobal_left_closest_team().lock()->setglobal_right_closest_team(player1->get_team_pointer().lock());
            player1->get_team_pointer().lock()->getglobal_right_closest_team().lock()->setglobal_left_closest_team(player1->get_team_pointer().lock());

        }
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
        if (!n1->getData()->get_team_pointer().lock()->remove_player_from_team_by_ID(player1))
            return StatusType::FAILURE;
        if (!n1->getData()->get_team_pointer().lock()->remove_player_from_team_by_Score(player1))
            return StatusType::FAILURE;
        if (!players_tree_by_score.remove(n1->getData(), &compare_players_by_Score))
            return StatusType::FAILURE;
      //check if top scorer for team or all players
        if (n1->getData()->get_team_pointer().lock()->getTeamTopScorer() == n1->getData()) {
            n1->getData()->get_team_pointer().lock()->setTeamTopScorer(n1->getData()->get_team_pointer().lock()->find_max_by_Score());
            if (top_scorer == n1->getData())
                top_scorer = players_tree_by_score.findMax();
        }
        //remove player from last tree and set nre team values
        std::shared_ptr<Player> tempplayer(new Player(0,0,0,0,0,0));
        tempplayer=n1->getData();
        if(!players_tree_by_id.remove(n1->getData(), &compare_players_by_ID))
            return StatusType::FAILURE;
        tempteam1.lock()->setPlayerCount(tempteam1.lock()->getPlayerCount()-1);
        tempteam1.lock()->setsum_of_player_score(tempteam1.lock()->getsum_of_player_score()-(tempplayer->get_goals_scored()-tempplayer->get_cards()));
        if(tempplayer->get_is_goalkeeper())
            tempteam1.lock()->setGoalkeeperCount(tempteam1.lock()->getGoalkeeperCount()-1);
        total_players_counter--;
        if((tempplayer->get_is_goalkeeper()&&tempteam1.lock()->getGoalkeeperCount()==0)||tempteam1.lock()->getPlayerCount()==10){
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
    try {
        std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, 0, 0));
        Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, &compare_players_by_ID);
        if (n1 == nullptr) {
            return StatusType::FAILURE;
        }
        if (!players_tree_by_score.remove(n1->getData(), &compare_players_by_Score))
            return StatusType::FAILURE;
        n1->getData()->set_global_left_closest(n1->getData()->get_global_right_closest());
        n1->getData()->set_global_right_closest(n1->getData()->get_global_left_closest());
        if (!n1->getData()->get_team_pointer().lock()->remove_player_from_team_by_Score(n1->getData()))
            return StatusType::FAILURE;
        n1->getData()->get_team_pointer().lock()->setsum_of_player_score(
                n1->getData()->get_team_pointer().lock()->getsum_of_player_score() + -cardsReceived + scoredGoals);
        n1->getData()->set_games_played(n1->getData()->get_games_played() + gamesPlayed);
        n1->getData()->set_goals_scored(n1->getData()->get_goals_scored() + scoredGoals);
        n1->getData()->set_cards(n1->getData()->get_cards() + cardsReceived);

        shared_ptr<Player> left(new Player(0,0,0,0,0,0));
        shared_ptr<Player> right(new Player(0,0,0,0,0,0));

        if (!players_tree_by_score.add(n1->getData(), &compare_players_by_Score,left,right))
            return StatusType::FAILURE;
        if(left->get_player_ID() != n1->getData()->get_player_ID()) {
            n1->getData()->set_global_left_closest(left);
        }
        if(right->get_player_ID() !=n1->getData()->get_player_ID()){
            n1->getData()->set_global_right_closest(right);
        }
        n1->getData()->get_global_left_closest().lock()->set_global_right_closest(n1->getData());
        n1->getData()->get_global_right_closest().lock()->set_global_left_closest(n1->getData());
        if (!n1->getData()->get_team_pointer().lock()->add_player_to_team_by_score(n1->getData()))
            return StatusType::FAILURE;
        if(compare_players_by_Score(n1->getData()->get_team_pointer().lock()->getTeamTopScorer(),n1->getData())==1){
                n1->getData()->get_team_pointer().lock()->setTeamTopScorer(n1->getData());
            if (compare_players_by_Score(top_scorer, n1->getData())==1)// check if it new top scorer
                top_scorer = n1->getData();
        }



        return StatusType::SUCCESS;
    }
    catch (std::bad_alloc){
        return StatusType::ALLOCATION_ERROR;
}}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0||teamId2<=0||teamId1==teamId2)
        return StatusType::INVALID_INPUT;
    try {
        std::shared_ptr<Team> team1(new Team(teamId1, 0));
        Node<shared_ptr<Team>> *n1 = current_active_teams.find(team1, &compare_teams_by_id);
        std::shared_ptr<Team> team2(new Team(teamId2, 0));
        Node<shared_ptr<Team>> *n2 = current_active_teams.find(team2, &compare_teams_by_id);
        if (n1 == nullptr || n2 == nullptr)
            return StatusType::FAILURE;
        n1->getData()->setAdditionalGamesPlayed(n1->getData()->getAdditionalGamesPlayed() + 1);
        n2->getData()->setAdditionalGamesPlayed(n2->getData()->getAdditionalGamesPlayed() + 1);
        if (n1->getData()->getTeamPoints() + n1->getData()->getsum_of_player_score() >
            n2->getData()->getTeamPoints() + n2->getData()->getsum_of_player_score()) {
            n1->getData()->setTeamPoints(n1->getData()->getTeamPoints() + 3);
        } else if (n1->getData()->getTeamPoints() + n1->getData()->getsum_of_player_score() <
                   n2->getData()->getTeamPoints() + n2->getData()->getsum_of_player_score()) {
            n2->getData()->setTeamPoints(n2->getData()->getTeamPoints() + 3);
        } else {
            n1->getData()->setTeamPoints(n1->getData()->getTeamPoints() + 1);
            n2->getData()->setTeamPoints(n2->getData()->getTeamPoints() + 1);
        }

        return StatusType::SUCCESS;
    }
    catch (std::bad_alloc){
        return StatusType::ALLOCATION_ERROR;
    }





    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId) {
    if (playerId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try {
        std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, 0, 0));
        Node<shared_ptr<Player>> *n1 = players_tree_by_id.find(player1, &compare_players_by_ID);
        if (n1 == nullptr)
            return output_t<int>(StatusType::FAILURE);
        int value = n1->getData()->get_games_played() +
                    n1->getData()->get_team_pointer().lock()->getAdditionalGamesPlayed();
        return output_t<int>(value);
    }
    catch (std::bad_alloc) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);}
}
output_t<int> world_cup_t::get_team_points(int teamId) {
    if (teamId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try {
        std::shared_ptr<Team> team1(new Team(teamId, 0));
        Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
        if (n1 == nullptr)
            return output_t<int>(StatusType::FAILURE);
        int value = n1->getData()->getTeamPoints();
        return output_t<int>(value);
    }
    catch (std::bad_alloc) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}
StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if(teamId1<=0||teamId2<=0||newTeamId<=0)
        return StatusType::INVALID_INPUT;
    try{
        std::shared_ptr<Team> team1(new Team(teamId1, 0));
        Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
        std::shared_ptr<Team> team2(new Team(teamId2, 0));
        Node<shared_ptr<Team>> *n2 = teams_tree.find(team2, &compare_teams_by_id);
        if (n1 == nullptr || n2 == nullptr)
            return StatusType::FAILURE;
        std::shared_ptr<Team> new_team(new Team(newTeamId,n1->getData()->getTeamPoints()+n2->getData()->getTeamPoints()));
        if(!teams_tree.add(new_team,&compare_teams_by_id))
            return StatusType::FAILURE;
        new_team->setsum_of_player_score(n1->getData()->getsum_of_player_score()+n2->getData()->getsum_of_player_score());
        new_team->setGoalkeeperCount(n1->getData()->getGoalkeeperCount()+n2->getData()->getGoalkeeperCount());
        new_team->setPlayerCount(n1->getData()->getPlayerCount()+n2->getData()->getPlayerCount());
        if(compare_players_by_Score((n1->getData()->getTeamTopScorer()),n2->getData()->getTeamTopScorer())==-1)
            new_team->setTeamTopScorer(n1->getData()->getTeamTopScorer());
        if(compare_players_by_Score((n1->getData()->getTeamTopScorer()),n2->getData()->getTeamTopScorer())==1)
            new_team->setTeamTopScorer(n2->getData()->getTeamTopScorer());
        shared_ptr<Player> *player_arr1_byId = new shared_ptr<Player>[n1->getData()->getPlayerCount()];
        shared_ptr<Player> *player_arr1_byScore = new shared_ptr<Player>[n1->getData()->getPlayerCount()];
        shared_ptr<Player> *player_arr2_byId = new shared_ptr<Player>[n2->getData()->getPlayerCount()];
        shared_ptr<Player> *player_arr2_byScore = new shared_ptr<Player>[n2->getData()->getPlayerCount()];
        shared_ptr<Player> *finalarray_byId = new shared_ptr<Player>[n1->getData()->getPlayerCount()+n2->getData()->getPlayerCount()];
        shared_ptr<Player> *finalarray_byScore = new shared_ptr<Player>[n1->getData()->getPlayerCount()+n2->getData()->getPlayerCount()];
        n1->getData()->treeToArrayInOrder_for_team_byId(player_arr1_byId,n1->getData()->getPlayerCount());
        n2->getData()->treeToArrayInOrder_for_team_byId(player_arr2_byId,n2->getData()->getPlayerCount());
        n1->getData()->treeToArrayInOrder_for_team_byscore(player_arr1_byScore,n1->getData()->getPlayerCount());
        n2->getData()->treeToArrayInOrder_for_team_byscore(player_arr2_byScore,n2->getData()->getPlayerCount());
        for(int i=0;i<n1->getData()->getPlayerCount();i++){
            player_arr1_byId[i]->set_games_played(player_arr1_byId[i]->get_games_played()+n1->getData()->getAdditionalGamesPlayed());
        }
        n1->getData()->setAdditionalGamesPlayed(0);
        for(int i=0;i<n2->getData()->getPlayerCount();i++){
            player_arr2_byScore[i]->set_games_played(player_arr2_byScore[i]->get_games_played()+n1->getData()->getAdditionalGamesPlayed());
        }
        n2->getData()->setAdditionalGamesPlayed(0);
        int i=0,j=0;
        while(i<n1->getData()->getPlayerCount()||j<n2->getData()->getPlayerCount()){
        if(i>=n1->getData()->getPlayerCount()){
            finalarray_byId[i+j]=player_arr2_byId[j];
            j++;
        }
        else if(j>=n2->getData()->getPlayerCount())
            finalarray_byId[i+j]=player_arr2_byId[i];
        else {
            if (compare_players_by_ID(player_arr1_byId[i], player_arr2_byId[j]) == -1)
                finalarray_byId[i + j] = player_arr2_byId[j];
            if (compare_players_by_ID(player_arr1_byId[i], player_arr2_byId[j]) == 1)
                finalarray_byId[i + j] = player_arr1_byId[i];
            i++;
            j++;


        }
        }
        i=0;
        j=0;
        while(i<n1->getData()->getPlayerCount()||j<n2->getData()->getPlayerCount()){
            if(i>=n1->getData()->getPlayerCount()){
                finalarray_byScore[i+j]=player_arr2_byScore[j];
                j++;
            }
            else if(j>=n2->getData()->getPlayerCount())
                finalarray_byScore[i+j]=player_arr2_byScore[i];
            else {
                if (compare_players_by_Score(player_arr1_byScore[i], player_arr2_byScore[j]) == -1)
                    finalarray_byScore[i + j] = player_arr2_byScore[j];
                if (compare_players_by_Score(player_arr1_byScore[i], player_arr2_byScore[j]) == 1)
                    finalarray_byScore[i + j] = player_arr1_byScore[i];
                i++;
                j++;


            }
        }
        if(!new_team->create_tree_from_array_by_Id(finalarray_byId,(n1->getData()->getPlayerCount()+n2->getData()->getPlayerCount())))
            return StatusType::FAILURE;
        if(!new_team->create_tree_from_array_by_Score(finalarray_byScore,n2->getData()->getPlayerCount()+n2->getData()->getPlayerCount()))
            return StatusType::FAILURE;

        if(new_team->getPlayerCount()>10&&new_team->getGoalkeeperCount()>0) {
            shared_ptr<Team> left(new Team(0,0));
            shared_ptr<Team> right(new Team(0,0));
            current_active_teams.add(new_team, compare_teams_by_id, left,right);
            if(left->getteamID() != new_team->getteamID()) {
                new_team->setglobal_left_closest_team(left);
            }
            if(right->getteamID() !=new_team->getteamID()){
                new_team->setglobal_right_closest_team(right);
            }
            new_team->getglobal_right_closest_team().lock()->setglobal_left_closest_team(new_team);
            new_team->getglobal_left_closest_team().lock()->setglobal_right_closest_team(new_team);
        }
        if(n1->getData()->getPlayerCount()>10&&n1->getData()->getGoalkeeperCount()>0) {
            current_active_teams.remove(n1->getData(), &compare_teams_by_id);
            n1->getData()->getglobal_right_closest_team().lock()->setglobal_left_closest_team(n1->getData()->getglobal_left_closest_team());
            n1->getData()->getglobal_left_closest_team().lock()->setglobal_right_closest_team(n1->getData()->getglobal_right_closest_team());
        }
        if(n2->getData()->getPlayerCount()>10&&n2->getData()->getGoalkeeperCount()>0) {
            current_active_teams.remove(n1->getData(), &compare_teams_by_id);
            n2->getData()->getglobal_right_closest_team().lock()->setglobal_left_closest_team(n2->getData()->getglobal_left_closest_team());
            n2->getData()->getglobal_left_closest_team().lock()->setglobal_right_closest_team(n2->getData()->getglobal_right_closest_team());
        }
        n1->getData()->setPlayerCount(0);
        n2->getData()->setPlayerCount(0);
        teams_tree.remove(n1->getData(),&compare_teams_by_id);
        teams_tree.remove(n2->getData(),&compare_teams_by_id);

    }
    catch (std::bad_alloc) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId) {
    if (teamId == 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try {
        if (teamId < 0)
            return output_t<int>(top_scorer->get_player_ID());
        if (teamId > 0) {
            std::shared_ptr<Team> team1(new Team(teamId, 0));
            Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
            if (n1 == nullptr)
                return output_t<int>(StatusType::FAILURE);
            return output_t<int>(n1->getData()->getTeamTopScorer()->get_player_ID());

        }
    }
    catch(std::bad_alloc){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return output_t<int>(StatusType::FAILURE);
}

output_t<int> world_cup_t::get_all_players_count(int teamId) {
    if (teamId == 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try {
        if (teamId < 0)
            return output_t<int>(total_players_counter);
        if (teamId > 0) {
            std::shared_ptr<Team> team1(new Team(teamId, 0));
            Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
            if (n1 == nullptr)
                return output_t<int>(StatusType::FAILURE);
            return output_t<int>(n1->getData()->getPlayerCount());
        }
    }
    catch (std::bad_alloc) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return output_t<int>(StatusType::FAILURE);
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(output==nullptr||teamId==0)
        return StatusType::INVALID_INPUT;
    try {
        if(teamId>0) {
            std::shared_ptr<Team> team1(new Team(teamId, 0));
            Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
            if (n1 == nullptr || n1->getData()->getPlayerCount() == 0)
                return StatusType::FAILURE;
            shared_ptr<Player> *player_arr = new shared_ptr<Player>[n1->getData()->getPlayerCount()];
            n1->getData()->treeToArrayInOrder_for_team_byscore(player_arr, n1->getData()->getPlayerCount());
            for (int i = 0; i < n1->getData()->getPlayerCount(); i++)
                output[i] = player_arr[i]->get_player_ID();
        }
            if(teamId>0)
        {
            if(total_players_counter==0)
                return StatusType::FAILURE;
            shared_ptr<Player>* player_arr=new shared_ptr<Player>[total_players_counter];
            players_tree_by_score.treeToArrayInOrder(player_arr,total_players_counter);
            for(int i=0; i<total_players_counter;i++) {
                output[i] = player_arr[i]->get_player_ID();
            }
        }


        return StatusType::SUCCESS;
        }
    catch (std::bad_alloc) {
        return StatusType::ALLOCATION_ERROR;
    }

}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	if(playerId<=0||teamId<=0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try{
        std::shared_ptr<Team> team1(new Team(teamId, 0));
        Node<shared_ptr<Team>> *n1 = teams_tree.find(team1, &compare_teams_by_id);
        std::shared_ptr<Player> player1(new Player(playerId, 0, 0, 0, teamId, 0));
        if (n1 == nullptr )
            return StatusType::FAILURE;
        Node<shared_ptr<Player>> *n2 =n1->getData()->find_player_by_ID(player1);
        if (n2 == nullptr|| total_players_counter == 1)
            return StatusType::FAILURE;
        if(n2->getData()->get_global_right_closest().lock()==nullptr)
            return output_t<int>(n2->getData()->get_global_left_closest().lock()->get_player_ID());
        if(n2->getData()->get_global_left_closest().lock()==nullptr)
            return output_t<int>(n2->getData()->get_global_right_closest().lock()->get_player_ID());
        if((n2->getData()->get_global_right_closest().lock()->get_goals_scored()-n2->getData()->get_goals_scored())<(n2->getData()->get_goals_scored()-n2->getData()->get_global_left_closest().lock()->get_goals_scored()))
            return output_t<int>(n2->getData()->get_global_right_closest().lock()->get_player_ID());
        if((n2->getData()->get_global_right_closest().lock()->get_goals_scored()-n2->getData()->get_goals_scored())>(n2->getData()->get_goals_scored()-n2->getData()->get_global_left_closest().lock()->get_goals_scored()))
            return output_t<int>(n2->getData()->get_global_left_closest().lock()->get_player_ID());
        if((n2->getData()->get_global_right_closest().lock()->get_cards()-n2->getData()->get_cards())>(n2->getData()->get_cards()-n2->getData()->get_global_left_closest().lock()->get_cards()))
            return output_t<int>(n2->getData()->get_global_right_closest().lock()->get_player_ID());
        if((n2->getData()->get_global_right_closest().lock()->get_cards()-n2->getData()->get_cards())<(n2->getData()->get_cards()-n2->getData()->get_global_left_closest().lock()->get_cards()))
            return output_t<int>(n2->getData()->get_global_right_closest().lock()->get_player_ID());
        if((n2->getData()->get_global_right_closest().lock()->get_player_ID()-n2->getData()->get_player_ID())<(n2->getData()->get_player_ID()-n2->getData()->get_global_left_closest().lock()->get_player_ID()))
            return output_t<int>(n2->getData()->get_global_right_closest().lock()->get_player_ID());
        if((n2->getData()->get_global_right_closest().lock()->get_player_ID()-n2->getData()->get_player_ID())>(n2->getData()->get_player_ID()-n2->getData()->get_global_left_closest().lock()->get_player_ID()))
            return output_t<int>(n2->getData()->get_global_left_closest().lock()->get_player_ID());
        return output_t<int>(n2->getData()->get_global_right_closest().lock()->get_player_ID());

    }
    catch(std::bad_alloc){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId) {
    if (minTeamId < 0 || maxTeamId < 0 || minTeamId > maxTeamId)
        return output_t<int>(StatusType::INVALID_INPUT);
    try {
        std::shared_ptr<Team> minteam(new Team(minTeamId, 0));
        std::shared_ptr<Team> maxteam(new Team(maxTeamId, 0));
        std::shared_ptr<Team> first_team(new Team(0, 0));
        std::shared_ptr<Team> last_team(new Team(0, 0));
        std::shared_ptr<Team> current_team(new Team(0, 0));


        first_team = *current_active_teams.findMinBiggerThanX(minteam, &compare_teams_by_id);
        last_team = *current_active_teams.findMaxLowerThanX(maxteam, &compare_teams_by_id);
        current_team = first_team;
        int r = 0;
        while (current_team != last_team && current_team != nullptr) {
            current_team = current_team->getglobal_right_closest_team().lock();
            r++;
        }
        if(r==0)
            return output_t<int>(StatusType::FAILURE);
        current_team = first_team;
        game_teams *playing_teams = new game_teams[r];
        for (int i = 0; i < r; i++) {
            playing_teams[i].game_points = current_team->getTeamPoints() + current_team->getsum_of_player_score();
            playing_teams[i].id = current_team->getteamID();
            current_team = current_team->getglobal_right_closest_team().lock();
        }

        int j = 0, i = 0, x = r;
        while (x > 0) {
            i = 0;
            if (x % 2 == 0) {
                while (i < x) {
                    if (playing_teams[i].game_points > playing_teams[i + 1].game_points)
                        playing_teams[j].id = playing_teams[i].id;
                    else if (playing_teams[i].game_points < playing_teams[i + 1].game_points)
                        playing_teams[j].id = playing_teams[i + 1].id;
                    else if (playing_teams[i].id > playing_teams[i + 1].id)
                        playing_teams[j].id = playing_teams[i].id;
                    else
                        playing_teams[j].id = playing_teams[i + 1].id;

                    playing_teams[j].game_points= playing_teams[i].game_points+playing_teams[i + 1].game_points+3;


                    i = i + 2;
                    j++;
                }
            } else {
                while (i < x - 1) {
                    if (playing_teams[i].game_points > playing_teams[i + 1].game_points)
                        playing_teams[j].id = playing_teams[i].id;
                    else if (playing_teams[i].game_points < playing_teams[i + 1].game_points)
                        playing_teams[j].id = playing_teams[i + 1].id;
                    else if (playing_teams[i].id > playing_teams[i + 1].id)
                        playing_teams[j].id = playing_teams[i].id;
                    else
                        playing_teams[j].id = playing_teams[i + 1].id;

                    playing_teams[j].game_points= playing_teams[i].game_points+playing_teams[i + 1].game_points+3;

                    i = i + 2;
                    j++;
                }
                playing_teams[j] = playing_teams[i];

            }
            x = x / 2;

        }
        int winner=playing_teams[0].id;
        delete[] playing_teams;
        return output_t<int>(playing_teams[0].id);
    }
    catch (bad_alloc) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

}
