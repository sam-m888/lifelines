/* 
   Copyright (c) 1991-1999 Thomas T. Wetmore IV

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
/*==============================================================
 * date.c -- Code to process dates
 * Copyright(c) 1992-94 by T. T. Wetmore IV; all rights reserved
 *   2.3.4 - 24 Jun 93    2.3.5 - 17 Aug 93
 *   3.0.0 - 20 Jan 94    3.0.2 - 10 Nov 94
 *   3.0.3 - 17 Jul 95
 *============================================================*/
/* modified 05 Jan 2000 by Paul B. McBride (pmcbride@tiac.net) */
/* modified 2000-04-12 J.F.Chandler */

#include "sys_inc.h"
#include <time.h>
#include "llstdlib.h"
#include "table.h"
#include "interp.h"


/*********************************************
 * external/imported variables
 *********************************************/

extern STRING datep_from,datep_to,datep_frto;
extern STRING dater_bef,dater_aft,dater_betan;
extern STRING datea_abt,datea_est,datea_cal;
extern STRING datetrl_bc1,datetrl_bc2,datetrl_bc3,datetrl_bc4;
extern STRING datetrl_ad1,datetrl_ad2,datetrl_ad3,datetrl_ad4;

/*********************************************
 * local function prototypes
 *********************************************/

/* alphabetical */
static void analyze_numbers(GDATEVAL, struct gdate_s *, struct nums_s *);
static void analyze_word(GDATEVAL gdv, struct gdate_s * pdate
	, struct nums_s * nums, INT ival, BOOLEAN * newdate);
static void format_complex(GDATEVAL gdv, STRING output, INT len, STRING ymd2
	, STRING ymd3);
static void format_day(INT da, INT dfmt, STRING output);
static STRING format_month(INT, INT);
static void format_origin(struct gdate_s * pdate, CNSTRING ymd, INT ofmt
	, STRING output, INT len);
static STRING format_year(INT, INT);
static void format_ymd(STRING, STRING, STRING, INT, STRING*, INT *len);
static INT get_date_tok(INT*, STRING*);
static void init_keywordtbl(void);
static BOOLEAN is_date_delim(char c);
static BOOLEAN is_valid_day(struct gdate_s * pdate, INT day);
static BOOLEAN is_valid_month(struct gdate_s * pdate, INT month);
static mark_freeform(GDATEVAL gdv);
static mark_invalid(GDATEVAL gdv);
static void set_date_string(STRING);
static void set_year(struct gdate_s * pdate, INT yr);

/*********************************************
 * local types & variables
 *********************************************/

enum { MONTH_TOK=1, CHAR_TOK, WORD_TOK, ICONS_TOK, CALENDAR_TOK, YEAR_TOK };
enum { GD_ABT=1, GD_EST, GD_CAL, GD_BEF, GD_AFT, GD_BET, GD_AND, GD_FROM, GD_TO, GD_END1 };
enum { GD_BC=GD_END1, GD_AD, GD_END2 };

struct dateword_s {
	char *sl, *su, *ll, *lu;
};

/* English names of Gregorian/Julian months */
/* We need to internationalize this */
/* possibly add "jan" & "january" to align with modifiers */
static struct dateword_s months_gj[] = {
	{ "Jan", "JAN", "January", "JANUARY" }
	,{ "Feb", "FEB", "February", "FEBRUARY" }
	,{ "Mar", "MAR", "March", "MARCH" }
	,{ "Apr", "APR", "April", "APRIL" }
	,{ "May", "MAY", "May", "MAY" }
	,{ "Jun", "JUN", "June", "JUNE" }
	,{ "Jul", "JUL", "July", "JULY" }
	,{ "Aug", "AUG", "August", "AUGUST" }
	,{ "Sep", "SEP", "September", "SEPTEMBER" }
	,{ "Oct", "OCT", "October", "OCTOBER" }
	,{ "Nov", "NOV", "November", "NOVEMBER" }
	,{ "Dec", "DEC", "December", "DECEMBER" }
};

struct gedcom_keywords_s {
	STRING keyword;
	INT value;
};

