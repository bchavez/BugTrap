/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Main server class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

import java.util.*;
import java.util.concurrent.*;
import java.net.*;
import java.io.*;
import java.nio.*;
import java.nio.channels.*;

/** Main server class. */
class AsyncServer extends Thread {
	/** Server parameters block. */
	final private ApplicationSettings applicationSettings;
	/** Notifies other threads about server initialization. */
	final private SyncEvent serverInit;
	/** Worker thread pool. */
	final private ExecutorService threadPool;
	/**
	 * Use the same byte buffer for all channels. A single thread is
	 * servicing all the channels, so no danger of concurrent acccess.
	 */
	final private ByteBuffer buffer;
	/** True after successful start. */
	private boolean started;

	/** Temporary file data.*/
	private class TempFileData {
		/** Create new temporary file. */
		public TempFileData() throws IOException {
			this.tempFile = File.createTempFile("BugTrapServer_", ".tmp");
			this.outputStream = new FileOutputStream(this.tempFile);
			this.dataSize = 0;
		}

		/** Close output stream and release used memory. */
		public void close() throws IOException {
			if (this.outputStream != null) {
				this.outputStream.close();
				this.outputStream = null;
			}
			if (this.tempFile != null) {
				this.tempFile.delete();
				this.tempFile = null;
			}
		}

		/** File descriptor. */
		public File tempFile;
		/** File output stream. */
		public FileOutputStream outputStream;
		/** Number of bytes stored in a file. */
		public int dataSize;
	};

	/**
	 * Initialize the object.
	 * @param applicationSettings - server parameters block.
	 */
	public AsyncServer(ApplicationSettings applicationSettings) throws Exception {
		super.setDaemon(true);
		this.applicationSettings = applicationSettings;
	    this.serverInit = new SyncEvent();
	    this.threadPool = Executors.newFixedThreadPool(4);
	    this.buffer = ByteBuffer.allocate(1024);
	}

	/** Executes server loop. */
	public void run() {
		try {
			ServerSocketChannel serverSocketChannel = null;
			try {
				Selector selector = null;
				try {
					System.out.println("Starting server listener...");
					// Create a new Selector for use below.
					selector = Selector.open();
					// Allocate an unbound server socket channel.
					serverSocketChannel = ServerSocketChannel.open();
					// Get the associated ServerSocket to bind it with.
					ServerSocket serverSocket = serverSocketChannel.socket();
					// Get server socket address.
					InetSocketAddress addr = new InetSocketAddress(this.applicationSettings.portNumber);
					// Set the port the server channel will listen to.
					serverSocket.bind(addr);
					// Set non-blocking mode for the listening socket.
					serverSocketChannel.configureBlocking(false);
					// Register the ServerSocketChannel with the Selector.
					serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
					System.out.println("Server listener is running");
					this.started = true;
				}
				finally {
					this.serverInit.setEvent();
				}

				while (! Thread.interrupted()) {
					// Selects a set of keys whose corresponding channels are ready for I/O operations.
					int numKeys = selector.select();
					if (numKeys == 0) {
						// Nothing to do.
						continue;
					}
					// Get an iterator over the set of selected keys.
					Iterator itKey = selector.selectedKeys().iterator();
					// Look at each key in the selected set.
					while (itKey.hasNext()) {
						// Get next key instance.
						SelectionKey key = (SelectionKey)itKey.next();
						// The ready set of channels can change while you are processing them.
						// So, you should remove the one you are processing when you process it.
						// Removal does not trigger a ConcurrentModificationException to be thrown.
						itKey.remove();
						SocketChannel clientSocketChannel = null;
						TempFileData data = null;
						try {
							// Is a new connection coming in?
							if (key.isAcceptable()) {
								// Accept client request.
								clientSocketChannel = serverSocketChannel.accept();
								// Set non-blocking mode for the client socket.
								clientSocketChannel.configureBlocking(false);
								// Register the SocketChannel with the Selector.
								SelectionKey readKey = clientSocketChannel.register(selector, SelectionKey.OP_READ);
								// Create new socket state object associated with the read key.
								readKey.attach(new TempFileData());
								// Don't close socket channel because it may have more data.
								clientSocketChannel = null;
							}
							// Is there data to read on this channel?
							if (key.isReadable()) {
								// Get client socket channel.
								clientSocketChannel = (SocketChannel)key.channel();
								// Make buffer empty.
								this.buffer.clear();
								// Loop while data available, channel is non-blocking.
								int received = clientSocketChannel.read(this.buffer);
								// Get socket state object.
								data = (TempFileData)key.attachment();
								if (received > 0) {
									// Skip extra large data packets.
									if (this.applicationSettings.maxReportSize < 0 || data.dataSize + received <= this.applicationSettings.maxReportSize + 1024) {
										// Make buffer readable.
										this.buffer.flip();
										// Copy data from temporary buffer to the stream.
										data.outputStream.write(this.buffer.array(), 0, received);
										data.dataSize += received;
										// Don't close socket channel because it might have more data.
										clientSocketChannel = null;
										// Keep file data.
										data = null;
									}
								}
								else if (received < 0) {
									// Close file stream before opening file in another thread.
									data.outputStream.close();
									data.outputStream = null;
									// Create request handler object.
									RequestHandler handler = new RequestHandler(this.applicationSettings, data.tempFile);
									// Detach file descriptor.
									data.tempFile = null;
									// Execute handler job.
									this.threadPool.execute(handler);
									// Trigger graceful shutdown.
									Socket socket = clientSocketChannel.socket();
									socket.shutdownInput();
									socket.shutdownOutput();
									clientSocketChannel.close();
									clientSocketChannel = null;
								}
							}
						}
						catch (InterruptedException error) {
							break;
						}
						catch (ClosedByInterruptException error) {
							break;
						}
						catch (Exception error) {
							error.printStackTrace();
						}
						finally {
							// Close client socket channel.
							if (clientSocketChannel != null) {
								Socket socket = clientSocketChannel.socket();
								socket.setSoLinger(true, 0);
								clientSocketChannel.close();
								clientSocketChannel = null;
							}
							// Close stream and delete file.
							if (data != null) {
								data.close();
								data = null;
							}
						}
					}
				}
			}
			finally {
				// Close server socket channel.
				if (serverSocketChannel != null) {
					serverSocketChannel.close();
					serverSocketChannel = null;
				}
				// Finish all thread pool tasks.
				this.threadPool.shutdown();
				this.threadPool.awaitTermination(10L, TimeUnit.SECONDS);
			}
		}
		catch (Exception error) {
			error.printStackTrace();
		}
		System.out.println("Server listener has been stopped");
	}

	/** Start server listener thread. */
	public void startServer() throws Exception {
		super.start();
		this.serverInit.waitEvent();
		if (! this.started) {
			throw new Exception("Unable to start server listener");
		}
	}

	/** Stop server listener thread.*/
	public void stopServer() throws Exception {
		super.interrupt();
		super.join();
	}

	/**
	 * Get server parameters.
	 * @return server parameters block.
	 */
	public ApplicationSettings getParams() {
		return this.applicationSettings;
	}
}
