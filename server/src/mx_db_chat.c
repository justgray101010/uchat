#include "server.h"

static int callback_get_chat(void *data, int argc,
                             char **argv, char **azColName);
static int callback_set_chats(void *data, int argc,
                              char **argv, char **azColName);

void mx_db_set_chat(char *chat_name) { 
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "CREATE TABLE ");
    sql = mx_strcat(sql, chat_name);
    sql = mx_strcat(sql, "_chat (User TEXT, Message TEXT);");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

t_check_data *mx_db_get_chat(char *chat_name) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(" ", " ");
    data->count = 0;

    sql = mx_strcat(sql, "SELECT * FROM ");
    sql = mx_strcat(sql, chat_name);
    sql = mx_strcat(sql, "_chat;");
    if (db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, callback_get_chat, data, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return data;
}

static int callback_get_chat(void *data, int argc,
                             char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    static int i = 0;
    mx_strncpy(((t_check_data*)data)->strarr[i],
               argv[0], mx_strlen(argv[0]));
    mx_strncpy(((t_check_data*)data)->strarr2[i],
               argv[1], mx_strlen(argv[1]));
    i++;
    return 0;
}

void mx_db_set_user_chats(char *login) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "CREATE TABLE ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "_chats (Name TEXT);");
    printf("\n%s", sql);
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    printf("\n----------\n");
}

void mx_db_add_user_chat(char *login, char *chat_name) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    
    sql = mx_strcat(sql, "INSERT INTO ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "_chats (Name) VALUES('");
    sql = mx_strcat(sql, chat_name);
    sql = mx_strcat(sql, "');");
    printf("\n%s", sql);
    if (chat_name || db || err_msg || rc || login) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    free(err_msg);
    sqlite3_close(db);
    printf("\n----------\n");
}

void mx_db_get_user_chats(t_user_info *user) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "SELECT * FROM ");
    sql = mx_strcat(sql, user->login);
    sql = mx_strcat(sql, "_chats;");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, callback_set_chats, user, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

static int callback_set_chats(void *data, int argc,
                              char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    
    static int i = 0;
    mx_strncpy(((t_user_info*)data)->chats[i],
               argv[0], mx_strlen(argv[0]));
    i++;
    return 0;
}