/* GEDCOM keywords (fixed, not language dependent) */
static struct gedcom_keywords_s gedkeys[] = {
/* Gregorian/Julian months are values 1 to 12 */
	{ "JAN", 1 }
	,{ "FEB", 2 }
	,{ "MAR", 3 }
	,{ "APR", 4 }
	,{ "MAY", 5 }
	,{ "JUN", 6 }
	,{ "JUL", 7 }
	,{ "AUG", 8 }
	,{ "SEP", 9 }
	,{ "OCT", 10 }
	,{ "NOV", 11 }
	,{ "DEC", 12 }
/* modifiers are values 1001 to 1000+GD_END2 */
	,{ "ABT", 1000+GD_ABT }
	,{ "EST", 1000+GD_EST }
	,{ "CAL", 1000+GD_CAL }
	,{ "BEF", 1000+GD_BEF }
	,{ "AFT", 1000+GD_AFT }
	,{ "BET", 1000+GD_BET }
	,{ "AND", 1000+GD_AND }
	,{ "FROM", 1000+GD_FROM }
	,{ "TO", 1000+GD_TO }
/* calendars are values 2001 to 2000+GDV_CALENDARS_IX */
	,{ "@#DGREGORIAN@", 2000+GDV_GREGORIAN }
	,{ "@#DJULIAN@", 2000+GDV_JULIAN }
	,{ "@#DHEBREW@", 2000+GDV_HEBREW }
	,{ "@#DFRENCH R@", 2000+GDV_FRENCH }
	,{ "@#DROMAN@", 2000+GDV_ROMAN }
/* BC */
	,{ "B.C.", 1000+GD_BC } /* TODO: handle "(B.C.)" ? */
/* Some liberal (non-GEDCOM) entries */
	,{ "BC", GD_BC }
	,{ "B.C.E.", GD_BC }
	,{ "BCE", GD_BC }
	,{ "A.D.", GD_AD }
	,{ "AD", GD_AD }
	,{ "C.E.", GD_AD }
	,{ "CE", GD_AD }
/* Some liberal (non-GEDCOM) but English-biased entries */
	,{ "JANUARY", 1 }
	,{ "FEBRUARY", 2 }
	,{ "MARCH", 3 }
	,{ "APRIL", 4 }
	,{ "MAY", 5 }
	,{ "JUNE", 6 }
	,{ "JULY", 7 }
	,{ "AUGUST", 8 }
	,{ "SEPTEMBER", 9 }
	,{ "OCTOBER", 10 }
	,{ "NOVEMBER", 11 }
	,{ "DECEMBER", 12 }
	,{ "ABOUT", 1000+GD_ABT }
	,{ "ESTIMATED", 1000+GD_EST }
	,{ "CALCULATED", 1000+GD_CAL }
	,{ "BEFORE", 1000+GD_BEF }
	,{ "AFTER", 1000+GD_AFT }
	,{ "BETWEEN", 1000+GD_BET }
};


/* GEDCOM Hebrew months */
/* keywordtbl values 101-113 */
static struct dateword_s months_heb[] = {
	{ "Tsh", "TSH", "Tishri", "TISHRI" }
	/* TODO: Finish & implement these, but wait til we
	internationalize the gregorian/julian ones */
};


/* English representation of GEDCOM date modifiers */
/* We need to internationalize this */
#ifdef NOT_USED_CURRENTLY
static struct dateword_s modifiers[] = {
	{ "abt", "ABT", "about", "ABOUT" }      /* 1 */
	,{ "est", "EST", "estimated", "ESTIMATED" }      /* 1 */
	,{ "cal", "CAL", "calculated", "CALCULATED" }      /* 1 */
	,{ "bef", "BEF", "before", "BEFORE" }   /* 2 */
	,{ "aft", "AFT", "after", "AFTER" }     /* 3 */
	,{ "bet", "BET", "between", "BETWEEN" } /* 4 - potential range */
	,{ "and", "AND", "and", "AND" }         /* 5 */
	,{ "from", "FROM", "from", "FROM" }     /* 6 - potential range */
	,{ "to", "TO", "to", "TO" }             /* 7 */
};
#endif

/* TODO: "B.C." is GEDCOM, but the rest are English, so
figure out what to do about internationalizing this */


/* used in parsing dates -- 1st, 2nd, & 3rd numbers found */
struct nums_s { INT num1; INT num2; INT num3; };

static STRING sstr = NULL;
static TABLE keywordtbl = NULL;

/*==========================================
 * do_format_date -- Do general date formatting
 * str - raw string containing a date
 *  dfmt; [IN]  day format code (see format_day function below)
 *  mfmt: [IN]  month format code (see format_month function below)
 *  yfmt: [IN]  year format code (see format_year function below)
 *  sfmt: [IN]  combining code (see format_ymd function below)
 *  ofmt: [IN]  origin format (see format_origin function below)
 * cmplx - 0 is year only, 1 is complex, including
 *         date modifiers, ranges, and/or double-dating
 * Returns static buffer
 *========================================*/
