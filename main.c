#include <libpq-fe.h>
#include <string.h>

#define LOG(...) { printf("[%s:%u] ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n"); }

int main(int argc, const char* argv[]) {
    if(argc != 3) {
        LOG("wrong number of arguments: expecting 2, given %i", argc-1);
        goto quit;
    }
    PGconn* pg = PQconnectdb("host=localhost port=5432 user=postgres password=medeve1$ dbname=Test");
    if(pg == NULL) {
        LOG("can't make connection");
        goto quit;
    }
    if(PQstatus(pg) != CONNECTION_OK) {
        LOG("can't connect: %s", PQerrorMessage(pg));
        goto quit_pg;
    }
    PGresult* stmt = PQprepare(pg, "add_test", "insert into test (name, value) values ($1, $2) returning id", 2, NULL);
    if(stmt == NULL) {
        LOG("can't make statement");
        goto quit_pg;
    }
    if(PQresultStatus(stmt) != PGRES_COMMAND_OK) {
        LOG("can't prepare statement: %s", PQerrorMessage(pg));
        goto quit_stmt;
    }
    const char *paramValues[2] = { argv[1], argv[2] };
    int paramLengths[2] = { strlen(argv[1]), strlen(argv[2]) };
    int paramFormats[2] = { 0, 0 };
    PGresult* res = PQexecPrepared(pg, "add_test", 2, paramValues, paramLengths, paramFormats, 0);
    if(res == NULL) {
        LOG("can't make query");
        goto quit_stmt;
    }
    if(PQresultStatus(res) != PGRES_TUPLES_OK) {
        LOG("can't exec query: %s", PQerrorMessage(pg));
        goto quit_res;
    }
    LOG("inserted: %s", PQgetvalue(res, 0, 0));
quit_res:
    PQclear(res);
quit_stmt:
    PQclear(stmt);
quit_pg:
    PQfinish(pg);
quit:
    return 0;
}
