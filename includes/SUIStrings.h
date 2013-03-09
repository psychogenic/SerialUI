/*
 *
 * SUIStrings.h -- SerialUI system strings selection.
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further informations on licensing
 * terms.
 *
 *
 * ************************* OVERVIEW *************************
 *
 * Select the appropriate language strings file, here.
 * See includes/SUIStrings_en.h for details on language resource files.
 *
 */

#ifndef SUIStringsINC_h
#define SUIStringsINC_h

#include "SUIConfig.h"

#ifdef SUI_STRINGS_LANGUAGE_EN
#include "SUIStrings_en.h"
#endif

#ifdef SUI_STRINGS_LANGUAGE_FR
#include "SUIStrings_fr.h"
#endif


#endif

