/*
	SCREEN_BASE.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/

import Foundation

/*
	CLASS SCREEN_BASE
	-----------------
*/
class screen_base
	{
	enum screen_width: Int
		{
		case fourty = 40
		case sixtyfour = 64
		case eighty = 80
		}

	var rendering_width = screen_width.fourty		// width of the screen in characters
	var flash_state = false								// the status of the flashing cursor (0 = off, 1 = on)
	var bitmap = [UInt32]()								// the bitmap of the screen buffer

	/*
		SET_SCREEN_BUFFER()
		-------------------
	*/
	func set_screen_buffer(screen_buffer: UnsafePointer<UInt8>?)
		{
		}

	/*
		SET_WIDTH()
		-----------
	*/
	func set_width(new_width: screen_width)
		{
		rendering_width = new_width
		}

	/*
		GET_WIDTH()
		-----------
	*/
	func get_width() -> screen_width
		{
		return rendering_width
		}

	/*
		RESET()
		-------
	*/
	func reset()
		{
		}

	/*
		RENDER_ENTIRE_SCREEN()
		----------------------
	*/
	func render_entire_screen()
		{
		}

	/*
		PRINT_CHARACTER()
		-----------------
	*/
	func print_character(raw_character: UInt8)
		{
		}

	/*
		SERIALISE()
		-----------
	*/
	func serialise(file: FileHandle)
		{
		}

	/*
		DESERIALISE()
		-------------
	*/
	func deserialise(file: FileHandle) throws
		{
		}
	}