STRING
do_format_date (STRING str, INT dfmt, INT mfmt,
             INT yfmt, INT sfmt, INT ofmt, INT cmplx)
{
	STRING smo, syr;
	static char daystr[3], ymd[50], ymd2[60], ymd3[60], complete[100];
	STRING p;
	INT len;
	GDATEVAL gdv = 0;
	if (!str) return NULL;
	if (sfmt==12) {
		/* This is what used to be the shrt flag */
		return shorten_date(str);
	}
	if (sfmt==14) {
		llstrncpy(complete, str, sizeof(complete));
		return complete;
	}
	if (!cmplx) {
		/* simple */
		gdv = extract_date(str);
		format_day(gdv->date1.day, dfmt, daystr);
		smo = format_month(gdv->date1.month, mfmt);
		syr = format_year(gdv->date1.year, yfmt);
		p = ymd;
		len = sizeof(ymd);
		*p = 0;
		format_ymd(syr, smo, daystr, sfmt, &p, &len);
		format_origin(&gdv->date1, ymd, ofmt, ymd2, sizeof(ymd2));
		free_gdateval(gdv);
		return ymd2;
	} else {
		/* complex (include modifier words) */
		gdv = extract_date(str);
		format_day(gdv->date1.day, dfmt, daystr);
		smo = format_month(gdv->date1.month, mfmt);
		syr = (gdv->date1.yearstr ? gdv->date1.yearstr 
			: format_year(gdv->date1.year, yfmt));
		p = ymd;
		len = sizeof(ymd);
		*p = 0;
		format_ymd(syr, smo, daystr, sfmt, &p, &len);
		format_origin(&gdv->date1, ymd, ofmt, ymd2, sizeof(ymd2));
		if (gdateval_isdual(gdv)) {
			/* build 2nd date string into ymd3 */
			format_day(gdv->date2.day, dfmt, daystr);
			smo = format_month(gdv->date2.month, mfmt);
			syr = (gdv->date2.yearstr ? gdv->date2.yearstr 
				: format_year(gdv->date2.year, yfmt));
			p = ymd;
			len = sizeof(ymd);
			*p = 0;
			format_ymd(syr, smo, daystr, sfmt, &p, &len);
			format_origin(&gdv->date2, ymd, ofmt, ymd3, sizeof(ymd3));
		} else {
			ymd3[0] = 0;
		}
		format_complex(gdv, complete, sizeof(complete), ymd2, ymd3);
		free_gdateval(gdv);
		return complete;
	}
}
/*===================================================
 * format_origin -- Add AD/BC info to date
 *  pdate:  [IN]  actual date information
 *  ymd:    [IN]  date string consisting of yr, mo, da portion
 *  ofmt:   [IN]  origin format code
 *                0 - no AD/BC marker
 *                1 - trailing B.C. if appropriate
 *                2 - trailing A.D. or B.C.
 *               11 - trailing BC if appropriate
 *               12 - trailng AD or BC
 *               21 - trailing B.C.E. if appropriate
 *               22 - trailing C.E. or B.C.E.
 *               31 - trailing BCE if appropriate
 *               32 - trailing CE or BCE
 *  output: [IN]  buffer in which to write
 *  len:    [IN]  size of buffer
 * Created: 2001/12/28 (Perry Rapp)
 *=================================================*/
static void
format_origin (struct gdate_s * pdate, CNSTRING ymd, INT ofmt, STRING output
	, INT len)
{
	/* TODO: calendar-specific handling */
	if (pdate->origin == GDV_BC) {
		if (ofmt > 0) {
			STRING p = output;
			STRING tag = 0;
			p[0] = 0;
			llstrcatn(&p, ymd, &len);
			switch (ofmt/10) {
				case 1: tag = datetrl_bc2; break;
				case 2: tag = datetrl_bc3; break;
				case 3: tag = datetrl_bc4; break;
			}
			/* this way we handle if, eg, datetrl_bc4 is blank */
			if (!tag || !tag[0])
				tag = datetrl_bc1;
			llstrcatn(&p, " ", &len);
			llstrcatn(&p, tag, &len);
			return;
		}
	} else {
		if (ofmt > 1) {
			STRING p = output;
			STRING tag = 0;
			p[0] = 0;
			llstrcatn(&p, ymd, &len);
			switch (ofmt/10) {
				case 1: tag = datetrl_ad2; break;
				case 2: tag = datetrl_ad3; break;
				case 3: tag = datetrl_ad4; break;
			}
			/* this way we handle if, eg, datetrl_ad4 is blank */
			if (!tag || !tag[0])
				tag = datetrl_ad1;
			llstrcatn(&p, " ", &len);
			llstrcatn(&p, tag, &len);
			return;
		}
	}
	/* no trailing tag at all */
	llstrncpy(output, ymd, len);

}
/*===================================================
 * format_complex -- Format date string with modifiers
 *  gdv:    [IN]   actual date_val
 *  output: [IN]   whither to write string
 *  len:    [IN]   size of output
 *  ymd2:   [IN]   formatted date1
 *  ymd3:   [IN]   formatted date2 (only used for dual dates)
 *                  (ie, full period or full range)
 * Created: 2001/12/28 (Perry Rapp)
 *=================================================*/
