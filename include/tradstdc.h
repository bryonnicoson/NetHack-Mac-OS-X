/* NetHack 3.6	tradstdc.h	$NHDT-Date: 1448210011 2015/11/22 16:33:31 $  $NHDT-Branch: master $:$NHDT-Revision: 1.27 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef TRADSTDC_H
#define TRADSTDC_H

#if defined(DUMB) && !defined(NOVOID)
#define NOVOID
#endif

#ifdef NOVOID
#define void int
#endif

/*
 * Borland C provides enough ANSI C compatibility in its Borland C++
 * mode to warrant this.  But it does not set __STDC__ unless it compiles
 * in its ANSI keywords only mode, which prevents use of <dos.h> and
 * far pointer use.
 */
#if (defined(__STDC__) || defined(__TURBOC__)) && !defined(NOTSTDC)
#define NHSTDC
#endif

#if defined(ultrix) && defined(__STDC__) && !defined(__LANGUAGE_C)
/* Ultrix seems to be in a constant state of flux.  This check attempts to
 * set up ansi compatibility if it wasn't set up correctly by the compiler.
 */
#ifdef mips
#define __mips mips
#endif
#ifdef LANGUAGE_C
#define __LANGUAGE_C LANGUAGE_C
#endif
#endif

/*
 * ANSI X3J11 detection.
 * Makes substitutes for compatibility with the old C standard.
 */

/* Decide how to handle variable parameter lists:
 * USE_STDARG means use the ANSI <stdarg.h> facilities (only ANSI compilers
 * should do this, and only if the library supports it).
 * USE_VARARGS means use the <varargs.h> facilities.  Again, this should only
 * be done if the library supports it.  ANSI is *not* required for this.
 * Otherwise, the kludgy old methods are used.
 * The defaults are USE_STDARG for ANSI compilers, and USE_OLDARGS for
 * others.
 */

/* #define USE_VARARGS */ /* use <varargs.h> instead of <stdarg.h> */
/* #define USE_OLDARGS */ /* don't use any variable argument facilities */

#if defined(apollo) /* Apollos have stdarg(3) but not stdarg.h */
#define USE_VARARGS
#endif

#if defined(NHSTDC) || defined(ULTRIX_PROTO) || defined(MAC)
#if !defined(USE_VARARGS) && !defined(USE_OLDARGS) && !defined(USE_STDARG)
#define USE_STDARG
#endif
#endif

/*
 * These have changed since 3.4.3.  VA_END() now provides an explicit
 * closing brace to complement VA_DECL()'s hidden opening brace, so code
 * started with VA_DECL() needs an extra opening brace to complement
 * the explicit final closing brace.  This was done so that the source
 * would look less strange, where VA_DECL() appeared to introduce a
 * function whose opening brace was missing; there are now visible and
 * invisible braces at beginning and end.  Sample usage:
 void foo VA_DECL(int, arg)  --macro expansion has a hidden opening brace
 {  --new, explicit opening brace (actually introduces a nested block)
 VA_START(bar);
 ...code for foo...
 VA_END();  --expansion now provides a closing brace for the nested block
 }  --existing closing brace, still pairs with the hidden one in VA_DECL()
 * Reading the code--or using source browsing tools which match braces--
 * results in seeing a matched set of braces.  Usage of VA_END() is
 * potentially trickier, but nethack uses it in a straightforward manner.
 */

#include <stdarg.h>
#define VA_INIT(var1, typ1)
#define VA_NEXT(var1, typ1) var1 = va_arg(the_args, typ1)
#define VA_ARGS the_args
#define VA_START(x) va_start(the_args, x)

#if defined(NHSTDC) || defined(MSDOS) || defined(MAC) \
    || defined(ULTRIX_PROTO) || defined(__BEOS__)

/*
 * Used for definitions of functions which take no arguments to force
 * an explicit match with the NDECL prototype.  Needed in some cases
 * (MS Visual C 2005) for functions called through pointers.
 */
#define VOID_ARGS void

/* generic pointer, always a macro; genericptr_t is usually a typedef */
#define genericptr void *

#if (defined(ULTRIX_PROTO) && !defined(__GNUC__)) || defined(OS2_CSET2)
/* Cover for Ultrix on a DECstation with 2.0 compiler, which coredumps on
 *   typedef void * genericptr_t;
 *   extern void a(void(*)(int, genericptr_t));
 * Using the #define is OK for other compiler versions too.
 */
/* And IBM CSet/2.  The redeclaration of free hoses the compile. */
#define genericptr_t genericptr
#else
#if !defined(NHSTDC) && !defined(MAC)
#define const
#define signed
#define volatile
#endif
#endif

/*
 * Suppress `const' if necessary and not handled elsewhere.
 * Don't use `#if defined(xxx) && !defined(const)'
 * because some compilers choke on `defined(const)'.
 * This has been observed with Lattice, MPW, and High C.
 */
#if (defined(ULTRIX_PROTO) && !defined(NHSTDC)) || defined(apollo)
/* the system header files don't use `const' properly */
#ifndef const
#define const
#endif
#endif

