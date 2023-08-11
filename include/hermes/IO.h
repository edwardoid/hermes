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

#ifndef HM_IO_H
#define HM_IO_H

#include <hermes/Config.h>
#include <hermes/Types.h>

namespace hermes
{

	/**
	 * This is as abstraction over communication channel.
	 */
	class IO
	{
	public:
		IO() = default;
		virtual ~IO() = default;

		IO(const IO&) = delete;
		const IO& operator = (const IO&) = delete;

		/**
		 * @return true if channel is still operatable.
		 */
		virtual bool good() const = 0;

		/**
		 * Drop data available.
		 * @see available()
		 */
		virtual void flush() = 0;

		/**
		 * @return Bytes available to read.
		 * @see read()
		 **/
		virtual buffer_length_t available() const = 0;

		/**
		 * Blocks while there is any data to read
		 * @param length Bytes needed to be available after block
		 * @return Bytes available to read
		*/
		virtual buffer_length_t wait(buffer_length_t length) = 0;

		/**
		 * @param buf Byte buffer to me transmitted.
		 * @param length Buffer length.
		 * @return Returns how many bytes has been successefuly transmitted or -1 if something went wrong.
		 */
		virtual buffer_length_t write(const byte_t* buf, buffer_length_t length) = 0;

		/**
		 * @param buf Byte buffer for placing data received.
		 * @param buf Buffer size.
		 * @return -1 If something went wrong.
		 */
		virtual buffer_length_t read(byte_t* buf, buffer_length_t length) = 0;

		/**
		 * @param obj Object to be transmitetd.
		 * @return true if object has been transmitetd successefully.
		 * @see write(const byte_t* buf, buffer_length_t length)
		 * @note There is not custom serialization implemented here.
		 */
		template<class T>
		inline bool write(const T& obj)
		{ return sizeof(T) == write(reinterpret_cast<const byte_t*>(&obj), sizeof(T)); }

		/**
		 * @param obj Target object to be read into.
		 * @return true if object has been read successefully.
		 * @see read(byte_t* buf, buffer_length_t length)
		 * @note There is not custom serialization implemented here
		 */
		template<class T>
		inline bool read(T& obj)
		{ return sizeof(T) == read(reinterpret_cast<byte_t*>(&obj), sizeof(T)); }

		/**
		 * Close communication channel.
		 * @param false if something went wrong.
		 */
		virtual bool close() = 0;
	};
}

#endif // HM_IO_H