static void
format_complex (GDATEVAL gdv, STRING output, INT len, STRING ymd2, STRING ymd3)
{
	switch (gdv->type) {
	case GDV_PERIOD:
		switch (gdv->subtype) {
		case GDVP_FROM:
			snprintf(output, len, datep_from, ymd2);
			break;
		case GDVP_TO:
			snprintf(output, len, datep_to, ymd2);
			break;
		case GDVP_FROM_TO:
			snprintf(output, len, datep_frto, ymd2, ymd3);
			break;
		default:
			FATAL(); /* invalid period subtype */
			break;
		}
		break;
	case GDV_RANGE:
		switch (gdv->subtype) {
		case GDVR_BEF:
			snprintf(output, len, dater_bef, ymd2);
			break;
		case GDVR_AFT:
		case GDVR_BET:
			snprintf(output, len, dater_aft, ymd2);
			break;
		case GDVR_BET_AND:
			snprintf(output, len, dater_betan, ymd2, ymd3);
			break;
		default:
			FATAL(); /* invalid period subtype */
			break;
		}
		break;
	case GDV_APPROX:
		switch (gdv->subtype) {
		case GDVA_ABT:
			snprintf(output, len, datea_abt, ymd2);
			break;
		case GDVA_EST:
			snprintf(output, len, datea_est, ymd2);
			break;
		case GDVA_CAL:
			snprintf(output, len, datea_cal, ymd2);
			break;
		}
		break;
	case GDV_DATE:
	default:
		snprintf(output, len, ymd2);
		break;
	}
}
/*===================================================
 * format_ymd -- Assembles date according to dateformat
 *  syr:    [IN]   year string
 *  smo:    [IN]   month string
 *  sda:    [IN]   day string
 *  sfmt:   [IN]   format code
 *                 0 - da mo yr
 *                 1 - mo da, yr
 *                 2 - mo/da/yr
 *                 3 - da/mo/yr
 *                 4 - mo-da-yr
 *                 5 - da-mo-yr
 *                 6 - modayr
 *                 7 - damoyr
 *                 8 - yr mo da
 *                 9 - yr/mo/da
 *                 10- yr-mo-da
 *                 11- yrmoda
 *                 12- yr   (year only, old short form)
 *                 13- dd/mo yr
 *                 14- as in GEDCOM (truncated to 50 chars)
 *  mod:    [IN]   modifier code (in bottom of monthstrs array)
 *  output: [I/O]  output string (is advanced)
 *  len:    [I/O]  length remaining in output string buffer (is decremented)
 *=================================================*/
static void
format_ymd (STRING syr, STRING smo, STRING sda, INT sfmt
	, STRING *output, INT * len)
{
	STRING p = *output;

	switch (sfmt) {
	case 0:		/* da mo yr */
		if (sda) {
			llstrcatn(&p, sda, len);
			llstrcatn(&p, " ", len);
		}
		if (smo) {
			llstrcatn(&p, smo, len);
			llstrcatn(&p, " ", len);
		}
		if (syr) {
			llstrcatn(&p, syr, len);
		}
		break;
	case 1:		/* mo da, yr */
		if (smo) {
			llstrcatn(&p, smo, len);
			llstrcatn(&p, " ", len);
		}
		if (sda) {
			llstrcatn(&p, sda, len);
			llstrcatn(&p, ", ", len);
		}
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 2:		/* mo/da/yr */
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, "/", len);
		if (sda)
			llstrcatn(&p, sda, len);
		llstrcatn(&p, "/", len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 3:		/* da/mo/yr */
		if (sda)
			llstrcatn(&p, sda, len);
		llstrcatn(&p, "/", len);
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, "/", len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 4:		/* mo-da-yr */
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, "-", len);
		if (sda)
			llstrcatn(&p, sda, len);
		llstrcatn(&p, "-", len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 5:		/* da-mo-yr */
		if (sda)
			llstrcatn(&p, sda, len);
		llstrcatn(&p, "-", len);
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, "-", len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 6:		/* modayr */
		if (smo)
			llstrcatn(&p, smo, len);
		if (sda)
			llstrcatn(&p, sda, len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 7:		/* damoyr */
		if (sda)
			llstrcatn(&p, sda, len);
		if (smo)
			llstrcatn(&p, smo, len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	case 8:         /* yr mo da */
		if (syr)
			llstrcatn(&p, syr, len);
		if (smo) {
			llstrcatn(&p, " ", len);
			llstrcatn(&p, smo, len);
		}
		if (sda) {
			llstrcatn(&p, " ", len);
			llstrcatn(&p, sda, len);
		}
		break;
	case 9:         /* yr/mo/da */
		if (syr)
			llstrcatn(&p, syr, len);
		llstrcatn(&p, "/", len);
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, "/", len);
		if (sda)
			llstrcatn(&p, sda, len);
		break;
	case 10:        /* yr-mo-da */
		if (syr)
			llstrcatn(&p, syr, len);
		llstrcatn(&p, "-", len);
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, "-", len);
		if (sda)
			llstrcatn(&p, sda, len);
		break;
	case 11:        /* yrmoda */
		if (syr)
			llstrcatn(&p, syr, len);
		if (smo)
			llstrcatn(&p, smo, len);
		if (sda)
			llstrcatn(&p, sda, len);
		break;
	/* 12 (year only) was handled directly in do_format_date */
	case 13:      /* mo/da yr */
		if (sda)
			llstrcatn(&p, sda, len);
		llstrcatn(&p, "/", len);
		if (smo)
			llstrcatn(&p, smo, len);
		llstrcatn(&p, " ", len);
		if (syr)
			llstrcatn(&p, syr, len);
		break;
	/* 14 (as GEDCOM) was handled directly in do_format_date */
        }
	*output = p;
	return;
}

/*=======================================
 * format_day -- Formats day part of date
 *  day:   [IN]  numeric day (0 for unknown)
 *  dfmt:  [IN]    0 - num, space
 *                 1 - num, lead 0
 *                 2 - num, as is
 * output: [I/O] buffer in which to write
 *                must be at least 3 characters
 *=====================================*/
