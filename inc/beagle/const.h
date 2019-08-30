#ifndef _BEAGLE_CONST_H
#define _BEAGLE_CONST_H

#ifdef __ASSEMBLY__
#define _AC(X,Y)        X
#define _AT(T,X)        X
#else
#define _AC(X,Y)        (X##Y)
#define _AT(T,X)        ((T)(X))
#endif /* !__ASSEMBLY__*/

#define _BITUL(x)       (_AC(1,UL) << (x))
#define _BITULL(x)      (_AC(1,ULL) << (x))


#ifdef __ASSEMBLY__
#define _ASSERT_SIZEOF(type, size)
#define _ASSERT_OFFSETOF(type, member, offset)
#else
#define _ASSERT_SIZEOF(type, size) \
	_Static_assert(sizeof(type) == (size), \
		#type " must be " #size " bytes");
#define _ASSERT_OFFSETOF(type, member, offset) \
	_Static_assert(offsetof(type, member) == (offset), \
		#type "." #member " must be at offset " #offset);
#endif /* !__ASSEMBLY__*/

#endif /* _BEAGLE_CONST_H */
