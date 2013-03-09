SerialUI: Serial User Interface.
Copyright (C) 2013 Pat Deegan.  All rights reserved.

http://www.flyingcarsandstuff.com/projects/SerialUI

SerialUI is released under the LGPL -- see 
LICENSE.txt for details.

*********************  SerialUI Overview  *********************
 
SerialUI is useful when you want to provide a user interface
through the serial channel (menus, sub-menus and command
execution). It provides built-in support for navigation
through the menu hierarchy and online help.

With SerialUI, you can create a hierarchy of menus and
submenus of arbitrary depth (limited only by ROM/RAM space).

Each menu contains a list of menu items. There are two
types of SerialUI menu items:

  Sub menus: lead you to another level of menu items

  Commands: actually perform some type of action.

Exactly _what_ happens when a user issues a command is
determined by the callback implementations.

A few commands are built-in and don't need to be defined:

	- '?': Help, which displays all the available menu keys and
			help messages, where defined.

	- '..': Up, which moves up to a parent menu from a sub-menu

	- 'quit': Exit the SerialUI interface (available in top level
			  menu).

Using SerialUI, you can create any serial user interface.
Example menu hierarchy:

 + information
 |
 |
 + enable -----+ on
 |             |
 |             + off
 |
 |
 + settings ---------+ red
                     |
                     + green
                     |
                     + blue
                     |
                     + deviceid
                     |
                     + show


So, here we'd have a three-option top level menu (information, 
enable, settings) with two of those options leading to 
sub-menus.

Every "leaf" (option that doesn't lead to a sub-menu) is a
command that uses a callback specified when setting up the
menu item.


 ************************** TRANSCIPT **************************

 Here's a sample of the interaction through the serial
 connection (created using the code in examples/SuperBlinker):

  +++ Welcome to the SuperBlinker +++
  Enter '?' to list available options.
  > ?
  *** Help for SuperBlinker Main Menu

  	information         Retrieve data and current settings
  	enable              Enable/disable device
  	settings            Perform setup and config

  	quit                Exit SerialUI
  	?                   List available menu items
  > settings
  SuperBlinker Settings
  > ?
  *** Help for SuperBlinker Settings

	red                 Set color [0-255]
	green
	blue
	deviceid            Set dev ID [string]
	show

	..                  Move up to parent menu
	?                   List available menu items

  > red
  ... 10
  OK
  > green
  ... 20
  OK
  > blue
  ... 30
  > deviceid
  ... YayDevice
  OK
  > show
  (Called 'show_info' from menu: SuperBlinker Settings)
  ID: YayDevice
  Color--> R:10 G:20 B:30
  Device is OFF
  > ..
  SuperBlinker Main Menu
  > quit



See examples/SuperBlinker for a full demo/tutorial.

See INSTALL.txt for installation instructions.

Please let me know if you use SerialUI in your projects, and
provide a URL if you'd like me to link to it from the SerialUI
home.

Enjoy!
Pat Deegan, psychogenic.com

