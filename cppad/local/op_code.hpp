# ifndef CPPAD_LOCAL_OP_CODE_HPP
# define CPPAD_LOCAL_OP_CODE_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <string>
# include <sstream>
# include <iomanip>

# include <cppad/local/define.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/pod_vector.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file op_code.hpp
Defines the OpCode enum type and functions related to it.

*/

/// type used to store OpCodes in vectors
typedef CPPAD_VEC_ENUM_TYPE opcode_t;


/*!
Type used to distinguish different AD< \a Base > atomic operations.

Each of the operators ends with the characters Op. Ignoring the Op at the end,
the operators appear in alphabetical order. Binary operation where both
operands have type AD< \a Base > use the following convention for thier endings:
\verbatim
    Ending  Left-Operand  Right-Operand
      pvOp     parameter       variable
      vpOp      variable      parameter
      vvOp      variable       variable
\endverbatim
For example, AddpvOp represents the addition operator where the left
operand is a parameter and the right operand is a variable.
*/
// alphabetical order is checked by bin/check_op_code.sh
enum OpCode {
	AbsOp,    // fabs(variable)
	AcosOp,   // acos(variable)
	AcoshOp,  // acosh(variable)
	AddpvOp,  // parameter  + variable
	AddvvOp,  // variable   + variable
	AsinOp,   // asin(variable)
	AsinhOp,  // asinh(variable)
	AtanOp,   // atan(variable)
	AtanhOp,  // atanh(variable)
	BeginOp,  // used to mark the beginning of the tape
	CExpOp,   // CondExpRel(left, right, trueCase, falseCase)
	// arg[0]     = the Rel operator: Lt, Le, Eq, Ge, Gt, or Ne
	// arg[1] & 1 = is left a variable
	// arg[1] & 2 = is right a variable
	// arg[1] & 4 = is trueCase a variable
	// arg[1] & 8 = is falseCase a variable
	// arg[2]     = index correspoding to left
	// arg[3]     = index correspoding to right
	// arg[4]     = index correspoding to trueCase
	// arg[5]     = index correspoding to falseCase
	CosOp,    // cos(variable)
	CoshOp,   // cosh(variable)
	CSkipOp,  // Conditional skip
	// arg[0]     = the Rel operator: Lt, Le, Eq, Ge, Gt, or Ne
	// arg[1] & 1 = is left a variable
	// arg[1] & 2 = is right a variable
	// arg[2]     = index correspoding to left
	// arg[3]     = index correspoding to right
	// arg[4] = number of operations to skip if CExpOp comparision is true
	// arg[5] = number of operations to skip if CExpOp comparision is false
	// arg[6] -> arg[5+arg[4]]               = skip operations if true
	// arg[6+arg[4]] -> arg[5+arg[4]+arg[5]] = skip operations if false
	// arg[6+arg[4]+arg[5]] = arg[4] + arg[5]
	CSumOp,   // Cummulative summation
	// arg[0] = index of parameter that initializes summation
	// arg[1] = end in arg of addition variables in summation
	// arg[2] = end in arg of subtraction variables in summation
	// arg[3] = end in arg of addition dynamic parameters in summation
	// arg[4] = end in arg of subtraction dynamic parameters in summation
	// arg[5],      ... , arg[arg[1]-1]: indices for addition variables
	// arg[arg[1]], ... , arg[arg[2]-1]: indices for subtraction variables
	// arg[arg[2]], ... , arg[arg[3]-1]: indices for additon dynamics
	// arg[arg[3]], ... , arg[arg[4]-1]: indices for subtraction dynamics
	// arg[arg[4]] = arg[4]
	DisOp,    // discrete::eval(index, variable)
	DivpvOp,  // parameter  / variable
	DivvpOp,  // variable   / parameter
	DivvvOp,  // variable   / variable
	EndOp,    // used to mark the end of the tape
	EqppOp,   // parameter  == parameter
	EqpvOp,   // parameter  == variable
	EqvvOp,   // variable   == variable
	ErfOp,    // erf(variable)
	ExpOp,    // exp(variable)
	Expm1Op,  // expm1(variable)
	InvOp,    // independent variable
	LdpOp,    // z[parameter] (parameter converted to index)
	LdvOp,    // z[variable]
	LeppOp,   // parameter <= parameter
	LepvOp,   // parameter <= variable
	LevpOp,   // variable  <= parameter
	LevvOp,   // variable  <= variable
	LogOp,    // log(variable)
	Log1pOp,  // log1p(variable)
	LtppOp,   // parameter < parameter
	LtpvOp,   // parameter < variable
	LtvpOp,   // variable  < parameter
	LtvvOp,   // variable  < variable
	MulpvOp,  // parameter  * variable
	MulvvOp,  // variable   * variable
	NeppOp,   // parameter  != parameter
	NepvOp,   // parameter  != variable
	NevvOp,   // variable   != variable
	ParOp,    // parameter
	PowpvOp,  // pow(parameter,   variable)
	PowvpOp,  // pow(variable,    parameter)
	PowvvOp,  // pow(variable,    variable)
	PriOp,    // PrintFor(text, parameter or variable, parameter or variable)
	SignOp,   // sign(variable)
	SinOp,    // sin(variable)
	SinhOp,   // sinh(variable)
	SqrtOp,   // sqrt(variable)
	StppOp,   // z[parameter] = parameter (first parameter converted to index)
	StpvOp,   // z[parameter] = variable  (parameter converted to index)
	StvpOp,   // z[variable]  = parameter
	StvvOp,   // z[variable]  = variable
	SubpvOp,  // parameter  - variable
	SubvpOp,  // variable   - parameter
	SubvvOp,  // variable   - variable
	TanOp,    // tan(variable)
	TanhOp,   // tan(variable)
	// user atomic operation codes
	UserOp,   // start of a user atomic operaiton
	// arg[0] = index of the operation if atomic_base<Base> class
	// arg[1] = extra information passed through by deprecated old atomic class
	// arg[2] = number of arguments to this atomic function
	// arg[3] = number of results for this atomic function
	UsrapOp,  // this user atomic argument is a parameter
	UsravOp,  // this user atomic argument is a variable
	UsrrpOp,  // this user atomic result is a parameter
	UsrrvOp,  // this user atomic result is a variable
	ZmulpvOp, // azmul(parameter, variable)
	ZmulvpOp, // azmul(variabe,  parameter)
	ZmulvvOp, // azmul(variable, variable)
	NumberOp  // number of operator codes (not an operator)
};
// Note that bin/check_op_code.sh assumes the pattern NumberOp occurs
// at the end of this list and only at the end of this list.

