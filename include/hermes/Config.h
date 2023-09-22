/**
 * Hermes - A RPC for IOT
 * Copyright (C) 2023  Eduard Sargsyan and Andrey Ovodov
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HM_CONFIG_H
#define HM_CONFIG_H

/**
 * To verride default config device HERMES_CONFIG_OVERRIDE with a header
 * file containing your definitions
 */
#ifdef HERMES_CONFIG_OVERRIDE
#include HERMES_CONFIG_OVERRIDE
#endif // HERMES_CONFIG_OVERRIDE

/**
 * Thit file contains common cussent configuration for the library
 */


/**
 * This should be uncommented is the library is going to be used in a single
 * thread environment(e.g. Arduino)
 */

// #define HM_SINGLE_THREAD

#define HM_CONCAT(X, Y) X ## Y

/**
 * If HM_DISABLE_LOGGING is not defined following logging macro will be
 * optimized out by compiler.
 */
#ifdef HM_DISABLE_LOGGING
#define HM_LOG_WRITE(...)
#else
#include LOGGING_HEADER_H
#endif // HM_DISABLE_LOGGING

#define HM_INFO(msg, ...) do{ HM_LOG_WRITE ("I: " msg "\n", \
								## __VA_ARGS__); \
							} while(0)

#define HM_DBG(msg, ...)  do{ HM_LOG_WRITE ("D: " msg " %s:%d\n", \
								## __VA_ARGS__, \
								__FILE__, __LINE__); \
							} while(0)

#define HM_WARN(msg, ...) do{ HM_LOG_WRITE ("W: " msg "\n", \
								## __VA_ARGS__); \
							} while(0)

#define HM_ERR(msg, ...)  do{ HM_LOG_WRITE ("E: " msg "\n", \
								## __VA_ARGS__); \
							} while(0)

#ifndef HERMES_SERIAL_LENGTH
#define HERMES_SERIAL_LENGTH 8
#endif // HERMES_SERIAL_LENGTH

#ifndef HERMES_TOKEN_LENGTH
#define HERMES_TOKEN_LENGTH 8
#endif // HERMES_TOKEN_LENGTH

#endif // HM_CONFIG_H
