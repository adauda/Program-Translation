	PUSH
		READ T0
		LOAD T0
		STACKW 0
		LOAD 0
		STORE T1
		STACKR 0
		SUB T1
BRZNEG	L0
		STACKR 0
		STORE T2
		WRITE T2
L0: 	NOOP
STOP
T0 0
T1 0
T2 0