/*!
Number of arguments for a specified operator.

\return
Number of arguments corresponding to the specified operator.

\param op
Operator for which we are fetching the number of arugments.

\par NumArgTable
this table specifes the number of arguments stored for each
occurance of the operator that is the i-th value in the OpCode enum type.
For example, for the first three OpCode enum values we have
\verbatim
OpCode   j   NumArgTable[j]  Meaning
AbsOp    0                1  index of variable we are taking absolute value of
AcosOp   1                1  index of variable we are taking acos of
AcoshOp  2                1  index of variable we are taking acosh of
\endverbatim
Note that the meaning of the arguments depends on the operator.
*/
inline size_t NumArg( OpCode op)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

	// agreement with OpCode is checked by bin/check_op_code.sh
	static const size_t NumArgTable[] = {
		1, // AbsOp
		1, // AcosOp
		1, // AcoshOp
		2, // AddpvOp
		2, // AddvvOp
		1, // AsinOp
		1, // AsinhOp
		1, // AtanOp
		1, // AtanhOp
		1, // BeginOp  offset first real argument to have index 1
		6, // CExpOp
		1, // CosOp
		1, // CoshOp
		0, // CSkipOp  (actually has a variable number of arguments, not zero)
		0, // CSumOp   (actually has a variable number of arguments, not zero)
		2, // DisOp
		2, // DivpvOp
		2, // DivvpOp
		2, // DivvvOp
		0, // EndOp
		2, // EqppOp
		2, // EqpvOp
		2, // EqvvOp
		3, // ErfOp
		1, // ExpOp
		1, // Expm1Op
		0, // InvOp
		3, // LdpOp
		3, // LdvOp
		2, // LeppOp
		2, // LepvOp
		2, // LevpOp
		2, // LevvOp
		1, // LogOp
		1, // Log1pOp
		2, // LtppOp
		2, // LtpvOp
		2, // LtvpOp
		2, // LtvvOp
		2, // MulpvOp
		2, // MulvvOp
		2, // NeppOp
		2, // NepvOp
		2, // NevvOp
		1, // ParOp
		2, // PowpvOp
		2, // PowvpOp
		2, // PowvvOp
		5, // PriOp
		1, // SignOp
		1, // SinOp
		1, // SinhOp
		1, // SqrtOp
		3, // StppOp
		3, // StpvOp
		3, // StvpOp
		3, // StvvOp
		2, // SubpvOp
		2, // SubvpOp
		2, // SubvvOp
		1, // TanOp
		1, // TanhOp
		4, // UserOp
		1, // UsrapOp
		1, // UsravOp
		1, // UsrrpOp
		0, // UsrrvOp
		2, // ZmulpvOp
		2, // ZmulvpOp
		2, // ZmulvvOp
		0  // NumberOp not used
	};
