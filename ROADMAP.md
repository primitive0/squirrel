### 0-ая итерация

Нужно реализовать парсинг SQL-выражений. Должны поддерживаться следующие операции:
- SELECT
- UPDATE
- INSERT
- DELETE

Должна быть верификация SQL-выражений с нормальными отчётами об ошибках.

Синтаксис SELECT:
```sql
SELECT ... FROM table1 WHERE ... ORDER BY ...;
```
Синтаксис INSERT:
```sql
INSERT INTO table1 ( ... ) VALUES ( ... );
```

Синтаксис DELETE:
```sql
DELETE FROM table1 WHERE ...;
```

Синтаксис UPDATE:
```sql
UPDATE table1 SET ... WHERE ...;
```
