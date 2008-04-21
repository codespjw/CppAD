/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_ode.cpp$$
$spell
	CppAD
	cppad
	hpp
	bool
$$

$section CppAD Speed: Grandient of Ode Solution$$

$index cppad, speed ode gradient$$
$index speed, cppad ode gradient$$
$index gradient, ode speed cppad$$
$index ode, gradient speed cppad$$

$head Operation Sequence$$
The operation sequence is taped for each repetition.
Otherwise, the operation sequence is only taped once
and that sequence is used for each repetition.

$head link_ode$$
$index link_ode$$
Routine that computes the gradient of determinant using CppAD:
$codep */
# include <cstring>
# include <cppad/cppad.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>

extern bool link_ode(
	size_t                     repeat     ,
	bool                       retape     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &gradient
)
{	// -------------------------------------------------------------
	// setup
	using CppAD::AD;

	size_t j;
	size_t m = 0;
	size_t n = x.size();
	CppAD::vector< AD<double> > X(n);
	CppAD::vector< AD<double> > Y(1);
	CppAD::vector<double>       w(1);
	w[0] = 1.;

	while(repeat--)
	{ 	// choose next x value
		uniform_01(n, x);
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// declare the independent variable vector
		Independent(X);

		// evaluate function
		CppAD::ode_evaluate(X, m, Y);

		// create function object f : X -> Y
		CppAD::ADFun<double>   F(X, Y);

		// use reverse mode to compute gradient
		gradient = F.Reverse(1, w);
	}
	return true;
}
