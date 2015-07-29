hour(cs11, mwf1).
hour(cs12, tths2).
hour(cs13, mwf1).

hour(ch11, mwf2).
hour(ch12, tths1).
hour(ch13, mwf6).

hour(bio11, mwf1).
hour(bio12, mwf1).
hour(bio13, mwf4).

hour(chem11, mwf1).
hour(chem12, tths2).
hour(chem13, tths4).

hour(ha11, tths4).
hour(ha12, tths4).
hour(ha13, mwf1).

hour(hb11, tths2).
hour(hb12, tths4).
hour(hb13, mwf4).

cdc_eligibility(a7, cs41, [cs11, cs21]).
cdc_eligibility(a7, cs42, [cs13, cs23]).
cdc_eligibility(a7, cs51, [cs21]).
cdc_eligibility(a7, cs61, [cs13]).
cdc_eligibility(a7, cs71, [cs22, cs23]).

cdc_eligibility(a1, ch21, [ch11, ch12]).
cdc_eligibility(a1, ch32, [ch11, ch22]).
cdc_eligibility(a1, ch43, [ch12, ch13, ch21]).
cdc_eligibility(a1, ch51, [ch22, ch31, ch41]).
cdc_eligibility(a1, ch72, [ch72, ch73]).

cdc_eligibility(b1, bio21, [bio11, bio12]).
cdc_eligibility(b1, bio31, [bio11, bio21]).
cdc_eligibility(b1, bio32, [bio11, bio13, bio21]).
cdc_eligibility(b1, bio42, [bio31, bio32, bio21]).
cdc_eligibility(b1, bio51, [bio32]).

cdc_eligibility(h1, ha21, [ha11, ha12]).
cdc_eligibility(h1, ha22, [ha12]).
cdc_eligibility(h1, ha31, [ha11, ha21, ha12]).
cdc_eligibility(h1, ha32, [ha11, ha13, ha21]).

cdc_eligibility(h2, hb21, [hb11, hb12]).
cdc_eligibility(h2, hb22, [hb12]).
cdc_eligibility(h2, hb31, [hb11, hb21, hb12]).
cdc_eligibility(h2, hb32, [hb11, hb13, hb21]).

student(s1, a7, single).
student(s2, [a1,b2], dual).
student(s3, a7, single).
student(s4, h1, higher).
student(s5, h2, higher).
student(s6, [a1,b1], dual).
student(s7, a1, single).
student(s8, h1, higher).

mess_fees_paid(S) :- contains(S, [s1, s2, s3, s5, s6, s7, s8]).

dues_paid(S) :- contains(S, [s1, s2, s4, s5, s6, s7, s8]).

gradeCard_withheld(S) :- contains(S, [s7]).

gradeCard_incomplete(S) :- contains(S, [s3]).

debarred(S) :- contains(S, [s8]).

exceptionally_allowed_by_dean(S) :- contains(S, [s7]).

get_SingleDegree(S, Deg) :- student(S, Deg, single).
get_FirstDegree(S, Deg) :- student(S, [Deg, _], dual).
get_SecondDegree(S, Deg) :- student(S, [_, Deg], dual).

approved_by_dean(S) :-
					exceptionally_allowed_by_dean(S) ; 
					(	student(S, _, _), 
						mess_fees_paid(S), 
						dues_paid(S), 
						\+gradeCard_withheld(S), 
						\+gradeCard_incomplete(S), 
						\+debarred(S)	).
						
is_degree(X) :- contains(X, [a1, a7, b1, b2, h1, h2]).

done_all_courses(a1, UL, N) :- 
							match_all_courses(N, UL, 
							[ [1,ch11,ch12,ch13],
							  [2,ch21,ch22,ch23],
							  [3,ch31,ch32,ch33],
							  [4,ch41,ch42,ch43],
							  [5,ch51,ch52,ch53],
							  [6,ch61,ch62,ch63],
							  [7,ch71,ch72,ch73],
							  [8,ch81,ch82,ch83] ] ).

done_all_courses(a7, UL, N) :- 
							match_all_courses(N, UL, 
							[ [1,cs11,cs12,cs13],
							  [2,cs21,cs22,cs23],
							  [3,cs31,cs32,cs33],
							  [4,cs41,cs42,cs43],
							  [5,cs51,cs52,cs53],
							  [6,cs61,cs62,cs63],
							  [7,cs71,cs72,cs73],
							  [8,cs81,cs82,cs83] ]  ).

done_all_courses(b1, UL, N) :- 
							match_all_courses(N, UL, 
							[ [1,bio11,bio12,bio13],
							  [2,bio21,bio22,bio23],
							  [3,bio31,bio32,bio33],
							  [4,bio41,bio42,bio43],
							  [5,bio51,bio52,bio53],
							  [6,bio61,bio62,bio63],
							  [7,bio71,bio72,bio73],
							  [8,bio81,bio82,bio83] ] ).

