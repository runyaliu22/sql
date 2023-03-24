#include <string>
#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

#include "query_funcs.h"


void addPlayerTable(const char *fileName, connection *C){

  int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
  double spg, bpg;
  string line, first_name, last_name;
  ifstream file(fileName);


  if(file.is_open()){

    while(getline(file,line)){

      stringstream ss;
      ss<<line;
      ss>>player_id>>team_id>>uniform_num>>first_name>>last_name>>mpg>>ppg>>rpg>>apg>>spg>>bpg;

      add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
    }

    file.close();
    return;

  }

  else{
    cerr<<"Unable to open player" <<endl;
    return;
  }
}

void addTeamTable(const char *fileName, connection *C){

  int team_id,state_id,color_id,wins,losses;
  string line, team_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){

      stringstream ss;
      ss<<line;
      ss>>team_id>>team_name>>state_id>>color_id>>wins>>losses;

      add_team(C,team_name,state_id,color_id,wins,losses);
    }
    file.close();
    return;
  }
  else{
    cerr<<"Unable to open the team!"<<endl;
    return;
  }
}

void addStateTable(const char *fileName, connection *C){
  int state_id;
  string line, state_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){

      stringstream ss;
      ss<<line;
      ss>>state_id>>state_name;

      add_state(C,state_name);
    }
    file.close();
    return;
  }
  else{
    cerr<<"Unable to open state!"<<endl;
    return;
  }
}



void addColorTable(const char *fileName, connection *C){
    
  int color_id;
  string line, color_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){
      stringstream ss;
      ss<<line;
      ss>>color_id>>color_name;

      add_color(C,color_name);
    }
    file.close();
    return;
  }
  else{
    cerr<<"Unable to open color!"<<endl;
    return;
  }
}
