/*
	SCREEN_POLY_1.SWIFT
	-------------------
	Copyright (c) 2025 Andrew Trotman
*/
import Foundation

/*
	CLASS SCREEN_POLY_1
	---------------------
*/
class screen_poly_1 : screen_base
	{
	var screen: UnsafePointer<UInt8>?	// the bytemap of the screen buffer
	let black:   UInt32 = 0x00000000
	let red:     UInt32 = 0x000000FF
	let green:   UInt32 = 0x0000FF00
	let yellow:  UInt32 = 0x0000FFFF
	let blue:    UInt32 = 0x00FF0000
	let magenta: UInt32 = 0x00FF00FF
	let cyan:    UInt32 = 0x00FFFF00
	let white:   UInt32 = 0x00FFFFFF

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
		RENDER_ENTIRE_SCREEN()
		----------------------
		The Poly text screen is 40 columns by 24 lines.  There is apparently an emulated 80-column mode
		using screen 5, but I've never seen it in action on a running Poly, so don't know what it looks like!
	*/
	override func render_entire_screen()
		{
		let width = 40
		var foreground: UInt32 = 0x00FFFFFF
		var background: UInt32 = 0x00000000

		if (screen != nil)
			{
			for y in 0 ..< 24
				{
				foreground = 0x00FFFFFF
				background = 0x00000000
				for x in 0 ..< width
					{
					switch screen![y * width + x]
						{
						case 1:
							foreground = red;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 2:
							foreground = green;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 3:
							foreground = yellow;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 4:
							foreground = blue;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 5:
							foreground = magenta;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 6:
							foreground = cyan;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 7:
							foreground = white;
							draw_screen(screen_x: x, screen_y: y, character: 32, foreground: background, background: background)
						case 10:
							print("10")
						case 13:
							print("13")
						default:
							draw_screen(screen_x: x, screen_y: y, character: screen![y * width + x], foreground: foreground, background: background)
						}
					}
				}
			}
		}

	/*
		DRAW_SCREEN()
		-------------
		Render one character to the screen
	*/
	private func draw_screen(screen_x: Int, screen_y: Int, character: UInt8, foreground: UInt32, background: UInt32)
		{
		let character_to_show = character == 0 ? 32 : character
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
