+<body> ::= {<operators>}
+<operators> ::= <operator><operators>|NONE
<operator> ::= 
				<special>
+				;
+				<expression>;
+				<declaration>;
+				<body>
+ 				<funcdef>

<forthing> ::= 
					<expression>
					<declaration>
					NONE

<special> ::= 
				for(<forthing>;<forthing>;<forthing>)<operator>
				for(<vardef> : <expression>)<action>
				if(<condition>)<action>
				if(<condition>) <action> else <action>

				while(<condition>)<action>
				init<action> while(condition)<action>
				init<action> while(conditio)<action> finally<action>
				while(conditio)<action> finally<action>

				while(<condition>)<action>else<action>
				init<action> while(condition)<action>else<action>
				init<action> while(conditio)<action> finally<action>else<action>
				while(conditio)<action> finally<action>else<action>

				do <action> while(condition)
				do <action> while(condition) else <action>

				switch (<expression>){<cases>}
				switch(<expression>){<cases>}else<action>

<cases> ::= <case><cases>
			NONE

+<condition> ::= 
				!<condition>
				<atomic_condition> || <condition>
				<atomic_condition>


+<atomic_condition> ::= 	
						<andCond>
						(<condition>)
						<expr1>
//<andCond> ::= 	
//				<comparision> && <andCond>
//				<comparision>


//+<comparision> ::=
//					+<expr1> == <expr1>
//					+<expr1> <= <expr1>
//					+<expr1> >= <expr1>
//					+<expr1> != <expr1>
/
//<is_in_Expression>::=
//						<expression> in <expression>
//						<expression> is <expression>


<expression> ::= 
					<funcall>
					<expr1>
					<assignment>





+<funcall> ::=
				//<funcall><arglist>
				+<name><arglist>

+<arglist> ::= 
				(<list>)

+<list> ::= 
			NONE
			<listElem>
			<listElem>, <list>

+<listElem> ::= 	
				<expr1>
				<assignment>
				<value>
				<list>



+<value> ::= 
			+<expr1>
			+<name>
			+<constant>
			+<funcall>
			+<funcall>.<value>
			+<funcall>-><value>
			<list>


+<name> :: =	
			<namespace>::<name>
			*<name1>
			<name1>			
			<name1> -> <name>
			<name1>.<name>

+<name1>	::= 
			<ID>
			(<name>)
			




+<namespace> ::= <ID>

+<ID> :: =
			<letter><idend>


+<idend> ::= <letter><idend>
			NONE
			<digit><idend>



+<letter> ::= ALPHABET + _
+<digit> :: = DIGIGIT


<assignment> ::= 
	+<name> = <value>
	<list> = <list>


<expr1> ::=		
		<expr2> 
		<expr2> {<<, >>} <expr1>

<expr2> ::= 
			-<condition>
			<condition>
			<expr3>
			<expr2> {+, -} <expr3>

+<expr3> ::= 
			<expr4>
			<expr4> {*, /, %} <expr3>

+<expr4> ::= 
			<expr5>
			<expr5> ** <expr5>

+<expr5> ::=	
			~<expr5>
			!<expr5>
			-<expr5>
			(<expr5>)
			(<expr4>)
			(<expr3>)
			(<expr2>)
			(<expr1>)
			<value>
		


<declaration> ::=
 +				<type> <list>
 -				<varQ> <declaration>

<type> ::= 
-			<class>
-			<typedef>
+			<predef_type>
-			(<type>*)
-			(<type> &)

/////
<class> ::= [<classQ>]class <ID> [: <supers>] <classbody>

				
<supers> ::= [<inheritQ>] <ID>, <supers> 
				<inheritQ> <ID>

<inheritQ> ::= 	INHERIT_Q
				INHERIT_Q, <inheritQ>


<classQ> ::= NONE
			CLASS_Q
			CLASS_Q <classQ>

<varQ> ::= VAR_Q
			VAR_Q <varQ>

<classbody> ::= {<classPadding>}

<classPadding> ::= <accessModifier>: <classMembers>
					<accessModifier>: <classMembers> <classPadding>


<accessModifier> ::= ACESS_MODIFIER

<classMembers> ::= <funcdef><classMembers>
					<declaration><classMembers>
					NONE
				


<funcdef> ::= <type> <name> (<list>) <body>