done_all_courses(b2, UL, N) :- 
							match_all_courses(N, UL, 
							[ [1,chem11,chem12,chem13],
							  [2,chem21,chem22,chem23],
							  [3,chem31,chem32,chem33],
							  [4,chem41,chem42,chem43],
							  [5,chem51,chem52,chem53],
							  [6,chem61,chem62,chem63],
							  [7,chem71,chem72,chem73],
							  [8,chem81,chem82,chem83] ] ).

done_all_courses(h1, UL, N) :- 
							match_all_courses(N, UL, 
							[ [1,ha11,ha12,ha13],
							  [2,ha21,ha22,ha23],
							  [3,ha31,ha32,ha33],
							  [4,ha41,ha42,ha43],
							  [5,ha51,ha52,ha53],
							  [6,ha61,ha62,ha63],
							  [7,ha71,ha72,ha73],
							  [8,ha81,ha82,ha83] ] ).
							  
done_all_courses(h2, UL, N) :- 
							match_all_courses(N, UL, 
							[ [1,hb11,hb12,hb13],
							  [2,hb21,hb22,hb23],
							  [3,hb31,hb32,hb33],
							  [4,hb41,hb42,hb43],
							  [5,hb51,hb52,hb53],
							  [6,hb61,hb62,hb63],
							  [7,hb71,hb72,hb73],
							  [8,hb81,hb82,hb83] ] ).
							  
find_CDC_course(N, CDC, a1) :- 
							find_CDC_sem(N, CDC, 
							[ [1,ch11,ch12,ch13],
							  [2,ch21,ch22,ch23],
							  [3,ch31,ch32,ch33],
							  [4,ch41,ch42,ch43],
							  [5,ch51,ch52,ch53],
							  [6,ch61,ch62,ch63],
							  [7,ch71,ch72,ch73],
							  [8,ch81,ch82,ch83] ] ).
							
find_CDC_course(N, CDC, a7) :- 
							find_CDC_sem(N, CDC, 
							[ [1,cs11,cs12,cs13],
							  [2,cs21,cs22,cs23],
							  [3,cs31,cs32,cs33],
							  [4,cs41,cs42,cs43],
							  [5,cs51,cs52,cs53],
							  [6,cs61,cs62,cs63],
							  [7,cs71,cs72,cs73],
							  [8,cs81,cs82,cs83] ] ).

find_CDC_course(N, CDC, b1) :- 
							find_CDC_sem(N, CDC, 
							[ [1,bio11,bio12,bio13],
							  [2,bio21,bio22,bio23],
							  [3,bio31,bio32,bio33],
							  [4,bio41,bio42,bio43],
							  [5,bio51,bio52,bio53],
							  [6,bio61,bio62,bio63],
							  [7,bio71,bio72,bio73],
							  [8,bio81,bio82,bio83] ] ).

find_CDC_course(N, CDC, b2) :- 
							find_CDC_sem(N, CDC, 
							[ [1,chem11,chem12,chem13],
							  [2,chem21,chem22,chem23],
							  [3,chem31,chem32,chem33],
							  [4,chem41,chem42,chem43],
							  [5,chem51,chem52,chem53],
							  [6,chem61,chem62,chem63],
							  [7,chem71,chem72,chem73],
							  [8,chem81,chem82,chem83] ] ).

find_CDC_course(N, CDC, h1) :- 
							find_CDC_sem(N, CDC, 
							[ [1,ha11,ha12,ha13],
							  [2,ha21,ha22,ha23],
							  [3,ha31,ha32,ha33],
							  [4,ha41,ha42,ha43],
							  [5,ha51,ha52,ha53],
							  [6,ha61,ha62,ha63],
							  [7,ha71,ha72,ha73],
							  [8,ha81,ha82,ha83] ] ).

find_CDC_course(N, CDC, h2) :- 
							find_CDC_sem(N, CDC, 
							[ [1,hb11,hb12,hb13],
							  [2,hb21,hb22,hb23],
							  [3,hb31,hb32,hb33],
							  [4,hb41,hb42,hb43],
							  [5,hb51,hb52,hb53],
							  [6,hb61,hb62,hb63],
							  [7,hb71,hb72,hb73],
							  [8,hb81,hb82,hb83] ] ).
							  
find_DCOC_course(DCOC, D, UL) :- 
							is_degree(D), 
							find_DCOC_deg(DCOC, 
											[ a1,[chd1, chd2, chd3], 
											  a7,[csd1, chd2, chd3],
											  b1,[biod1, biod2, biod3],
											  b2,[chemd1, chemd2],
											  b3,[had1, had2, had3, had4],
											  b4,[hbd1, hbd2, hbd3] ], 
										  Deg), 
							(Deg == D -> dcoc_ownDeg(Deg, UL) ; dcoc_otherDeg(D, UL) ).

not(find_DCOC_deg(DCOC, [], Deg)).
find_DCOC_deg(DCOC, [Deg,CL|Rest], D2) :- 
									(contains(DCOC, CL), D2=Deg, !) ; find_DCOC_deg(DCOC, Rest, D2).
								
dcoc_ownDeg(D, UL) :- done_all_courses(D, UL, 8). 
dcoc_otherDeg(D, UL) :- done_all_courses(D, UL, 8). 

