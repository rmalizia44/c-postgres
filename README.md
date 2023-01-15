Minimum example of using PostgreSQL in C

This assumes that you have a database called Test with the following structure:

```sql
create table test (
    id serial primary key,
    name text,
    value int);
```