static void
format_day (INT da, INT dfmt, STRING output)
{
	STRING p;
	if (da < 0 || da > 99 || dfmt < 0 || dfmt > 2) {
		output[0] = 0;
		return;
	}
	strcpy(output, "  ");
	if (da >= 10) {
		/* dfmt irrelevant with 2-digit days */
		output[0] = da/10 + '0';
		output[1] = da%10 + '0';
		return;
	}
	p = output;
	if (da == 0) {
		if (dfmt == 2)
			output[0] = 0;
		return;
	}
	if (dfmt == 0)
		p++; /* leading space */
	else if (dfmt == 1)
		*p++ = '0'; /* leading 0 */
	*p++ = da + '0';
	*p = 0;
}
/*===========================================
 * format_month -- Formats month part of date
 *  mo:    [IN]  numeric month (0 for unknown)
 *  mfmt:  [IN]    0 - num, space
 *                 1 - num, lead 0
 *                 2 - num, as is
 *                 3 - eg, MAR
 *                 4 - eg, Mar
 *                 5 - eg, MARCH
 *                 6 - eg, March
 *TODO: Add roman numerals (as seen in Central Europe)
 * but wait to see what happens with cmplx numbers
 * because we might add "mar" & "march" here
 * and it would be nice to keep all spelt ones contiguous
 *  returns static buffer or string constant or 0
 *=========================================*/
static STRING
format_month (INT mo, INT mfmt)
{
	static char scratch[3];
	if (mo < 0 || mo > 12 || mfmt < 0 || mfmt > 6) return NULL;
	if (mfmt <= 2)  {
		format_day(mo, mfmt, scratch);
		return scratch;
	}
	if (mo == 0) return (STRING) "   ";
	/* TODO: we need to deal with calendars here */
	switch (mfmt) {
	case 3: return (STRING) months_gj[mo-1].su;
	case 4: return (STRING) months_gj[mo-1].sl;
	case 5: return (STRING) months_gj[mo-1].lu;
	case 6: return (STRING) months_gj[mo-1].ll;
	}
	return NULL;
}
/*=========================================
 * format_year -- Formats year part of date
 *  yr:    [IN]  numeric year (0 for unknown)
 *  yfmt:  [IN]    0 - num, space
 *                 1 - num, lead 0
 *                 2 - num, as is
 *
 * (No point in supporting negative numbers here, because parser only
 *  picks up positive numbers.)
 *  returns static buffer or 0
 *=======================================*/
static STRING
format_year (INT yr, INT yfmt)
{
	static char scratch[7];
	STRING p;
	if (yr > 9999 || yr < 0) {
		switch(yfmt) {
		case 0:
			return "    ";
		case 1:
			return "0000";
		default:
			return NULL;
		}
	}
	if (yr > 999 || yfmt == 2) {
		sprintf(scratch, "%d", yr);
		return scratch;
	}
	p = (yfmt==1 ? "000" : "   ");
	if (yr < 10)
		strcpy(scratch, p);
	else if (yr < 100)
		llstrncpy(scratch, p, 2+1);
	else
		llstrncpy(scratch, p, 1+1);
	sprintf(scratch+strlen(scratch), "%d", yr);
	return scratch;
}
/*=====================================================
 * mark_invalid -- Set a gdate_val to invalid
 *  gdv:  [I/O]  date_val we are building
 *===================================================*/
static
mark_invalid (GDATEVAL gdv)
{
	gdv->valid = -1;
}
/*=====================================================
 * mark_freeform -- Set a gdate_val to freeform (unless invalid)
 *  gdv:  [I/O]  date_val we are building
 *===================================================*/
static
mark_freeform (GDATEVAL gdv)
{
	if (gdv->valid > 0)
		gdv->valid = 0;
}
/*=====================================================
 * extract_date -- Extract date from free format string
 *  str:  [IN]  date to parse
 * returns new date_val
 *===================================================*/
GDATEVAL
extract_date (STRING str)
{
	/* we accumulate numbers to figure when we finish a
	date (with a full period or range, we may finish the
	first date partway thru) */
	INT tok, ival;
	struct nums_s nums = { -99999, -99999, -99999 };
	STRING sval;
	static unsigned char yrstr[10];
	GDATEVAL gdv = create_gdateval();
	struct gdate_s * pdate = &gdv->date1;
	BOOLEAN newdate;
	if (str) set_date_string(str);
	while ((tok = get_date_tok(&ival, &sval))) {
		switch (tok) {
		case MONTH_TOK:
			if (!pdate->month) {
				pdate->month = ival;
				if (nums.num1 != -99999) {
					/* if number before month, it is a day if legal */
					if (nums.num2 == -99999 && is_valid_day(pdate, nums.num1)) {
						pdate->day = nums.num1;
						nums.num1 = -99999;
					} else {
						mark_freeform(gdv);
					}
				}
			}
			else
				mark_invalid(gdv);
			continue;
		case CALENDAR_TOK:
			if (!pdate->calendar)
				pdate->calendar = ival;
			else
				mark_invalid(gdv);
			continue;
		case YEAR_TOK:
			if (pdate->year == -99999) {
				pdate->year = ival;
				if (sval) /* alphanum year */
					pdate->yearstr = strdup(sval);
			} else {
				mark_invalid(gdv);
			}
			continue;
		case CHAR_TOK:
			/* this was anything unrecognized, including unusable
			numeric strings */
			mark_freeform(gdv);
			continue;
		case WORD_TOK:
			analyze_word(gdv, pdate, &nums, ival, &newdate);
			if (newdate) {
				analyze_numbers(gdv, pdate, &nums);
				nums.num1 = nums.num2 = nums.num3 = -99999;
				pdate = &gdv->date2;
			}
			continue;
		case ICONS_TOK:
			/* number */
			if (nums.num1 == -99999)
				nums.num1 = ival;
			else if (nums.num2 == -99999)
				nums.num2 = ival;
			else if (nums.num3 == -99999)
				nums.num3 = ival;
			else
				mark_freeform(gdv);
			continue;
		default:
			FATAL();
		}
	}
	/* now analyze what numbers we got */
	analyze_numbers(gdv, pdate, &nums);
	gdv->text = strdup(str);
	return gdv;
}
/*===============================================
 * analyze_word -- Interpret word found in date parsing
 *  gdv:     [I/O] current date_val we are building
 *  pdate:   [IN]  points to which date we're on
 *                 (&gdv->date1, unless finishing a range or period)
 *  nums     [I/O] accumulated potential numbers
 *  ival:    [IN]  word enum value (eg, GD_AFT)
 *  newdate: [OUT] flag we set if we are switching to 2nd date now
 * Created: 2001/12/28 (Perry Rapp)
 *=============================================*/
