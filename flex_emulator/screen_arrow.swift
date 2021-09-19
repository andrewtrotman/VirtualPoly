/*
	SCREEN_ARROW.SWIFT
	------------------
	Copyright (c) 2021 Andrew Trotman
*/

import Foundation

/*
	CLASS SCREEN_ARROW
	------------------
*/
class screen_arrow : screen_base
	{
	var screen: UnsafePointer<UInt8>?	// the bytemap of the screen buffer

	/*
		SET_SCREEN_BUFFER()
		-------------------
	*/
	override func set_screen_buffer(screen_buffer: UnsafePointer<UInt8>?)
		{
		screen = screen_buffer!
		}

	/*
		RESET()
		-------
	*/
	override func reset()
		{
		bitmap = [UInt32](repeating: 0, count: 480 * 240)
		}

	/*
		RENDER_ENTURE_SCREEN()
		----------------------
	*/
	override func render_entire_screen()
		{
		let width = 64

		if (screen != nil)
			{
			for y in 0 ..< 24
				{
				for x in 0 ..< width
					{
					draw_screen(screen_x: x, screen_y: y, character: screen![y * width + x])
					}
				}
			}
		}

	/*
		DRAW_SCREEN()
		-------------
		64-column text screen renderer
	*/
	private func draw_screen(screen_x: Int, screen_y: Int, character: UInt8)
		{
		let character_to_show = character == 0 ? 32 : character
		let foreground: UInt32 = 0x00FFFFFF
		let background: UInt32 = 0x00000000
		let glyph_base = 0
		let from = (Int(character_to_show) - 32 + glyph_base) * 10

		for y in 0 ..< 10
			{
			let pos = get_saa5050_font()[from + y]
			var into = (screen_x * 6 + screen_y * 10 * 480) + (y * 480)

			for x in 2 ..< 8
				{
				bitmap[into] = ((pos & (0x80 >> x)) != 0) ? foreground : background
				into = into + 1
				}
			}
		}
	}
