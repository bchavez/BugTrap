/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Main application class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

import java.io.*;
import java.util.*;

/** Main application class. */
class ServerApp {
	/** This event is being fired when application should quit. */
	private static SyncEvent stopEvent = new SyncEvent();

	/**
	 * Application entry point.
	 * @param args - command line arguments.
	 */
	public static void main(String args[]) {
		try {
			Runtime.getRuntime().addShutdownHook(new Thread() {
						public void run() {
							ServerApp.stopEvent.setEvent();
						}
					});
			System.out.println("Server initialization...");
			ApplicationSettings applicationSettings = new ApplicationSettings();
			applicationSettings.load();
			AsyncServer server = new AsyncServer(applicationSettings);
			server.setName("BugTrap Server Listener");
			server.startServer();
			System.out.println(" Press Ctrl+C to stop application");
			try {
				ServerApp.stopEvent.waitEvent();
			} catch (InterruptedException error) {
			}
			System.out.println("Stopping server listener...");
			server.stopServer();
		}
		catch (Exception error) {
			error.printStackTrace();
		}
	}
}