static void
analyze_word (GDATEVAL gdv, struct gdate_s * pdate, struct nums_s * nums
	, INT ival, BOOLEAN * newdate)
{
	/* GEDCOM word modifiers */
	*newdate = FALSE;
	if (gdv->type) {
		/* already have a modifier -- very few 2nd modifiers are allowed */
		switch (ival) {
		case GD_AND:
			if (gdv->type==GDV_RANGE && gdv->subtype==GDVR_BET) {
				gdv->subtype = GDVR_BET_AND;
				*newdate = TRUE;
			} else {
				mark_invalid(gdv);
			}
			break;
		case GD_TO:
			if (gdv->type==GDV_PERIOD && gdv->subtype==GDVP_FROM) {
				gdv->subtype = GDVP_FROM_TO;
				*newdate = TRUE;
			} else {
				mark_invalid(gdv);
			}
			break;
		case GD_AD:
			if (pdate->origin)
				mark_invalid(gdv);
			else {
				mark_freeform(gdv); /* AD is not in GEDCOM */
				pdate->origin = GDV_AD;
			}
			break;
		case GD_BC:
			if (pdate->origin)
				mark_invalid(gdv);
			else
				pdate->origin = GDV_BC;
			break;
		default:
			mark_invalid(gdv);
			break;
		}
	} else {
		/* first modifier */
		switch (ival) {
		case GD_ABT:
			gdv->type = GDV_APPROX;
			gdv->subtype = GDVA_ABT;
			break;
		case GD_EST:
			gdv->type = GDV_APPROX;
			gdv->subtype = GDVA_EST;
			break;
		case GD_CAL:
			gdv->type = GDV_APPROX;
			gdv->subtype = GDVA_CAL;
			break;
		case GD_BEF:
			gdv->type = GDV_RANGE;
			gdv->subtype = GDVR_BEF;
			break;
		case GD_AFT:
			gdv->type = GDV_RANGE;
			gdv->subtype = GDVR_AFT;
			break;
		case GD_BET:
			gdv->type = GDV_RANGE;
			gdv->subtype = GDVR_BET;
			break;
		/* AND is not a legal first modifier */
		case GD_FROM:
			gdv->type = GDV_PERIOD;
			gdv->subtype = GDVP_FROM;
			break;
		case GD_TO:
			if (pdate->day || pdate->month || pdate->year != -99999
				|| nums->num1 != -99999) {
				/* if we have a date before TO, switch to 2nd date */
				/* (This is not legal GEDCOM syntax, however */
				*newdate = TRUE;
				gdv->type = GDV_PERIOD;
				gdv->subtype = GDVP_FROM_TO;
				analyze_numbers(gdv, pdate, nums);
				mark_freeform(gdv);
			} else {
				gdv->type = GDV_PERIOD;
				gdv->subtype = GDVP_TO;
			}
			break;
		case GD_AD:
			if (pdate->origin)
				mark_invalid(gdv);
			else {
				mark_freeform(gdv); /* AD is not in GEDCOM */
				pdate->origin = GDV_AD;
			}
			break;
		case GD_BC:
			if (pdate->origin)
				mark_invalid(gdv);
			else
				pdate->origin = GDV_BC;
			break;
		default:
			mark_invalid(gdv);
			break;
		}
	}
}
/*===============================================
 * analyze_numbers -- Parse numbers found in date
 *  gdv:  [I/O]  date_val we are building
 *  pdate: [IN]  pointer to current date (usually &gdv->date1)
 *  nums:  [IN]  unassigned numbers found in date line
 * This function does not clear the numbers -- caller must do so.
 * Created: 2001/12/28 (Perry Rapp)
 *=============================================*/
