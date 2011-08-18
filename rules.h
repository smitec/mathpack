//
//  rules.h
//  mathpack-xcode
//
//  Created by Elliot Smith on 18/08/11.
//  Copyright 2011 smitec. All rights reserved.
//

/*
this class is used to apply some general mathematical rules to functions in order to try and simplify them
rules will be based on the "name" property of the classes. for example.
 
if you had a multiply node with two cos terms they could be changed to 1/2(cos(A+B) + cos(A-B))
 
these rules would produce alternative trees for a brance. when trying to simplify the rules class could they try rules
to make the tree as small as possible.
 
this is probably going to be a tonne harder than i think it will so we will ahve to see how it works out. perhaps a function
could store vectors of vectors as in a vector of alternative children for each rule that can be applied.
 
hopefully this can somehow then apply to things like A(B+C) -> AB + BC and so on (escpecially the reverse)
 
going to leave this blank for now need more thinking first. this is no doubt going to get crazy fast.
*/
#ifndef _rules_h_
#define _rules_h_



#endif
