#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"
#include "tools.hpp"

using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  //create tables(4)
  work W(*C);

  W.exec("DROP TABLE IF EXISTS PLAYER;");
  
  W.exec("CREATE TABLE PLAYER("
    "PLAYER_ID  SERIAL PRIMARY KEY,"
    "TEAM_ID    INT NOT NULL,"
    "UNIFORM_NUM INT NOT NULL,"
    "FIRST_NAME TEXT NOT NULL,"
    "LAST_NAME  TEXT NOT NULL,"
    "MPG        INT NOT NULL,"
    "PPG        INT NOT NULL,"
    "RPG        INT NOT NULL,"
    "APG        INT NOT NULL,"
    "SPG        NUMERIC(2,1) NOT NULL,"
    "BPG        NUMERIC(2,1) NOT NULL"
    ");");

  //W.commit();


  //work W(*C);

  W.exec("DROP TABLE IF EXISTS TEAM;");

  W.exec("CREATE TABLE TEAM ("
   "TEAM_ID SERIAL PRIMARY KEY,"
   "NAME VARCHAR(50),"
   "STATE_ID INT,"
   "COLOR_ID INT,"
   "WINS INT,"
   "LOSSES INT"
");");


  W.exec("DROP TABLE IF EXISTS STATE;");
  W.exec("CREATE TABLE STATE ("
   "STATE_ID SERIAL PRIMARY KEY,"
   "NAME VARCHAR(2)"
");");


  W.exec("DROP TABLE IF EXISTS COLOR;");
  W.exec("CREATE TABLE COLOR ("
   "COLOr_ID SERIAL PRIMARY KEY,"
   "NAME TEXT NOT NULL"
");");



    W.commit();   
    

  //add entries


  // W.exec("COPY player (team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg) FROM 'player.txt';");

  //W.exec("INSERT INTO PLAYER VALUES(1, 1, 1,'Jerome', 'Robinson', 34, 19, 4, 3, 1.7, 0.4);");

    addPlayer("player.txt", C);

    addTeam("team.txt", C);

    addState("state.txt", C);

    addColor("color.txt", C);


    exercise(C);

  //Close database connection

  C->disconnect();

  return 0;
}


