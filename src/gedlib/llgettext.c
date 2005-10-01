/* 
   Copyright (c) 2000-2005 Perry Rapp
   "The MIT license"
   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*=============================================================
 * llgettext.c -- Some gettext related functions
 *==============================================================*/

#include "llstdlib.h"
#include "gedcom.h"
#include "codesets.h"
#include "lloptions.h"
#include "zstr.h"



/*==================================================
 * llgettext_init -- initialize gettext with initially
 *  desired codeset
 *================================================*/
void
llgettext_init (CNSTRING domain, CNSTRING codeset)
{
#if ENABLE_NLS
	STRING e;

	/* until we have an internal codeset (which is until we open a database)
	we want output in display codeset */
	set_gettext_codeset(PACKAGE, gui_codeset_out);

	/* allow run-time specification of locale directory */
	/* (LOCALEDIR is compile-time) */
	e = getoptstr("LocaleDir", "");
	if (e && *e) {
		bindtextdomain(PACKAGE, e);
		locales_notify_language_change(); /* TODO: is this necessary ? 2002-09-29, Perry */
	}

#else /* ENABLE_NLS */
	domain = domain;
	codeset = codeset;
#endif /* ENABLE_NLS */
}
/*=================================
 * init_win32_gettext_shim -- 
 *  Handle user-specified iconv dll path
 *===============================*/
void
init_win32_gettext_shim (void)
{
#if ENABLE_NLS
#ifdef WIN32_INTL_SHIM
	STRING e;
	/* (re)load gettext dll if specified */
	e = getoptstr("gettext.path", "");
	if (e && *e)
	{
		if (intlshim_set_property("dll_path", e))
		{
			bindtextdomain(PACKAGE, LOCALEDIR);
			textdomain(PACKAGE);
		}
		/* tell gettext where to find iconv */
		e = getoptstr("iconv.path", "");
		if (e && *e)
			gt_set_property("iconv_path", e);
	}
	/*
	We could be more clever, and if our iconv_path is no good, ask gettext
	if it found iconv, but that would make this logic tortuous due to our having
	different shim macros (we'd have to save gettext's iconv path before setting it,
	in case ours is bad & its is good).
	*/
#endif
#endif
}
/*=================================
 * set_gettext_codeset -- Tell gettext what codeset we want
 * Created: 2002/11/28 (Perry Rapp)
 *===============================*/
void
set_gettext_codeset (CNSTRING domain, CNSTRING codeset)
{
#if ENABLE_NLS
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
	static STRING prevcodeset = 0;
	if (eqstr_ex(prevcodeset, codeset))
		return;
	if (codeset && codeset[0]) {
		ZSTR zcsname=zs_new();
		/* extract just the codeset name, without any subcodings */
		/* eg, just "UTF-8" out of "UTF-8//TrGreekAscii//TrCyrillicAscii" */
		transl_parse_codeset(codeset, zcsname, 0);
		if (zs_str(zcsname)) {
			strupdate(&prevcodeset, zs_str(zcsname));
			/* gettext automatically appends //TRANSLIT */
		} else {
			/* what do we do if they gave us an empty one ? */
			strupdate(&prevcodeset, "ASCII");
		}
		zs_free(&zcsname);
	} else {
		/* 
		We need to set some codeset, in case it was set to 
		UTF-8 in last db 
		*/
		strupdate(&prevcodeset, gui_codeset_out);
	}
	bind_textdomain_codeset(domain, prevcodeset);
	if (eqstr(domain, PACKAGE))
		locales_notify_uicodeset_changes();
#else /* HAVE_BIND_TEXTDOMAIN_CODESET */
	/*
	local gettext doesn't give us an interface to specify codeset
	so nothing to do here
	*/
	domain = domain; /* unused */
	codeset = codeset; /* unused */
#endif /* HAVE_BIND_TEXTDOMAIN_CODESET */
#else
	domain = domain; /* unused */
	codeset = codeset; /* unused */
#endif /* ENABLE_NLS */
}