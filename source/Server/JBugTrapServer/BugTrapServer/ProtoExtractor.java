/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Protocol stream extractors.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

import java.io.*;

/** Protocol stream extractors. */
class ProtoExtractor {
	/** Input stream with protcol data. */
	final private InputStream stream;

	/** Initialize the object. */
	public ProtoExtractor(InputStream stream) {
		this.stream = stream;
	}

	/**
	 * Reads one byte and returns true if that byte is nonzero, false if that byte is zero.
	 * @return boolean value.
	 */
	public boolean readBoolean() throws IOException {
		return (this.stream.read() != 0);
	}

	/**
	 * Reads one byte and returns byte value.
	 * @return byte value.
	 */
	public byte readByte() throws IOException {
		return (byte)this.stream.read();
	}

	/**
	 * Reads two bytes in low-endian encoding and returns an int value.
	 * @return integer value.
	 */
	public short readShort() throws IOException {
		int b1 = this.stream.read();
		int b2 = this.stream.read();
		return (short)((b1 & 0xFF) | ((b2 & 0xFF) << 8));
	}

	/**
	 * Reads four bytes in low-endian encoding and returns an int value.
	 * @return integer value.
	 */
	public int readInt() throws IOException {
		int b1 = this.stream.read();
		int b2 = this.stream.read();
		int b3 = this.stream.read();
		int b4 = this.stream.read();
		return ((b1 & 0xFF) | ((b2 & 0xFF) << 8) | ((b3 & 0xFF) << 16) | ((b4 & 0xFF) << 24));
	}

	/**
	 * Read a 32-bit integer in a compressed format to the buffer.
	 * @return decoded integer value.
	 */
	public int read7biInt() throws IOException {
		int value = 0, shift = 0;
		for (;;) {
			byte temp = (byte)this.stream.read();
			value |= (temp & 0x7F) << (shift & 0x1F);
			shift += 7;
			if ((temp & 0x80) == 0)
				break;
		}
		return value;
	}

	/**
	 * Reads the string and returns string value.
	 * @return string value.
	 */
	public String readString() throws IOException {
		int length = this.read7biInt();
		byte[] array = new byte[length];
		this.readBytes(array);
		return new String(array, "UTF-8");
	}

	/**
	 * Fills byte array and fom the stream.
	 * @param array - array of bytes.
	 */
	public void readBytes(byte[] array) throws IOException {
		for (int index = 0; index < array.length; index++)
			array[index] = (byte)this.stream.read();
	}

	/**
	 * Returns the number of bytes that can be read (or skipped over) from this input stream without blocking.
	 * @return the number of bytes that can be read from this input stream without blocking.
	 */
	public int available() throws IOException {
		return this.stream.available();
	}

	/**
	 * Closes this input stream and releases any system resources associated with the stream.
	 */
	public void close() throws IOException {
		this.stream.close();
	}
 };