# ifndef NDEBUG
	// only do these checks once to save time
	static bool first = true;
	if( first )
	{	first = false;
		// check that NumberOp is last value in op code table
		CPPAD_ASSERT_UNKNOWN(
			size_t(NumberOp) + 1 == sizeof(NumArgTable)/sizeof(NumArgTable[0])
		);
		//Check that the type CPPAD_VEC_ENUM_TYPE as required by define.hpp
		CPPAD_ASSERT_UNKNOWN( is_pod<opcode_t>() );
		CPPAD_ASSERT_UNKNOWN(
			size_t(NumberOp) < std::numeric_limits<opcode_t>::max()
		);
	}
	// do this check every time
	CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );
# endif

	return NumArgTable[op];
}

/*!
Number of variables resulting from the specified operation.

\param op
Operator for which we are fecching the number of results.

\par NumResTable
table specifes the number of varibles that result for each
occurance of the operator that is the i-th value in the OpCode enum type.
For example, for the first three OpCode enum values we have
\verbatim
OpCode   j   NumResTable[j]  Meaning
AbsOp    0                1  variable that is the result of the absolute value
AcosOp   1                2  acos(x) and sqrt(1-x*x) are required for this op
AcoshOp  2                2  acosh(x) and sqrt(x*x-1) are required for this op
\endverbatim
*/
inline size_t NumRes(OpCode op)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

	// agreement with OpCode is checked by bin/check_op_code.sh
	static const size_t NumResTable[] = {
		1, // AbsOp
		2, // AcosOp
		2, // AcoshOp
		1, // AddpvOp
		1, // AddvvOp
		2, // AsinOp
		2, // AsinhOp
		2, // AtanOp
		2, // AtanhOp
		1, // BeginOp  offsets first variable to have index one (not zero)
		1, // CExpOp
		2, // CosOp
		2, // CoshOp
		0, // CSkipOp
		1, // CSumOp
		1, // DisOp
		1, // DivpvOp
		1, // DivvpOp
		1, // DivvvOp
		0, // EndOp
		0, // EqppOp
		0, // EqpvOp
		0, // EqvvOp
		5, // ErfOp
		1, // ExpOp
		1, // Expm1Op
		1, // InvOp
		1, // LdpOp
		1, // LdvOp
		0, // LeppOp
		0, // LepvOp
		0, // LevpOp
		0, // LevvOp
		1, // LogOp
		1, // Log1pOp
		0, // LtppOp
		0, // LtpvOp
		0, // LtvpOp
		0, // LtvvOp
		1, // MulpvOp
		1, // MulvvOp
		0, // NeppOp
		0, // NepvOp
		0, // NevvOp
		1, // ParOp
		3, // PowpvOp
		3, // PowvpOp
		3, // PowvvOp
		0, // PriOp
		1, // SignOp
		2, // SinOp
		2, // SinhOp
		1, // SqrtOp
		0, // StppOp
		0, // StpvOp
		0, // StvpOp
		0, // StvvOp
		1, // SubpvOp
		1, // SubvpOp
		1, // SubvvOp
		2, // TanOp
		2, // TanhOp
		0, // UserOp
		0, // UsrapOp
		0, // UsravOp
		0, // UsrrpOp
		1, // UsrrvOp
		1, // ZmulpvOp
		1, // ZmulvpOp
		1, // ZmulvvOp
		0  // NumberOp not used and avoids g++ 4.3.2 warn when pycppad builds
	};
	// check ensuring conversion to size_t is as expected
	CPPAD_ASSERT_UNKNOWN( size_t(NumberOp) + 1 ==
		sizeof(NumResTable) / sizeof(NumResTable[0])
	);
	// this test ensures that all indices are within the table
	CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );

	return NumResTable[op];
}


