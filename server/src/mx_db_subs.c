#include "server.h"

static int callback_get_all_users(void *data, int argc,
                                  char **argv, char **azColName);
static int callback_nick_search(void *data, int argc,
                                char **argv, char **azColName);
static int callback_auth(void *data, int argc,
                         char **argv, char **azColName);

t_check_data *mx_db_get_all_users(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(" ", " ");

    sql = mx_strcat(sql, "SELECT * FROM Users;");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, callback_get_all_users, data, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return data;
}

static int callback_get_all_users(void *data, int argc,
                                  char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    static int i = 0;
    mx_strncpy(((t_check_data*)data)->strarr[i],
               argv[0], mx_strlen(argv[2]));
    i++;
    return 0;
}

t_user_info *mx_db_get_user_info(char *nick) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(nick, " ");
    
    data->count = 0;
    sql = mx_strcat(sql, "SELECT * FROM Users;");
    if (nick || db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, callback_nick_search, data, &err_msg);
    free(sql);
    mx_db_get_user_chats(data->user);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    t_user_info *new = data->user;
    //mx_free_check_data(data);
    return new;
}

static int callback_nick_search(void *data, int argc,
                                char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    if(((t_check_data*)data)->first_re 
            &&((t_check_data*)data)->second_re)
        return 0;
    if (!mx_strcmp((const char*)((t_check_data*)data)->first,
        (const char*)(argv[0]))) {
        ((t_check_data*)data)->user = mx_create_user_info(argv[0],
                                                          argv[1]);
        mx_strncpy(((t_check_data*)data)->user->nickname,
                   argv[2], mx_strlen(argv[2]));
        mx_strncpy(((t_check_data*)data)->user->ava_img_path,
                   argv[3], mx_strlen(argv[3]));
        ((t_check_data*)data)->first_re = true;
        ((t_check_data*)data)->second_re = true;
    }
    return 0;
}

t_check_data *mx_db_auth(t_user_info *user) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(user->login,
                                                 user->password);

    sql = mx_strcat(sql, "SELECT * FROM Users;");
    if (user->login || db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, callback_auth, data, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return data;
}

static int callback_auth(void *data, int argc,
                         char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    if(((t_check_data*)data)->first_re 
            &&((t_check_data*)data)->second_re)
        return 0;
    if (!mx_strcmp((const char*)((t_check_data*)data)->first,
       (const char*)(argv[0])))
        ((t_check_data*)data)->first_re = true;
    if (!mx_strcmp((const char*)((t_check_data*)data)->second,
       (const char*)(argv[1])))
        ((t_check_data*)data)->second_re = true;
    return 0;
}

