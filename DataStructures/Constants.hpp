#pragma once

#include "String.hpp"

const double EPSILON = 0.001;

const String STRINGVIEWMODE_DASHES = "----------------";

const String STRINGVIEWMODE = "STRING VIEW MODE";

const String STRINGVIEWMODE_PAGE = "Page ";

const String STRINGVIEWMODE_COMMANDWINDOW = "Command: ";

const String STRINGVIEWMODE_HELP_MESSAGE = "This is the string view mode. It's used to easily view big strings. "
"It splits the string to pages, with no more than n lines per page. "
"You can navigate between pages using the commands \"nextpage\", that goes to the next page, "
"\"prevpage\", that goes to the previous page, \"goto <page number>\" that goes to a specific page "
"and you can exit the string view mode with the \"exit\" command. ";

const String STRINGVIEWMODE_NOTVALIDCOMMAND_MESSAGE = "Not a valid command. Type \"help\" if you don't know what to do, "
"or \"exit\" to exit the string view mode.";

const String STRINGVIEWMODE_NEXTPAGE_COMMAND = "nextpage";
const String STRINGVIEWMODE_PREVPAGE_COMMAND = "prevpage";
const String STRINGVIEWMODE_GOTO_COMMAND = "goto";
const String STRINGVIEWMODE_HELP_COMMAND = "help";
const String STRINGVIEWMODE_EXIT_COMMAND = "exit";

const String STRINGVIEWMODE_TESTSTRING = "alabala\nhmm\ni ko sq\nnqkvi redove tuka\noshte edin\nyea\nnz ko sq\n"
"mii aide oshte\nda ima tam\nda testvam\noshte redove\ndadada\npickle riiiick\nfrench\n"
"hiphop\naide de\nkolko oshte\nmii oshte tolkova\nmaleee\nmn ue\nne e nujno da sa tolko dulgi ama da vidim\n"
"oshteee\nalabala\noctopus\nmi e lubimiq\nalbum\nno lubimata mi\npesen\nne e ot nego\n"
"a e lateralus\nna tool\nbasi qkata pesen\naide stiga";