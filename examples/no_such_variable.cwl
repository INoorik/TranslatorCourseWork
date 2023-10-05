VAR A, B, C, FirstBit, SecondBit: LOGICAL;
BEGIN
	READ(A);
	READ(B);
	READ(C);

	FirstBit = A AND B;
	SecondBit = NOT (A EQU B);

	FristBit = FirstBit OR SecondBit AND C;
	SecondBit = NOT(SecondBit EQU C);

	IF FirstBit THEN
		WRITE(FirstBit, SecondBit);
	ELSE
		WRITE(SecondBit);
	END_IF;
END
