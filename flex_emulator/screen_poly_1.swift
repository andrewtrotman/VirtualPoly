/*
	SCREEN_POLY.SWIFT
	-----------------
	Copyright (c) 2025 Andrew Trotman
*/
import Foundation

/*
	CLASS SCREEN_POLY
	-----------------
*/
class screen_poly : screen_base
	{
	var machine : UnsafeRawPointer?
	
	/*
		RESET()
		-------
	*/
	override func reset()
		{
		bitmap = [UInt32](repeating: 0, count: 480 * 240)
		}

	/*
		SET_MACHINE()
		-------------
	*/
	func set_machine(poly : UnsafeRawPointer?)
		{
		machine = poly
		}

	/*
		RENDER_ENTIRE_SCREEN()
		----------------------
		The Poly text screen is 40 columns by 24 lines.  There is apparently an emulated 80-column mode
		using screen 5, but I've never seen it in action on a running Poly, so don't know what it looks like!
	*/
	override func render_entire_screen()
		{
		if machine != nil
			{
			for which in 0 ..< bitmap.count
				{
				bitmap[which] = 0;
				}
			machine_poly_render_screen(machine, &bitmap, flash_state)
			}
		}
	}
