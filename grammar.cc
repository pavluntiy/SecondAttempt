+<body> ::= {<operators>}
+<operators> ::= <operator><operators>|NONE
<operator> ::= 
+				<special>
+				;
+				<expression>;
+				<declaration>;
+				<body>
+ 				<funcdef>

<forthing> ::= 
				+	<expression>
				+	<declaration>
				+	NONE

+<special> ::= 
	+			for(<forthing>;<forthing>;<forthing>)<operator>
	+			foreach(<vardef> : <expression>)<operator>
	+			if(<expression>)<operator>
	+			if(<expression>) <operator> else <operator>

	+			while(<expression>) <operator>
	+			init <operator> while(<expression>) <operator>
	+			init <operator> while(<expression>) <operator> finally <operator>
	+			while(<expression>) <operator> finally <operator>

	+			while(<expression>)<operator>else<operator>
	+			init<operator> while(<expression>)<operator> else <operator>
	+			init<operator> while(<expression>) <operator> finally <operator> else <operator>
	+			while(<expression>) <operator> finally <operator> else <operator>

	+			do <operator> while(<expression>);
//	+			do <operator> while(<expression>) else <operator>

	+			switch (<expression>){<cases>}
	+			switch(<expression>){<cases>} else<operator>
				return <expression>;
				return;

+<cases> ::= 
+			<case><cases>
+			NONE


+<case> ::=	
		+	case <expression> : <operator> continue;
		+	case <expression> : <operator>

//<condition> ::= 
//				!<condition>
//				<atomic_condition> || <condition>
//				<atomic_condition>


//<atomic_condition> ::= 	
//						<andCond>
//						(<condition>)
//						<expr1>
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


+<expression> ::= 	
+					<assignment>
+					<l_or>
					





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
				<argList>





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

+<value> ::= 
			<name>[<expression>]
			<funcall>[<expression>]
			+<name>
			+<constant>
			+<funcall>
			+<funcall>.<value>
			+<funcall>-><value>
			+<list>


+<name> :: =	
			<namespace>::<name>
			*<name1>
			<name1>			
			<name1> -> <name>
			<name1>.<name>

<assignment> ::= 
	+<name> = <expression>
	+<name> += <expression>
	+<name> /= <expression>
	+<name> -= <expression>
	+<name> *= <expression>
	+<name> %= <expression>
	+<name> |= <expression>
	+<name> ||= <expression>
	+<name> &= <expression>
	+<name> &&= <expression>
	+<name> ~= <expression>

/*
+<expr1> ::=		
		<expr2> 
		<expr2> {<<, >>} <expr1>

<expr2> ::= 
			-<condition>
			<condition>


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


*/

<l_or> ::=	
			<l_and>
			<l_and> | <l_or>

<l_and> ::= 
				<is_in_Expression> 
				<is_in_Expression> & <l_and>

<is_in_Expression> ::=
			<comparision>
			<comparision> is <comparision>
			<comparision> in <comparision>

<comparision> ::=
				<b_or>
				<b_or> {>=, ==, <=, !=, >, < } <b_or>



<b_or> ::=	
			<b_xor>
			<b_xor> | <b_or>

<b_xor> ::=	
			<b_and>
			<b_and> | <b_xor>

<b_and> ::= 
				<expr1> 
				<expr1> & <b_and>
<expr1> ::=
			<expr2> {<< ,>>} <expr1>
			<expr2>


<expr2> ::= 							
			<expr3> {+, -} <expr2>
			<expr2>


<expr3> ::=
			<expr4> {*, /, %} <expr3>
			<expr4>
			



<expr4> ::=	
			~<expr5>
			!<expr5>
			-<expr5>
			+<expr5>

<expr5> ::= 
			<expr6> ** <expr6>
<expr6> ::= 
			(<expression>)
			<value>

		


<declaration> ::=
 +				<type> <varlist>
 //-				<varQ> <declaration>

<varlist> ::=
			<assignment>, <varlist>
			<name>, <varlist>;
			<assignment>
			<name>
<type> ::= 
//-			<class>
//-			<typedef>
+			<predef_type>
-			(<type>*)
-			(<type> &)

/////
/*<class> ::= [<classQ>]class <ID> [: <supers>] <classbody>

				
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
*/				


+<funcdef> ::= <type> <name> (<decllist>) <body>

+<decllist> ::=	
				<decl_atom>
				<decl_atom>,<decllist>
				NONE

+<decl_atom> ::=	
				<type> <assignment>
				<type> <name>











