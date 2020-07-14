#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include<string.h>
#include <sqlite3.h>

#define COLS_IN_USERS 4
#define COLS_IN_MESSAGES 3
#define HISTORY_DEPTH 50

typedef struct s_user {
	const char *user_login;
	const char *user_password;
	const char *user_nickname;
} t_user;

typedef struct s_message {
	const char *user_nickname;
	time_t msg_time;
	const char *msg_body;
} t_message;

void db_open(sqlite3 **db, const char *db_name);
void db_add_table(sqlite3 **db, const char *sql_stmt);
void init_users(sqlite3 **db);
void init_messages(sqlite3 **db);
void db_init(sqlite3 **db);
bool db_add_user(sqlite3 **db, t_user new_user);
bool db_add_message(sqlite3 **db, t_message new_message);
void db_print_users(sqlite3 **db);
t_message *db_get_history(sqlite3 **db, int depth, int *fact_depth);
void clear_history(t_message **history, int fact_depth);

int main() {
	sqlite3 *db;
    const char *db_name = "test.db";
	
	db_open(&db, db_name);
	db_init(&db);

	t_user new_user1 = {"omisakov", "123-sqlboss", "Lepicus"};
	t_user new_user2 = {"Bogdan-777", "passw_123", "Supervisor"};
	t_user new_user3 = {"NikitosUA", "Qwerty987", "Maslenok"};
	t_user new_user4 = {"V_Kniaziev", "Warshipper", "Vled"};

	db_add_user(&db, new_user1);
	db_add_user(&db, new_user2);
	db_add_user(&db, new_user3);
	db_add_user(&db, new_user4);

	db_print_users(&db);

	t_message new_message1 = {"Lepicus", time(NULL), "Hi, all, it's Lepicus"};
	t_message new_message2 = {"Maslenok", time(NULL), "Hi, all, it's Maslenok"};
	t_message new_message3 = {"Vled", time(NULL), "Hi, all, it's Vled"};
	t_message new_message4 = {"Supervisor", time(NULL), "Hi, all, it's Supervisor"};

	db_add_message(&db, new_message1);
	db_add_message(&db, new_message2);
	db_add_message(&db, new_message3);
	db_add_message(&db, new_message4);

	int fact_depth = 0;
	t_message *msg_history = db_get_history(&db, HISTORY_DEPTH, &fact_depth);

	for (int i = 0; i < fact_depth; i++) {
		printf("%s; %ld; %s\n", 
			msg_history[i].user_nickname,
			msg_history[i].msg_time,
			msg_history[i].msg_body);
	}
	// ============ example ===============
	// const char* sql =
	// 	"DROP TABLE price_list;"
	// 	"CREATE TABLE price_list ( item TEXT, price INTEGER)";
	// while ( *sql ) {
	// 	sqlite3_stmt* statement;
	// 	sqlite3_prepare_v2(db, sql, -1, &statement, &sql);
	// 	sqlite_step(statement);
	// 	sqlite_finalize(statement);
	// }
	// ======== end of example ============
	clear_history(&msg_history, fact_depth);
	sqlite3_close(db);
	msg_history = NULL;

	system("leaks db");
	return EXIT_SUCCESS;
}

void db_open(sqlite3 **db, const char *db_name) {
	if (sqlite3_open(db_name, db) != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));

		exit(1);
	}
}

void db_add_table(sqlite3 **db, const char *sql_stmt) {
	sqlite3_stmt *res;
	
	if (sqlite3_prepare_v2(*db, sql_stmt, -1, &res, 0) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
		exit(1);
	}
	if (sqlite3_step(res) != SQLITE_DONE) {
    	fprintf(stderr, "Error creating table: %s\n", sqlite3_errmsg(*db));
    	sqlite3_close(*db);
		exit(1);
	}
	sqlite3_finalize(res);
}

void init_users(sqlite3 **db) {
	const char *sql_stmt = "CREATE TABLE IF NOT EXISTS users ("
			   "user_id INTEGER PRIMARY KEY,"
			   "user_login TEXT NOT NULL UNIQUE,"
			   "user_password TEXT NOT NULL,"
			   "user_nickname TEXT NOT NULL UNIQUE"
			   ");";
	
	db_add_table(db, sql_stmt);
}

