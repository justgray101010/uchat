#include "server.h"

static int callback_get_all_tags(void *data, int argc,
                                 char **argv, char **azColName);
static int callback_get_user_tags(void *data, int argc,
                                  char **argv, char **azColName);

void mx_db_set_tag_table(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "CREATE TABLE Tags(Tag TEXT);");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

void mx_db_add_tag(const char* tag) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "INSERT INTO Tags(Tag) VALUES('");
    sql = mx_strcat(sql, tag);
    sql = mx_strcat(sql, "');");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

void mx_db_set_user_tag_table(char *login) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);

    sql = mx_strcat(sql, "CREATE TABLE ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "_tags (Tags TEXT);");
    if (db || err_msg || rc) {}
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

void mx_db_set_user_tags(char *login, char **tags) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    for (int i = 0; tags[i][0] ; i++) {
        sql = mx_strcat(sql, "INSERT INTO ");
        sql = mx_strcat(sql, login);
        sql = mx_strcat(sql, "_tags (Tags) VALUES ('");
        sql = mx_strcat(sql, tags[i]);
        sql = mx_strcat(sql, "');");
        if (db || err_msg || rc) {}
        printf("%s\n", sql);
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        free(sql);
        sql = mx_strnew(256);
    }
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

char **mx_db_get_all_tags(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(" ", " ");
    
    data->count = 0;
    sql = mx_strcat(sql, "SELECT * FROM Tags;");
    if (db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, callback_get_all_tags, data, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return data->strarr;
}

static int callback_get_all_tags(void *data, int argc,
                                 char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    int i = ((t_check_data*)data)->count;
    mx_strncpy(((t_check_data*)data)->strarr[i],
               argv[0], mx_strlen(argv[0]));
    ((t_check_data*)data)->count++;
    return 0;
}

char **mx_db_get_user_tags(char *login) { 
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("sql/data.db", &db);
    char *sql = mx_strnew(256);
    t_check_data *data = mx_db_create_check_data(" ", " ");
    data->count = 0;

    sql = mx_strcat(sql, "SELECT * FROM ");
    sql = mx_strcat(sql, login);
    sql = mx_strcat(sql, "_tags;");
    if (db || err_msg || rc ) {}
    rc = sqlite3_exec(db, sql, callback_get_user_tags, data, &err_msg);
    free(sql);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return data->strarr;
}

static int callback_get_user_tags(void *data, int argc,
                                  char **argv, char **azColName) {
    if (data || argc || argv || azColName ) {}
    int i = ((t_check_data*)data)->count;
    mx_strncpy(((t_check_data*)data)->strarr[i],
               argv[0], mx_strlen(argv[0]));
    ((t_check_data*)data)->count++;
    return 0;
}