static void
analyze_numbers (GDATEVAL gdv, struct gdate_s * pdate, struct nums_s * nums)
{
	if (nums->num1 == -99999) {
		/* if we have no numbers, we're done */
		return;
	}
	/* we have at least 1 number */
	if (pdate->day && pdate->month && pdate->year != -99999) {
		/* if we already have day & month & year, we're done */
		return;
	}
	/* we need something */
	if (nums->num2 == -99999) {
		/* if we only have 1 number */
		if (pdate->year == -99999) {
			/* if we need year, it is year */
			set_year(pdate, nums->num1);
			return;
		}
		if (pdate->month && is_valid_day(pdate, nums->num1)) {
			/* if we only need day, it is day (if legal) */
			pdate->day = nums->num1;
			return;
		}
		/* otherwise give up (ignore it) */
		return;
	}
	/* we have at least 2 numbers */
	if (pdate->day && pdate->month) {
		/* if all we need is year, then it is year */
		set_year(pdate, nums->num1);
		return;
	}
	/* we've exhausted all the reasonably GEDCOM cases */
	mark_freeform(gdv);
	if (pdate->month && pdate->year != -99999) {
		/* if all we need is day, see if it can be day */
		if (is_valid_day(pdate, nums->num1)) {
			pdate->day = nums->num1;
		}
		return;
	}
	if (pdate->month) {
		/* if we get here, we need year */
		/* prefer first num for day, if legal */
		if (is_valid_day(pdate, nums->num1)) {
			pdate->day = nums->num1;
			set_year(pdate, nums->num2);
		} else {
			set_year(pdate, nums->num1);
			if (is_valid_day(pdate, nums->num2))
				pdate->day = nums->num2;
		}
		return;
	}
	/* if we get here, we need month */
	if (pdate->year != -99999) {
		/* we have year, but not month, and we have at least 2 numbers */
		/* TODO */
		return;
	}
	/* if we get here, we need month & year */
	if (nums->num3 == -99999) {
		/* we have no month & only two numbers, so only do year */
		set_year(pdate, nums->num1);
		return;
	}
	/* we have three numbers, and need at least month & year */
	if (pdate->day) {
		/* we have 3 numbers, but only need month & year, so only do year */
		set_year(pdate, nums->num1);
		return;
	}
	/* we have 3 numbers, and need day, month, & year */
	/* how about day, month, year ? */
	if (is_valid_day(pdate, nums->num1) && is_valid_month(pdate, nums->num2)) {
		pdate->day = nums->num1;
		pdate->month = nums->num2;
		set_year(pdate, nums->num3);
	}
	/* how about month, day, year ? */
	if (is_valid_month(pdate, nums->num1) && is_valid_day(pdate, nums->num2)) {
		pdate->day = nums->num2;
		pdate->month = nums->num1;
		set_year(pdate, nums->num3);
	}
	/* how about year, month, day ? */
	if (is_valid_day(pdate, nums->num3) && is_valid_month(pdate, nums->num2)) {
		pdate->day = nums->num3;
		pdate->month = nums->num2;
		set_year(pdate, nums->num1);
	}
	/* give up */
}
/*===============================================
 * set_year -- Helper to assign year number
 *  (fills in the year string also)
 *  pdate:  [I/O]  date we are building
 *  yr:     [IN]   new year number
 * Created: 2001/12/28 (Perry Rapp)
 *=============================================*/
static void
set_year (struct gdate_s * pdate, INT yr)
{
	pdate->year = yr;
	/* we leave yearstr as 0 because we have a normal numeric year */
	pdate->yearstr = 0;
}
/*===============================================
 * create_gdateval -- Create new, empty GEDCOM date_val
 * Created: 2001/12/28 (Perry Rapp)
 *=============================================*/
GDATEVAL
create_gdateval (void)
{
	GDATEVAL gdv = (GDATEVAL)stdalloc(sizeof(*gdv));
	memset(gdv, 0, sizeof(*gdv));
	gdv->date1.year = -99999;
	gdv->date2.year = -99999;
	gdv->valid = 1;
	return gdv;

}
/*===============================================
 * free_gdateval -- Delete existing GEDCOM date_val
 * Created: 2001/12/28 (Perry Rapp)
 *=============================================*/
void
free_gdateval (GDATEVAL gdv)
{
	if (!gdv) return;
	if (gdv->date1.yearstr)
		stdfree(gdv->date1.yearstr);
	if (gdv->date2.yearstr)
		stdfree(gdv->date2.yearstr);
	if (gdv->text)
		stdfree(gdv->text);
	stdfree(gdv);
}
/*===============================================
 * set_date_string -- Store date extraction string
 *  in static buffer for use during subsequent parsing
 *=============================================*/
static void
set_date_string (STRING str)
{
	sstr = str;
	if (!keywordtbl)
		init_keywordtbl();
}
/*==================================================
 * get_date_tok -- Return next date extraction token
 *  pival:   [OUT]  word enum value (eg, GD_AFT)
 *  psval:   [OUT]  pointer to (static) copy of original text
 *                   (only used for slash years)
 *================================================*/