/*!
Fetch the name for a specified operation.

\return
name of the specified operation.

\param op
Operator for which we are fetching the name
*/
inline const char* OpName(OpCode op)
{	// agreement with OpCode is checked by bin/check_op_code.sh
	static const char *OpNameTable[] = {
		"Abs"   ,
		"Acos"  ,
		"Acosh" ,
		"Addpv" ,
		"Addvv" ,
		"Asin"  ,
		"Asinh" ,
		"Atan"  ,
		"Atanh" ,
		"Begin" ,
		"CExp"  ,
		"Cos"   ,
		"Cosh"  ,
		"CSkip" ,
		"CSum"  ,
		"Dis"   ,
		"Divpv" ,
		"Divvp" ,
		"Divvv" ,
		"End"   ,
		"Eqpp"  ,
		"Eqpv"  ,
		"Eqvv"  ,
		"Erf"   ,
		"Exp"   ,
		"Expm1" ,
		"Inv"   ,
		"Ldp"   ,
		"Ldv"   ,
		"Lepp"  ,
		"Lepv"  ,
		"Levp"  ,
		"Levv"  ,
		"Log"   ,
		"Log1p" ,
		"Ltpp"  ,
		"Ltpv"  ,
		"Ltvp"  ,
		"Ltvv"  ,
		"Mulpv" ,
		"Mulvv" ,
		"Nepp"  ,
		"Nepv"  ,
		"Nevv"  ,
		"Par"   ,
		"Powpv" ,
		"Powvp" ,
		"Powvv" ,
		"Pri"   ,
		"Sign"  ,
		"Sin"   ,
		"Sinh"  ,
		"Sqrt"  ,
		"Stpp"  ,
		"Stpv"  ,
		"Stvp"  ,
		"Stvv"  ,
		"Subpv" ,
		"Subvp" ,
		"Subvv" ,
		"Tan"   ,
		"Tanh"  ,
		"User"  ,
		"Usrap" ,
		"Usrav" ,
		"Usrrp" ,
		"Usrrv" ,
		"Zmulpv",
		"Zmulvp",
		"Zmulvv",
		"Number"  // not used
	};
	// check ensuring conversion to size_t is as expected
	CPPAD_ASSERT_UNKNOWN(
		size_t(NumberOp) + 1 == sizeof(OpNameTable)/sizeof(OpNameTable[0])
	);
	// this test ensures that all indices are within the table
	CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );

	return OpNameTable[op];
}

/*!
Prints a single field corresponding to an operator.

A specified leader is printed in front of the value
and then the value is left justified in the following width character.

\tparam Type
is the type of the value we are printing.

\param os
is the stream that we are printing to.

\param leader
are characters printed before the value.

\param value
is the value being printed.

\param width
is the number of character to print the value in.
If the value does not fit in the width, the value is replace
by width '*' characters.
*/
template <class Type>
void printOpField(
	std::ostream      &os ,
	const char *   leader ,
	const Type     &value ,
	size_t          width )
{
	std::ostringstream buffer;
	std::string        str;

	// first print the leader
	os << leader;

	// print the value into an internal buffer
	buffer << std::setw( int(width) ) << value;
	str = buffer.str();

	// length of the string
	size_t len = str.size();
	if( len > width )
	{	size_t i;
		for(i = 0; i < width-1; i++)
			os << str[i];
		os << "*";
		return;
	}

	// count number of spaces at begining
	size_t nspace = 0;
	while(str[nspace] == ' ' && nspace < len)
		nspace++;

	// left justify the string
	size_t i = nspace;
	while( i < len )
		os << str[i++];

	i = width - len + nspace;
	while(i--)
		os << " ";
}

