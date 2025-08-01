/*
 * Copyright (C) 2009, Pino Toscano <pino@kde.org>
 * Copyright (C) 2018, 2019, Albert Astals Cid <aacid@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef POPPLER_VERSION_H
#define POPPLER_VERSION_H

#include "poppler-export.h"

#include <QString>

// glibc < 2.28 used to include sys/sysmacros.h
// from sys/types.h and sysmacros.h defines minor and major so
// undefine them. You may need to undefine them in your code too.
#undef minor
#undef major

#define POPPLER_VERSION "25.07.90"
#define POPPLER_VERSION_MAJOR 25
#define POPPLER_VERSION_MINOR 7
#define POPPLER_VERSION_MICRO 90

namespace Poppler
{

namespace Version
{

/**
 \returns the version string of the current poppler-qt6 library
 */
POPPLER_QT6_EXPORT QString string();

/**
 \returns the "major" number of the version of the current poppler-qt6 library
 */
POPPLER_QT6_EXPORT unsigned int major();

/**
 \returns the "minor" number of the version of the current poppler-qt6 library
 */
POPPLER_QT6_EXPORT unsigned int minor();

/**
 \returns the "micro" number of the version of the current poppler-qt6 library
 */
POPPLER_QT6_EXPORT unsigned int micro();

}

}

#endif
