#include "../../arguments/processors/nsk_option_version.h"
#include "../../output/nsk_output_inf.h"

#if !defined(NSK_PROGRAM_NAME)
    #error "NSK_PROGRAM_NAME should be defined with the program name"
#endif

#if !defined(NSK_VERSION_MAJOR)
    #error "NSK_VERSION_MAJOR should be defined as major program version number"
#endif

#if !defined(NSK_VERSION_MINOR)
    #error "NSK_VERSION_MINOR should be defined as minor program version number"
#endif

#if !defined(NSK_VERSION_BUILD)
    #error "NSK_VERSION_BUILD should be defined as build program version number"
#endif

/*!
 * \brief  Show program version and exit
 */
void nsk_option_version(void) {
    nsk_inf(
        "%s %d.%d.%d\n",
        NSK_PROGRAM_NAME,
        NSK_VERSION_MAJOR,
        NSK_VERSION_MINOR,
        NSK_VERSION_BUILD
    );
    nsk_inf("Copyright (C) 2025 Codemeow\n");
    nsk_inf("\n");
    nsk_inf("License: MIT <https://opensource.org/license/MIT>\n");
    nsk_inf("Permission is hereby granted, free of charge, to any person obtaining a copy\n");
    nsk_inf("of this software and associated documentation files (the \"Software\"), to deal\n");
    nsk_inf("in the Software without restriction, including without limitation the rights\n");
    nsk_inf("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
    nsk_inf("copies of the Software, subject to the above copyright notice and this\n");
    nsk_inf("permission notice being included in all copies or substantial portions of\n");
    nsk_inf("the Software.\n");
    nsk_inf("\n");
    nsk_inf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
    nsk_inf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
    nsk_inf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n");
}