/*!
Prints a single operator and its operands

\tparam Base
Is the base type for these AD< \a Base > operations.

\param os
is the output stream that the information is printed on.

\param play
Is the entire recording for the tape that this operator is in.

\param i_op
is the index for the operator corresponding to this operation.

\param i_var
is the index for the variable corresponding to the result of this operation
(if NumRes(op) > 0).

\param op
The operator code (OpCode) for this operation.

\param ind
is the vector of argument indices for this operation
(must have NumArg(op) elements).
*/
template <class Base>
void printOp(
	std::ostream&          os     ,
	const local::player<Base>* play,
	size_t                 i_op   ,
	size_t                 i_var  ,
	OpCode                 op     ,
	const addr_t*          ind    )
{	size_t i;
	CPPAD_ASSERT_KNOWN(
		! thread_alloc::in_parallel() ,
		"cannot print trace of AD operations in parallel mode"
	);
	static const char *CompareOpName[] =
		{ "Lt", "Le", "Eq", "Ge", "Gt", "Ne" };

	// print operator
	printOpField(os,  "o=",      i_op,  5);
	if( NumRes(op) > 0 && op != BeginOp )
		printOpField(os,  "v=",      i_var, 5);
	else	printOpField(os,  "v=",      "",    5);
	if( op == CExpOp || op == CSkipOp )
	{	printOpField(os, "", OpName(op), 5);
		printOpField(os, "", CompareOpName[ ind[0] ], 3);
	}
	else	printOpField(os, "", OpName(op), 8);

	// print other fields
	size_t ncol = 5;
	switch( op )
	{
		case CSkipOp:
		/*
		ind[0]     = the Rel operator: Lt, Le, Eq, Ge, Gt, or Ne
		ind[1] & 1 = is left a variable
		ind[1] & 2 = is right a variable
		ind[2]     = index correspoding to left
		ind[3]     = index correspoding to right
		ind[4] = number of operations to skip if CExpOp comparision is true
		ind[5] = number of operations to skip if CExpOp comparision is false
		ind[6] -> ind[5+ind[4]]               = skip operations if true
		ind[6+ind[4]] -> ind[5+ind[4]+ind[5]] = skip operations if false
		ind[6+ind[4]+ind[5]] = ind[4] + ind[5]
		*/
		CPPAD_ASSERT_UNKNOWN( ind[6+ind[4]+ind[5]] == ind[4]+ind[5] );
		CPPAD_ASSERT_UNKNOWN(ind[1] != 0);
		if( ind[1] & 1 )
			printOpField(os, " vl=", ind[2], ncol);
		else	printOpField(os, " pl=", play->GetPar(ind[2]), ncol);
		if( ind[1] & 2 )
			printOpField(os, " vr=", ind[3], ncol);
		else	printOpField(os, " pr=", play->GetPar(ind[3]), ncol);
		if( size_t(ind[4]) < 3 )
		{	for(i = 0; i < size_t(ind[4]); i++)
				printOpField(os, " ot=", ind[6+i], ncol);
		}
		else
		{	printOpField(os, "\n\tot=", ind[6+0], ncol);
			for(i = 1; i < size_t(ind[4]); i++)
				printOpField(os, " ot=", ind[6+i], ncol);
		}
		if( size_t(ind[5]) < 3 )
		{	for(i = 0; i < size_t(ind[5]); i++)
				printOpField(os, " of=", ind[6+ind[4]+i], ncol);
		}
		else
		{	printOpField(os, "\n\tof=", ind[6+ind[4]+0], ncol);
			{	for(i = 1; i < size_t(ind[5]); i++)
					printOpField(os, " of=", ind[6+ind[4]+i], ncol);
			}
		}
		break;

		case CSumOp:
		/*
		ind[0] = index of parameter that initializes summation
		ind[1] = end in ind of addition variables in summation
		ind[2] = end in ind of subtraction variables in summation
		ind[3] = end in ind of addition dynamic parameters in summation
		ind[4] = end in ind of subtraction dynamic parameters in summation
		ind[5],      ... , ind[ind[1]-1]: indices for addition variables
		ind[ind[1]], ... , ind[ind[2]-1]: indices for subtraction variables
		ind[ind[2]], ... , ind[ind[3]-1]: indices for additon dynamics
		ind[ind[3]], ... , ind[ind[4]-1]: indices for subtraction dynamics
		ind[ind[4]] = ind[4]
		*/
		CPPAD_ASSERT_UNKNOWN( ind[ind[4]] == ind[4] );
		printOpField(os, " pr=", play->GetPar(ind[0]), ncol);
		for(i = 5; i < size_t(ind[1]); i++)
			 printOpField(os, " +v=", ind[i], ncol);
		for(i = size_t(ind[1]); i < size_t(ind[2]); i++)
			 printOpField(os, " -v=", ind[i], ncol);
		for(i = size_t(ind[2]); i < size_t(ind[3]); i++)
			 printOpField(os, " +d=", play->GetPar(ind[i]), ncol);
		for(i = size_t(ind[3]); i < size_t(ind[4]); i++)
			 printOpField(os, " -d=", play->GetPar(ind[i]), ncol);
		break;

		case LdpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, "idx=", ind[1], ncol);
		break;

		case LdvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, "  v=", ind[1], ncol);
		break;

		case StppOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, "idx=", ind[1], ncol);
		printOpField(os, " pr=", play->GetPar(ind[2]), ncol);
		break;

		case StpvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, "idx=", ind[1], ncol);
		printOpField(os, " vr=", ind[2], ncol);
		break;

		case StvpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, " vl=", ind[1], ncol);
		printOpField(os, " pr=", play->GetPar(ind[2]), ncol);
		break;

		case StvvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, " vl=", ind[1], ncol);
		printOpField(os, " vr=", ind[2], ncol);
		break;

		case AddvvOp:
		case DivvvOp:
		case EqvvOp:
		case LevvOp:
		case LtvvOp:
		case NevvOp:
		case MulvvOp:
		case PowvvOp:
		case SubvvOp:
		case ZmulvvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		printOpField(os, " vl=", ind[0], ncol);
		printOpField(os, " vr=", ind[1], ncol);
		break;

		case AddpvOp:
		case EqpvOp:
		case DivpvOp:
		case LepvOp:
		case LtpvOp:
		case NepvOp:
		case SubpvOp:
		case MulpvOp:
		case PowpvOp:
		case ZmulpvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		printOpField(os, " pl=", play->GetPar(ind[0]), ncol);
		printOpField(os, " vr=", ind[1], ncol);
		break;

		case DivvpOp:
		case LevpOp:
		case LtvpOp:
		case PowvpOp:
		case SubvpOp:
		case ZmulvpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		printOpField(os, " vl=", ind[0], ncol);
		printOpField(os, " pr=", play->GetPar(ind[1]), ncol);
		break;

		case AbsOp:
		case AcosOp:
		case AcoshOp:
		case AsinOp:
		case AsinhOp:
		case AtanOp:
		case AtanhOp:
		case CosOp:
		case CoshOp:
		case ExpOp:
		case Expm1Op:
		case LogOp:
		case Log1pOp:
		case SignOp:
		case SinOp:
		case SinhOp:
		case SqrtOp:
		case UsravOp:
		case TanOp:
		case TanhOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
		printOpField(os, "  v=", ind[0], ncol);
		break;

		case ErfOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		// ind[1] points to the parameter 0
		// ind[2] points to the parameter 2 / sqrt(pi)
		printOpField(os, "  v=", ind[0], ncol);
		break;

		case ParOp:
		case UsrapOp:
		case UsrrpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
		printOpField(os, "  p=", play->GetPar(ind[0]), ncol);
		break;

		case UserOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 4 );
		{	std::string name =  atomic_base<Base>::class_name(ind[0]);
			printOpField(os, " f=",   name.c_str(), ncol);
			printOpField(os, " i=", ind[1], ncol);
			printOpField(os, " n=", ind[2], ncol);
			printOpField(os, " m=", ind[3], ncol);
		}
		break;

		case PriOp:
		CPPAD_ASSERT_NARG_NRES(op, 5, 0);
		if( ind[0] & 1 )
			printOpField(os, " v=", ind[1], ncol);
		else	printOpField(os, " p=", play->GetPar(ind[1]), ncol);
		os << "before=\"" << play->GetTxt(ind[2]) << "\"";
		if( ind[0] & 2 )
			printOpField(os, " v=", ind[3], ncol);
		else	printOpField(os, " p=", play->GetPar(ind[3]), ncol);
		os << "after=\"" << play->GetTxt(ind[4]) << "\"";
		break;

		case BeginOp:
		// argument not used (created by independent)
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
		break;

		case EndOp:
		case InvOp:
		case UsrrvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
		break;

		case DisOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		{	const char* name = discrete<Base>::name(ind[0]);
			printOpField(os, " f=", name, ncol);
			printOpField(os, " x=", ind[1], ncol);
		}
		break;


		case CExpOp:
		CPPAD_ASSERT_UNKNOWN(ind[1] != 0);
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 6 );
		if( ind[1] & 1 )
			printOpField(os, " vl=", ind[2], ncol);
		else	printOpField(os, " pl=", play->GetPar(ind[2]), ncol);
		if( ind[1] & 2 )
			printOpField(os, " vr=", ind[3], ncol);
		else	printOpField(os, " pr=", play->GetPar(ind[3]), ncol);
		if( ind[1] & 4 )
			printOpField(os, " vt=", ind[4], ncol);
		else	printOpField(os, " pt=", play->GetPar(ind[4]), ncol);
		if( ind[1] & 8 )
			printOpField(os, " vf=", ind[5], ncol);
		else	printOpField(os, " pf=", play->GetPar(ind[5]), ncol);
		break;

		case EqppOp:
		case LeppOp:
		case LtppOp:
		case NeppOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		printOpField(os, " pl=", play->GetPar(ind[0]), ncol);
		printOpField(os, " pr=", play->GetPar(ind[1]), ncol);
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(0);
	}
}