#else /* NHSTDC */ /* a "traditional" C  compiler */

#define f(void) f()
#define FDECL(f, p) f()
#define VDECL(f, p) f()

#define VOID_ARGS /*empty*/

#if defined(AMIGA) || defined(HPUX) || defined(POSIX_TYPES) \
    || defined(__DECC) || defined(__BORLANDC__)
#define genericptr void *
#endif
#ifndef genericptr
#define genericptr char *
#endif

/*
 * Traditional C compilers don't have "signed", "const", or "volatile".
 */
#define signed
#define const
#define volatile

#endif /* NHSTDC */

#ifndef genericptr_t
typedef genericptr genericptr_t; /* (void *) or (char *) */
#endif

#if defined(MICRO) || defined(WIN32)
/* We actually want to know which systems have an ANSI run-time library
 * to know which support the %p format for printing pointers.
 * Due to the presence of things like gcc, NHSTDC is not a good test.
 * So we assume microcomputers have all converted to ANSI and bigger
 * computers which may have older libraries give reasonable results with
 * casting pointers to unsigned long int (fmt_ptr() in alloc.c).
 */
#define HAS_PTR_FMT
#endif

/*
 * According to ANSI, prototypes for old-style declarations must widen the
 * arguments to int.  However, the MSDOS compilers accept shorter arguments
 * (char, short, etc.) in prototypes and do typechecking with them.  Therefore
 * this mess to allow the better typechecking while also allowing some
 * prototypes for the ANSI compilers so people quit trying to fix the
 * prototypes to match the standard and thus lose the typechecking.
 */
#if defined(MSDOS) && !defined(__GO32__)
#define UNWIDENED_PROTOTYPES
#endif
#if defined(AMIGA) && !defined(AZTEC_50)
#define UNWIDENED_PROTOTYPES
#endif
#if defined(macintosh) && (defined(__SC__) || defined(__MRC__))
#define WIDENED_PROTOTYPES
#endif
#if defined(__MWERKS__) && defined(__BEOS__)
#define UNWIDENED_PROTOTYPES
#endif
#if defined(WIN32)
#define UNWIDENED_PROTOTYPES
#endif

#if defined(ULTRIX_PROTO) && defined(ULTRIX_CC20)
#define UNWIDENED_PROTOTYPES
#endif
#if defined(apollo)
#define UNWIDENED_PROTOTYPES
#endif

#ifndef UNWIDENED_PROTOTYPES
#if defined(NHSTDC) || defined(ULTRIX_PROTO) || defined(THINK_C)
#ifndef WIDENED_PROTOTYPES
#define WIDENED_PROTOTYPES
#endif
#endif
#endif

/* These are used for arguments within FDECL/VDECL prototype declarations.
 */
#define ALIGNTYP_P aligntyp

/* OBJ_P and MONST_P should _only_ be used for declaring function pointers.
 */
#if defined(ULTRIX_PROTO) && !defined(__STDC__)
/* The ultrix 2.0 and 2.1 compilers (on Ultrix 4.0 and 4.2 respectively) can't
 * handle "struct obj *" constructs in prototypes.  Their bugs are different,
 * but both seem to work if we put "void*" in the prototype instead.  This
 * gives us minimal prototype checking but avoids the compiler bugs.
 */
#define OBJ_P void *
#define MONST_P void *
#else
#define OBJ_P struct obj *
#define MONST_P struct monst *
#endif

#if 0
/* The problem below is still the case through 4.0.5F, but the suggested
 * compiler flags in the Makefiles suppress the nasty messages, so we don't
 * need to be quite so drastic.
 */
#if defined(__sgi) && !defined(__GNUC__)
/*
 * As of IRIX 4.0.1, /bin/cc claims to be an ANSI compiler, but it thinks
 * it's impossible for a prototype to match an old-style definition with
 * unwidened argument types.  Thus, we have to turn off all NetHack
 * prototypes, and avoid declaring several system functions, since the system
 * include files have prototypes and the compiler also complains that
 * prototyped and unprototyped declarations don't match.
 */
#undef NDECL
#undef FDECL
#undef VDECL
#define f(void) f()
#define FDECL(f, p) f()
#define VDECL(f, p) f()
#undef VOID_ARGS
#define VOID_ARGS /*empty*/
#endif
#endif

/* MetaWare High-C defaults to unsigned chars */
/* AIX 3.2 needs this also */
#if defined(__HC__) || defined(_AIX32)
#undef signed
#endif

/*
 * Allow gcc2 to check parameters of printf-like calls with -Wformat;
 * append this to a prototype declaration (see pline() in extern.h).
 */
#ifdef __GNUC__
#if __GNUC__ >= 2
#define PRINTF_F(f, v) __attribute__((format(printf, f, v)))
#endif
#if __GNUC__ >= 3
#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#endif
#endif

#ifndef PRINTF_F
#define PRINTF_F(f, v)
#endif
#ifndef UNUSED
#define UNUSED
#endif
#ifndef NORETURN
#define NORETURN
#endif

#endif /* TRADSTDC_H */
