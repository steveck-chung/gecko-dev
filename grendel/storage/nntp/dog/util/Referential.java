/*
 * Referential.java
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Knife.
 *
 * The Initial Developer of the Original Code is dog.
 * Portions created by dog are Copyright (C) 1998 dog <dog@dog.net.uk>. All Rights Reserved.
 *
 * Contributor(s): n/a.
 *
 * You may retrieve the latest version of this package at the knife home page,
 * located at http://www.dog.net.uk/knife/
 */

package dog.util;

/**
 * An interface implemented by objects that maintain an external object reference.
 * <p>
 * This reference can be used to tag objects with a datasource id or store a hashtable of associated values.
 *
 * @author dog@dog.net.uk
 * @version 1.0final
 */
public interface Referential {

	/**
	 * Returns the object reference.
	 */
	public Object getReference();

	/**
	 * Sets the object reference.
	 * @param reference the object reference to store.
	 */
	public void setReference(Object reference);

}