static INT
get_date_tok (INT *pival, STRING *psval)
{
	static unsigned char scratch[30];
	STRING p = scratch;
	INT c;
	if (!sstr) return 0;
	*psval = NULL;
	while (iswhite((uchar)*sstr++))
		;
	sstr--;
	if (isletter((uchar)*sstr)) {
		INT i;
		/* collect all letters (to end or whitespace) into scratch */
		do {
			*p++ = *sstr++;
		} while (sstr[0] && !iswhite((uchar)sstr[0]));
		*p = 0;
		/* look it up in our big table of words */
		i = valueof_int(keywordtbl, upper(scratch), 0);
		if (!i) {
			/* unrecognized word */
			return CHAR_TOK;
		}
		if ((i = valueof_int(keywordtbl, upper(scratch), 0)) > 0 && i <= 999) {
			*pival = i % 100;
			/* TODO: we need to use the fact that calendar is i/100 */
			return MONTH_TOK;
		}
		*pival = 0;
		if (i >= 2001 && i < 2000 + GDV_CALENDARS_IX) {
			*pival = i - 2000;
			return CALENDAR_TOK;
		}
		if (i >= 1001 && i < 1000 + GD_END2) {
			*pival = i - 1000;
			return WORD_TOK;
		}
		FATAL(); /* something unexpected is in the keywordtbl ? Find out what! */
		return WORD_TOK;
	}
	if (chartype(*sstr) == DIGIT) {
		INT j=-99999, i=0;
		*pival = *sstr;
		while (chartype(c = (uchar)*p++ = *sstr++) == DIGIT)
			i = i*10 + c - '0';
		if (i > 9999) {
			/* 5+ digit numbers are not recognized */
			return CHAR_TOK;
		}
		if (c == '/') {
			STRING saves = sstr, savep = p;
			j=0;
			while (chartype(c = (uchar)*p++ = *sstr++) == DIGIT) 
				j = j*10 + c - '0';
			if (j != i+1 && j != i-1) {
				/* slash years only valid if differ by one year */
				sstr = saves;
				p = savep;
				j=-99999;
			}
			/* One drawback to this scheme is that 
			   we will parse 08/09/1995 as a slash year 8 */
		}
		*--p = 0;
		sstr--;
		if (*sstr && !is_date_delim(*sstr)) {
			/* number only valid if followed by date delimiter */
			return CHAR_TOK;
		}
		*pival = i;
		if (j != -99999) {
			*psval = scratch;
			return YEAR_TOK;
		}
		return ICONS_TOK;
	}
	if (*sstr == 0)  {
		sstr = NULL;
		return 0;
	}
	*pival = *sstr++;
	return CHAR_TOK;
}
/*=========================================
 * init_keywordtbl -- Set up table of known
 *  keywords which we recognize in parsing dates
 *=======================================*/
static void
init_keywordtbl (void)
{
	INT i, j;
	keywordtbl = create_table();
	/* Load GEDCOM keywords & values into keyword table */
	for (i=0; i<ARRSIZE(gedkeys); ++i) {
		j = gedkeys[i].value;
		insert_table_int(keywordtbl, gedkeys[i].keyword, j);
	}
	/* TODO: We need to load months of other calendars here */

}
/*=============================
 * get_todays_date -- Get today's date
 *===========================*/
STRING
get_todays_date (void)
{
	struct tm *pt;
	time_t curtime;
	static unsigned char dat[20];
	curtime = time(NULL);
	pt = localtime(&curtime);
	sprintf(dat, "%d %s %d", pt->tm_mday, months_gj[pt->tm_mon].su,
	    1900 + pt->tm_year);
	return dat;
}
/*=============================
 * gdateval_isdual -- Does gdateval contain
 * two dates ?
 * Created: 2001/12/28 (Perry Rapp)
 *===========================*/
BOOLEAN
gdateval_isdual (GDATEVAL gdv)
{
	if (gdv->type == GDV_PERIOD)
		return (gdv->subtype == GDVP_FROM_TO);
	else if (gdv->type == GDV_RANGE)
		return (gdv->subtype == GDVR_BET_AND);
	return FALSE;
}
/*=============================
 * is_valid_day -- Is this day legal for this date ?
 * Created: 2001/12/28 (Perry Rapp)
 *===========================*/
static BOOLEAN
is_valid_day (struct gdate_s * pdate, INT day)
{
	/* To consider: Fancy code with calendars */
	/* for now, just use Gregorian/Julian rules */
	return (day>=1 && day<=31);
}
/*=============================
 * is_valid_month -- Is this month legal for this date ?
 * Created: 2001/12/28 (Perry Rapp)
 *===========================*/
static BOOLEAN
is_valid_month (struct gdate_s * pdate, INT month)
{
	/* To consider: Fancy code with calendars */
	/* for now, just use Gregorian/Julian rules */
	return (month>=1 && month<=12);
}
/*=============================
 * is_date_delim -- Is this a valid character to end
 *  a number in a date ?
 * Created: 2001/12/28 (Perry Rapp)
 *===========================*/
static BOOLEAN
is_date_delim (char c)
{
	if (iswhite((uchar)c))
		return TRUE;
	/* TODO: Any other characters here ? Do we internationalize it ? */
	if (c=='/' || c=='-' || c=='.')
		return TRUE;
	return FALSE;
}
