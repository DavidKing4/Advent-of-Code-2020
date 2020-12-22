-- TRANSACT-SQL

--Dropping tables from last run
DROP VIEW import;
DROP TABLE input;
DROP TABLE memory;
DROP FUNCTION maskBranching;

--Input table
CREATE TABLE input
(
    id integer IDENTITY PRIMARY KEY,
    op VARCHAR(40),
    num VARCHAR(40)
);

--Table to represent the memory in the question
CREATE TABLE memory
(
    addr INT UNIQUE,
    val BIGINT
);

--Creates a view so we can load the input table with data while preserving the id field
GO
CREATE VIEW import
WITH SCHEMABINDING
AS
SELECT
    I.op,
    I.num
FROM dbo.input AS I;

--Inserting data into import/input
--Sometimes 0x0A is needed as the rowterminator and sometimes \n is needed
GO
BULK INSERT import
FROM "C:\Users\David King\Documents\AOC 2020\input14.txt"
WITH
(
    CODEPAGE = '65001',
    FIELDTERMINATOR = ' = ',
    ROWTERMINATOR = '0x0A'
);

DECLARE @i INT = 0; --itterate through lines
DECLARE @j BIGINT = 0; --iterate through characters in mask
DECLARE @c CHAR; --character in mask
DECLARE @bigtwo BIGINT = 2;
DECLARE @p BIGINT; --power of two
DECLARE @n INT = (SELECT COUNT(*) FROM input); --number of lines
DECLARE @addr INT; --adress read from the operation
DECLARE @result BIGINT; --value to be written to the adress (LHS of memory lines)
DECLARE @mask VARCHAR(36);
DECLARE @value INT; --RHS of memory lines

WHILE @i <= @n
BEGIN
    SET @mask = (SELECT TOP 1 num FROM input WHERE id = @i);
    SET @i = @i + 1;
    WHILE ((SELECT TOP 1 op FROM input WHERE id = @i) LIKE 'mem%')
    BEGIN
        SET @j = 0;
        SET @result = 0;
        SET @value = (SELECT TOP 1 num FROM input WHERE id = @i);
        WHILE @j < 36
        BEGIN
            SET @p = POWER(@bigtwo, (@j));
            SET @c = SUBSTRING(@mask, 36 - @j, 1);
            IF (@c = 'x' AND (@value & @p) <> 0) OR @c = '1'
            BEGIN
                SET @result = @result + @p;
            END;
            SET @j = @j + 1;
        END;
        SET @addr = CAST((SELECT TOP 1 SUBSTRING(op, 5, LEN(op) - 5) FROM input WHERE id = @i) AS INT);
        IF EXISTS(SELECT * FROM memory WHERE addr = @addr)
        BEGIN
            UPDATE memory
            SET val = @result
            WHERE addr = @addr;
        END;
        ELSE
        BEGIN
            INSERT INTO memory VALUES
            (
                @addr,
                @result
            )
        END;
        SET @i = @i + 1;
    END;
END;

PRINT "Part 1: "
SELECT SUM(val) FROM memory;

DROP TABLE memory;
CREATE TABLE memory
(
    addr BIGINT UNIQUE,
    val BIGINT
);

GO
CREATE FUNCTION [dbo].[maskBranching]
(
    @k int,
    @maskf VARCHAR(36),
    @addrf INT,
    @addrresultf BIGINT,
    @value INT,
    @limit INT
)
RETURNS @addresses TABLE
(
    a BIGINT,
    val INT
)
WITH EXECUTE AS CALLER
AS
BEGIN
    DECLARE @bigtwo BIGINT = 2;
    DECLARE @p BIGINT;
    DECLARE @char CHAR;
    IF (@k = @limit)
    BEGIN
        INSERT INTO @addresses VALUES (@addrresultf, @value);
    END;
    ELSE
    BEGIN
        SET @p = POWER(@bigtwo, (@k));
        SET @char = SUBSTRING(@maskf, 36 - @k, 1);
        SET @k = @k + 1;
        DECLARE @returntable TABLE(a INT);
        IF (@char = 'X')
        BEGIN
            INSERT INTO @addresses SELECT * FROM maskBranching (@k, @maskf, @addrf, @addrresultf, @value, @limit);
            SET @addrresultf = @addrresultf + @p;
            INSERT INTO @addresses SELECT * FROM maskBranching (@k, @maskf, @addrf, @addrresultf, @value, @limit);
        END;
        ELSE IF (@char = '0')
        BEGIN
            SET @addrresultf = @addrresultf + (@addrf & @p);
            INSERT INTO @addresses SELECT * FROM maskBranching (@k, @maskf, @addrf, @addrresultf, @value, @limit);
        END;
        ELSE IF (@char = '1')
        BEGIN
            SET @addrresultf = @addrresultf + @p;
            INSERT INTO @addresses SELECT * FROM maskBranching (@k, @maskf, @addrf, @addrresultf, @value, @limit);
        END;
    END;
    RETURN
END;
GO

DECLARE @i INT = 0; --itterate through lines
DECLARE @n INT = (SELECT COUNT(*) FROM input); --number of lines
DECLARE @mask VARCHAR(36);
DECLARE @addr INT;
DECLARE @value INT;
DECLARE @addresses1 TABLE
(
    a1 BIGINT UNIQUE,
    val1 INT
);
DECLARE @addresses2 TABLE
(
    a2 BIGINT UNIQUE,
    val2 INT
)
DECLARE @addresses TABLE
(
    a BIGINT UNIQUE,
    val INT
)

WHILE @i < @n
BEGIN
    SET @mask = (SELECT TOP 1 num FROM input WHERE id = @i);
    SET @i = @i + 1;
    WHILE ((SELECT TOP 1 op FROM input WHERE id = @i) LIKE 'mem%')
    BEGIN
        SET @addr = CAST((SELECT TOP 1 SUBSTRING(op, 5, LEN(op) - 5) FROM input WHERE id = @i) AS INT);
        SET @value = (SELECT TOP 1 num FROM input WHERE id = @i)
        INSERT INTO @addresses1 SELECT * FROM maskBranching (0, @mask, @addr, 0, @value, 10);
        INSERT INTO @addresses2 SELECT * FROM maskBranching (10, @mask, @addr, 0, @value, 36);
        SET @i = @i + 1;
        -- INSERT INTO memory SELECT * FROM @addresses;
        INSERT INTO @addresses
        SELECT (a1 + a2), val1
        FROM @addresses1 CROSS JOIN @addresses2;

        MERGE memory as T
        USING @addresses AS S
        ON T.addr = S.a
        WHEN NOT MATCHED BY TARGET
            THEN INSERT (addr, val) VALUES (S.a, S.val)
        WHEN MATCHED
            THEN UPDATE SET T.val = S.val;
        DELETE FROM @addresses;
        DELETE FROM @addresses1;
        DELETE FROM @addresses2;
    END;
END;

PRINT "Part 2: "
SELECT SUM(val) FROM memory;