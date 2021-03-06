# ifndef CPPAD_CORE_DIV_EQ_HPP
# define CPPAD_CORE_DIV_EQ_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base>& AD<Base>::operator /= (const AD<Base> &right)
{
	// compute the Base part
	Base left;
	left    = value_;
	value_ /= right.value_;

	// check if there is a recording in progress
	local::ADTape<Base>* tape = AD<Base>::tape_ptr();
	if( tape == CPPAD_NULL )
		return *this;
	tape_id_t tape_id = tape->id_;
	// tape_id cannot match the default value for tape_id_; i.e., 0
	CPPAD_ASSERT_UNKNOWN( tape_id > 0 );

	// check if left and right tapes match
	bool match_left  = tape_id_       == tape_id;
	bool match_right = right.tape_id_ == tape_id;

	// check if left and right are dynamic parameters
# ifndef NDEBUG
	bool dyn_left  = match_left  & dynamic_;
# endif
	bool dyn_right = match_right & right.dynamic_;

	// check if left and right are variables
	bool var_left  = match_left  & (! dynamic_);
	bool var_right = match_right & (! right.dynamic_);

	CPPAD_ASSERT_KNOWN(
		tape_id_ == right.tape_id_ || ! match_left || ! match_right ,
		"/= : AD variables or dynamic parameters on different threads."
	);
	if( var_left )
	{	if( var_right )
		{	// this = variable / variable
			CPPAD_ASSERT_UNKNOWN( local::NumRes(local::DivvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( local::NumArg(local::DivvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec_.PutArg(taddr_, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(local::DivvvOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( tape_id_ == tape_id );
		}
		else if( IdenticalOne( right.value_ ) )
		{	// this = variable * 1
		}
		else
		{	// this = variable / parameter
			CPPAD_ASSERT_UNKNOWN( local::NumRes(local::DivvpOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( local::NumArg(local::DivvpOp) == 2 );

			// put operand addresses in tape
			addr_t p = right.taddr_;
			if( ! dyn_right )
				p = tape->Rec_.put_con_par(right.value_);
			tape->Rec_.PutArg(taddr_, p);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(local::DivvpOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( tape_id_ == tape_id );
		}
	}
	else if( var_right  )
	{	if( IdenticalZero(left) )
		{	// this = 0 / variable
		}
		else
		{	// this = parameter / variable
			CPPAD_ASSERT_UNKNOWN( local::NumRes(local::DivpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( local::NumArg(local::DivpvOp) == 2 );

			// put operand addresses in tape
			CPPAD_ASSERT_KNOWN( ! dyn_left,
				"binary /=: left operand is a dynamic parameter"
			);
			addr_t p = tape->Rec_.put_con_par(left);
			tape->Rec_.PutArg(p, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(local::DivpvOp);
			// make this a variable
			tape_id_ = tape_id;
		}
	}
	else
	{	CPPAD_ASSERT_KNOWN( ! (dyn_left | dyn_right) ,
		"binary /=: one operand is a dynamic parameter and other not a variable"
		);
	}
	return *this;
}

CPPAD_FOLD_ASSIGNMENT_OPERATOR(/=)

} // END CppAD namespace

# endif
