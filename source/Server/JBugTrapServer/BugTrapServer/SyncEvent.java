/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Synchronization event class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

/**
 * Synchronization event object.
 */
class SyncEvent {
	/** True if event is signaled. */
	private boolean signalled;
	/** True if event must be automatically reset. */
	private final boolean autoReset;

	/**
	 * Create non-signalled manual reset event.
	 */
	public SyncEvent() {
		this.signalled = false;
		this.autoReset = false;
	}

	/**
	 * Create new event.
	 * @param signalled - true if event is signaled.
	 * @param autoReset - true if event must be automatically reset.
	 */
	public SyncEvent(boolean signalled, boolean autoReset) {
		this.signalled = signalled;
		this.autoReset = autoReset;
	}

	/**
	 * Wait for the event.
	 */
	public synchronized void waitEvent() throws InterruptedException {
		if (! this.signalled) {
			this.wait();
		}
		if (this.autoReset) {
			this.signalled = false;
		}
	}

	/**
	 * Wait for the event.
	 * @param timeout - timeout value in milliseconds.
	 * @return true if event was fired.
	 */
	public synchronized boolean waitEvent(long timeout) throws InterruptedException {
		if (! this.signalled) {
			this.wait(timeout);
		}
		boolean result = this.signalled;
		if (this.autoReset) {
			this.signalled = false;
		}
		return result;
	}

	/**
	 * Notify waiting threads about the event.
	 */
	public synchronized void setEvent() {
		this.signalled = true;
		if (this.autoReset)
			this.notify();
		else
			this.notifyAll();
	}

	/**
	 * Reset event.
	 */
	public synchronized void resetEvent() {
		this.signalled = false;
	}
}
