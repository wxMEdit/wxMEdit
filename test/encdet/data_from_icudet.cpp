///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Cases from ICU
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "data_from_icudet.h"
#include <boost/assign/list_of.hpp>

std::vector<ICUEncDetTestCase> data_icudet_tests = boost::assign::list_of
(
ICUEncDetTestCase("IUC10-ar", "UTF-8 UTF-32BE UTF-32LE ISO-8859-6/ar windows-1256/ar", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u0623\\u0648\\u0631\\u0648\\u0628\\u0627, \\u0628\\u0631\\u0645\\u062c\\u064a\\u0627\\u062a \\u0627\\u0644\\u062d\\u0627\\u0633\\u0648\\u0628 + \\u0627\\u0646\\u062a\\u0631\\u0646\\u064a\\u062a :\n"
	"    \\u062a\\u0635\\u0628\\u062d \\u0639\\u0627\\u0644\\u0645\\u064a\\u0627 \\u0645\\u0639 \\u064a\\u0648\\u0646\\u064a\\u0643\\u0648\\u062f\n"
	"    IUC10\n"
	"    \\u062a\\u0633\\u062c\\u0651\\u0644 \\u0627\\u0644\\u0622\\u0646 \\u0644\\u062d\\u0636\\u0648\\u0631 \\u0627\\u0644\\u0645\\u0624\\u062a\\u0645\\u0631 \\u0627\\u0644\\u062f\\u0648\\u0644\\u064a \\u0627\\u0644\\u0639\\u0627\\u0634\\u0631 \\u0644\\u064a\\u0648\\u0646\\u064a\\u0643\\u0648\\u062f, \\u0627\\u0644\\u0630\\u064a \\u0633\\u064a\\u0639\\u0642\\u062f \\u0641\\u064a 10-12 \\u0622\\u0630\\u0627\\u0631 1997 \\u0628\\u0645\\u062f\\u064a\\u0646\\u0629 \\u0645\\u0627\\u064a\\u0646\\u062a\\u0633,\n"
	"    \\u0623\\u0644\\u0645\\u0627\\u0646\\u064a\\u0627. \\u0648\\u0633\\u064a\\u062c\\u0645\\u0639 \\u0627\\u0644\\u0645\\u0624\\u062a\\u0645\\u0631 \\u0628\\u064a\\u0646 \\u062e\\u0628\\u0631\\u0627\\u0621 \\u0645\\u0646 \\u0643\\u0627\\u0641\\u0629 \\u0642\\u0637\\u0627\\u0639\\u0627\\u062a \\u0627\\u0644\\u0635\\u0646\\u0627\\u0639\\u0629 \\u0639\\u0644\\u0649 \\u0627\\u0644\\u0634\\u0628\\u0643\\u0629 \\u0627\\u0644\\u0639\\u0627\\u0644\\u0645\\u064a\\u0629 \\u0627\\u0646\\u062a\\u0631\\u0646\\u064a\\u062a \\u0648\\u064a\\u0648\\u0646\\u064a\\u0643\\u0648\\u062f, \\u062d\\u064a\\u062b \\u0633\\u062a\\u062a\\u0645,\n"
	"    \\u0639\\u0644\\u0649 \\u0627\\u0644\\u0635\\u0639\\u064a\\u062f\\u064a\\u0646 \\u0627\\u0644\\u062f\\u0648\\u0644\\u064a \\u0648\\u0627\\u0644\\u0645\\u062d\\u0644\\u064a \\u0639\\u0644\\u0649 \\u062d\\u062f \\u0633\\u0648\\u0627\\u0621 \\u0645\\u0646\\u0627\\u0642\\u0634\\u0629 \\u0633\\u0628\\u0644 \\u0627\\u0633\\u062a\\u062e\\u062f\\u0627\\u0645 \\u064a\\u0648\\u0646\\u0643\\u0648\\u062f \\u0641\\u064a \\u0627\\u0644\\u0646\\u0638\\u0645 \\u0627\\u0644\\u0642\\u0627\\u0626\\u0645\\u0629 \\u0648\\u0641\\u064a\\u0645\\u0627 \\u064a\\u062e\\u0635 \\u0627\\u0644\\u062a\\u0637\\u0628\\u064a\\u0642\\u0627\\u062a\n"
	"    \\u0627\\u0644\\u062d\\u0627\\u0633\\u0648\\u0628\\u064a\\u0629, \\u0627\\u0644\\u062e\\u0637\\u0648\\u0637, \\u062a\\u0635\\u0645\\u064a\\u0645 \\u0627\\u0644\\u0646\\u0635\\u0648\\u0635 \\u0648\\u0627\\u0644\\u062d\\u0648\\u0633\\u0628\\u0629 \\u0645\\u062a\\u0639\\u062f\\u062f\\u0629 \\u0627\\u0644\\u0644\\u063a\\u0627\\u062a. \n"
	"\n"
	"    Unicode\n"
	"    \\u0639\\u0646\\u062f\\u0645\\u0627 \\u064a\\u0631\\u064a\\u062f \\u0627\\u0644\\u0639\\u0627\\u0644\\u0645 \\u0623\\u0646 \\u064a\\u062a\\u0643\\u0644\\u0651\\u0645, \\u0641\\u0647\\u0648 \\u064a\\u062a\\u062d\\u062f\\u0651\\u062b \\u0628\\u0644\\u063a\\u0629 \\u064a\\u0648\\u0646\\u064a\\u0643\\u0648\\u062f\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-da-Q", "UTF-8 UTF-32BE UTF-32LE windows-1252/da", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software + Internet:\n"
	"    Bliv global med Unicode\n"
	"    IUC10\n"
	"    Indskriv dig nu til den tiende internationale Unicode-konference, der holdes den 10-12\n"
	"    marts 1997 i Mainz, Tyskland. Konferencen samler eksperter fra hele verden inden for det\n"
	"    globale Internet og Unicode, internationalisering og lokalisering, implementering af\n"
	"    Unicode i styresystemer og programmer, skrifttyper, tekst-layout og flersproget databehandling.\n"
	"\n"
	"    Unicode\n"
	"    N\\u00e5r verden vil tale, taler den \\u201cUnicode.\\u201d\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-da", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/da", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software + Internet:\n"
	"    Bliv global med Unicode\n"
	"    IUC10\n"
	"    Indskriv dig nu til den tiende internationale Unicode-konference, der holdes den 10-12\n"
	"    marts 1997 i Mainz, Tyskland. Konferencen samler eksperter fra hele verden inden for det\n"
	"    globale Internet og Unicode, internationalisering og lokalisering, implementering af\n"
	"    Unicode i styresystemer og programmer, skrifttyper, tekst-layout og flersproget databehandling.\n"
	"\n"
	"    Unicode\n"
	"    N\\u00e5r verden vil tale, taler den Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-de", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/de", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software + das Internet:\n"
	"    International mit Unicode\n"
	"    IUC10\n"
	"    Melden Sie sich jetzt f\\u00fcr die 10. Internationale Unicode Konferenz an, die in der Zeit vom 10.-12. M\\u00e4rz 1997 in\n"
	"    Mainz stattfinden wird. Die Konferenz ist ein Treffpunkt f\\u00fcr Betriebsexperten aus den Bereichen globales\n"
	"    Internet und Unicode, Internationalisierung und Lokalisierung, die Implementierung von Unicode in\n"
	"    Betriebssystemen und Programmen, sowie f\\u00fcr Schriftarten, Textlayout und mehrsprachige Computeranwendungen.\n"
	"\n"
	"    Unicode\n"
	"    Wenn die Welt miteinander spricht, spricht sie Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-en", "UTF-32BE UTF-32LE ISO-8859-1/en", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europe, Software + the Internet:\n"
	"    Going Global with Unicode\n"
	"    IUC10\n"
	"    Register now for the Tenth International Unicode Conference, to be held on March 10-12, 1997,\n"
	"    in Mainz, Germany. The Conference will bring together industry-wide experts on the global Internet and\n"
	"    Unicode, internationalization and localization, implementation of Unicode in operating systems and applications,\n"
	"    fonts, text layout, and multilingual computing.\n"
	"\n"
	"    Unicode\n"
	"    When the world wants to talk, it speaks Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-es", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/es", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software + el Internet:\n"
	"    Mundializando con Unicode\n"
	"    IUC10\n"
	"    Inscr\\u00edbase ahora para la D\\u00e9cima Conferencia Internacional Unicode, que tendr\\u00e1 lugar del 10 al 12 de marzo de\n"
	"    1997 en Maguncia, Alemania. La Conferencia reunir\\u00e1 expertos de los sectores de la mundializaci\\u00f3n del Internet y\n"
	"    Unicode, la internacionalizaci\\u00f3n y localizaci\\u00f3n, implementaci\\u00f3n de Unicode en sistemas operativos y aplicaciones,\n"
	"    tipos, composici\\u00f3n de texto e inform\\u00e1tica multiling\\u00fce.\n"
	"\n"
	"    Unicode\n"
	"    Cuando el mundo quiere conversar, habla Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-fr", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/fr", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    L'Europe, le logiciel et l'Internet :\n"
	"    la mondialisation avec Unicode\n"
	"    IUC10\n"
	"    Inscrivez-vous d\\u00e8s maintenant \\u00e0 la dixi\\u00e8me Conf\\u00e9rence internationale sur Unicode, qui se tiendra du 10 au 12\n"
	"    mars 1997 \\u00e0 Mayence, en Allemagne. Cette conf\\u00e9rence rassemblera des experts de tous les horizons industriels\n"
	"    sur les sujets suivants : l'Internet mondial et Unicode, l'internationalisation et l'adaptation locale,\n"
	"    l'impl\\u00e9mentation d'Unicode dans les syst\\u00e8mes d'exploitation et les applications, les polices de caract\\u00e8res,\n"
	"    la disposition de texte, l'informatique plurilingue.\n"
	"\n"
	"    Unicode\n"
	"    Quand le monde veut communiquer, il parle en Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-he", "UTF-8 UTF-32BE UTF-32LE ISO-8859-8-I/he", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u05d0\\u05d9\\u05e8\\u05d5\\u05e4\\u05d4, \\u05ea\\u05d5\\u05db\\u05e0\\u05d4 \\u05d5\\u05d4\\u05d0\\u05d9\\u05e0\\u05d8\\u05e8\\u05e0\\u05d8:\n"
	"    Unicode \\u05d9\\u05d5\\u05e6\\u05d0 \\u05dc\\u05e9\\u05d5\\u05e7 \\u05d4\\u05e2\\u05d5\\u05dc\\u05de\\u05d9\n"
	"    IUC10\n"
	"    \\u05d4\\u05d9\\u05e8\\u05e9\\u05de\\u05d5 \\u05db\\u05e2\\u05ea \\u05dc\\u05db\\u05e0\\u05e1 Unicode \\u05d4\\u05d1\\u05d9\\u05e0\\u05dc\\u05d0\\u05d5\\u05de\\u05d9 \\u05d4\\u05e2\\u05e9\\u05d9\\u05e8\\u05d9, \\u05e9\\u05d9\\u05d9\\u05e2\\u05e8\\u05da \\u05d1\\u05d9\\u05df \\u05d4\\u05ea\\u05d0\\u05e8\\u05d9\\u05db\\u05d9\\u05dd 12-10 \\u05d1\\u05de\\u05e8\\u05e5 1997, \\u05d1\\u05de\\u05d9\\u05d9\\u05e0\\u05e5 \\u05e9\\u05d1\\u05d2\\u05e8\\u05de\\u05e0\\u05d9\\u05d4. \\u05d1\\u05db\\u05e0\\u05e1\n"
	"    \\u05d9\\u05e9\\u05ea\\u05ea\\u05e4\\u05d5 \\u05de\\u05d5\\u05de\\u05d7\\u05d9\\u05dd \\u05de\\u05db\\u05dc \\u05e2\\u05e0\\u05e4\\u05d9 \\u05d4\\u05ea\\u05e2\\u05e9\\u05d9\\u05d9\\u05d4 \\u05d1\\u05e0\\u05d5\\u05e9\\u05d0 \\u05d4\\u05d0\\u05d9\\u05e0\\u05d8\\u05e8\\u05e0\\u05d8 \\u05d4\\u05e2\\u05d5\\u05dc\\u05de\\u05d9 \\u05d5\\u05d4-Unicode, \\u05d1\\u05d4\\u05ea\\u05d0\\u05de\\u05d4 \\u05dc\\u05e9\\u05d5\\u05e7 \\u05d4\\u05d1\\u05d9\\u05e0\\u05dc\\u05d0\\u05d5\\u05de\\u05d9 \\u05d5\\u05d4\\u05de\\u05e7\\u05d5\\u05de\\u05d9, \\u05d1\\u05d9\\u05d9\\u05e9\\u05d5\\u05dd\n"
	"    Unicode \\u05d1\\u05de\\u05e2\\u05e8\\u05db\\u05d5\\u05ea \\u05d4\\u05e4\\u05e2\\u05dc\\u05d4 \\u05d5\\u05d1\\u05d9\\u05d9\\u05e9\\u05d5\\u05de\\u05d9\\u05dd, \\u05d1\\u05d2\\u05d5\\u05e4\\u05e0\\u05d9\\u05dd, \\u05d1\\u05e4\\u05e8\\u05d9\\u05e1\\u05ea \\u05d8\\u05e7\\u05e1\\u05d8 \\u05d5\\u05d1\\u05de\\u05d7\\u05e9\\u05d5\\u05d1 \\u05e8\\u05d1-\\u05dc\\u05e9\\u05d5\\u05e0\\u05d9.\n"
	"\n"
	"    Unicode\n"
	"    \\u05db\\u05d0\\u05e9\\u05e8 \\u05d4\\u05e2\\u05d5\\u05dc\\u05dd \\u05e8\\u05d5\\u05e6\\u05d4 \\u05dc\\u05d3\\u05d1\\u05e8, \\u05d4\\u05d5\\u05d0 \\u05de\\u05d3\\u05d1\\u05e8 \\u05d1-Unicode\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-he-Q", "UTF-8 UTF-32BE UTF-32LE windows-1255/he", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u05d0\\u05d9\\u05e8\\u05d5\\u05e4\\u05d4, \\u05ea\\u05d5\\u05db\\u05e0\\u05d4 \\u05d5\\u05d4\\u05d0\\u05d9\\u05e0\\u05d8\\u05e8\\u05e0\\u05d8:\n"
	"    Unicode \\u05d9\\u05d5\\u05e6\\u05d0 \\u05dc\\u05e9\\u05d5\\u05e7 \\u05d4\\u05e2\\u05d5\\u05dc\\u05de\\u05d9\n"
	"    IUC10\n"
	"    \\u05d4\\u05d9\\u05e8\\u05e9\\u05de\\u05d5 \\u05db\\u05e2\\u05ea \\u05dc\\u05db\\u05e0\\u05e1 Unicode \\u05d4\\u05d1\\u05d9\\u05e0\\u05dc\\u05d0\\u05d5\\u05de\\u05d9 \\u05d4\\u05e2\\u05e9\\u05d9\\u05e8\\u05d9, \\u05e9\\u05d9\\u05d9\\u05e2\\u05e8\\u05da \\u05d1\\u05d9\\u05df \\u05d4\\u05ea\\u05d0\\u05e8\\u05d9\\u05db\\u05d9\\u05dd 12-10 \\u05d1\\u05de\\u05e8\\u05e5 1997, \\u05d1\\u05de\\u05d9\\u05d9\\u05e0\\u05e5 \\u05e9\\u05d1\\u05d2\\u05e8\\u05de\\u05e0\\u05d9\\u05d4. \\u05d1\\u05db\\u05e0\\u05e1\n"
	"    \\u05d9\\u05e9\\u05ea\\u05ea\\u05e4\\u05d5 \\u05de\\u05d5\\u05de\\u05d7\\u05d9\\u05dd \\u05de\\u05db\\u05dc \\u05e2\\u05e0\\u05e4\\u05d9 \\u05d4\\u05ea\\u05e2\\u05e9\\u05d9\\u05d9\\u05d4 \\u05d1\\u05e0\\u05d5\\u05e9\\u05d0 \\u05d4\\u05d0\\u05d9\\u05e0\\u05d8\\u05e8\\u05e0\\u05d8 \\u05d4\\u05e2\\u05d5\\u05dc\\u05de\\u05d9 \\u05d5\\u05d4-Unicode, \\u05d1\\u05d4\\u05ea\\u05d0\\u05de\\u05d4 \\u05dc\\u05e9\\u05d5\\u05e7 \\u05d4\\u05d1\\u05d9\\u05e0\\u05dc\\u05d0\\u05d5\\u05de\\u05d9 \\u05d5\\u05d4\\u05de\\u05e7\\u05d5\\u05de\\u05d9, \\u05d1\\u05d9\\u05d9\\u05e9\\u05d5\\u05dd\n"
	"    Unicode \\u05d1\\u05de\\u05e2\\u05e8\\u05db\\u05d5\\u05ea \\u05d4\\u05e4\\u05e2\\u05dc\\u05d4 \\u05d5\\u05d1\\u05d9\\u05d9\\u05e9\\u05d5\\u05de\\u05d9\\u05dd, \\u05d1\\u05d2\\u05d5\\u05e4\\u05e0\\u05d9\\u05dd, \\u05d1\\u05e4\\u05e8\\u05d9\\u05e1\\u05ea \\u05d8\\u05e7\\u05e1\\u05d8 \\u05d5\\u05d1\\u05de\\u05d7\\u05e9\\u05d5\\u05d1 \\u05e8\\u05d1-\\u05dc\\u05e9\\u05d5\\u05e0\\u05d9.\n"
	"\n"
	"    Unicode\n"
	"    \\u05db\\u05d0\\u05e9\\u05e8 \\u05d4\\u05e2\\u05d5\\u05dc\\u05dd \\u05e8\\u05d5\\u05e6\\u05d4 \\u05dc\\u05d3\\u05d1\\u05e8, \\u05d4\\u05d5\\u05d0 \\u05de\\u05d3\\u05d1\\u05e8 -\\u201cUnicode.\\u201d\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-hu", "UTF-8 UTF-32BE UTF-32LE ISO-8859-2/hu", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Eur\\u00f3pa, a Szoftver s az Internet -\n"
	"    Glob\\u00e1liss\\u00e1 V\\u00e1ltozik a Unicode \\u00c1ltal\n"
	"    IUC10\n"
	"    Iratkozzon m\\u00e1r most a Tizedik Nemzetk\\u00f6zi Unicode Konferenci\\u00e1ra, amely M\\u00e1rcius 10-12 1997\n"
	"    lesz megtartva, Meinz-be, N\\u00e9metorsz\\u00e1gba. Ebben a Konferenci\\u00e1n az ipar\\u00e1g szerte sok szak\\u00e9rt\\u0151\n"
	"    fog r\\u00e9szt venni: a glob\\u00e1lis Internet s Unicode nemzetk\\u00f6zis\\u00edt\\u00e9se s lokaliz\\u00e1l\\u00e1sa, a\n"
	"    Unicode beteljes\\u00edt\\u00e9se a m\\u0171k\\u00f6d\\u0151 rendszerekben s alkalmaz\\u00e1sokban, fontokba, sz\\u00f6veg\n"
	"    t\\u00e9rbeoszt\\u00e1sba s t\\u00f6bbnyelv\\u0171 computerekben.\n"
	"\n"
	"    Unicode\n"
	"    Ha a vil\\u00e1g besz\\u00e9lni akar, azt Unicode-ul mondja.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-hu-Q", "UTF-8 UTF-32BE UTF-32LE windows-1250/hu", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Eur\\u00f3pa, a Szoftver s az Internet -\n"
	"    Glob\\u00e1liss\\u00e1 V\\u00e1ltozik a Unicode \\u00c1ltal\n"
	"    IUC10\n"
	"    Iratkozzon m\\u00e1r most a Tizedik Nemzetk\\u00f6zi Unicode Konferenci\\u00e1ra, amely M\\u00e1rcius 10-12 1997\n"
	"    lesz megtartva, Meinz-be, N\\u00e9metorsz\\u00e1gba. Ebben a Konferenci\\u00e1n az ipar\\u00e1g szerte sok szak\\u00e9rt\\u0151\n"
	"    fog r\\u00e9szt venni: a glob\\u00e1lis Internet s Unicode nemzetk\\u00f6zis\\u00edt\\u00e9se s lokaliz\\u00e1l\\u00e1sa, a\n"
	"    Unicode beteljes\\u00edt\\u00e9se a m\\u0171k\\u00f6d\\u0151 rendszerekben s alkalmaz\\u00e1sokban, fontokba, sz\\u00f6veg\n"
	"    t\\u00e9rbeoszt\\u00e1sba s t\\u00f6bbnyelv\\u0171 computerekben.\n"
	"\n"
	"    Unicode\n"
	"    Ha a vil\\u00e1g besz\\u00e9lni akar, azt \\u201cUnicode\\u201d-ul mondja.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-it", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/it", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, software e Internet:\n"
	"    Globalizzazione con Unicode\n"
	"    IUC10\n"
	"    Iscrivetevi subito alla X Conferenza Internazionale su Unicode, che si terr\\u00e0 dal 10 al 12 marzo 1997 a\n"
	"    Mainz in Germania. Alla Conferenza parteciperanno esperti di tutti i settori per discutere di Internet globale e\n"
	"    Unicode, internazionalizzazione e localizzazione, implementazione di Unicode in sistemi operativi e applicazioni,\n"
	"    caratteri, composizione dei testi ed elaborazione multilingue.\n"
	"\n"
	"    Unicode\n"
	"    Quando il mondo vuole comunicare, parla Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-jp", "UTF-8 UTF-32BE UTF-32LE Shift_JIS/ja ISO-2022-JP", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u30e8\\u30fc\\u30ed\\u30c3\\u30d1\\u3001\\u30bd\\u30d5\\u30c8\\u30a6\\u30a7\\u30a2\\u3001\\u305d\\u3057\\u3066\\u30a4\\u30f3\\u30bf\\u30fc\\u30cd\\u30c3\\u30c8:\n"
	"    Unicode \\u3067\\u30b0\\u30ed\\u30fc\\u30d0\\u30eb\\u5316\\u3092\\u72d9\\u3063\\u3066\n"
	"    IUC10\n"
	"    \\u7b2c 10 \\u56de\\u306e Unicode \\u4f1a\\u8b70\\u306f 1997 \\u5e74 3 \\u6708 10-12\\u65e5\\u3001\\u30c9\\u30a4\\u30c4\\u306e\\u30de\\u30a4\\u30f3\\u30c4\\u3067\\u958b\\u304b\\u308c\\u307e\\u3059\\u3002\\u53c2\\u52a0\\u5e0c\\u671b\\u306e\\u65b9\\u306f\\u4eca\\u3059\\u3050\\u767b\\u9332\\u3057\\u3066\\u304f\\u3060\\u3055\\u3044\\u3002\n"
	"    \\u3053\\u306e\\u4f1a\\u8b70\\u3067\\u306f\\u3001\\u30b0\\u30ed\\u30fc\\u30d0\\u30eb\\u306a\\u30a4\\u30f3\\u30bf\\u30cd\\u30c3\\u30c8\\u3001Unicode\\u3001\\u30bd\\u30d5\\u30c8\\u30a6\\u30a7\\u30a2\\u306e\\u56fd\\u969b\\u5316\\u304a\\u3088\\u3073\\u30ed\\u30fc\\u30ab\\u30ea\\u30bc\\u30fc\\u30b7\\u30e7\\u30f3\\u3001OS \\u304a\\u3088\\u3073\\u30a2\\u30d7\\u30ea\\u30b1\\u30fc\\u30b7\\u30e7\\u30f3\\u3067\\u306e\n"
	"    Unicode \\u306e\\u30a4\\u30f3\\u30d7\\u30ea\\u30e1\\u30f3\\u30c6\\u30fc\\u30b7\\u30e7\\u30f3\\u3001\\u30d5\\u30a9\\u30f3\\u30c8\\u3001\\u30c6\\u30ad\\u30b9\\u30c8\\u8868\\u793a\\u3001\\u30de\\u30eb\\u30c1\\u8a00\\u8a9e\\u30b3\\u30f3\\u30d4\\u30e5\\u30fc\\u30c6\\u30a3\\u30f3\\u30b0\\u306b\\u304a\\u3051\\u308b\\u696d\\u754c\\u306e\\u5c02\\u9580\\u5bb6\\u304c\\u96c6\\u307e\\u308a\\u307e\\u3059\\u3002\n"
	"\n"
	"    Unicode\n"
	"    \\u4e16\\u754c\\u7684\\u306b\\u8a71\\u3059\\u306a\\u3089\\u3001Unicode \\u3067\\u3059\\u3002\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-ko", "UTF-8 UTF-32BE UTF-32LE EUC-KR/ko ISO-2022-KR", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\uc720\\ub7fd, \\uc18c\\ud504\\ud2b8\\uc6e8\\uc5b4 \\uadf8\\ub9ac\\uace0 \\uc778\\ud130\\ub137:\n"
	"    \\uc720\\ub2c8\\ucf54\\ub4dc\\uc640 \\ud568\\uaed8 \\uc138\\uacc4\\ub85c\n"
	"    IUC10\n"
	"    \\uc81c10\\ud68c \\uc720\\ub2c8\\ucf54\\ub4dc \\uad6d\\uc81c \\ud68c\\uc758\\uac00 1997\\ub144 3\\uc6d4 10\\uc77c\\ubd80\\ud130 12\\uc77c\\uae4c\\uc9c0 \\ub3c5\\uc77c\\uc758 \\ub9c8\\uc778\\uc988\\uc5d0\\uc11c \\uc5f4\\ub9bd\\ub2c8\\ub2e4. \\uc9c0\\uae08 \\ub4f1\\ub85d\\ud558\\uc2ed\\uc2dc\\uc624.\n"
	"    \\uc774 \\ud68c\\uc758\\uc5d0\\uc11c\\ub294 \\uc5c5\\uacc4 \\uc804\\ubc18\\uc758 \\uc804\\ubb38\\uac00\\ub4e4\\uc774 \\ud568\\uaed8 \\ubaa8\\uc5ec \\ub2e4\\uc74c\\uacfc \\uac19\\uc740 \\ubd84\\uc57c\\ub97c \\ub2e4\\ub8f9\\ub2c8\\ub2e4. - \\uc778\\ud130\\ub137\\uacfc \\uc720\\ub2c8\\ucf54\\ub4dc, \\uad6d\\uc81c\\ud654\\uc640 \\uc9c0\\uc5ed\\ud654,\n"
	"    \\uc6b4\\uc601 \\uccb4\\uc81c\\uc640 \\uc751\\uc6a9 \\ud504\\ub85c\\uadf8\\ub7a8\\uc5d0\\uc11c \\uc720\\ub2c8\\ucf54\\ub4dc\\uc758 \\uad6c\\ud604, \\uae00\\uaf34, \\ubb38\\uc790 \\ubc30\\uc5f4, \\ub2e4\\uad6d\\uc5b4 \\ucef4\\ud4e8\\ud305.\n"
	"\n"
	"    Unicode\n"
	"    \\uc138\\uacc4\\ub97c \\ud5a5\\ud55c \\ub300\\ud654, \\uc720\\ub2c8\\ucf54\\ub4dc\\ub85c \\ud558\\uc2ed\\uc2dc\\uc624\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-nl", "UTF-32BE UTF-32LE ISO-8859-1/nl", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software + het Internet:\n"
	"    wereldwijd met Unicode\n"
	"    IUC10\n"
	"    Meld u nu aan voor de Tiende Internationale Unicode-conferentie, die van 10 tot 12 maart 1997 in\n"
	"    Mainz (Duitsland) wordt gehouden. De Conferentie is een ontmoetingsplaats voor experts uit de industrie op het\n"
	"    gebied van het wereldwijde Internet en Unicode, internationalisatie en localisatie, implementatie van Unicode in\n"
	"    besturingssystemen en applicaties, lettertypes, tekstopmaak en meertalig computergebruik.\n"
	"\n"
	"    Unicode\n"
	"    Als de wereld wil praten, spreekt hij Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-no-NO", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/da", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Programvare og Internet:\n"
	"    Global fokus med Unicode\n"
	"    IUC10\n"
	"    Registrer deg som deltager p\\u00e5 den tiende inernasjonale Unicode konferansen i Mainz, Tyskland, fra 10. til 12. mars,\n"
	"    1997. Konferansen vil samle eksperter p\\u00e5 Internet, Unicode, internasjonalisering og integrasjon av Unicode i\n"
	"    operativsystemer og programmer, fonter, tekst layout og flerspr\\u00e5klig databehandling.\n"
	"\n"
	"    Unicode\n"
	"    N\\u00e5r verden vil snakke, snakker den Unicode\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-no-NO-NY", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/no", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, programvare og Internett:\n"
	"    Femn verda med Unicode\n"
	"    IUC10\n"
	"    Meld deg p\\u00e5 den 10. internasjonale Unicode-konferansen. Han g\\u00e5r f\\u00f8re seg i Mainz i Tyskland i dagane 10.--12. mars\n"
	"    1997, og samlar fagkunnige innan konferansetemaet fr\\u00e5 heile databransjen. Tema: Det globale Internettet og\n"
	"    Unicode, internasjonalisering og nasjonal tilpassing, implementering av Unicode i operativsystem og brukarprogram,\n"
	"    skriftsnitt (fontar), tekstutlegg, og fleirspr\\u00e5kleg databehandling.\n"
	"\n"
	"    Unicode\n"
	"    N\\u00e5r verda \\u00f8nskjer \\u00e5 snakke, talar ho Unicode\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-pt-BR", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/pt", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software e a Internet:\n"
	"    Globaliza\\u00e7\\u00e3o com o Unicode\n"
	"    IUC10\n"
	"    Inscreva-se agora para a D\\u00e9cima Confer\\u00eancia Internacional Sobre O Unicode, realizada entre os dias 10 e 12 de\n"
	"    mar\\u00e7o de 1997 em Mainz na Alemanha. A Confer\\u00eancia reunir\\u00e1 peritos de todas as \\u00e1reas da ind\\u00fastria especializados\n"
	"    em assuntos relacionados com a Internet global e o Unicode, internacionaliza\\u00e7\\u00e3o e localiza\\u00e7\\u00e3o de software,\n"
	"    implementa\\u00e7\\u00e3o do Unicode em sistemas operacionais e aplicativos, fontes, layout de texto e inform\\u00e1tica multil\\u00edng\\u00fce.\n"
	"\n"
	"    Unicode\n"
	"    Quando o mundo quer falar, fala Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-pt-PT", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/pt", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software e a Internet:\n"
	"    Globaliza\\u00e7\\u00e3o com o Unicode\n"
	"    IUC10\n"
	"    Inscreva-se agora para a D\\u00e9cima Confer\\u00eancia Internacional Sobre O Unicode, a ser realizada entre os dias 10 e 12\n"
	"    de Mar\\u00e7o de 1997 em Mainz na Alemanha. A Confer\\u00eancia reunir\\u00e1 peritos de todas as \\u00e1reas da ind\\u00fastria\n"
	"    especializados em assuntos relacionados com a Internet global e o Unicode, internacionaliza\\u00e7\\u00e3o e localiza\\u00e7\\u00e3o de\n"
	"    software, implementa\\u00e7\\u00e3o do Unicode em sistemas operativos e aplica\\u00e7\\u00f5es, tipos de letra, esquematiza\\u00e7\\u00e3o de\n"
	"    texto e inform\\u00e1tica multil\\u00edngue.\n"
	"\n"
	"    Unicode\n"
	"    Quando o mundo quer falar, fala Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-ro", "UTF-8 UTF-32BE UTF-32LE ISO-8859-2/ro", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, Software \\u015fi Internet:\n"
	"    Globalizarea cu Unicode\n"
	"    IUC10\n"
	"    Inscrie\\u0163i-v\\u0103 acum la a Zecea Conferin\\u0163\\u0103 Interna\\u0163ional\\u0103 \"Unicode\" ce va avea loc in\n"
	"    perioada de 10-12 martie, 1997 \\u00een Mainz, Germania. Conferin\\u0163a va \\u00eentruni exper\\u0163i din\n"
	"    variate domenii: Internet global \\u015fi Unicode, interna\\u0163ionalizare \\u015fi localizare,\n"
	"    implementarede Unicode \\u00een sisteme de operare \\u015fi aplica\\u0163ii, fonturi, aranjare de text \\u00een\n"
	"    pagin\\u0103, computerizare multilingual\\u0103.\n"
	"\n"
	"    Unicode\n"
	"    C\\u00e2nd lumea vrea s\\u0103 comunice, vorbe\\u015fte Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-ru", "UTF-8 UTF-32BE UTF-32LE ISO-8859-5/ru windows-1251/ru KOI8-R/ru", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u0415\\u0432\\u0440\\u043e\\u043f\\u0430, \\u041f\\u0440\\u043e\\u0433\\u0440\\u0430\\u043c\\u043c\\u043d\\u043e\\u0435 \\u043e\\u0431\\u0435\\u0441\\u043f\\u0435\\u0447\\u0435\\u043d\\u0438\\u0435 + \\u0418\\u043d\\u0442\\u0435\\u0440\\u043d\\u0435\\u0442:\n"
	"    Unicode \\u0432 \\u0433\\u043b\\u043e\\u0431\\u0430\\u043b\\u044c\\u043d\\u044b\\u0445 \\u043c\\u0430\\u0441\\u0448\\u0442\\u0430\\u0431\\u0430\\u0445\n"
	"    IUC10\n"
	"    \\u0417\\u0430\\u0440\\u0435\\u0433\\u0438\\u0441\\u0442\\u0440\\u0438\\u0440\\u0443\\u0439\\u0442\\u0435\\u0441\\u044c \\u0441\\u0435\\u0439\\u0447\\u0430\\u0441 \\u043d\\u0430 \\u0414\\u0435\\u0441\\u044f\\u0442\\u0443\\u044e \\u041c\\u0435\\u0436\\u0434\\u0443\\u043d\\u0430\\u0440\\u043e\\u0434\\u043d\\u0443\\u044e \\u041a\\u043e\\u043d\\u0444\\u0435\\u0440\\u0435\\u043d\\u0446\\u0438\\u044e \\u043f\\u043e Unicode, \\u043a\\u043e\\u0442\\u043e\\u0440\\u0430\\u044f \\u0441\\u043e\\u0441\\u0442\\u043e\\u0438\\u0442\\u0441\\u044f\n"
	"    10-12 \\u043c\\u0430\\u0440\\u0442\\u0430 1997 \\u0433\\u043e\\u0434\\u0430 \\u0432 \\u041c\\u0430\\u0439\\u043d\\u0446\\u0435 \\u0432 \\u0413\\u0435\\u0440\\u043c\\u0430\\u043d\\u0438\\u0438. \\u041a\\u043e\\u043d\\u0444\\u0435\\u0440\\u0435\\u043d\\u0446\\u0438\\u044f \\u0441\\u043e\\u0431\\u0435\\u0440\\u0435\\u0442 \\u0448\\u0438\\u0440\\u043e\\u043a\\u0438\\u0439 \\u043a\\u0440\\u0443\\u0433 \\u044d\\u043a\\u0441\\u043f\\u0435\\u0440\\u0442\\u043e\\u0432 \\u043f\\u043e \\u0438\n"
	"    \\u0432\\u043e\\u043f\\u0440\\u043e\\u0441\\u0430\\u043c \\u0433\\u043b\\u043e\\u0431\\u0430\\u043b\\u044c\\u043d\\u043e\\u0433\\u043e \\u0418\\u043d\\u0442\\u0435\\u0440\\u043d\\u0435\\u0442\\u0430 \\u0438 Unicode, \\u043b\\u043e\\u043a\\u0430\\u043b\\u0438\\u0437\\u0430\\u0446\\u0438\\u0438 \\u0438 \\u0438\\u043d\\u0442\\u0435\\u0440\\u043d\\u0430\\u0446\\u0438\\u043e\\u043d\\u0430\\u043b\\u0438\\u0437\\u0430\\u0446\\u0438\\u0438, \\u0432\\u043e\\u043f\\u043b\\u043e\\u0449\\u0435\\u043d\\u0438\\u044e\n"
	"    \\u043f\\u0440\\u0438\\u043c\\u0435\\u043d\\u0435\\u043d\\u0438\\u044e Unicode \\u0432 \\u0440\\u0430\\u0437\\u043b\\u0438\\u0447\\u043d\\u044b\\u0445 \\u043e\\u043f\\u0435\\u0440\\u0430\\u0446\\u0438\\u043e\\u043d\\u043d\\u044b\\u0445 \\u0441\\u0438\\u0441\\u0442\\u0435\\u043c\\u0430\\u0445 \\u0438 \\u043f\\u0440\\u043e\\u0433\\u0440\\u0430\\u043c\\u043c\\u043d\\u044b\\u0445 \\u043f\\u0440\\u0438\\u043b\\u043e\\u0436\\u0435\\u043d\\u0438\\u044f\\u0445,\n"
	"    \\u0448\\u0440\\u0438\\u0444\\u0442\\u0430\\u0445, \\u0432\\u0435\\u0440\\u0441\\u0442\\u043a\\u0435 \\u0438 \\u043c\\u043d\\u043e\\u0433\\u043e\\u044f\\u0437\\u044b\\u0447\\u043d\\u044b\\u0445 \\u043a\\u043e\\u043c\\u043f\\u044c\\u044e\\u0442\\u0435\\u0440\\u043d\\u044b\\u0445 \\u0441\\u0438\\u0441\\u0442\\u0435\\u043c\\u0430\\u0445.\n"
	"\n"
	"    Unicode\n"
	"    \\u041a\\u043e\\u0433\\u0434\\u0430 \\u043c\\u0438\\u0440 \\u0436\\u0435\\u043b\\u0430\\u0435\\u0442 \\u043e\\u0431\\u0449\\u0430\\u0442\\u044c\\u0441\\u044f, \\u043e\\u043d \\u043e\\u0431\\u0449\\u0430\\u0435\\u0442\\u0441\\u044f \\u043d\\u0430 Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-sv", "UTF-8 UTF-32BE UTF-32LE ISO-8859-1/sv", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Europa, programvara och Internet:\n"
	"    globalisera med Unicode\n"
	"    IUC10\n"
	"    Anm\\u00e4l Dig till den tionde internationella Unicode-konferensen, som h\\u00e5lls den 10-12 mars 1997 i Mainz,\n"
	"    Tyskland. Vid konferensen kommer experter inom f\\u00f6ljande omr\\u00e5den att delta: det globala Internet och Unicode,\n"
	"    internationalisering och lokalisering, implementering av Unicode i operativsystem, till\\u00e4mpningar, typsnitt,\n"
	"    textlayout och m\\u00e5ngspr\\u00e5klig datoranv\\u00e4ndning.\n"
	"\n"
	"    Unicode\n"
	"    N\\u00e4r v\\u00e4rlden vill tala, s\\u00e5 talar den Unicode.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-yi", "UTF-8 UTF-32BE UTF-32LE", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u05d0\\u05f2\\u05e8\\u05d0\\u05b8\\u05e4\\u05bc\\u05e2: \\u05e4\\u05bc\\u05e8\\u05d0\\u05b8\\u05d2\\u05e8\\u05d0\\u05b7\\u05de\\u05f0\\u05d0\\u05b7\\u05e8\\u05d2 \\u05d0\\u05d5\\u05df \\u05d3\\u05d9 \\u05f0\\u05e2\\u05dc\\u05d8\\u05e0\\u05e2\\u05e5:\n"
	"    \\u05d0\\u05d5\\u05e0\\u05d9\\u05e7\\u05d0\\u05b8\\u05d3 \\u05d0\\u05d9\\u05d1\\u05e2\\u05e8 \\u05d3\\u05e2\\u05e8 \\u05d2\\u05d0\\u05b8\\u05e8\\u05e2\\u05e8 \\u05f0\\u05e2\\u05dc\\u05d8\n"
	"    IUC10\n"
	"    \\u05e4\\u05bf\\u05d0\\u05b7\\u05e8\\u05e9\\u05e8\\u05f2\\u05b7\\u05d1\\u05d8 \\u05d6\\u05d9\\u05da \\u05e9\\u05f1\\u05df \\u05d0\\u05f1\\u05e3 \\u05d3\\u05e2\\u05e8 \\u05e6\\u05e2\\u05e0\\u05d8\\u05e2\\u05e8 \\u05d0\\u05d9\\u05e0\\u05d8\\u05e2\\u05e8\\u05e0\\u05d0\\u05b7\\u05e6\\u05d9\\u05d0\\u05b8\\u05e0\\u05d0\\u05b7\\u05dc\\u05e2\\u05e8 \\u05d0\\u05d5\\u05e0\\u05d9\\u05e7\\u05d0\\u05b8\\u05d3-\\u05e7\\u05d0\\u05b8\\u05e0\\u05e4\\u05bf\\u05e2\\u05e8\\u05e2\\u05e0\\u05e5, \\u05f0\\u05d0\\u05b8\\u05e1 \\u05f0\\u05e2\\u05d8 \\u05e4\\u05bf\\u05d0\\u05b8\\u05e8\\u05e7\\u05d5\\u05de\\u05e2\\u05df \\u05d3\\u05e2\\u05dd\n"
	"    10\\u05d8\\u05df \\u05d1\\u05d9\\u05d6\\u05df 12\\u05d8\\u05df \\u05de\\u05d0\\u05b7\\u05e8\\u05e5, 1997, \\u05d0\\u05d9\\u05df \\u05de\\u05f2\\u05b7\\u05e0\\u05e5, \\u05d3\\u05f2\\u05b7\\u05d8\\u05e9\\u05dc\\u05d0\\u05b7\\u05e0\\u05d3. \\u05d3\\u05d9 \\u05e7\\u05d0\\u05b8\\u05e0\\u05e4\\u05bf\\u05e2\\u05e8\\u05e2\\u05e0\\u05e5 \\u05f0\\u05e2\\u05d8 \\u05e6\\u05d5\\u05d6\\u05d0\\u05b7\\u05de\\u05e2\\u05e0\\u05d1\\u05e8\\u05e2\\u05e0\\u05d2\\u05df \\u05de\\u05d1\\u05bf\\u05d9\\u05e0\\u05d9\\u05dd \\u05e4\\u05bf\\u05d5\\u05df \\u05f0\\u05e2\\u05dc\\u05d8\\u05e0\\u05e2\\u05e5,\n"
	"     \\u05d0\\u05d5\\u05e0\\u05d9\\u05e7\\u05d0\\u05b8\\u05d3, \\u05d0\\u05d9 \\u05d0\\u05b7\\u05dc\\u05f0\\u05e2\\u05dc\\u05d8\\u05dc\\u05e2\\u05db\\u05df \\u05d0\\u05d9 \\u05e1\\u05d1\\u05bf\\u05d9\\u05d1\\u05bf\\u05d4\\u05d3\\u05d9\\u05e7\\u05df \\u05e4\\u05bc\\u05e8\\u05d0\\u05b8\\u05d2\\u05e8\\u05d0\\u05b7\\u05de\\u05f0\\u05d0\\u05b7\\u05e8\\u05d2, \\u05d0\\u05b7\\u05e8\\u05f2\\u05b7\\u05e0\\u05e9\\u05d8\\u05e2\\u05dc\\u05df \\u05d0\\u05d5\\u05e0\\u05d9\\u05e7\\u05d0\\u05b8\\u05d3 \\u05d0\\u05d9\\u05df \\u05d0\\u05b8\\u05e4\\u05bc\\u05e2\\u05e8\\u05d9\\u05e8-\\u05e1\\u05d9\\u05e1\\u05d8\\u05e2\\u05de\\u05e2\\u05df \\u05d0\\u05d5\\u05df\n"
	"    \\u05d0\\u05b8\\u05e0\\u05f0\\u05e2\\u05e0\\u05d3\\u05d5\\u05e0\\u05d2\\u05e2\\u05df, \\u05e9\\u05e8\\u05d9\\u05e4\\u05bf\\u05d8\\u05df, \\u05d8\\u05e2\\u05e7\\u05e1\\u05d8-\\u05d0\\u05f1\\u05e1\\u05e9\\u05d8\\u05e2\\u05dc, \\u05d0\\u05d5\\u05df \\u05de\\u05e2\\u05e8\\u05e9\\u05e4\\u05bc\\u05e8\\u05d0\\u05b7\\u05db\\u05d9\\u05e7\\u05e2 \\u05e7\\u05d0\\u05b8\\u05de\\u05e4\\u05bc\\u05d9\\u05d5\\u05d8\\u05e2\\u05e8\\u05f2\\u05b7.\n"
	"\n"
	"    Unicode\n"
	"    \\u05d0\\u05b7\\u05d6 \\u05d3\\u05d9 \\u05f0\\u05e2\\u05dc\\u05d8 \\u05f0\\u05d9\\u05dc \\u05e8\\u05e2\\u05d3\\u05df, \\u05e8\\u05e2\\u05d3\\u05d8 \\u05d6\\u05d9 \\u05d0\\u05d5\\u05e0\\u05d9\\u05e7\\u05d0\\u05b8\\u05d3\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-zh-Hant", "UTF-8 UTF-32BE UTF-32LE Big5/zh", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u6b50\\u6d32\\uff0c\\u8edf\\u9ad4\\u53ca\\u7db2\\u969b\\u7db2\\u8def\\uff1a\n"
	"    \\u8b93\\u7d71\\u4e00\\u78bc\\uff08Unicode\\uff09\\u9818\\u4f60\\u9032\\u5165\\u5168\\u4e16\\u754c\n"
	"    IUC10\n"
	"    \\u4f60\\u73fe\\u5728\\u5c31\\u61c9\\u5831\\u540d\\u5c07\\u5728\\uff11\\uff19\\uff19\\uff17\\u5e74\\uff13\\u6708\\uff11\\uff10\\u81f3\\uff11\\uff12\\u65e5\\u65bc\\u5fb7\\u570b\\u7f8e\\u59ff\\u57ce\\uff08Mainz\\uff09\\u53ec\\u958b\\u7684\\u7b2c\\u5341\\u5c46\\u570b\\u969b\\u7d71\\u4e00\\u78bc\\u7814\\u8a0e\\u6703\\u3002\n"
	"    \\u672c\\u6b21\\u7814\\u8a0e\\u6703\\u5c07\\u9080\\u8acb\\u591a\\u4f4d\\u696d\\u754c\\u5c08\\u5bb6\\u7814\\u8a0e\\u95dc\\u65bc\\u5168\\u7403\\u7db2\\u969b\\u7db2\\u8def\\u53ca\\u7d71\\u4e00\\u78bc\\u767c\\u5c55\\u3001\\u570b\\u969b\\u5316\\u53ca\\u672c\\u571f\\u5316\\u3001\n"
	"    \\u652f\\u63f4\\u7d71\\u4e00\\u78bc\\u7684\\u4f5c\\u696d\\u7cfb\\u7d71\\u53ca\\u61c9\\u7528\\u7a0b\\u5f0f\\u3001\\u5b57\\u578b\\u3001\\u6587\\u5b57\\u6392\\u7248\\u3001\\u96fb\\u8166\\u591a\\u570b\\u8a9e\\u6587\\u5316\\u7b49\\u591a\\u9805\\u8ab2\\u984c\\u3002\n"
	"\n"
	"    Unicode\n"
	"    \\u7576\\u4e16\\u754c\\u9700\\u8981\\u6e9d\\u901a\\u6642\\uff0c\\u8acb\\u7528\\u7d71\\u4e00\\u78bc\\uff08Unicode\\uff09\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("IUC10-zh-Hans", "UTF-8 UTF-32BE UTF-32LE ISO-2022-CN GB18030/zh", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u6b27\\u6d32\\uff0c\\u8f6f\\u4ef6\\uff0b\\u4e92\\u8054\\u7f51\n"
	"    \\u7528\\u7edf\\u4e00\\u7801 (Unicode) \\u8d70\\u904d\\u4e16\\u754c\n"
	"    IUC10\n"
	"    \\u5c06\\u4e8e1997\\u5e74 3 \\u670810\\u65e5\\uff0d12\\u65e5\\u5728\\u5fb7\\u56fd Mainz \\u5e02\\u4e3e\\u884c\\u7684\\u7b2c\\u5341\\u5c4a\\u7edf\\u4e00\\u7801\\u56fd\\u9645\\u7814\\u8ba8\\u4f1a\\u73b0\\u5728\\u5f00\\u59cb\\u6ce8\\u518c\\u3002\n"
	"    \\u672c\\u6b21\\u4f1a\\u8bae\\u5c06\\u6c47\\u96c6\\u5404\\u65b9\\u9762\\u7684\\u4e13\\u5bb6\\u3002\\u6d89\\u53ca\\u7684\\u9886\\u57df\\u5305\\u62ec\\uff1a\\u56fd\\u9645\\u4e92\\u8054\\u7f51\\u548c\\u7edf\\u4e00\\u7801\\uff0c\\u56fd\\u9645\\u5316\\u548c\\u672c\\u5730\\u5316\\uff0c\n"
	"    \\u7edf\\u4e00\\u7801\\u5728\\u64cd\\u4f5c\\u7cfb\\u7edf\\u548c\\u5e94\\u7528\\u8f6f\\u4ef6\\u4e2d\\u7684\\u5b9e\\u73b0\\uff0c\\u5b57\\u578b\\uff0c\\u6587\\u672c\\u683c\\u5f0f\\u4ee5\\u53ca\\u591a\\u6587\\u79cd\\u8ba1\\u7b97\\u7b49\\u3002\n"
	"\n"
	"    Unicode\n"
	"    \\u5f53\\u4e16\\u754c\\u9700\\u8981\\u6c9f\\u901a\\u65f6\\uff0c\\u8bf7\\u7528Unicode\\uff01\n"
	"\n"
	"Conference Program\n"
	"    ")
) 
(
ICUEncDetTestCase("WIU-cz", "UTF-8 UTF-32BE UTF-32LE ISO-8859-2/cs", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Co je Unicode?\n"
	"\n"
	"    Unicode p\\u0159i\\u0159azuje ka\\u017ed\\u00e9mu znaku jedine\\u010dn\\u00e9 \\u010d\\u00edslo,\n"
	"    nez\\u00e1visle na platform\\u011b,\n"
	"    nez\\u00e1visle na programu,\n"
	"    nez\\u00e1visle na jazyce.\n"
	"\n"
	"    Po\\u010d\\u00edta\\u010de, ze sv\\u00e9 podstaty, pracuj\\u00ed pouze s \\u010d\\u00edsly. P\\u00edsmena a dal\\u0161\\u00ed znaky ukl\\u00e1daj\\u00ed tak, \\u017ee ka\\u017ed\\u00e9mu z nich\n"
	"    p\\u0159i\\u0159ad\\u00ed \\u010d\\u00edslo. P\\u0159ed vznikem Unicode existovaly stovky rozd\\u00edln\\u00fdch k\\u00f3dovac\\u00edch syst\\u00e9m\\u016f pro p\\u0159i\\u0159azov\\u00e1n\\u00ed t\\u011bchto\n"
	"    \\u010d\\u00edsel. \\u017d\\u00e1dn\\u00e9 z t\\u011bchto k\\u00f3dov\\u00e1n\\u00ed nemohlo obsahovat dostatek znak\\u016f: nap\\u0159\\u00edklad Evropsk\\u00e1 unie sama pot\\u0159ebuje\n"
	"    n\\u011bkolik r\\u016fzn\\u00fdch k\\u00f3dov\\u00e1n\\u00ed, aby pokryla v\\u0161echny sv\\u00e9 jazyky. Dokonce i pro jeden jedin\\u00fd jazyk, jako je angli\\u010dtina,\n"
	"    nevyhovovalo \\u017e\\u00e1dn\\u00e9 k\\u00f3dov\\u00e1n\\u00ed pro v\\u0161echny p\\u00edsmena, interpunkci a b\\u011b\\u017en\\u011b pou\\u017e\\u00edvan\\u00e9 technick\\u00e9 symboly.\n"
	"\n"
	"    Tyto k\\u00f3dovac\\u00ed syst\\u00e9my tak\\u00e9 byly v konfliktu jeden s druh\\u00fdm. To znamen\\u00e1, \\u017ee dv\\u011b k\\u00f3dov\\u00e1n\\u00ed mohou pou\\u017e\\u00edvat\n"
	"    stejn\\u00e9 \\u010d\\u00edslo pro dva r\\u016fzn\\u00e9 znaky, nebo pou\\u017e\\u00edvat r\\u016fzn\\u00e1 \\u010d\\u00edsla pro stejn\\u00fd znak. Jak\\u00fdkoli po\\u010d\\u00edta\\u010d (zvl\\u00e1\\u0161t\\u011b servery)\n"
	"    mus\\u00ed podporovat mnoho r\\u016fzn\\u00fdch k\\u00f3dov\\u00e1n\\u00ed; p\\u0159esto, kdykoli jsou data p\\u0159ed\\u00e1v\\u00e1na mezi r\\u016fzn\\u00fdmi k\\u00f3dov\\u00e1n\\u00edmi nebo\n"
	"    platformami, hroz\\u00ed, \\u017ee tato data budou po\\u0161kozena.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("WIU-el", "UTF-8 UTF-32BE UTF-32LE ISO-8859-7/el", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u03a4\\u03b9 \\u03b5\\u03af\\u03bd\\u03b1\\u03b9 \\u03c4\\u03bf Unicode;\n"
	"\n"
	"    \\u0397 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1 Unicode \\u03c0\\u03c1\\u03bf\\u03c4\\u03b5\\u03af\\u03bd\\u03b5\\u03b9 \\u03ad\\u03bd\\u03b1\\u03bd \\u03ba\\u03b1\\u03b9 \\u03bc\\u03bf\\u03bd\\u03b1\\u03b4\\u03b9\\u03ba\\u03cc \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03cc \\u03b3\\u03b9\\u03b1 \\u03ba\\u03ac\\u03b8\\u03b5 \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b1,\n"
	"    \\u03b1\\u03bd\\u03b5\\u03be\\u03ac\\u03c1\\u03c4\\u03b7\\u03c4\\u03b1 \\u03b1\\u03c0\\u03cc \\u03c4\\u03bf \\u03bb\\u03b5\\u03b9\\u03c4\\u03bf\\u03c5\\u03c1\\u03b3\\u03b9\\u03ba\\u03cc \\u03c3\\u03cd\\u03c3\\u03c4\\u03b7\\u03bc\\u03b1,\n"
	"    \\u03b1\\u03bd\\u03b5\\u03be\\u03ac\\u03c1\\u03c4\\u03b7\\u03c4\\u03b1 \\u03b1\\u03c0\\u03cc \\u03c4\\u03bf \\u03bb\\u03bf\\u03b3\\u03b9\\u03c3\\u03bc\\u03b9\\u03ba\\u03cc,\n"
	"    \\u03b1\\u03bd\\u03b5\\u03be\\u03ac\\u03c1\\u03c4\\u03b7\\u03c4\\u03b1 \\u03b1\\u03c0\\u03cc \\u03c4\\u03b7\\u03bd \\u03b3\\u03bb\\u03ce\\u03c3\\u03c3\\u03b1.\n"
	"\n"
	"    \\u039f\\u03b9 \\u03b7\\u03bb\\u03b5\\u03ba\\u03c4\\u03c1\\u03bf\\u03bd\\u03b9\\u03ba\\u03bf\\u03af \\u03c5\\u03c0\\u03bf\\u03bb\\u03bf\\u03b3\\u03b9\\u03c3\\u03c4\\u03ad\\u03c2, \\u03c3\\u03b5 \\u03c4\\u03b5\\u03bb\\u03b9\\u03ba\\u03ae \\u03b1\\u03bd\\u03ac\\u03bb\\u03c5\\u03c3\\u03b7, \\u03c7\\u03b5\\u03b9\\u03c1\\u03af\\u03b6\\u03bf\\u03bd\\u03c4\\u03b1\\u03b9 \\u03b1\\u03c0\\u03bb\\u03ce\\u03c2 \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03bf\\u03cd\\u03c2. \\u0391\\u03c0\\u03bf\\u03b8\\u03b7\\u03ba\\u03b5\\u03cd\\u03bf\\u03c5\\u03bd \\u03b3\\u03c1\\u03ac\\u03bc\\u03bc\\u03b1\\u03c4\\u03b1 \\u03ba\\u03b1\\u03b9\n"
	"    \\u03ac\\u03bb\\u03bb\\u03bf\\u03c5\\u03c2 \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b5\\u03c2 \\u03b1\\u03bd\\u03c4\\u03b9\\u03c3\\u03c4\\u03bf\\u03b9\\u03c7\\u03ce\\u03bd\\u03c4\\u03b1\\u03c2 \\u03c3\\u03c4\\u03bf \\u03ba\\u03b1\\u03b8\\u03ad\\u03bd\\u03b1 \\u03c4\\u03bf\\u03c5\\u03c2 \\u03b1\\u03c0\\u03cc \\u03ad\\u03bd\\u03b1\\u03bd \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03cc (\\u03bf\\u03bd\\u03bf\\u03bc\\u03ac\\u03b6\\u03bf\\u03c5\\u03bc\\u03b5 \\u03bc\\u03af\\u03b1 \\u03c4\\u03ad\\u03c4\\u03bf\\u03b9\\u03b1 \\u03b1\\u03bd\\u03c4\\u03b9\\u03c3\\u03c4\\u03bf\\u03b9\\u03c7\\u03af\\u03b1\n"
	"    \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1). \\u03a0\\u03c1\\u03b9\\u03bd \\u03c4\\u03b7\\u03bd \\u03b5\\u03c6\\u03b5\\u03cd\\u03c1\\u03b5\\u03c3\\u03b7 \\u03c4\\u03bf\\u03c5 Unicode, \\u03c5\\u03c0\\u03ae\\u03c1\\u03c7\\u03b1\\u03bd \\u03b5\\u03ba\\u03b1\\u03c4\\u03bf\\u03bd\\u03c4\\u03ac\\u03b4\\u03b5\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ad\\u03c2 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b5\\u03c2. \\u039b\\u03cc\\u03b3\\u03c9\n"
	"    \\u03c0\\u03b5\\u03c1\\u03b9\\u03bf\\u03c1\\u03b9\\u03c3\\u03bc\\u03ce\\u03bd \\u03bc\\u03b5\\u03b3\\u03ad\\u03b8\\u03bf\\u03c5\\u03c2 \\u03cc\\u03bc\\u03c9\\u03c2, \\u03c3\\u03b5 \\u03ba\\u03b1\\u03bc\\u03af\\u03b1 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1 \\u03b4\\u03b5\\u03bd \\u03c7\\u03c9\\u03c1\\u03bf\\u03cd\\u03c3\\u03b1\\u03bd \\u03b1\\u03c1\\u03ba\\u03b5\\u03c4\\u03bf\\u03af \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b5\\u03c2: \\u03bb\\u03cc\\u03b3\\u03bf\\u03c5 \\u03c7\\u03ac\\u03c1\\u03b9\\u03bd,\n"
	"    \\u03b7 \\u0395\\u03c5\\u03c1\\u03c9\\u03c0\\u03b1\\u03ca\\u03ba\\u03ae \\u0388\\u03bd\\u03c9\\u03c3\\u03b7 \\u03c7\\u03c1\\u03b5\\u03b9\\u03b1\\u03b6\\u03cc\\u03c4\\u03b1\\u03bd \\u03c0\\u03bb\\u03ae\\u03b8\\u03bf\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ce\\u03bd \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03c9\\u03bd \\u03b3\\u03b9\\u03b1 \\u03bd\\u03b1 \\u03ba\\u03b1\\u03bb\\u03cd\\u03c8\\u03b5\\u03b9 \\u03cc\\u03bb\\u03b5\\u03c2 \\u03c4\\u03b9\\u03c2 \\u03b3\\u03bb\\u03ce\\u03c3\\u03c3\\u03b5\\u03c2 \\u03c4\\u03c9\\u03bd\n"
	"    \\u03c7\\u03c9\\u03c1\\u03ce\\u03bd-\\u03bc\\u03b5\\u03bb\\u03ce\\u03bd \\u03c4\\u03b7\\u03c2. \\u0391\\u03ba\\u03cc\\u03bc\\u03b1 \\u03ba\\u03b1\\u03b9 \\u03b3\\u03b9\\u03b1 \\u03bc\\u03af\\u03b1 \\u03ba\\u03b1\\u03b9 \\u03bc\\u03cc\\u03bd\\u03b7 \\u03b3\\u03bb\\u03ce\\u03c3\\u03c3\\u03b1, \\u03cc\\u03c0\\u03c9\\u03c2 \\u03c0.\\u03c7. \\u03c4\\u03b1 \\u0391\\u03b3\\u03b3\\u03bb\\u03b9\\u03ba\\u03ac, \\u03bc\\u03af\\u03b1 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1 \\u03b4\\u03b5\\u03bd\n"
	"    \\u03b5\\u03c0\\u03b1\\u03c1\\u03ba\\u03bf\\u03cd\\u03c3\\u03b5 \\u03b3\\u03b9\\u03b1 \\u03bd\\u03b1 \\u03ba\\u03b1\\u03bb\\u03cd\\u03c8\\u03b5\\u03b9 \\u03cc\\u03bb\\u03b1 \\u03c4\\u03b1 \\u03b3\\u03c1\\u03ac\\u03bc\\u03bc\\u03b1\\u03c4\\u03b1, \\u03c3\\u03b7\\u03bc\\u03b5\\u03af\\u03b1 \\u03c3\\u03c4\\u03af\\u03be\\u03b7\\u03c2 \\u03ba\\u03b1\\u03b9 \\u03c4\\u03b5\\u03c7\\u03bd\\u03b9\\u03ba\\u03ac \\u03c3\\u03cd\\u03bc\\u03b2\\u03bf\\u03bb\\u03b1 \\u03b5\\u03c5\\u03c1\\u03b5\\u03af\\u03b1\\u03c2 \\u03c7\\u03c1\\u03ae\\u03c3\\u03b7\\u03c2.\n"
	"\n"
	"    \\u0395\\u03ba\\u03c4\\u03cc\\u03c2 \\u03b1\\u03c5\\u03c4\\u03bf\\u03cd, \\u03bf\\u03b9 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b5\\u03c2 \\u03b1\\u03c5\\u03c4\\u03ad\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03c9\\u03bd\\u03bf\\u03cd\\u03c3\\u03b1\\u03bd \\u03bc\\u03b5\\u03c4\\u03b1\\u03be\\u03cd \\u03c4\\u03bf\\u03c5\\u03c2. \\u0388\\u03c4\\u03c3\\u03b9, \\u03b4\\u03cd\\u03bf \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b5\\u03c2 \\u03bc\\u03c0\\u03bf\\u03c1\\u03bf\\u03cd\\u03c3\\u03b1\\u03bd \\u03ba\\u03ac\\u03bb\\u03bb\\u03b9\\u03c3\\u03c4\\u03b1 \\u03bd\\u03b1\n"
	"    \\u03c7\\u03c1\\u03b7\\u03c3\\u03b9\\u03bc\\u03bf\\u03c0\\u03bf\\u03b9\\u03bf\\u03cd\\u03bd \\u03c4\\u03bf\\u03bd \\u03af\\u03b4\\u03b9\\u03bf \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03cc \\u03b3\\u03b9\\u03b1 \\u03b4\\u03cd\\u03bf \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03bf\\u03cd\\u03c2 \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b5\\u03c2, \\u03ae \\u03bd\\u03b1 \\u03c7\\u03c1\\u03b7\\u03c3\\u03b9\\u03bc\\u03bf\\u03c0\\u03bf\\u03b9\\u03bf\\u03cd\\u03bd \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03bf\\u03cd\\u03c2 \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03bf\\u03cd\\u03c2 \\u03b3\\u03b9\\u03b1\n"
	"    \\u03c4\\u03bf\\u03bd \\u03af\\u03b4\\u03b9\\u03bf \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b1. \\u039a\\u03ac\\u03b8\\u03b5 \\u03c5\\u03c0\\u03bf\\u03bb\\u03bf\\u03b3\\u03b9\\u03c3\\u03c4\\u03ae\\u03c2 (\\u03ba\\u03b1\\u03b9 \\u03b9\\u03b4\\u03af\\u03c9\\u03c2 \\u03b5\\u03ac\\u03bd \\u03ae\\u03c4\\u03b1\\u03bd \\u03b4\\u03b9\\u03b1\\u03ba\\u03bf\\u03bc\\u03b9\\u03c3\\u03c4\\u03ae\\u03c2) \\u03ad\\u03c0\\u03c1\\u03b5\\u03c0\\u03b5 \\u03bd\\u03b1 \\u03c5\\u03c0\\u03bf\\u03c3\\u03c4\\u03b7\\u03c1\\u03af\\u03b6\\u03b5\\u03b9 \\u03c0\\u03bb\\u03ae\\u03b8\\u03bf\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ce\\u03bd\n"
	"    \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03c9\\u03bd \\u03c4\\u03b1\\u03c5\\u03c4\\u03cc\\u03c7\\u03c1\\u03bf\\u03bd\\u03b1 \\u03ba\\u03ac\\u03b8\\u03b5 \\u03c6\\u03bf\\u03c1\\u03ac \\u03c0\\u03bf\\u03c5 \\u03b4\\u03b5\\u03b4\\u03bf\\u03bc\\u03ad\\u03bd\\u03b1 \\u03bc\\u03b5\\u03c4\\u03b1\\u03c6\\u03ad\\u03c1\\u03bf\\u03bd\\u03c4\\u03b1\\u03bd \\u03bc\\u03b5\\u03c4\\u03b1\\u03be\\u03cd \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ce\\u03bd \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03c9\\u03bd \\u03ae\n"
	"    \\u03bb\\u03b5\\u03b9\\u03c4\\u03bf\\u03c5\\u03c1\\u03b3\\u03b9\\u03ba\\u03ce\\u03bd \\u03c3\\u03c5\\u03c3\\u03c4\\u03b7\\u03bc\\u03ac\\u03c4\\u03c9\\u03bd, \\u03c4\\u03b1 \\u03b4\\u03b5\\u03b4\\u03bf\\u03bc\\u03ad\\u03bd\\u03b1 \\u03b1\\u03c5\\u03c4\\u03ac \\u03ba\\u03b9\\u03bd\\u03b4\\u03cd\\u03bd\\u03b5\\u03c5\\u03b1\\u03bd \\u03bd\\u03b1 \\u03b1\\u03bb\\u03bb\\u03bf\\u03b9\\u03c9\\u03b8\\u03bf\\u03cd\\u03bd.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("WIU-el-Q", "UTF-8 UTF-32BE UTF-32LE windows-1253/el", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u03a4\\u03b9 \\u03b5\\u03af\\u03bd\\u03b1\\u03b9 \\u03c4\\u03bf \\u201cUnicode\\u201d;\n"
	"\n"
	"    \\u0397 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1 Unicode \\u03c0\\u03c1\\u03bf\\u03c4\\u03b5\\u03af\\u03bd\\u03b5\\u03b9 \\u03ad\\u03bd\\u03b1\\u03bd \\u03ba\\u03b1\\u03b9 \\u03bc\\u03bf\\u03bd\\u03b1\\u03b4\\u03b9\\u03ba\\u03cc \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03cc \\u03b3\\u03b9\\u03b1 \\u03ba\\u03ac\\u03b8\\u03b5 \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b1,\n"
	"    \\u03b1\\u03bd\\u03b5\\u03be\\u03ac\\u03c1\\u03c4\\u03b7\\u03c4\\u03b1 \\u03b1\\u03c0\\u03cc \\u03c4\\u03bf \\u03bb\\u03b5\\u03b9\\u03c4\\u03bf\\u03c5\\u03c1\\u03b3\\u03b9\\u03ba\\u03cc \\u03c3\\u03cd\\u03c3\\u03c4\\u03b7\\u03bc\\u03b1,\n"
	"    \\u03b1\\u03bd\\u03b5\\u03be\\u03ac\\u03c1\\u03c4\\u03b7\\u03c4\\u03b1 \\u03b1\\u03c0\\u03cc \\u03c4\\u03bf \\u03bb\\u03bf\\u03b3\\u03b9\\u03c3\\u03bc\\u03b9\\u03ba\\u03cc,\n"
	"    \\u03b1\\u03bd\\u03b5\\u03be\\u03ac\\u03c1\\u03c4\\u03b7\\u03c4\\u03b1 \\u03b1\\u03c0\\u03cc \\u03c4\\u03b7\\u03bd \\u03b3\\u03bb\\u03ce\\u03c3\\u03c3\\u03b1.\n"
	"\n"
	"    \\u039f\\u03b9 \\u03b7\\u03bb\\u03b5\\u03ba\\u03c4\\u03c1\\u03bf\\u03bd\\u03b9\\u03ba\\u03bf\\u03af \\u03c5\\u03c0\\u03bf\\u03bb\\u03bf\\u03b3\\u03b9\\u03c3\\u03c4\\u03ad\\u03c2, \\u03c3\\u03b5 \\u03c4\\u03b5\\u03bb\\u03b9\\u03ba\\u03ae \\u03b1\\u03bd\\u03ac\\u03bb\\u03c5\\u03c3\\u03b7, \\u03c7\\u03b5\\u03b9\\u03c1\\u03af\\u03b6\\u03bf\\u03bd\\u03c4\\u03b1\\u03b9 \\u03b1\\u03c0\\u03bb\\u03ce\\u03c2 \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03bf\\u03cd\\u03c2. \\u0391\\u03c0\\u03bf\\u03b8\\u03b7\\u03ba\\u03b5\\u03cd\\u03bf\\u03c5\\u03bd \\u03b3\\u03c1\\u03ac\\u03bc\\u03bc\\u03b1\\u03c4\\u03b1 \\u03ba\\u03b1\\u03b9\n"
	"    \\u03ac\\u03bb\\u03bb\\u03bf\\u03c5\\u03c2 \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b5\\u03c2 \\u03b1\\u03bd\\u03c4\\u03b9\\u03c3\\u03c4\\u03bf\\u03b9\\u03c7\\u03ce\\u03bd\\u03c4\\u03b1\\u03c2 \\u03c3\\u03c4\\u03bf \\u03ba\\u03b1\\u03b8\\u03ad\\u03bd\\u03b1 \\u03c4\\u03bf\\u03c5\\u03c2 \\u03b1\\u03c0\\u03cc \\u03ad\\u03bd\\u03b1\\u03bd \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03cc (\\u03bf\\u03bd\\u03bf\\u03bc\\u03ac\\u03b6\\u03bf\\u03c5\\u03bc\\u03b5 \\u03bc\\u03af\\u03b1 \\u03c4\\u03ad\\u03c4\\u03bf\\u03b9\\u03b1 \\u03b1\\u03bd\\u03c4\\u03b9\\u03c3\\u03c4\\u03bf\\u03b9\\u03c7\\u03af\\u03b1\n"
	"    \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1). \\u03a0\\u03c1\\u03b9\\u03bd \\u03c4\\u03b7\\u03bd \\u03b5\\u03c6\\u03b5\\u03cd\\u03c1\\u03b5\\u03c3\\u03b7 \\u03c4\\u03bf\\u03c5 Unicode, \\u03c5\\u03c0\\u03ae\\u03c1\\u03c7\\u03b1\\u03bd \\u03b5\\u03ba\\u03b1\\u03c4\\u03bf\\u03bd\\u03c4\\u03ac\\u03b4\\u03b5\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ad\\u03c2 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b5\\u03c2. \\u039b\\u03cc\\u03b3\\u03c9\n"
	"    \\u03c0\\u03b5\\u03c1\\u03b9\\u03bf\\u03c1\\u03b9\\u03c3\\u03bc\\u03ce\\u03bd \\u03bc\\u03b5\\u03b3\\u03ad\\u03b8\\u03bf\\u03c5\\u03c2 \\u03cc\\u03bc\\u03c9\\u03c2, \\u03c3\\u03b5 \\u03ba\\u03b1\\u03bc\\u03af\\u03b1 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1 \\u03b4\\u03b5\\u03bd \\u03c7\\u03c9\\u03c1\\u03bf\\u03cd\\u03c3\\u03b1\\u03bd \\u03b1\\u03c1\\u03ba\\u03b5\\u03c4\\u03bf\\u03af \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b5\\u03c2: \\u03bb\\u03cc\\u03b3\\u03bf\\u03c5 \\u03c7\\u03ac\\u03c1\\u03b9\\u03bd,\n"
	"    \\u03b7 \\u0395\\u03c5\\u03c1\\u03c9\\u03c0\\u03b1\\u03ca\\u03ba\\u03ae \\u0388\\u03bd\\u03c9\\u03c3\\u03b7 \\u03c7\\u03c1\\u03b5\\u03b9\\u03b1\\u03b6\\u03cc\\u03c4\\u03b1\\u03bd \\u03c0\\u03bb\\u03ae\\u03b8\\u03bf\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ce\\u03bd \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03c9\\u03bd \\u03b3\\u03b9\\u03b1 \\u03bd\\u03b1 \\u03ba\\u03b1\\u03bb\\u03cd\\u03c8\\u03b5\\u03b9 \\u03cc\\u03bb\\u03b5\\u03c2 \\u03c4\\u03b9\\u03c2 \\u03b3\\u03bb\\u03ce\\u03c3\\u03c3\\u03b5\\u03c2 \\u03c4\\u03c9\\u03bd\n"
	"    \\u03c7\\u03c9\\u03c1\\u03ce\\u03bd-\\u03bc\\u03b5\\u03bb\\u03ce\\u03bd \\u03c4\\u03b7\\u03c2. \\u0391\\u03ba\\u03cc\\u03bc\\u03b1 \\u03ba\\u03b1\\u03b9 \\u03b3\\u03b9\\u03b1 \\u03bc\\u03af\\u03b1 \\u03ba\\u03b1\\u03b9 \\u03bc\\u03cc\\u03bd\\u03b7 \\u03b3\\u03bb\\u03ce\\u03c3\\u03c3\\u03b1, \\u03cc\\u03c0\\u03c9\\u03c2 \\u03c0.\\u03c7. \\u03c4\\u03b1 \\u0391\\u03b3\\u03b3\\u03bb\\u03b9\\u03ba\\u03ac, \\u03bc\\u03af\\u03b1 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b1 \\u03b4\\u03b5\\u03bd\n"
	"    \\u03b5\\u03c0\\u03b1\\u03c1\\u03ba\\u03bf\\u03cd\\u03c3\\u03b5 \\u03b3\\u03b9\\u03b1 \\u03bd\\u03b1 \\u03ba\\u03b1\\u03bb\\u03cd\\u03c8\\u03b5\\u03b9 \\u03cc\\u03bb\\u03b1 \\u03c4\\u03b1 \\u03b3\\u03c1\\u03ac\\u03bc\\u03bc\\u03b1\\u03c4\\u03b1, \\u03c3\\u03b7\\u03bc\\u03b5\\u03af\\u03b1 \\u03c3\\u03c4\\u03af\\u03be\\u03b7\\u03c2 \\u03ba\\u03b1\\u03b9 \\u03c4\\u03b5\\u03c7\\u03bd\\u03b9\\u03ba\\u03ac \\u03c3\\u03cd\\u03bc\\u03b2\\u03bf\\u03bb\\u03b1 \\u03b5\\u03c5\\u03c1\\u03b5\\u03af\\u03b1\\u03c2 \\u03c7\\u03c1\\u03ae\\u03c3\\u03b7\\u03c2.\n"
	"\n"
	"    \\u0395\\u03ba\\u03c4\\u03cc\\u03c2 \\u03b1\\u03c5\\u03c4\\u03bf\\u03cd, \\u03bf\\u03b9 \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b5\\u03c2 \\u03b1\\u03c5\\u03c4\\u03ad\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03c9\\u03bd\\u03bf\\u03cd\\u03c3\\u03b1\\u03bd \\u03bc\\u03b5\\u03c4\\u03b1\\u03be\\u03cd \\u03c4\\u03bf\\u03c5\\u03c2. \\u0388\\u03c4\\u03c3\\u03b9, \\u03b4\\u03cd\\u03bf \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03b5\\u03c2 \\u03bc\\u03c0\\u03bf\\u03c1\\u03bf\\u03cd\\u03c3\\u03b1\\u03bd \\u03ba\\u03ac\\u03bb\\u03bb\\u03b9\\u03c3\\u03c4\\u03b1 \\u03bd\\u03b1\n"
	"    \\u03c7\\u03c1\\u03b7\\u03c3\\u03b9\\u03bc\\u03bf\\u03c0\\u03bf\\u03b9\\u03bf\\u03cd\\u03bd \\u03c4\\u03bf\\u03bd \\u03af\\u03b4\\u03b9\\u03bf \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03cc \\u03b3\\u03b9\\u03b1 \\u03b4\\u03cd\\u03bf \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03bf\\u03cd\\u03c2 \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b5\\u03c2, \\u03ae \\u03bd\\u03b1 \\u03c7\\u03c1\\u03b7\\u03c3\\u03b9\\u03bc\\u03bf\\u03c0\\u03bf\\u03b9\\u03bf\\u03cd\\u03bd \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03bf\\u03cd\\u03c2 \\u03b1\\u03c1\\u03b9\\u03b8\\u03bc\\u03bf\\u03cd\\u03c2 \\u03b3\\u03b9\\u03b1\n"
	"    \\u03c4\\u03bf\\u03bd \\u03af\\u03b4\\u03b9\\u03bf \\u03c7\\u03b1\\u03c1\\u03b1\\u03ba\\u03c4\\u03ae\\u03c1\\u03b1. \\u039a\\u03ac\\u03b8\\u03b5 \\u03c5\\u03c0\\u03bf\\u03bb\\u03bf\\u03b3\\u03b9\\u03c3\\u03c4\\u03ae\\u03c2 (\\u03ba\\u03b1\\u03b9 \\u03b9\\u03b4\\u03af\\u03c9\\u03c2 \\u03b5\\u03ac\\u03bd \\u03ae\\u03c4\\u03b1\\u03bd \\u03b4\\u03b9\\u03b1\\u03ba\\u03bf\\u03bc\\u03b9\\u03c3\\u03c4\\u03ae\\u03c2) \\u03ad\\u03c0\\u03c1\\u03b5\\u03c0\\u03b5 \\u03bd\\u03b1 \\u03c5\\u03c0\\u03bf\\u03c3\\u03c4\\u03b7\\u03c1\\u03af\\u03b6\\u03b5\\u03b9 \\u03c0\\u03bb\\u03ae\\u03b8\\u03bf\\u03c2 \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ce\\u03bd\n"
	"    \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03c9\\u03bd \\u03c4\\u03b1\\u03c5\\u03c4\\u03cc\\u03c7\\u03c1\\u03bf\\u03bd\\u03b1 \\u03ba\\u03ac\\u03b8\\u03b5 \\u03c6\\u03bf\\u03c1\\u03ac \\u03c0\\u03bf\\u03c5 \\u03b4\\u03b5\\u03b4\\u03bf\\u03bc\\u03ad\\u03bd\\u03b1 \\u03bc\\u03b5\\u03c4\\u03b1\\u03c6\\u03ad\\u03c1\\u03bf\\u03bd\\u03c4\\u03b1\\u03bd \\u03bc\\u03b5\\u03c4\\u03b1\\u03be\\u03cd \\u03b4\\u03b9\\u03b1\\u03c6\\u03bf\\u03c1\\u03b5\\u03c4\\u03b9\\u03ba\\u03ce\\u03bd \\u03ba\\u03c9\\u03b4\\u03b9\\u03ba\\u03bf\\u03c3\\u03b5\\u03bb\\u03af\\u03b4\\u03c9\\u03bd \\u03ae\n"
	"    \\u03bb\\u03b5\\u03b9\\u03c4\\u03bf\\u03c5\\u03c1\\u03b3\\u03b9\\u03ba\\u03ce\\u03bd \\u03c3\\u03c5\\u03c3\\u03c4\\u03b7\\u03bc\\u03ac\\u03c4\\u03c9\\u03bd, \\u03c4\\u03b1 \\u03b4\\u03b5\\u03b4\\u03bf\\u03bc\\u03ad\\u03bd\\u03b1 \\u03b1\\u03c5\\u03c4\\u03ac \\u03ba\\u03b9\\u03bd\\u03b4\\u03cd\\u03bd\\u03b5\\u03c5\\u03b1\\u03bd \\u03bd\\u03b1 \\u03b1\\u03bb\\u03bb\\u03bf\\u03b9\\u03c9\\u03b8\\u03bf\\u03cd\\u03bd.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("WIU-pl", "UTF-8 UTF-32BE UTF-32LE ISO-8859-2/pl", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Czym jest Unikod ?\n"
	"\n"
	"    Unikod przypisuje unikalny numer ka\\u017cdemu znakowi, niezale\\u017cny od u\\u017cywanej platformy, programu czy j\\u0119zyka.\n"
	"\n"
	"    Zasadniczo, komputery rozumiej\\u0105 tylko liczby. Zapisuj\\u0105 litery i inne znaki przypisuj\\u0105c ka\\u017cdemu z nich liczb\\u0119.\n"
	"    Nim powsta\\u0142 Unikod, by\\u0142o wiele r\\u00f3\\u017cnych system\\u00f3w kodowania przypisuj\\u0105cych te liczby. Brakowa\\u0142o jednego,\n"
	"    kt\\u00f3ry m\\u00f3g\\u0142by pomie\\u015bci\\u0107 wystarczaj\\u0105co du\\u017c\\u0105 liczb\\u0119 znak\\u00f3w. Przyk\\u0142adowo, sama Unia Europejska potrzebowa\\u0142a\n"
	"    kilku r\\u00f3\\u017cnych kodowa\\u0144, by m\\u00f3c u\\u017cywa\\u0107 wszystkich u\\u017cywanych w niej j\\u0119zyk\\u00f3w. Nawet dla pojedynczego j\\u0119zyka\n"
	"    takiego jak np. angielski brakowa\\u0142o jednego kodowania, kt\\u00f3re by\\u0142oby odpowiednie dla zaprezentowania\n"
	"    wszystkich liter, znak\\u00f3w przestankowych i popularnych symboli technicznych.\n"
	"\n"
	"    Innym problemem by\\u0142o, \\u017ce kodowania te kolidowa\\u0142y ze sob\\u0105. Dwa, r\\u00f3\\u017cne kodowania u\\u017cywa\\u0142y jednej liczby dla dwu\n"
	"    r\\u00f3\\u017cnych znak\\u00f3w lub r\\u00f3\\u017cnych liczb dla tego samego znaku. Wszystkie komputery (mi\\u0119dzy innymi serwery) musz\\u0105\n"
	"    wspiera\\u0107 wszystkie te kodowania, gdy\\u017c dane przesy\\u0142ane mi\\u0119dzy r\\u00f3\\u017cnymi systemami operacyjnymi zawsze\n"
	"    nara\\u017cone s\\u0105 na uszkodzenie.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("WIU-tr", "UTF-8 UTF-32BE UTF-32LE ISO-8859-9/tr", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    Evrensel Kod Nedir?\n"
	"\n"
	"    Evrensel Kod her yaz\\u0131 karakteri i\\u00e7in bir ve yaln\\u0131z bir say\\u0131 \\u015fart ko\\u015far,\n"
	"    hangi altyap\\u0131,\n"
	"    hangi yaz\\u0131l\\u0131m,\n"
	"    hangi dil olursa olsun.\n"
	"\n"
	"    \\u0130lke olarak, bilgisayarlar sadece say\\u0131larla i\\u015flem yaparlar. Kelimelerin ve yaz\\u0131 karakterlerinin her biri i\\u00e7in\n"
	"    birer say\\u0131 atarlar ve b\\u00f6yle saklarlar. Evrensel Kod ke\\u015ffedilmeden \\u00f6nce, bu say\\u0131lar\\u0131 atamak i\\u00e7in bir\\u00e7ok \\u015fifreleme\n"
	"    y\\u00f6ntemi vard\\u0131. Ancak, t\\u00fcm bu dilleri g\\u00f6sterebilecek, \\u00f6rne\\u011fin; Avrupa Toplulu\\u011fu b\\u00fcnyesindeki t\\u00fcm \\u00fclkelerin dillerini\n"
	"    kapsayacak bir tek \\u015fifreleme y\\u00f6ntemi yoktu. Bunun yan\\u0131s\\u0131ra, sadece \\u0130ngilizcedeki harfleri, noktalama\n"
	"    i\\u015faretlerini ve teknik sembolleri kapsayan tek bir \\u015fifreleme y\\u00f6ntemi de bulunmamaktayd\\u0131.\n"
	"\n"
	"    Bu \\u015fifreleme y\\u00f6ntemleri kendi aralar\\u0131nda \\u00e7eli\\u015fmektedir. \\u0130ki farkl\\u0131 \\u015fifreleme, ayn\\u0131 say\\u0131y\\u0131 iki farkl\\u0131 karaktere\n"
	"    vermi\\u015f olabilir ya da farkl\\u0131 say\\u0131lar ayn\\u0131 karekteri kodlayabilir. Bilgisayarlar, \\u00f6zellikle sunucular, bir\\u00e7ok\n"
	"    \\u015fifrelemeyi desteklemek zorundad\\u0131rlar; veriler, farkl\\u0131 \\u015fifreleme ve altyap\\u0131lardan ge\\u00e7erken bozulma riski ta\\u015f\\u0131rlar.\n"
	"\n"
	"    ")
) 
(
ICUEncDetTestCase("WIU-tr-Q", "UTF-8 UTF-32BE UTF-32LE windows-1254/tr", ""
	/*  Copyright (C) 1991-2005 Unicode, Inc. All rights reserved.  */
	"\n"
	"\n"
	"    \\u201cEvrensel Kod\\u201d Nedir?\n"
	"\n"
	"    Evrensel Kod her yaz\\u0131 karakteri i\\u00e7in bir ve yaln\\u0131z bir say\\u0131 \\u015fart ko\\u015far,\n"
	"    hangi altyap\\u0131,\n"
	"    hangi yaz\\u0131l\\u0131m,\n"
	"    hangi dil olursa olsun.\n"
	"\n"
	"    \\u0130lke olarak, bilgisayarlar sadece say\\u0131larla i\\u015flem yaparlar. Kelimelerin ve yaz\\u0131 karakterlerinin her biri i\\u00e7in\n"
	"    birer say\\u0131 atarlar ve b\\u00f6yle saklarlar. Evrensel Kod ke\\u015ffedilmeden \\u00f6nce, bu say\\u0131lar\\u0131 atamak i\\u00e7in bir\\u00e7ok \\u015fifreleme\n"
	"    y\\u00f6ntemi vard\\u0131. Ancak, t\\u00fcm bu dilleri g\\u00f6sterebilecek, \\u00f6rne\\u011fin; Avrupa Toplulu\\u011fu b\\u00fcnyesindeki t\\u00fcm \\u00fclkelerin dillerini\n"
	"    kapsayacak bir tek \\u015fifreleme y\\u00f6ntemi yoktu. Bunun yan\\u0131s\\u0131ra, sadece \\u0130ngilizcedeki harfleri, noktalama\n"
	"    i\\u015faretlerini ve teknik sembolleri kapsayan tek bir \\u015fifreleme y\\u00f6ntemi de bulunmamaktayd\\u0131.\n"
	"\n"
	"    Bu \\u015fifreleme y\\u00f6ntemleri kendi aralar\\u0131nda \\u00e7eli\\u015fmektedir. \\u0130ki farkl\\u0131 \\u015fifreleme, ayn\\u0131 say\\u0131y\\u0131 iki farkl\\u0131 karaktere\n"
	"    vermi\\u015f olabilir ya da farkl\\u0131 say\\u0131lar ayn\\u0131 karekteri kodlayabilir. Bilgisayarlar, \\u00f6zellikle sunucular, bir\\u00e7ok\n"
	"    \\u015fifrelemeyi desteklemek zorundad\\u0131rlar; veriler, farkl\\u0131 \\u015fifreleme ve altyap\\u0131lardan ge\\u00e7erken bozulma riski ta\\u015f\\u0131rlar.\n"
	"\n"
	"    ")
) 
; // data_icudet_tests
