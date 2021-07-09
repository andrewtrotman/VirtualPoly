/*
	TERMINAL.SWIFT
	--------------
*/

import Foundation

/*
	CLASS TERMINAL
	--------------
*/
class terminal
	{
	enum screen_width: Int
		{
		case fourty = 40
		case eighty = 80
		}

	var flash_state = false								// the status of the flashing cursor (0 = off, 1 = on)
	private var rendering_width = screen_width.fourty		// how wide the screen appeara
	private var row = 0												// current cursor row
	private var column = 0											// current cursor column
	private var escape_mode = false								// are we buffering escape characters?
	private var escape_sequence = [UInt8]()					// escapr characgters being buffered
	private let width = 80											// width of the screen buffer
	private let height = 24											// height of the screen buffer
	private var screen = [UInt8]()								// the screen buffer

	var bitmap = [UInt32]()											// the bitmap of the screen buffer

	/*
		INIT()
		------
	*/
	init()
		{
		reset()
		}

	/*
		RESET()
		-------
	*/
	func reset()
		{
		rendering_width = .fourty
		row = 0
		column = 0
		escape_mode = false;
		escape_sequence = []

		screen = [UInt8](repeating: 32, count: width * height)
		bitmap = [UInt32](repeating: 0, count: 480 * 240)
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
		EXTRACT_INTEGER()
		-----------------
		Turn ascii chacters into an integer (atoi())
	*/
	private func extract_integer(sequence: ArraySlice<UInt8>) -> Int
		{
		var result: Int = 0

		for digit in sequence
			{
			result = result * 10 + Int(digit) - 48
			}
//print("->" + String(result))
		return result
		}

	/*
		GOTO_XY()
		---------
	*/
	private func goto_xy(new_row: Int, new_column: Int) -> Bool
		{
		row = new_row <= height && new_row > 0 ? new_row - 1 : row
		column = new_column <= rendering_width.rawValue && new_column > 0 ? new_column - 1 : column

		return true
		}

	/*
		CLEAR_SCREEN()
		--------------
	*/
	private func clear_screen() -> Bool
		{
		for x in 0 ..< width * height
			{
			screen[x] = 32
			}
		row = 0
		column = 0

		return true
		}

	/*
		DELETE_TO_EOLN()
		----------------
	*/
	private func delete_to_eoln() -> Bool
		{
		for pos in column ..< width
			{
			screen[row * width + pos] = 32
			}

		return true
		}

	/*
		INSERT_LINE()
		-------------
		Insert a line below the line the cursor is currently on
	*/
	private func insert_line() -> Bool
		{
		if row != height - 1
			{
			for y in ((row + 1) ..< height).reversed()
				{
				for x in 0 ..< width
					{
					let into = y * width + x
					screen[into] = screen[into - width]
					}
				}

			/*
				Blank the cursor row
			*/
			let into =  row * width
			for byte in into ..< into + width
				{
				screen[byte] = 32
				}
			}
		return true
		}

	/*
		DELETE_LINE()
		-------------
	*/
	private func delete_line() -> Bool
		{
		for x in row * width ..< (height - 1) * width
			{
			screen[x] = screen[x + width]
			}
		/*
			Blank the bottom row
		*/
		for byte in 23 * width ..< 24 * width
			{
			screen[byte] = 32
			}

		return true
		}

	/*
		MANAGE_ESCAPE_COMMANDS()
		------------------------
	*/
	private func manage_escape_commands(character: UInt8)
		{
		var done = false

		switch character
			{
			case Character("H").asciiValue:			// set cursor position	ESC [ <row> ; <column> H
				if escape_sequence == []
					{
					done = goto_xy(new_row: 0, new_column: 0)
					}
				else if escape_sequence[0] == 91
					{
					var position_of_semicolon = 1
					while escape_sequence[position_of_semicolon] != 59
						{
						position_of_semicolon = position_of_semicolon + 1
						}
					let new_row = extract_integer(sequence: escape_sequence[1 ..< position_of_semicolon])
					let new_column = extract_integer(sequence: escape_sequence[(position_of_semicolon + 1)...])
					done = goto_xy(new_row: new_row, new_column: new_column)
					}
//print("GotoYX(" + String(row) + "," + String(column) + ")")
			case Character("J").asciiValue:			// Clear screen
				if (escape_sequence == [91, 50])		// ESC [ 2 J Erase all of the display
					{
					done = clear_screen()
					}
			case Character("K").asciiValue:
//print("DeleteEOLN")
				if escape_sequence == [91]			// ESC [ K erase to end of line (inclusive)
					{
					done = delete_to_eoln()
					}
			case Character("L").asciiValue:					// ESC [ L insert line below
				if escape_sequence == []
					{
					done = insert_line()
					}
			case Character("M").asciiValue:					// ESC [ M delete current line
//print("DeleteLineAt")
				if escape_sequence == [91]
					{
					done = delete_line()
					}
			default:
				escape_sequence.append(character)
				escape_mode = escape_sequence.count >= 16 ? false : escape_mode
			}
		if done
			{
			escape_sequence.removeAll()
			escape_mode = false
			}
	}
	/*
		PRINT_CHARACTER()
		-----------------
		VED2 uses:
			char bep[1]  = 0x07				/* beep */
			char eol[3]  = ESC [ K			/* Erase to EOL */
			char clr[4]  = ESC [ 2 J		/* Clear screen & pos curser at 1,1 */
			char hom[3]  = ESC [ H			/* Cursor Home */
			char ins[3]  = ESC [ L			/* Insert Line */
			char dell[3] = ESC [ M			/* Delete Line */
	*/
	func print_character(raw_character: UInt8)
		{
		var character = raw_character

		if escape_mode
			{
			manage_escape_commands(character: raw_character)
			return
			}

		if raw_character == 27			// ESC (so enter VT100 Escape sequence processing mode)
			{
			escape_mode = true;
			return
			}

		if raw_character == 0 || raw_character == 7		// 7 is the bell 0 is a null
			{
			return
			}

		if raw_character < 32 || raw_character >= 0x7F
			{
			character = 32;
			}

		/*
			LF (0x0A) is the new line character
		*/
		if raw_character == 0x0A
			{
			row = row + 1
			column = 0;
			}
		/*
			CR (0x0D) is the new line character
		*/
		if raw_character == 0x0D
			{
			column = 0;
			return;
			}
		/*
			BS (0x08) is the backspace character
		*/
		if raw_character == 0x08
			{
			column = column > 0 ? column - 1 : 0
			screen[row * width + column] = character
			return;
			}
		/*
			Check for scrolling
		*/
		if column >= rendering_width.rawValue
			{
			column = 0
			row = row + 1
			}
		if row >= height
			{
			/*
				Scroll
			*/
			for byte in width ..< (width * height)
				{
				screen[byte - width] = screen[byte]
				}
			/*
				Blank the bottom row
			*/
			for byte in ((height - 1) * width) ..< (height * width)
				{
				screen[byte] = 32
				}
			/*
				Go to the bottom row
			*/
			row = height - 1
			}

		if raw_character != 0x0A && raw_character != 0x7F
			{
			screen[row * width + column] = character
			column = column + 1
			}
		}

	/*
		RENDER_ENTURE_SCREEN()
		----------------------
	*/
	func render_entire_screen()
		{
		let on: UInt32 = 0x00FFFFFF
		let off: UInt32 = 0x00000000

		if rendering_width == .fourty
			{
			for y in 0 ..< height
				{
				for x in 0 ..< rendering_width.rawValue
					{
					draw_screen_40(screen_x: x, screen_y: y, character: screen[y * width + x], on: on, off: off)
					}
				}
			}
		else
			{
			for y in 0 ..< height
				{
				for x in 0 ..< rendering_width.rawValue
					{
					draw_screen_80(screen_x: x, screen_y: y, character: screen[y * width + x], on: on, off: off)
					}
				}
			}
		}

	/*
		DRAW_SCREEN_40()
		----------------
		40-column text screen renderer

		red = dataType[offset]
		green   = dataType[offset + 1]
		blue = dataType[offset + 2]
		alpha  = dataType[offset + 3]
	*/
	private func draw_screen_40(screen_x: Int, screen_y: Int, character: UInt8, on: UInt32, off: UInt32)
		{
		var foreground = on
		var background = off
		let glyph_base = 0
		let from = (Int(character) - 32 + glyph_base) * 10

		if (flash_state && screen_y == row) && (screen_x == column || (screen_x == 39 && column >= 39))
			{
			swap(&foreground, &background);
			}

		for y in 0 ..< 10
			{
			let pos = get_saa5050_font()[from + y]
			var into = (screen_x * 12 + screen_y * 10 * 480) + (y * 480)

			for x in 2 ..< 8
				{
				if (pos & (0x80 >> x)) != 0
					{
					bitmap[into] = foreground
					bitmap[into + 1] = foreground
					into = into + 2
					}
				else
					{
					bitmap[into] = background
					bitmap[into + 1] = background
					into = into + 2
					}
				}
			}
		}

	/*
		DRAW_SCREEN_80()
		----------------
		80-column text screen renderer
		red = dataType[offset]
		green   = dataType[offset + 1]
		blue = dataType[offset + 2]
		alpha  = dataType[offset + 3]
	*/
	private func draw_screen_80(screen_x: Int, screen_y: Int, character: UInt8, on: UInt32, off: UInt32)
		{
		var foreground = on
		var background = off
		let glyph_base = 0
		let from = (Int(character) - 32 + glyph_base) * 10

		if (flash_state && screen_y == row) && (screen_x == column || (screen_x == 39 && column >= 39))
			{
			swap(&foreground, &background);
			}

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

	/*
		SERIALISE()
		-----------
	*/
	func serialise(file: FileHandle)
		{
		file.write(Data(screen))

		file.write(Data(bytes: &row, count:MemoryLayout.size(ofValue:row)))
		file.write(Data(bytes: &column, count:MemoryLayout.size(ofValue:column)))
		file.write(Data(bytes: &escape_mode, count:MemoryLayout.size(ofValue:escape_mode)))

		var escape_length = escape_sequence.count
		file.write(Data(bytes: &escape_length, count:MemoryLayout.size(ofValue:escape_length)))
		file.write(Data(escape_sequence))
		}

	/*
		DESERIALISE()
		-----------
	*/
	func deserialise(file: FileHandle) throws
		{
			var data = try file.read(upToCount: screen.count)
			data?.copyBytes(to: &screen, count: screen.count)

			data = try file.read(upToCount: MemoryLayout.size(ofValue:row))
			row = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: row).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:column))
			column = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: column).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:escape_mode))
			escape_mode = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: escape_mode).self)})

			var escape_length = escape_sequence.count
			data = try file.read(upToCount: MemoryLayout.size(ofValue:escape_length))
			escape_length = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: escape_length).self)})

			escape_sequence = [UInt8](repeating: 32, count: escape_length)

			data = try file.read(upToCount: escape_sequence.count)
			data?.copyBytes(to: &escape_sequence, count: escape_sequence.count)
		}
	}
