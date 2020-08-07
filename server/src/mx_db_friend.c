#include "../inc/server.h"

static int callback_get_friend(void *data, int argc,
                               char **argv, char **azColName);

void mx_db_set_friends_table(char *login) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "CREATE TABLE ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "(Name TEXT, Tags TEXT);");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

t_check_data *mx_db_get_friends_arr(char *login) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(login, " ");
    data->count = 0;

    sql = mx_strcat(sql, "SELECT * FROM ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, ";");
    if (login || db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, callback_get_friend, data, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return data;
}

static int callback_get_friend(void *data, int argc,
                               char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    int i = ((t_check_data*)data)->count;
    int len = mx_strlen(((t_check_data*)data)->strarr[i]);
    char *sub = mx_strcat(mx_strnew(len),
                         ((t_check_data*)data)->strarr[i]);
    sub = mx_strcat(sub, ((t_check_data*)data)->strarr[i]);
    ((t_check_data*)data)->strarr[i] = mx_strcat(sub, argv[0]);
    free(sub);
    ((t_check_data*)data)->count++;
    return 0;
}

void mx_db_add_friend(char *login, char *amigo) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    
    sql = mx_strcat(sql, "INSERT INTO ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "(Name) VALUES('");
    sql = mx_strcat(sql, amigo);
    sql = mx_strcat(sql, "');");
    mx_printstr(sql);
    mx_printstr("\n");
    if (login || db || err_msg || rc || amigo ) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

void mx_db_del_friend(char *login, char *amigo) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    
    sql = mx_strcat(sql, "DELETE FROM ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, " WHERE Name = '");
    sql = mx_strcat(sql, amigo);
    sql = mx_strcat(sql, "';");
    if (login || db || err_msg || rc || amigo ) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}