match_all_courses(N, UL, []).
match_all_courses(N, UL, [[I|SemL]|SemRest]) :- I =< N, contains_all(UL, SemL), match_all_courses(N, UL, SemRest).
match_all_courses(N, UL, [[I|SemL]|SemRest]) :- I > N, !.

find_CDC_sem(N, CDC, [[I|SemL]|SemRest]) :- contains(CDC, SemL), N is I, !.
find_CDC_sem(N, CDC, [[I|SemL]|SemRest]) :- find_CDC_sem(N, CDC, SemRest).

contains_all(UL, []). 
contains_all(UL, [SupLh|SupLr]) :- contains(SupLh, UL), contains_all(UL, SupLr). 

not(contains(X, [])).
contains(X, [X|Xs]).
contains(X, [H|Xs]):-contains(X, Xs).

not(schedule_clash([CLh|[]])).
schedule_clash([CLh|CLrest]) :- (single_clash(CLh, CLrest) ; schedule_clash(CLrest)), !.

not(single_clash(C, [])).
single_clash(C, [CRh|CRrest]) :- hour(C, T), hour(CRh, T).
single_clash(C, [CRh|CRrest]) :- hour(C, T1), hour(CRh, T2), single_clash(C, CRrest).

eligible_for_PS1(D, L) :- 
						is_degree(D), 
						done_all_courses(D, L, 4).

eligible_for_PS1(D1, D2, L) :- 
						is_degree(D1), 
						is_degree(D2),
						done_all_courses(D1, L, 4),
						done_all_courses(D2, L, 4).

eligible_for_PS1(S, L) :- 
						approved_by_dean(S), 
						student(S, _, single), 
						((get_SingleDegree(S, D), done_all_courses(D, L, 4)) 
						;(get_FirstDegree(S, D1), get_SecondDegree(S, D2), 
						done_all_courses(D1, L, 4), done_all_courses(D2, L, 4))).

eligible_for_PS2(D, L) :- 
						is_degree(D), 
						done_all_courses(D, L, 6).

eligible_for_PS2(D1, D2, L) :- 
						is_degree(D1),
						is_degree(D2),						
						done_all_courses(D1, L, 6),
						done_all_courses(D2, L, 6).

eligible_for_PS2(S, L) :- 
						approved_by_dean(S), 
						student(S, _, single), 
						((get_SingleDegree(S, D), done_all_courses(D, L, 6));
						get_FirstDegree(S, D1), done_all_courses(D1, L, 6),
						get_FirstDegree(S, D2), done_all_courses(D2, L, 6)).

eligible_for_CDC_SingleDegree(D, CDC, UL) :- 
											find_CDC_course(N, CDC, D), 
											done_all_courses(D, UL, N-1).
						
eligible_for_CDC_SingleDegree(S, CDC, UL) :- 
											approved_by_dean(S), 
											student(S, _, single), 
											get_SingleDegree(S, D), 
											find_CDC_course(N, CDC, D), 
											done_all_courses(D, UL, N-1).

eligible_for_CDC_FirstDegree(D, CDC, UL) :- 
											is_degree(D),
											find_CDC_course(N, CDC, D), 
											done_all_courses(D, UL, N-1).
											
eligible_for_CDC_FirstDegree(S, CDC, UL) :- 
											approved_by_dean(S), 
											student(S, _, dual), 
											get_FirstDegree(S, D), 
											find_CDC_course(N, CDC, D), 
											done_all_courses(D, UL, N-1).

eligible_for_CDC_SecondDegree(D1, D2, CDC, UL) :- 
											find_CDC_course(N, CDC, D2), 
											done_all_courses(D1, UL, N-1), 
											done_all_courses(D2, UL, N-1).
											
eligible_for_CDC_SecondDegree(S, CDC, UL) :- 
											approved_by_dean(S), 
											student(S, _, dual), 
											get_SecondDegree(S, D2), 
											find_CDC_course(N, CDC, D2), 
											get_FirstDegree(S, D1), 
											done_all_courses(D1, UL, N-1), 
											done_all_courses(D2, UL, N-1).

eligible_for_CDC_OtherDegree(D, CDC, UL) :- cdc_eligibility(D, CDC, ReqL), contains_all(UL, ReqL).

eligible_for_higher_EL(D, UL) :- 
								done_all_courses(D, UL, 8).
								
eligible_for_higher_EL(S, UL) :- 
								approved_by_dean(S), 
								student(S, _, _), 
								get_SingleDegree(S, D), 
								done_all_courses(D, UL, 8).

eligible_for_higher_EL_Dual(D1, D2, UL) :- 
										done_all_courses(D1, UL, 8).
										
eligible_for_higher_EL_Dual(S, UL) :- 
									approved_by_dean(S), 
									student(S, _, _), 
									get_FirstDegree(S, D), 
									done_all_courses(D, UL, 8).

eligible_for_DCOC(DCOC, D, UL) :- find_DCOC_course(DCOC, D, UL).