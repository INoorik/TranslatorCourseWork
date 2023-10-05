VAR Result: LOGICAL;
BEGIN
	Result = 0 AND 1 OR 1;
	WRITE(Result);
	Result = 1 OR 1 AND 0;
	WRITE(Result);
	Result = (1 OR 1) AND 0;
	WRITE(Result);
	Result = 0 AND (1 OR 1);
	WRITE(Result);
END