void init_messages(sqlite3 **db) {
	const char *sql_stmt = "CREATE TABLE IF NOT EXISTS messages ("
			   "user_nickname TEXT NOT NULL,"
			   "msg_time INTEGER NOT NULL,"
			   "msg_body TEXT NOT NULL"
			   ");";
	
	db_add_table(db, sql_stmt);
}

void db_init(sqlite3 **db) {
	init_users(db);
	init_messages(db);
}

bool db_add_user(sqlite3 **db, t_user new_user) {
	sqlite3_stmt *res;
	const char *sql_stmt = "INSERT INTO "
	                       "users (user_login, user_password, user_nickname) "
	                       "VALUES (?1, ?2, ?3);";

	if (sqlite3_prepare_v2(*db, sql_stmt, -1, &res, 0) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(*db));
		return false;
	}

	sqlite3_bind_text(res, 1, new_user.user_login, -1, SQLITE_STATIC);
	sqlite3_bind_text(res, 2, new_user.user_password, -1, SQLITE_STATIC);
	sqlite3_bind_text(res, 3, new_user.user_nickname, -1, SQLITE_STATIC);

	if (sqlite3_step(res) != SQLITE_DONE) {
		fprintf(stderr, "Error inserting data: %s\n", sqlite3_errmsg(*db));
		return false;
	}
	sqlite3_finalize(res);
	return true;
}

bool db_add_message(sqlite3 **db, t_message new_message) {
	sqlite3_stmt *res;
	const char *sql_stmt = "INSERT INTO "
	                       "messages (user_nickname, msg_time, msg_body) "
	                       "VALUES (?1, ?2, ?3);";

	if (sqlite3_prepare_v2(*db, sql_stmt, -1, &res, 0) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(*db));
		return false;
	}

	sqlite3_bind_text(res, 1, new_message.user_nickname, -1, SQLITE_STATIC);
	sqlite3_bind_int(res, 2, new_message.msg_time);
	sqlite3_bind_text(res, 3, new_message.msg_body, -1, SQLITE_STATIC);

	if (sqlite3_step(res) != SQLITE_DONE) {
		fprintf(stderr, "Error inserting data: %s\n", sqlite3_errmsg(*db));
		return false;
	}
	sqlite3_finalize(res);
	return true;
}

void db_print_users(sqlite3 **db) {
	sqlite3_stmt *res;
	const char *sql_stmt = "SELECT t.* FROM users AS t;";

	if (sqlite3_prepare_v2(*db, sql_stmt, -1, &res, 0) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(*db));
		exit(1);
	}

	printf("id\tlogin\t\tpassword\tnickname\n");
	while (sqlite3_step(res) == SQLITE_ROW) {
		for (int i = 0; i < COLS_IN_USERS; i++) {
			printf("%s", sqlite3_column_text(res, i));
			if (i < COLS_IN_USERS - 1) {
				printf("\t");
			}
		}
		printf("\n");
	}
	sqlite3_finalize(res);
}

t_message *db_get_history(sqlite3 **db, int depth, int *fact_depth) {
	int i = 0;
	t_message *history = (t_message *)malloc(sizeof(t_message) * depth);
	sqlite3_stmt *res;
	const char *sql_stmt = "SELECT user_nickname, msg_time, msg_body "
	                       "FROM messages "
	                       "ORDER BY msg_time DESC "
	                       "LIMIT ?1";

	if (sqlite3_prepare_v2(*db, sql_stmt, -1, &res, 0) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(*db));
		return false;
	}
	sqlite3_bind_int(res, 1, depth);
   	while (sqlite3_step(res) == SQLITE_ROW) {
		history[i].user_nickname = strdup((const char*)sqlite3_column_text(res, 0));
		history[i].msg_time = sqlite3_column_int(res, 1);
		history[i].msg_body = strdup((const char*)sqlite3_column_text(res, 2));
		++i;
	}
	sqlite3_finalize(res);
	*fact_depth = i;
	return history;
}

void clear_history(t_message **history, int fact_depth) {
	t_message *temp = *history;
	for (int i = 0; i < fact_depth; i++) {
		free((void *)temp[i].user_nickname);
		free((void *)temp[i].msg_body);
	}
	free(temp);
	*history = NULL;
}
