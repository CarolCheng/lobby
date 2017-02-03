#include <iostream>
#include <list>
#include "pq_conn_pool.h"
#include "crow.h"

struct player
{
	int id;
	std::string username; 
	std::string password;
	std::string userdata;
};

// execute a transactional SQL
bool executeSQL(const std::string sql)
{
	bool bsuccess = false; 
	auto instance = pq_conn_pool::instance();
	auto dbconn = instance->burrow();
	try 
	{
		if (!dbconn->is_open()) {
			std::cout << "Open database failed"  << std::endl;
			return false;
		}
		// Execute SQL query on PostgreSQL
		pqxx::work work(*dbconn);
		pqxx::result res( work.exec(sql));
		work.commit();  
		
		if (res.affected_rows()>0)
			bsuccess = true;
	} catch (const std::exception &ex) {
		std::cout<< "executeSQL failed: " << ex.what() << std::endl;
		bsuccess = false;
	}
	instance->unburrow(dbconn);
	return bsuccess;
}
int add_user(player &user) 
{
	auto instance = pq_conn_pool::instance();
	auto dbconn = instance->burrow();
	std::string insertsql = "INSERT INTO players (username, password, userdata) VALUES \
		('"+ user.username +"','"+ user.password + "'," 
		+ user.userdata +") RETURNING id;";
	// Execute SQL query on PostgreSQL
	pqxx::work work(*dbconn);
	pqxx::result res( work.exec(insertsql));
	work.commit();  
	// Getting the id of a newly inserted row (user data)
	for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
		user.id = c[0].as<int>();
	}
	instance->unburrow(dbconn);
	return (user.id > 0) ? 201 : 404;
}
int get_user(const int id, player &user) 
{
	auto instance = pq_conn_pool::instance();
	auto dbconn = instance->burrow();
	std::string selectsql = "select * from players where ID = " 
		+ std::to_string(id);
	int nrow = -1; 
	try {
		// Execute SQL query on PostgreSQL
		pqxx::nontransaction work(*dbconn);
		pqxx::result res( work.exec(selectsql));
		// Getting the id of a newly inserted row (user data)
		for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
			user.id = c[0].as<int>();
			user.username = c[1].as<std::string>();
			user.password = c[2].as<std::string>();
			user.userdata = c[3].as<std::string>();
		}
		nrow = res.size();
	} catch (const std::exception &ex) {
		nrow = -1;
		std::cout<< "Select failed: " << ex.what() << std::endl;
	}
	instance->unburrow(dbconn);
	return (nrow > 0) ? 200 : 404;
}
int get_users(std::list<player> &lst_user) 
{
	auto instance = pq_conn_pool::instance();
	auto dbconn = instance->burrow();
	std::string selectsql = "select * from players"; 
	int nrow = -1; 
	try {
		// Execute SQL query on PostgreSQL
		pqxx::nontransaction work(*dbconn);
		pqxx::result res( work.exec(selectsql));
		// Getting the id of a newly inserted row (user data)
		for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
			player user;
			user.id = c[0].as<int>();
			user.username = c[1].as<std::string>();
			user.password = c[2].as<std::string>();
			user.userdata = c[3].as<std::string>();
			lst_user.push_back(user);
		}
		nrow = res.size();
	} catch (const std::exception &ex) {
		nrow = -1;
		std::cout<< "Select failed: " << ex.what() << std::endl;
	}
	instance->unburrow(dbconn);
	return (nrow > 0) ? 200 : 404;
}

int update_user(const int id, const std::string username, const std::string password , const std::string userdata) 
{
	std::string updatesql = "Update players set username = '"+ username +"'\
		, password = '"+ password +"' \
		, userdata = '"+ userdata +"' \
		WHERE id = " + std::to_string(id) + ";";
	// 200: successful update 404: not found
	return (executeSQL(updatesql)) ? 200 : 404;
}
int delete_user(const int id)
{
	std::string deletesql = "DELETE from players where ID = " + std::to_string(id);
	// 200: successful update 404: not found
	return (executeSQL(deletesql)) ? 200 : 404;
}
int main(int argc, char* argv[])
{
	std::string sql = "CREATE TABLE IF NOT EXISTS players( \
		id 				serial		primary key, \
		username		TEXT    	NOT NULL, \
		password 		TEXT    	NOT NULL, \
		userdata		TEXT		NOT NULL);";
	bool is_ready = executeSQL(sql);

	crow::SimpleApp	app;
	CROW_ROUTE(app, "/")([is_ready](){
		if(is_ready)
			return "Hello world!";
		else
			return "Connection to PostgreSQL database failed.";
	});

	CROW_ROUTE(app, "/api/users")
		.methods("POST"_method, "GET"_method)
	([](const crow::request &req){
		if(req.method == "POST"_method) {
			auto msg = crow::json::load(req.body);
			if(!msg) 
				return crow::response(400);
			player user = {-1, msg["username"].s()
				, msg["password"].s(), msg["userdata"].s()};
			int response = add_user(user);
			if(response != 201) // Not found
				return crow::response(response);
			crow::json::wvalue result = msg;
			result["id"] = user.id; 
			auto res = crow::response(response, result);// Created 
			res.set_header("Location", req.url + "/" + std::to_string(user.id));
			return res;
		} else if(req.method == "GET"_method) {
			std::list<player> lst_users;
			int response = get_users(lst_users);
			if(response != 200) // Not found
				return crow::response(response);
			crow::json::wvalue result;
			unsigned int index = 0;
			for (auto iter = lst_users.begin(); iter != lst_users.end(); iter++, index++) {
				result[index]["id"] = iter->id;
				result[index]["username"] = iter->username;
				result[index]["password"] = iter->password;
				result[index]["userdata"] = iter->userdata;
			}
			return crow::response(response, result);  
		}
		return crow::response(400);
	});
	
	CROW_ROUTE(app, "/api/users/<int>")
		.methods("DELETE"_method, "PUT"_method, "GET"_method)
	([=](const crow::request &req, int id){
		if(req.method == "DELETE"_method) {
			return crow::response(delete_user(id));
		} else if(req.method == "PUT"_method) {
			auto msg = crow::json::load(req.body);
			// Bad request : requested data is not JSON
			if(!msg) 
				return crow::response(400);
			// Bad request : one of JSON data was not in the correct format
			if(!msg.has("username") || msg["username"].t() != crow::json::type::String)
				return crow::response(400);
			if(!msg.has("password") || msg["password"].t() != crow::json::type::String)
				return crow::response(400);
			if(!msg.has("userdata") || msg["userdata"].t() != crow::json::type::String)
				return crow::response(400);
			crow::json::wvalue result = msg;
			result["id"] = id; 
			// Update a user data in PostgreSQ
			return crow::response( update_user(id, msg["username"].s(), msg["password"].s()
				, msg["userdata"].s()), result);
		} else if(req.method == "GET"_method) {
			player user;
			int response = get_user(id, user);
			if(response != 200) // Not found
				return crow::response(response);
			crow::json::wvalue result;
			result["id"] = user.id;
			result["username"] =  user.username;
			result["password"] =  user.password;
			result["userdata"] =  user.userdata;
			return crow::response(response, result);  
		}
		return crow::response(400);
	});
	
	app.port(18080).multithreaded().run();
	pq_conn_pool::instance()->release_pool();
}