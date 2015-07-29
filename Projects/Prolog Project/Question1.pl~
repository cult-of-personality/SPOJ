simplify(div(0, X), Value) :-
  atomic(X), Value is 0, !.  
simplify(div(X, 1), Value) :-
  atomic(X), Value = X, !.
simplify(div(X, X), Value) :-
  atomic(X), Value is 1, !.
  
simplify(times(_, 0), Value) :-
  Value is 0, !.
simplify(times(0, _), Value) :-
  Value is 0, !.
simplify(times(X, 1), Value) :-
  atomic(X), Value = X, !.
simplify(times(1, X), Value) :-
  atomic(X), Value = X, !.

simplify(plus(X, 0), Value) :-
  atomic(X), Value = X, !.
simplify(plus(0, X), Value) :-
  atomic(X), Value = X, !.
simplify(plus(X, X), Value) :-
  atomic(X), atomic_list_concat([2,.,X], '', Value), !.
% simplify(plus(X, Y), Value) :-
%  atomic(X), atomic(Y), atomic_list_concat([X,+,Y], '', Value), assert(newatomic(Value)), !. 
% simplify(plus(X, Y), Value) :-
  % (newatomic(X) ; atomic(X)), (newatomic(Y) ; atomic(Y)), atomic_list_concat([X,+,Y], '', Value), assert(newatomic(Value)).
  
simplify(minus(X, X), Value) :-
  atomic(X), Value is 0, !.
simplify(minus(X, 0), Value) :-
  atomic(X), Value = X, !.

simplify(exp(0, _), Value) :-
  Value is 0, !.
simplify(exp(1, _), Value) :-
  Value is 1, !.
simplify(exp(X, 1), Value) :-
  atomic(X), Value = X, !.
simplify(exp(X, 0), Value) :-
  atomic(X), Value is 1, !.  
  
simplify(Num, Num):-number(Num).
simplify(T, T):-atomic(T).

simplify(times(Left, Right), Value) :-
  simplify(Right, RightVal),
  simplify(Left, LeftVal),
  simplify(times(LeftVal, RightVal), Value).

simplify(plus(Left, Right), Value) :-
  simplify(Left, LeftVal),
  simplify(Right, RightVal), 
  simplify(plus(LeftVal, RightVal), Value). 
  
simplify(minus(Left, Right), Value) :-
  simplify(Left, LeftVal),
  simplify(Right, RightVal),
  simplify(minus(LeftVal, RightVal), Value).
  
simplify(div(Left, Right), Value) :-
  simplify(Left, LeftVal),
  simplify(Right, RightVal),
  simplify(div(LeftVal, RightVal), Value).
