#include "server.h"
#define Cr_T_Users1 "CREATE TABLE Users(Login TEXT, Password TEXT, "
#define Cr_T_Users2 "Nick TEXT, AvaPath TEXT, Chats TEXT);"
#define INS_Users_S "INSERT INTO Users(Login, Password, Nick, AvaPath) VALUES ('"
#define INS_Users_F "');"

void mx_db_set_user_info(t_user_info *user) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    
    sql = mx_strcat(sql, INS_Users_S);
    sql = mx_strcat(sql, mx_strcat(mx_strtrim(user->login),"','"));
    sql = mx_strcat(sql, mx_strcat(mx_strtrim(user->password),"','"));
    sql = mx_strcat(sql, mx_strcat(mx_strtrim(user->nickname),"','"));
    sql = mx_strcat(sql, mx_strcat(mx_strtrim(user->ava_img_path),""));
    sql = mx_strcat(sql, INS_Users_F);
    mx_printstr(sql);
    mx_printstr("\n");
    if (user->login || db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

void mx_db_set_tables(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, Cr_T_Users1);
    sql = mx_strcat(sql, Cr_T_Users2);
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

void mx_db_set_nickname(char *login, char *new_name) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "UPDATE Users SET Nick = '");
    sql = mx_strcat(sql, new_name);
    sql = mx_strcat(sql, "' WHERE Login = '");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "' ;");
    if (login || db || err_msg || rc || new_name ) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return;
}

void mx_db_add_msg(char* chat_name,char *name, char *msg) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    
    sql = mx_strcat(sql, "INSERT INTO ");
    sql = mx_strcat(sql, chat_name);
    sql = mx_strcat(sql, "_chat (User, Message) VALUES('");
    sql = mx_strcat(sql, name);
    sql = mx_strcat(sql, "', '");
    sql = mx_strcat(sql, msg);
    sql = mx_strcat(sql, "');");
    mx_printstr(sql);
    mx_printstr("\n");
    if (name || db || err_msg || rc || msg) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