/*!
Prints the result values correspnding to an operator.

\tparam Base
Is the base type for these AD< \a Base > operations.

\tparam Value
Determines the type of the values that we are printing.

\param os
is the output stream that the information is printed on.

\param nfz
is the number of forward sweep calculated values of type Value
that correspond to this operation
(ignored if NumRes(op) == 0).

\param fz
points to the first forward calculated value
that correspond to this operation
(ignored if NumRes(op) == 0).

\param nrz
is the number of reverse sweep calculated values of type Value
that correspond to this operation
(ignored if NumRes(op) == 0).

\param rz
points to the first reverse calculated value
that correspond to this operation
(ignored if NumRes(op) == 0).
*/
template <class Value>
void printOpResult(
	std::ostream          &os     ,
	size_t                 nfz    ,
	const  Value          *fz     ,
	size_t                 nrz    ,
	const  Value          *rz     )
{
	size_t k;
	for(k = 0; k < nfz; k++)
		os << "| fz[" << k << "]=" << fz[k];
	for(k = 0; k < nrz; k++)
		os << "| rz[" << k << "]=" << rz[k];
}

/*!
Determines which arguments are variaibles for an operator.

\param op
is the operator. Note that CSkipOp and CSumOp are special cases
because the true number of arguments is not equal to NumArg(op)
and the true number of arguments num_arg can be large.
It may be more efficient to handle these cases separately
(see below).

\param arg
is the argument vector for this operator.

\param is_variable
If the input value of the elements in this vector do not matter.
Upon return, resize has been used to set its size to the true number
of arguments to this operator.
If op != CSkipOp and op != CSumOp, is_variable.size() = NumArg(op).
The j-th argument for this operator is a
variable index if and only if is_variable[j] is true. Note that the variable
index 0, for the BeginOp, does not correspond to a real variable and false
is returned for this case.

\par CSkipOp
In the case of CSkipOp,
\code
		is_variable.size()  = 7 + arg[4] + arg[5];
		is_variable[2]      = (arg[1] & 1) != 0;
		is_variable[3]      = (arg[1] & 2) != 0;
\endcode
and all the other is_variable[j] values are false.

\par CSumOp
In the case of CSumOp,
\code
		is_variable.size() = arg[4]
		for(size_t j = 5; j < arg[2]; ++j)
			is_variable[j] = true;
\endcode
and all the other is_variable values are false.
*/
template <class Addr>
inline void arg_is_variable(
	OpCode            op          ,
	const Addr*       arg         ,
	pod_vector<bool>& is_variable )
{	size_t num_arg = NumArg(op);
	is_variable.resize( num_arg );
	//
	switch(op)
	{
		// -------------------------------------------------------------------
		// cases where true number of arugments = NumArg(op) == 0

		case EndOp:
		case InvOp:
		case UsrrvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
		break;

		// -------------------------------------------------------------------
		// cases where NumArg(op) == 1
		case AbsOp:
		case AcoshOp:
		case AcosOp:
		case AsinhOp:
		case AsinOp:
		case AtanhOp:
		case AtanOp:
		case CoshOp:
		case CosOp:
		case Expm1Op:
		case ExpOp:
		case Log1pOp:
		case LogOp:
		case SignOp:
		case SinhOp:
		case SinOp:
		case SqrtOp:
		case TanhOp:
		case TanOp:
		case UsravOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
		is_variable[0] = true;
		break;

		case BeginOp:
		case ParOp:
		case UsrapOp:
		case UsrrpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
		is_variable[0] = false;
		break;


		// -------------------------------------------------------------------
		// cases where NumArg(op) == 2

		case AddpvOp:
		case DisOp:
		case DivpvOp:
		case EqpvOp:
		case LepvOp:
		case LtpvOp:
		case MulpvOp:
		case NepvOp:
		case PowpvOp:
		case SubpvOp:
		case ZmulpvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		is_variable[0] = false;
		is_variable[1] = true;
		break;

		case DivvpOp:
		case LevpOp:
		case LtvpOp:
		case PowvpOp:
		case SubvpOp:
		case ZmulvpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		is_variable[0] = true;
		is_variable[1] = false;
		break;

		case AddvvOp:
		case DivvvOp:
		case EqvvOp:
		case LevvOp:
		case LtvvOp:
		case MulvvOp:
		case NevvOp:
		case PowvvOp:
		case SubvvOp:
		case ZmulvvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		is_variable[0] = true;
		is_variable[1] = true;
		break;

		case ErfOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		is_variable[0] = true;
		is_variable[1] = false; // parameter index corresponding to zero
		is_variable[2] = false; // parameter index corresponding to one
		break;

		// --------------------------------------------------------------------
		// cases where NumArg(op) == 3

		case LdpOp:
		case StppOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		is_variable[0] = false;
		is_variable[1] = false;
		is_variable[2] = false;
		break;

		case LdvOp:
		case StvpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		is_variable[0] = false;
		is_variable[1] = true;
		is_variable[2] = false;
		break;

		case StpvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		is_variable[0] = false;
		is_variable[1] = false;
		is_variable[2] = true;
		break;

		case StvvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
		is_variable[0] = false;
		is_variable[1] = true;
		is_variable[2] = true;
		break;

		// --------------------------------------------------------------------
		// case where NumArg(op) == 4
		case UserOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 4 );
		for(size_t i = 0; i < 4; i++)
			is_variable[i] = false;
		break;

		// --------------------------------------------------------------------
		// case where NumArg(op) == 5
		case PriOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 5 );
		is_variable[0] = false;
		is_variable[1] = (arg[0] & 1) != 0;
		is_variable[2] = false;
		is_variable[3] = (arg[0] & 2) != 0;
		is_variable[4] = false;
		break;

		// --------------------------------------------------------------------
		// case where NumArg(op) == 6
		case CExpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 6 );
		is_variable[0] = false;
		is_variable[1] = false;
		is_variable[2] = (arg[0] & 1) != 0;
		is_variable[3] = (arg[0] & 2) != 0;
		is_variable[4] = (arg[0] & 4) != 0;
		is_variable[5] = (arg[0] & 8) != 0;
		break;

		// -------------------------------------------------------------------
		// CSkipOp:
		case CSkipOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 )
		//
		// true number of arguments
		num_arg = 7 + arg[4] + arg[5];
		is_variable.resize(num_arg);
		is_variable[0] = false;
		is_variable[1] = false;
		is_variable[2] = (arg[1] & 1) != 0;
		is_variable[3] = (arg[1] & 2) != 0;
		for(size_t i = 4; i < num_arg; ++i)
			is_variable[i] = false;
		break;

		// -------------------------------------------------------------------
		// CSumOp:
		case CSumOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 )
		//
		// true number of arguments
		num_arg = arg[4];
		//
		is_variable.resize( num_arg );
		for(size_t i = 0; i < num_arg; ++i)
			is_variable[i] = (5 <= i) & (i < size_t(arg[2]));
		break;

		case EqppOp:
		case LeppOp:
		case LtppOp:
		case NeppOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		is_variable[0] = false;
		is_variable[1] = false;
		break;

		// --------------------------------------------------------------------
		default:
		CPPAD_ASSERT_UNKNOWN(false);
		break;
	}
	return;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
