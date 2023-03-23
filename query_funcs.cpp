#include "query_funcs.h"



void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);

    stringstream query;

    query<<"INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG," 
        <<"PPG, RPG, APG, SPG, BPG) VALUES ("<<team_id<<", "<<jersey_num<<", "
        <<W.quote(first_name)<<", "<<W.quote(last_name)<<", "<<mpg<<", "<<ppg
        <<", "<<rpg<<", "<<apg<<", "<<spg<<", "<<bpg<<");";

    W.exec(query.str());

    W.commit();
}

void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);

    stringstream query;

    query << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
        <<W.quote(name)<<", "<<state_id<<", "<<color_id<<", "<<wins<<", "<<losses<<");";

    W.exec(query.str());

    W.commit();
}



void add_state(connection *C, string name)
{
    work W(*C);

    stringstream query;

    query << "INSERT INTO STATE (NAME) VALUES ("<<W.quote(name)<<");";

    W.exec(query.str());

    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);

    stringstream query;

    query << "INSERT INTO COLOR (NAME) VALUES ("<<W.quote(name)<<");";

    W.exec(query.str());

    W.commit();
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    try {

        work W(*C);

        stringstream query;

        query << "SELECT * FROM player "
                 "WHERE (CASE WHEN " << use_mpg << " = 1 THEN MPG >= " << min_mpg << " AND MPG <= " << max_mpg << " ELSE true END) AND "
                 "(CASE WHEN " << use_ppg << " = 1 THEN PPG >= " << min_ppg << " AND PPG <= " << max_ppg << " ELSE true END) AND "
                 "(CASE WHEN " << use_rpg << " = 1 THEN RPG >= " << min_rpg << " AND RPG <= " << max_rpg << " ELSE true END) AND "
                 "(CASE WHEN " << use_apg << " = 1 THEN APG >= " << min_apg << " AND APG <= " << max_apg << " ELSE true END) AND "
                 "(CASE WHEN " << use_spg << " = 1 THEN SPG >= " << min_spg << " AND SPG <= " << max_spg << " ELSE true END) AND "
                 "(CASE WHEN " << use_bpg << " = 1 THEN BPG >= " << min_bpg << " AND BPG <= " << max_bpg << " ELSE true END);";

        
        result R = W.exec(query.str());

        cout<<"PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;

        for (const auto &row : R) {
            for (const auto &field : row) {
                std::cout << field.as<string>() << " ";
            }
            cout << endl;
        }

        W.commit();
    } 
    catch (const std::exception &e) {
        cerr << e.what() << endl;
    }

}


void query2(connection *C, string team_color)
{
   try {
        work W(*C);

        stringstream query;
        query << "SELECT T.NAME "
                 "FROM TEAM T "
                 "JOIN COLOR C ON T.COLOR_ID = C.COLOR_ID "
                 "WHERE C.NAME = " << W.quote(team_color) << ";";

        result R = W.exec(query.str());

        cout<<"NAME"<<endl;
        for (const auto &row : R) {
            for (const auto &field : row) {
                cout << field.as<string>() << endl;
            }
        }

        W.commit();
    } 
    catch (const std::exception &e) {
        cerr << e.what() << endl;
    }

}


void query3(connection *C, string team_name)
{
    try {
        work W(*C);

        std::ostringstream query;
        
        query << "SELECT P.FIRST_NAME, P.LAST_NAME "
                 "FROM PLAYER P "
                 "JOIN TEAM T ON P.TEAM_ID = T.TEAM_ID "
                 "WHERE T.NAME = " << W.quote(team_name) << " "
                 "ORDER BY P.PPG DESC;";

        result R = W.exec(query.str());

        cout<<"FIRST_NAME LAST_NAME"<<endl;

        for (const auto &row : R) {
            for (const auto &field : row) {
                cout << field.as<string>() << " ";
            }
            cout << endl;
        }

        W.commit();
    } 
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    try {
        work W(*C);

        ostringstream query;
        query << "SELECT P.UNIFORM_NUM, P.FIRST_NAME, P.LAST_NAME "
                 "FROM PLAYER P "
                 "JOIN TEAM T ON P.TEAM_ID = T.TEAM_ID "
                 "JOIN STATE S ON T.STATE_ID = S.STATE_ID "
                 "JOIN COLOR C ON T.COLOR_ID = C.COLOR_ID "
                 "WHERE S.NAME = " << W.quote(team_state) << " AND C.NAME = " << W.quote(team_color) << ";";

        result R = W.exec(query.str());

        cout<<"UNIFORM_NUM FIRST_NAME LAST_NAME"<<endl;

        for (const auto &row : R) {
            for (const auto &field : row) {
                std::cout << field.as<string>() << " ";
            }
            std::cout << std::endl;
        }

        W.commit();
    } 
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


void query5(connection *C, int num_wins)
{
    try {
        work W(*C);

        stringstream query;
        query << "SELECT P.FIRST_NAME, P.LAST_NAME, T.NAME, T.WINS "
                 "FROM PLAYER P "
                 "JOIN TEAM T ON P.TEAM_ID = T.TEAM_ID "
                 "WHERE T.WINS > " << num_wins << ";";

        result R = W.exec(query.str());

        cout<<"FIRST_NAME LAST_NAME NAME WINS"<<endl;
        for (const auto &row : R) {
            for (const auto &field : row) {
                std::cout << field.as<string>() << " ";
            }
            std::cout << std::endl;
        }

        W.commit();
    } 
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}
